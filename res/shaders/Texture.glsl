#include "_COOL_RES_/shaders/shader-utils.glsl" // For unnormalize_uv_with_aspect_ratio()

struct Cool_Texture {
    sampler2D tex;
    float     aspect_ratio;
};

vec4 sample_cool_texture(Cool_Texture tex, vec2 uv)
{
    return texture(tex.tex, unnormalize_uv_with_aspect_ratio(uv, tex.aspect_ratio));
}
vec4 sample_opencv_texture(Cool_Texture tex, vec2 uv)
{
    uv.y *= -1.; // OpenCV textures are upside-down compared to what we expect
    return sample_cool_texture(tex, uv);
}