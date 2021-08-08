#version 450

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inColour;
layout(location = 0) out vec3 fragColour;

void main()
{
    // gl_Position = vec4(inPosition, 0.0, 1.0); // Copy 2D position to 3D + depth
    fragColour = inColour; // Copy colour to the fragment shader.
}

// #version 430

// varying vec2 vTexCoords;

// // We use a smart trick explained here https://stackoverflow.com/a/59739538
// // to draw fullscreen with a single triangle
// // This is supposed to be more efficient than two triangles

// void main() {
//     vec2 vertices[3] = vec2[3](vec2(-1.,-1.), vec2(3.,-1.), vec2(-1., 3.));
//     gl_Position = vec4(vertices[gl_VertexID], 0., 1.);
//     vTexCoords = 0.5 * vertices[gl_VertexID] + vec2(0.5);
// }