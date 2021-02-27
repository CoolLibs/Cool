#pragma once

namespace Cool {

class IParam {
public:
	IParam(std::string_view name)
		: _name(name)
	{}
	virtual bool ImGui() = 0;

	[[nodiscard]] inline const std::string& name() const { return _name; }

private:
	std::string _name = "";
};

} // namespace Cool