#pragma once

#include "Shader.h"

namespace Cool {

class ComputeShader {
public:
	ComputeShader() = default;
	ComputeShader(const char* filePath);
	ComputeShader(ComputeShader&& o) noexcept;
	~ComputeShader() = default;
	void operator=(ComputeShader&& o);

	void createProgramFromFile(const char* filePath);
	void createProgramFromCode(const std::string& sourceCode);

	void compute(unsigned int nbComputations);
	inline Shader& get() { return m_shader; }

private:
	Shader m_shader;
};

} // namespace Cool