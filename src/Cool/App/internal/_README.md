# AppManager

## Prevent *move* and *resize* events from freezing the app

Check out https://discourse.glfw.org/t/glfwpollevents-on-background-thread/1185

Basically event handling must happen on the main thread so it's updating and rendering that we move to another thread ```_update_thread```.

BUT we don't do that for OpenGL, because OpenGL was not designed to support multithreading. We could make it work by doing *all* actions on the update thread, including creating the window and the OpenGL context, but I don't want to bother with that complexity because people should start to move away from OpenGL anyways.