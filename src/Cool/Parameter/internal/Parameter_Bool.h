#pragma once

#include "Parameter_Base.h"

namespace Cool::Parameter {

struct BoolDesc {
    using Value         = bool;
    using InternalValue = bool;

    std::string name;
    bool        default_value = 0;

    bool value(bool rep) const { return rep; }

    bool imgui(bool& value)
    {
        bool b = ImGui::Checkbox(name.c_str(), &value);
        return b;
    }

private:
    //Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Name", name),
                cereal::make_nvp("Default", default_value));
    }
};

using Bool = Parameter_Base<BoolDesc>;

} // namespace Cool::Parameter