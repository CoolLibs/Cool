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
struct from_impl<"UV"> {
    using type = Cool::Point2D;
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
struct from_impl<"Oklab"> {
    using type = Cool::Color;
};

template<>
struct from_impl<"LinearRGB"> {
    using type = Cool::Color;
};

template<>
struct from_impl<"sRGB"> {
    using type = Cool::Color;
};

template<>
struct from_impl<"Okhsl"> {
    using type = Cool::Color;
};

template<>
struct from_impl<"Oklab_PremultipliedA"> {
    using type = Cool::ColorAndAlpha;
};

template<>
struct from_impl<"Oklab_StraightA"> {
    using type = Cool::ColorAndAlpha;
};

template<>
struct from_impl<"LinearRGB_PremultipliedA"> {
    using type = Cool::ColorAndAlpha;
};

template<>
struct from_impl<"LinearRGB_StraightA"> {
    using type = Cool::ColorAndAlpha;
};

template<>
struct from_impl<"sRGB_PremultipliedA"> {
    using type = Cool::ColorAndAlpha;
};

template<>
struct from_impl<"sRGB_StraightA"> {
    using type = Cool::ColorAndAlpha;
};

template<>
struct from_impl<"Okhsl_PremultipliedA"> {
    using type = Cool::ColorAndAlpha;
};

template<>
struct from_impl<"Okhsl_StraightA"> {
    using type = Cool::ColorAndAlpha;
};

template<>
struct from_impl<"Camera"> {
    using type = Cool::Camera;
};

template<>
struct from_impl<"Camera2D"> {
    using type = Cool::Camera2D;
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

template<>
struct from_impl<"Texture"> {
    using type = Cool::TextureDescriptor_Image;
};

template<>
struct from_impl<"Webcam"> {
    using type = Cool::TextureDescriptor_Webcam;
};

template<>
struct from_impl<"MathExpression"> {
    using type = Cool::MathExpression;
};

template<>
struct from_impl<"mat2"> {
    using type = glm::mat2;
};

template<>
struct from_impl<"mat3"> {
    using type = glm::mat3;
};

template<>
struct from_impl<"mat4"> {
    using type = glm::mat4;
};

template<>
struct from_impl<"Midi"> {
    using type = Cool::MidiChannel;
};

template<>
struct from_impl<"OSC"> {
    using type = Cool::OSCChannel;
};

template<>
struct from_impl<"TimeSpeed"> {
    using type = Cool::TimeSpeed;
};

template<>
struct from_impl<"Video"> {
    using type = Cool::TextureDescriptor_Video;
};

template<>
struct from_impl<"Time"> {
    using type = Cool::Time;
};
