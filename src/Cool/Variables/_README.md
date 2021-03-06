## How to add a new Variable type

- Create the corresponding StrongType if necessary (e.g. `Cool::Color`, `Cool::Angle`, etc.) in the "StrongTypes" folder. Then implement the corresponding `set_uniform()` function in "Gpu/OpenGL/Shader.h" for your StrongType.

- Copy-paste one of the existing Variable files (`Variable_float.h` for example) and replace the implemention of all the required functions for your type.

- Include your new file in `Variables.h`.

- Add your new type in the type-from-string associations in "Cool/lib/type_from_string/generator.py" and run the script.

- _If you are working on CoolLab:_ Add your new type in "generate_variables.py" and run the script.

- You might need to delete "last-session-cache.json" if you get weird errors like "An Input has an invalid default variable!".

- Update documentation of the "input_definitions.glsl" and "res/shaders/_README.md"

- Add a define in "input_definitions.glsl"

- Add a specialization of `value_from_string` in `String`
- Add a specialization of `value_from_string` in `String`
