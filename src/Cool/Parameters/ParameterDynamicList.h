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
    void imgui(std::function<void()> on_value_change = []() {});

#if defined(COOL_OPENGL)
    /**
       * @brief Sets all the uniforms for all the parameters
       * 
       * @param shader 
       */
    void set_uniforms_in_shader(OpenGL::Shader& shader);
#endif

    /**
       * @brief Tries to find the parameter in the list
       * 
       * @param parameter_name The name of the parameter to look for
       * @return The index of the parameter in the list, or -1 if it was not found 
       */
    size_t index_of(std::string_view parameter_name);

    inline std::vector<AnyParameter>& operator*() { return _parameters; }
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
            cereal::make_nvp("Vector", _parameters));
    }
};

} // namespace Cool