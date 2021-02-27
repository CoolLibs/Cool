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

template <typename T>
class Param : public IParam {
public:
	Param(std::string_view name, const T& default_value = T(0))
		: IParam(name), _value(default_value)
	{}
	inline T& operator* () { return  _value; }
	inline T* operator->() { return &_value; }

protected:
	T _value;

private:
	//Serialization
	friend class cereal::access;
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(
			cereal::make_nvp(name(), _value)
		);
	}
};

} // namespace Cool