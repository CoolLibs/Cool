## How to add a new Variable type

- Create the corresponding StrongType if necessary (e.g. `Cool::Color`, `Cool::Angle`, etc.) in the "StrongTypes" folder. Then implement the corresponding `set_uniform()` function in "Gpu/OpenGL/Shader.h" for your StrongType.

- Add your new type in "generator_variables.py" AT THE END OF THE LIST (otherwise the serialization will get messed up) and run the script.

- Copy-paste one of the existing Variable files (`Variable_float.h`/`Variable_float.cpp` for example) and replace the implementation of all the required functions for your type.

- Update the documentation on the website (TODO(Variables) Explain more).

- If your variable needs special code to be generated in the shader (other than replacing `INPUT` with `uniform`), add it to `gen_input_shader_code()`.

- If you are working on Coollab, re-run the "src/Commands/generate_commands.py" and "src/Commands/generate_set_variable_commands.py" scripts.

- Add it to *TestVariables.h".