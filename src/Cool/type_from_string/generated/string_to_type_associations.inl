/* -----------------------------------------------------------------------------
 * This file was automatically generated by a Python script.
 * PLEASE DON'T EDIT IT DIRECTLY, your changes would be overwritten the next time the script is run.
 * Instead, go to "generator_tfs.py" and edit the "string_to_type_associations" function there.
 * -----------------------------------------------------------------------------
 */

template<>
struct from_impl<"bool"> {
    using type = bool;
};

template<>
struct from_impl<"int"> {
    using type = int;
};

template<>
struct from_impl<"float"> {
    using type = float;
};

template<>
struct from_impl<"Point2D"> {
    using type = Cool::Point2D;
};

template<>
struct from_impl<"vec2"> {
    using type = glm::vec2;
};

template<>
struct from_impl<"vec3"> {
    using type = glm::vec3;
};

template<>
struct from_impl<"vec4"> {
    using type = glm::vec4;
};

template<>
struct from_impl<"RgbColor"> {
    using type = Cool::RgbColor;
};

template<>
struct from_impl<"Camera"> {
    using type = Cool::Camera;
};

template<>
struct from_impl<"Angle"> {
    using type = Cool::Angle;
};

template<>
struct from_impl<"Direction2D"> {
    using type = Cool::Direction2D;
};

template<>
struct from_impl<"Hue"> {
    using type = Cool::Hue;
};

template<>
struct from_impl<"ColorPalette"> {
    using type = Cool::ColorPalette;
};

template<>
struct from_impl<"Gradient"> {
    using type = Cool::Gradient;
};
