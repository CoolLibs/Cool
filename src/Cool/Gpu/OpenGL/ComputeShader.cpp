#pragma once
#include "glm/common.hpp"
#include "glm/fwd.hpp"
#include "glm/vector_relational.hpp"
#if defined(COOL_OPENGL)

#include "ComputeShader.h"

#include <filesystem>
#include <glm/glm.hpp>

#include "Cool/File/File.h"
#include "Shader.h"

namespace Cool::OpenGL {

static void assert_compute_shader_is_bound(GLuint id)
{
#if DEBUG
    GLint current_id = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &current_id);
    assert(static_cast<GLuint>(current_id) == id && "The compute shader must be bound before calling compute().");
#else
    (void)id;
#endif
}

ComputeShader::ComputeShader(glm::uvec3 working_group_size, std::string_view source_code) :
Shader(
    ShaderModule{{
        generate_boilderplate_for_size(working_group_size) + std::string(source_code),
        ShaderKind::Compute,
    }}
),
_working_group_size(working_group_size)
{}

ComputeShader::ComputeShader(glm::uvec2 working_group_size, std::string_view source_code) : ComputeShader(glm::uvec3(working_group_size, 1), source_code) {}
ComputeShader::ComputeShader(unsigned int working_group_size, std::string_view source_code) : ComputeShader(glm::uvec3(working_group_size, 1, 1), source_code) {}

void ComputeShader::compute(glm::uvec3 size)
{
    assert(size != glm::uvec3(0));
    set_uniform("DispatchSize", size);
    glm::uvec3 dispatch_group_count = size / _working_group_size + glm::uvec3(1);
    assert_compute_shader_is_bound(id());
    GLDebug(glDispatchCompute(dispatch_group_count.x, dispatch_group_count.y, dispatch_group_count.z));
    GLDebug(glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT));
}

auto load_compute_shader_from_file(glm::uvec3 working_group_size, const std::filesystem::path& filePath)
-> ComputeShader
{
    return ComputeShader(working_group_size, *File::to_string(filePath));
}

auto generate_boilderplate_for_size(glm::uvec3 working_group_size) -> std::string {
    return std::string(R"V0G0N(
#version 430

uniform uvec3 DispatchSize;

void cool_main();

)V0G0N")
+ "layout(local_size_x = "
+ std::to_string(working_group_size.x)
+ ", local_size_y = "
+ std::to_string(working_group_size.y)
+ ", local_size_z = "
+ std::to_string(working_group_size.z)
+ ") in;"
+ R"V0G0N(
void main() {
    if (gl_GlobalInvocationID.x < DispatchSize.x
     && gl_GlobalInvocationID.y < DispatchSize.x
     && gl_GlobalInvocationID.z < DispatchSize.y
    ) {
        cool_main();
    }
}
)V0G0N";
};

} // namespace Cool::OpenGL

#endif