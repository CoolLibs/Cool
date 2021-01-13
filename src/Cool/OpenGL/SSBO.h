#pragma once

namespace Cool {

template <typename T>
/// <summary>
/// A SSBO (Shader Storage Buffer Object) is a buffer that lives on the GPU.
/// </summary>
/// <typeparam name="T"></typeparam>
class SSBO {
public:
	SSBO(unsigned int binding)
		: m_binding(binding)
	{
		GLCall(glGenBuffers(1, &m_id));
		GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_binding, m_id));
	}
	~SSBO() {
		GLCall(glDeleteBuffers(1, &m_id));
	}

	void uploadData(size_t nbOfT, T* data, GLenum usage = GL_STREAM_READ) {
		GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_id));
		GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, nbOfT * sizeof(T), data, usage));
	}

	void uploadData(std::vector<T>& v) {
		uploadData(v.size(), v.data());
	}

	void downloadData(size_t nbOfT, T* data) {
		GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_id));
		GLCall(GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY));
		memcpy(data, p, nbOfT * sizeof(T));
		GLCall(glUnmapBuffer(GL_SHADER_STORAGE_BUFFER));
	}

	void downloadData(std::vector<T>& v) {
		downloadData(v.size(), v.data());
	}

private:
	unsigned int m_binding;
	unsigned int m_id;
};

} // namespace Cool