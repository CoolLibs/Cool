#pragma once

#include "Parameter_Base.h"

namespace Cool::Parameter {

struct Vec2Desc {
    using Value         = glm::vec2;
    using InternalValue = glm::vec2;

    std::string name;
    glm::vec2   default_value = glm::vec2{0.f};
    glm::vec2   min_value     = glm::vec2{0.f};
    glm::vec2   max_value     = glm::vec2{1.f};

    glm::vec2 value(const glm::vec2& rep) const { return rep; }

    bool imgui(glm::vec2& value) const
    {
        ImGui::PushID(this + 34);
        ImGui::PushItemWidth(150);
        bool b = ImGui::SliderFloat("", &value.x, min_value.x, max_value.x);
        ImGui::PopID();
        ImGui::SameLine();
        b |= ImGui::SliderFloat(name.c_str(), &value.y, min_value.y, max_value.y);
        ImGui::PopItemWidth();
        return b;
    }

private:
    //Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Name", name),
                cereal::make_nvp("Default value", default_value),
                cereal::make_nvp("Min value", min_value),
                cereal::make_nvp("Max value", max_value));
    }
};

using Vec2 = Parameter_Base<Vec2Desc>;

} // namespace Cool::Parameter