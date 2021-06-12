#pragma once

#include "AnyParameter.h"

namespace Cool {

/**
 * @brief Wrapper around a std::vector<AnyParameter>
 * 
 */
class ParameterDynamicList {
public:
      /**
       * @brief Displays all the imgui widgets of all the parameters
       * 
       * @param on_value_change Callback that is triggered whenever one of the parameters changes
       */
      void imgui(std::function<void()> on_value_change = [](){});

      inline std::vector<AnyParameter>& operator* () { return _parameters; }
      inline std::vector<AnyParameter>* operator->() { return &operator*(); }

private:
      std::vector<AnyParameter> _parameters;

private:
      // Serialization
	friend class cereal::access;
	template<class Archive>
	void serialize(Archive& archive)
	{
		archive(
			cereal::make_nvp("Vector", _parameters)
		);
	}
};

} // namespace Cool