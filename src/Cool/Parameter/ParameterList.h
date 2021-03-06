#pragma once

#include "Parameter.h"

namespace Cool {

/**
 * @brief Wrapper around a std::vector<Parameter::Any>
 *
 */
class ParameterList {
public:
    ParameterList(const std::vector<Parameter::AnyDesc>& descriptions = {});

    /**
     * @brief Displays all the imgui widgets of all the parameters
     *
     * @param on_value_change Callback that is triggered whenever one of the parameters changes
     */
    void imgui(std::function<void()> on_value_change = []() {});

    std::vector<Parameter::Any>&       operator*() { return _parameters; }
    std::vector<Parameter::Any>*       operator->() { return &operator*(); }
    const std::vector<Parameter::Any>& operator*() const { return _parameters; }
    const std::vector<Parameter::Any>* operator->() const { return &operator*(); }

private:
    std::vector<Parameter::Any> _parameters;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("List", _parameters));
    }
};

} // namespace Cool