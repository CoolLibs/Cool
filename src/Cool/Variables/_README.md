## How to add a new Variable type

- Copy-paste one of the existing Variable files (`Variable_float.h` for example) and replace the implemention of all the required functions for your type.

- If it doesn't already exist, implement a `to_string()` function:

```cpp
inline auto to_string(const Variable<Angle>& angle) -> std::string
{
    return angle.as_turns() + " turn";
}
```

- Include your new file in `Variables.h`.

- Add your new type in the type-from-string associations in "Cool/lib/type_from_string/generator.py" and run the script.

- _If you are working on CoolLab:_ Add your new type in "code_generator.py" and run the script.

TODO: in the shader we can `#define Color vec3` so that the shader compiles just fine and when we parse we can distinguish color from vec3.
