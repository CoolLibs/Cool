## How to add a new Variable type

- Create the corresponding StrongType if necessary (e.g. `Cool::Color`, `Cool::Angle`, etc.) in the "StrongTypes" folder. Then implement the corresponding `set_uniform()` function in "Gpu/OpenGL/Shader.h" for your StrongType.

- Copy-paste one of the existing Variable files (`Variable_float.h` for example) and replace the implemention of all the required functions for your type.

- Add your new type in "generate_variables.py" and run the script.

- Update the documentation in "res/shaders/_README.md".

- Add a specialization of `value_from_string` in `String`.

- Add an overload of `set_uniform()` in "Gpu/OpenGL/Shader".

- If your variable needs special code to be generated in the shader (other than replacing `INPUT` with `uniform`), implement a specialization of `instantiate_shader_code__impl()`.

- If you are working on CoolLab, re-run the "src/Commands/generate_commands.py" and "src/Commands/generate_set_variable_commands.py" scripts.
