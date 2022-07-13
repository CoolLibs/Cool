#version 410
#if !defined(COOL_VULKAN) // COOL_OPENGL
#define gl_VertexIndex gl_VertexID
#endif

layout(location = 0) out vec2 _uv;

// We use a smart trick explained here https://stackoverflow.com/a/59739538
// to draw fullscreen with a single triangle
// This is supposed to be more efficient than two triangles

void main()
{
    vec2 vertices[3] = vec2[3](vec2(-1., -1.), vec2(3., -1.), vec2(-1., 3.));
    gl_Position      = vec4(vertices[gl_VertexIndex], 0., 1.);
    _uv              = 0.5 * vertices[gl_VertexIndex] + vec2(0.5);
}