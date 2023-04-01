#pragma once

#include <imgui-node-editor/imgui_node_editor.h>

namespace ImNodes = ax::NodeEditor;

namespace Cool::internal {

class UniqueImNodeContext {
public:
    UniqueImNodeContext()
        : _context{ImNodes::CreateEditor(nullptr /*TODO(JF) use a Config*/)}
    {
        // ImNodes::SetCurrentContext(_context);
        // static const bool b                                   = true;
        // ImNodes::GetIO().LinkDetachWithModifierClick.Modifier = &b;
        // ImNodes::GetIO().AltMouseButton                       = ImGuiMouseButton_Right;
        // ImNodes::GetIO().EmulateThreeButtonMouse.Modifier     = &ImGui::GetIO().KeyShift;
    }
    ~UniqueImNodeContext()
    {
        if (_context)
            ImNodes::DestroyEditor(_context);
    }
    UniqueImNodeContext(const UniqueImNodeContext&)            = delete;
    auto operator=(const UniqueImNodeContext&) -> UniqueImNodeContext& = delete;
    UniqueImNodeContext(UniqueImNodeContext&& rhs) noexcept
        : _context{rhs._context}
    {
        rhs._context = nullptr;
    };
    auto operator=(UniqueImNodeContext&& rhs) noexcept -> UniqueImNodeContext&
    {
        if (this != &rhs)
        {
            ImNodes::DestroyEditor(_context);
            _context     = rhs._context;
            rhs._context = nullptr;
        }
        return *this;
    }

    auto operator*() -> ImNodes::EditorContext& { return *_context; }
    auto operator->() -> ImNodes::EditorContext* { return _context; }
    auto operator*() const -> ImNodes::EditorContext const& { return *_context; }
    auto operator->() const -> ImNodes::EditorContext const* { return _context; }

private:
    ImNodes::EditorContext* _context{nullptr};
};

} // namespace Cool::internal