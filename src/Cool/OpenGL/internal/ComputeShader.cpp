#include "../ComputeShader.h"

namespace Cool {

static constexpr unsigned int WORK_GROUP_SIZE = 256;

ComputeShader::ComputeShader(const char* filePath)
    : m_shader({ ShaderCode(
        ShaderType::Compute,
        filePath)
      })
{}

ComputeShader::ComputeShader(ComputeShader&& o) noexcept
    : m_shader(std::move(o.m_shader))
{}
void ComputeShader::operator=(ComputeShader&& o) {
    m_shader = std::move(o.m_shader);
}

void ComputeShader::createProgramFromFile(const char* filePath) {
    m_shader.createProgram({ ShaderCode(
        ShaderType::Compute,
        filePath)
    });
}

void ComputeShader::createProgramFromCode(const char* sourceCode) {
    m_shader.createProgram({ ShaderCode::FromCode(
        ShaderType::Compute,
        sourceCode
    )});
}

void ComputeShader::compute(unsigned int nbComputations) {
    m_shader.bind();
    glDispatchCompute(nbComputations / WORK_GROUP_SIZE + ((nbComputations % WORK_GROUP_SIZE == 0) ? 0 : 1), 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);
}

} // namespace Cool