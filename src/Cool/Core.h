#pragma once
namespace Cool {

	class Core {
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="spdlogPattern">: Sets the log messages pattern ; see here to configure it : https://github.com/gabime/spdlog/wiki/3.-Custom-formatting#pattern-flags</param>
		static void Initialize(const char* spdlogPattern = "%^[%e] %n: %v%$");
	};

}