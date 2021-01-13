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

	/// <summary>
	/// Sends some data (an array of T) from the CPU (your C++ code) to the GPU (your shader code).
	/// </summary>
	/// <param name="nbOfT">Number of elements in the array</param>
	/// <param name="data">Pointer to the beginning of the array</param>
	/// <param name="usage">A hint that allows OpenGL to optimize our buffer. You can see all possible values here : http://docs.gl/gl4/glBufferData</param>
	void uploadData(size_t nbOfT, T* data, GLenum usage = GL_STREAM_READ) {
		GLCall(glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_id));
		GLCall(glBufferData(GL_SHADER_STORAGE_BUFFER, nbOfT * sizeof(T), data, usage));
	}

	/// <summary>
	/// Sends some data from the CPU (your C++ code) to the GPU (your shader code).
	/// </summary>
	/// <param name="v">The vector containing the data.</param>
	/// <param name="usage">A hint that allows OpenGL to optimize our buffer. You can see all possible values here : http://docs.gl/gl4/glBufferData</param>
	void uploadData(std::vector<T>& v, GLenum usage = GL_STREAM_READ) {
		uploadData(v.size(), v.data(), usage);
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