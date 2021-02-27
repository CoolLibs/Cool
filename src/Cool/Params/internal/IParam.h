#pragma once

namespace Cool {

template <typename T>
class IParam {
public:
	IParam(std::string_view name, const T& default_value = T(0))
		: _name(name), _value(default_value)
	{}
	virtual bool ImGui() = 0;

	inline T& operator* () { return  _value; }
	inline T* operator->() { return &_value; }
	[[nodiscard]] inline const std::string& name() const { return _name; }

protected:
	T _value;

private:
	std::string _name = "";

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