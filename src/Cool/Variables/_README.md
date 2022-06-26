## How to add a new Variable type

- Create the corresponding StrongType if necessary (e.g. `Cool::Color`, `Cool::Angle`, etc.) in the "StrongTypes" folder. Then implement the corresponding `set_uniform()` function in "Gpu/OpenGL/Shader.h" for your StrongType.

- Copy-paste one of the existing Variable files (`Variable_float.h` for example) and replace the implemention of all the required functions for your type.

- Include your new file in `Variables.h`.

- Add your new type in the type-from-string associations in "Cool/lib/type_from_string/generator.py" and run the script.

- _If you are working on CoolLab:_ Add your new type in "code_generator.py" and run the script.

- You might need to delete "last-session-cache.json" if you get weird errors like "An Input has an invalid default variable!".

TODO: in the shader we can `#define Color vec3` so that the shader compiles just fine and when we parse we can distinguish color from vec3.
