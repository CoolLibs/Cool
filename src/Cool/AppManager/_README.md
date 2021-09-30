# AppManager

## Prevent *move* and *resize* events from freezing the app

On Windows (and maybe elsewhere too) moving or resizing the window is a blocking event (well its complicated actually). To prevent that we want to use different threads for event handling and app updating.

Since glfw's event handling must happen on the main thread (see [here](https://discourse.glfw.org/t/glfwpollevents-on-background-thread/1185) for details on that) this is the updating and the rendering that we move to another thread, the ```_update_thread```.

BUT we don't do that for OpenGL, because OpenGL was not designed to support multithreading. We could make it work by doing *all* actions on the update thread, including creating the window and the OpenGL context, but I don't want to bother with that complexity because people should start to move away from OpenGL anyways.

### A nasty threading bug

My first implementation was something similar to
```cpp
void AppManager::run() {
    auto update_thread = std::jthread{[&]() {
        NFD_Init();
        while (!glfwWindowShouldClose(_window_manager.main_window().glfw())) {
            update();
        }
    }};
    while (!glfwWindowShouldClose(_window_manager.main_window().glfw())) {
        glfwWaitEvents();
    }
}
```

but the problem is that if the the window is closed at the beginning of `update()`, we would stop listening to events on the main thread (`glfwWaitEvents()`), but we would still have to finish running `update()` on the `update_thread`, which can require *glfw* to still respond to some events. We were thus frozen because events weren't handled anymore. 

In our case it was *ImGui* calling `glfwSetWindowAttrib(window, GLFW_MOUSE_PASSTHROUGH, window_no_input);` which in turn was calling `SetWindowLongW(window->win32.handle, GWL_EXSTYLE, exStyle);` on Windows, which froze.

The solution was to make sure that the `update_thread` finishes first and then stop listening to events :

```cpp
void AppManager::run() {
    auto should_stop   = false;
    auto update_thread = std::jthread{[&]() {
        NFD_Init();
        while (!glfwWindowShouldClose(_window_manager.main_window().glfw())) {
            update();
        }
        should_stop = true;
    }};
    while (!should_stop) {
        glfwWaitEvents();
    }
}
```