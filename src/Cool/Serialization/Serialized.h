#pragma once

template <typename T>
class Serialized {
public:
	Serialized(const char* filePath)
		: m_filePath(filePath)
	{
		if (MyFile::Exists(m_filePath)) {
			std::ifstream is(m_filePath);
			{
				try {
					cereal::JSONInputArchive archive(is);
					archive(
						m_base
					);
				}
				catch (std::exception e) {
					spdlog::warn("Corrupted {} file. Starting with default values instead. \n{}", m_filePath, e.what());
				}
			}
		}
		else {
			spdlog::warn("{} not found. Starting with default values instead.", m_filePath);
		}
	}

	~Serialized() {
		//MyFile::CreateFolderIfDoesntExist(m_filePath);
		std::ofstream os(m_filePath);
		{
			cereal::JSONOutputArchive archive(os);
			archive(
				m_base
			);
		}
	}

	inline operator T& () { return m_base; }
	inline operator T*() { return &m_base; }
	inline T* operator ->() { return &m_base; }

private:
	T m_base;
	const char* m_filePath;
};