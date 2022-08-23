## How to add a new Variable type

- Create the corresponding StrongType if necessary (e.g. `Cool::Color`, `Cool::Angle`, etc.) in the "StrongTypes" folder. Then implement the corresponding `set_uniform()` function in "Gpu/OpenGL/Shader.h" for your StrongType.

- Copy-paste one of the existing Variable files (`Variable_float.h` for example) and replace the implemention of all the required functions for your type.

- Add your new type in "generate_variables.py" and run the script.

- Add a define in "input_definitions.glsl".

- Update the documentation in "res/shaders/_README.md".

- Add a specialization of `value_from_string` in `String`.

- If you are working on CoolLab, re-run the "src/Commands/generate_commands.py" and "src/Commands/generate_set_variable_commands.py" scripts.
