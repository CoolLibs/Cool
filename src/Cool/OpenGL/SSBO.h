#pragma once

namespace Cool {

template <typename T>
class SSBO {
public:
	SSBO(unsigned int binding, GLenum usageHint)
		: m_binding(binding), m_hint(usageHint)
	{
		GLCall(glGenBuffers(1, &m_id));
		GLCall(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_binding, m_id));
	}
	~SSBO() {
		GLCall(glDeleteBuffers(1, &m_id));
	}

	void uploadData(size_t nbOfT, T* data) {
		GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_id));
		GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, nbOfT * sizeof(T), data, m_hint));
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
	GLuint m_hint;
	unsigned int m_id;
};

} // namespace Cool