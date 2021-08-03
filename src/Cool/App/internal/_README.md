## Prevent *move* and *resize* events from freezing the app

Check out https://discourse.glfw.org/t/glfwpollevents-on-background-thread/1185

Basically event handling must happen on the main thread so it's updating and rendering that we move to another thread ```_update_thread```.

TODO : when using OpenGL this might require us to call ```glfwMakeContextCurrent(_main_window);```