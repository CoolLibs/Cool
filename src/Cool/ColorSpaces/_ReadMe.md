
## How to add a color space

- Go in *generator_colors.py* and add it in the `color_spaces()` function.
- Run the *generator_colors.py* script you just modified.
- Go in *Cool/res/shaders/color_conversions.glsl* and add all the conversions (from and to) between you new color space and all the other color spaces. This means you have 2 * previous_number_of_color_spaces functions to write. You can probably reuse some functions: for example `Cool_sRGB_from_CIELAB(c)` is implemented as simply `Cool_sRGB_from_XYZ(Cool_XYZ_from_CIELAB(c))`.
- Go to *conversions.h* / *conversions.cpp* in this folder and implement the conversion functions. They should match the ones you just wrote in glsl and it should just be a matter of copy-pasting what you did and adjusting the syntax.
- If you are working on CoolLab: go to *src/Module_Nodes/generator.py* and run the script.
- If you are working on CoolLab: edit the website to explain what your color space is and when it makes sense to use it. https://coollibs.github.io/lab/Writing%20Nodes/Color%20Spaces