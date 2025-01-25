#pragma once
#include "Cool/Serialization/JsonAutoSerializer.hpp"

namespace Cool {

class StyleEditor {
public:
    StyleEditor();

    void imgui();

private:
    JsonAutoSerializer _serializer;
};

inline auto style_editor() -> std::optional<StyleEditor>& // It is optional because we want to control its lifetime (it must be destroyed before destroying the ImGui context to make sure it can still access the ImGuiStyle to serialize it)
{
    static auto instance = std::optional<StyleEditor>{};
    return instance;
}

} // namespace Cool