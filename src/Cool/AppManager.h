#pragma once

#include "Cool/Framework/GLWindow.h"
#include "Cool/App.h"

namespace Cool {

class AppManager {
public:
	AppManager();
	~AppManager();

	int run(Cool::App& app);

private:
	std::vector<GLWindow> m_glWindows;
};

} // namespace Cool