#include "imgui-node-editor/imgui_node_editor.h"
#include "utilities/builders.h"
#include "utilities/widgets.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_internal.h>
#include <algorithm>
#include <map>
#include <string>
#include <utility>
#include <vector>

static inline ImRect ImGui_GetItemRect()
{
    return ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
}

static inline ImRect ImRect_Expanded(const ImRect& rect, float x, float y)
{
    auto result = rect;
    result.Min.x -= x;
    result.Min.y -= y;
    result.Max.x += x;
    result.Max.y += y;
    return result;
}

namespace ed   = ax::NodeEditor;
namespace util = ax::NodeEditor::Utilities;

using namespace ax;

using ax::Widgets::IconType;

static ed::EditorContext* m_Editor = nullptr;

// extern "C" __declspec(dllimport) short __stdcall GetAsyncKeyState(int vkey);
// extern "C" bool Debug_KeyPress(int vkey)
//{
//     static std::map<int, bool> state;
//     auto lastState = state[vkey];
//     state[vkey] = (GetAsyncKeyState(vkey) & 0x8000) != 0;
//     if (state[vkey] && !lastState)
//         return true;
//     else
//         return false;
// }

enum class PinType {
    Flow,
    Bool,
    Int,
    Float,
    String,
    Object,
    Function,
    Delegate,
};

enum class PinKind {
    Output,
    Input
};

enum class NodeType {
    Blueprint,
    Comment,
};

struct Node;

struct Pin {
    ed::PinId   ID;
    ::Node*     Node;
    std::string Name;
    PinType     Type;
    PinKind     Kind;

    Pin(int id, const char* name, PinType type)
        : ID(id), Node(nullptr), Name(name), Type(type), Kind(PinKind::Input)
    {
    }
};

struct Node {
    ed::NodeId       ID;
    std::string      Name;
    std::vector<Pin> Inputs;
    std::vector<Pin> Outputs;
    ImColor          Color;
    NodeType         Type;
    ImVec2           Size;

    std::string State;
    std::string SavedState;

    Node(int id, const char* name, ImColor color = ImColor(255, 255, 255))
        : ID(id), Name(name), Color(color), Type(NodeType::Blueprint), Size(0, 0)
    {
    }
};

struct Link {
    ed::LinkId ID;

    ed::PinId StartPinID;
    ed::PinId EndPinID;

    ImColor Color;

    Link(ed::LinkId id, ed::PinId startPinId, ed::PinId endPinId)
        : ID(id), StartPinID(startPinId), EndPinID(endPinId), Color(255, 255, 255)
    {
    }
};

struct NodeIdLess {
    bool operator()(const ed::NodeId& lhs, const ed::NodeId& rhs) const
    {
        return lhs.AsPointer() < rhs.AsPointer();
    }
};

static bool Splitter(bool split_vertically, float thickness, float* size1, float* size2, float min_size1, float min_size2, float splitter_long_axis_size = -1.0f)
{
    using namespace ImGui;
    ImGuiContext& g      = *GImGui;
    ImGuiWindow*  window = g.CurrentWindow;
    ImGuiID       id     = window->GetID("##Splitter");
    ImRect        bb;
    bb.Min = window->DC.CursorPos + (split_vertically ? ImVec2(*size1, 0.0f) : ImVec2(0.0f, *size1));
    bb.Max = bb.Min + CalcItemSize(split_vertically ? ImVec2(thickness, splitter_long_axis_size) : ImVec2(splitter_long_axis_size, thickness), 0.0f, 0.0f);
    return SplitterBehavior(bb, id, split_vertically ? ImGuiAxis_X : ImGuiAxis_Y, size1, size2, min_size1, min_size2, 0.0f);
}

struct Example {
    int GetNextId()
    {
        return m_NextId++;
    }

    // ed::NodeId GetNextNodeId()
    //{
    //     return ed::NodeId(GetNextId());
    // }

    ed::LinkId GetNextLinkId()
    {
        return ed::LinkId(GetNextId());
    }

    Node* FindNode(ed::NodeId id)
    {
        for (auto& node : m_Nodes)
            if (node.ID == id)
                return &node;

        return nullptr;
    }

    Link* FindLink(ed::LinkId id)
    {
        for (auto& link : m_Links)
            if (link.ID == id)
                return &link;

        return nullptr;
    }

    Pin* FindPin(ed::PinId id)
    {
        if (!id)
            return nullptr;

        for (auto& node : m_Nodes)
        {
            for (auto& pin : node.Inputs)
                if (pin.ID == id)
                    return &pin;

            for (auto& pin : node.Outputs)
                if (pin.ID == id)
                    return &pin;
        }

        return nullptr;
    }

    bool IsPinLinked(ed::PinId id)
    {
        if (!id)
            return false;

        for (auto& link : m_Links)
            if (link.StartPinID == id || link.EndPinID == id)
                return true;

        return false;
    }

    bool CanCreateLink(Pin* a, Pin* b)
    {
        if (!a || !b || a == b || a->Kind == b->Kind || a->Type != b->Type || a->Node == b->Node)
            return false;

        return true;
    }

    void BuildNode(Node* node)
    {
        for (auto& input : node->Inputs)
        {
            input.Node = node;
            input.Kind = PinKind::Input;
        }

        for (auto& output : node->Outputs)
        {
            output.Node = node;
            output.Kind = PinKind::Output;
        }
    }

    Node* SpawnInputActionNode()
    {
        m_Nodes.emplace_back(GetNextId(), "InputAction Fire", ImColor(255, 128, 128));
        m_Nodes.back().Outputs.emplace_back(GetNextId(), "", PinType::Delegate);
        m_Nodes.back().Outputs.emplace_back(GetNextId(), "Pressed", PinType::Flow);
        m_Nodes.back().Outputs.emplace_back(GetNextId(), "Released", PinType::Flow);

        BuildNode(&m_Nodes.back());

        return &m_Nodes.back();
    }

    Node* SpawnBranchNode()
    {
        m_Nodes.emplace_back(GetNextId(), "Branch");
        m_Nodes.back().Inputs.emplace_back(GetNextId(), "", PinType::Flow);
        m_Nodes.back().Inputs.emplace_back(GetNextId(), "Condition", PinType::Bool);
        m_Nodes.back().Outputs.emplace_back(GetNextId(), "True", PinType::Flow);
        m_Nodes.back().Outputs.emplace_back(GetNextId(), "False", PinType::Flow);

        BuildNode(&m_Nodes.back());

        return &m_Nodes.back();
    }

    Node* SpawnDoNNode()
    {
        m_Nodes.emplace_back(GetNextId(), "Do N");
        m_Nodes.back().Inputs.emplace_back(GetNextId(), "Enter", PinType::Flow);
        m_Nodes.back().Inputs.emplace_back(GetNextId(), "N", PinType::Int);
        m_Nodes.back().Inputs.emplace_back(GetNextId(), "Reset", PinType::Flow);
        m_Nodes.back().Outputs.emplace_back(GetNextId(), "Exit", PinType::Flow);
        m_Nodes.back().Outputs.emplace_back(GetNextId(), "Counter", PinType::Int);

        BuildNode(&m_Nodes.back());

        return &m_Nodes.back();
    }

    Node* SpawnOutputActionNode()
    {
        m_Nodes.emplace_back(GetNextId(), "OutputAction");
        m_Nodes.back().Inputs.emplace_back(GetNextId(), "Sample", PinType::Float);
        m_Nodes.back().Outputs.emplace_back(GetNextId(), "Condition", PinType::Bool);
        m_Nodes.back().Inputs.emplace_back(GetNextId(), "Event", PinType::Delegate);

        BuildNode(&m_Nodes.back());

        return &m_Nodes.back();
    }

    Node* SpawnPrintStringNode()
    {
        m_Nodes.emplace_back(GetNextId(), "Print String");
        m_Nodes.back().Inputs.emplace_back(GetNextId(), "", PinType::Flow);
        m_Nodes.back().Inputs.emplace_back(GetNextId(), "In String", PinType::String);
        m_Nodes.back().Outputs.emplace_back(GetNextId(), "", PinType::Flow);

        BuildNode(&m_Nodes.back());

        return &m_Nodes.back();
    }

    Node* SpawnComment()
    {
        m_Nodes.emplace_back(GetNextId(), "Test Comment");
        m_Nodes.back().Type = NodeType::Comment;
        m_Nodes.back().Size = ImVec2(300, 200);

        return &m_Nodes.back();
    }

    void BuildNodes()
    {
        for (auto& node : m_Nodes)
            BuildNode(&node);
    }

    void OnStart()
    {
        ed::Config config;

        config.SettingsFile = "Blueprints.json";

        config.UserPointer = this;

        config.LoadNodeSettings = [](ed::NodeId nodeId, char* data, void* userPointer) -> size_t {
            auto self = static_cast<Example*>(userPointer);

            auto node = self->FindNode(nodeId);
            if (!node)
                return 0;

            if (data != nullptr)
                memcpy(data, node->State.data(), node->State.size());
            return node->State.size();
        };

        config.SaveNodeSettings = [](ed::NodeId nodeId, const char* data, size_t size, ed::SaveReasonFlags reason, void* userPointer) -> bool {
            auto self = static_cast<Example*>(userPointer);

            auto node = self->FindNode(nodeId);
            if (!node)
                return false;

            node->State.assign(data, size);


            return true;
        };

        m_Editor = ed::CreateEditor(&config);
        ed::SetCurrentEditor(m_Editor);

        Node* node;
        node = SpawnInputActionNode();
        ed::SetNodePosition(node->ID, ImVec2(-252, 220));
        node = SpawnBranchNode();
        ed::SetNodePosition(node->ID, ImVec2(-300, 351));
        node = SpawnDoNNode();
        ed::SetNodePosition(node->ID, ImVec2(-238, 504));
        node = SpawnOutputActionNode();
        ed::SetNodePosition(node->ID, ImVec2(71, 80));

        node = SpawnComment();
        ed::SetNodePosition(node->ID, ImVec2(112, 576));
        ed::SetGroupSize(node->ID, ImVec2(384, 154));
        node = SpawnComment();
        ed::SetNodePosition(node->ID, ImVec2(800, 224));
        ed::SetGroupSize(node->ID, ImVec2(640, 400));

        ed::NavigateToContent();

        BuildNodes();

        // m_HeaderBackground = LoadTexture("data/BlueprintBackground.png");
        // m_SaveIcon         = LoadTexture("data/ic_save_white_24dp.png");
        // m_RestoreIcon      = LoadTexture("data/ic_restore_white_24dp.png");

        // auto& io = ImGui::GetIO();
    }

    void OnStop()
    {
        // auto releaseTexture = [this](ImTextureID& id) {
        //     if (id)
        //     {
        //         DestroyTexture(id);
        //         id = nullptr;
        //     }
        // };

        // releaseTexture(m_RestoreIcon);
        // releaseTexture(m_SaveIcon);
        // releaseTexture(m_HeaderBackground);

        if (m_Editor)
        {
            ed::DestroyEditor(m_Editor);
            m_Editor = nullptr;
        }
    }

    ImColor GetIconColor(PinType type)
    {
        switch (type)
        {
        default:
        case PinType::Flow: return ImColor(255, 255, 255);
        case PinType::Bool: return ImColor(220, 48, 48);
        case PinType::Int: return ImColor(68, 201, 156);
        case PinType::Float: return ImColor(147, 226, 74);
        case PinType::String: return ImColor(124, 21, 153);
        case PinType::Object: return ImColor(51, 150, 215);
        case PinType::Function: return ImColor(218, 0, 183);
        case PinType::Delegate: return ImColor(255, 48, 48);
        }
    };

    void DrawPinIcon(const Pin& pin, bool connected, int alpha)
    {
        IconType iconType;
        ImColor  color = GetIconColor(pin.Type);
        color.Value.w  = alpha / 255.0f;
        switch (pin.Type)
        {
        case PinType::Flow: iconType = IconType::Flow; break;
        case PinType::Bool: iconType = IconType::Circle; break;
        case PinType::Int: iconType = IconType::Circle; break;
        case PinType::Float: iconType = IconType::Circle; break;
        case PinType::String: iconType = IconType::Circle; break;
        case PinType::Object: iconType = IconType::Circle; break;
        case PinType::Function: iconType = IconType::Circle; break;
        case PinType::Delegate: iconType = IconType::Square; break;
        default:
            return;
        }

        ax::Widgets::Icon(ImVec2(static_cast<float>(m_PinIconSize), static_cast<float>(m_PinIconSize)), iconType, connected, color, ImColor(32, 32, 32, alpha));
    };

    void ShowStyleEditor(bool* show = nullptr)
    {
        if (!ImGui::Begin("Style", show))
        {
            ImGui::End();
            return;
        }

        auto paneWidth = ImGui::GetContentRegionAvail().x;

        auto& editorStyle = ed::GetStyle();
        ImGui::BeginHorizontal("Style buttons", ImVec2(paneWidth, 0), 1.0f);
        ImGui::TextUnformatted("Values");
        ImGui::Spring();
        if (ImGui::Button("Reset to defaults"))
            editorStyle = ed::Style();
        ImGui::EndHorizontal();
        ImGui::Spacing();
        ImGui::DragFloat4("Node Padding", &editorStyle.NodePadding.x, 0.1f, 0.0f, 40.0f);
        ImGui::DragFloat("Node Rounding", &editorStyle.NodeRounding, 0.1f, 0.0f, 40.0f);
        ImGui::DragFloat("Node Border Width", &editorStyle.NodeBorderWidth, 0.1f, 0.0f, 15.0f);
        ImGui::DragFloat("Hovered Node Border Width", &editorStyle.HoveredNodeBorderWidth, 0.1f, 0.0f, 15.0f);
        ImGui::DragFloat("Selected Node Border Width", &editorStyle.SelectedNodeBorderWidth, 0.1f, 0.0f, 15.0f);
        ImGui::DragFloat("Pin Rounding", &editorStyle.PinRounding, 0.1f, 0.0f, 40.0f);
        ImGui::DragFloat("Pin Border Width", &editorStyle.PinBorderWidth, 0.1f, 0.0f, 15.0f);
        ImGui::DragFloat("Link Strength", &editorStyle.LinkStrength, 1.0f, 0.0f, 500.0f);
        // ImVec2  SourceDirection;
        // ImVec2  TargetDirection;
        ImGui::DragFloat("Scroll Duration", &editorStyle.ScrollDuration, 0.001f, 0.0f, 2.0f);
        ImGui::DragFloat("Flow Marker Distance", &editorStyle.FlowMarkerDistance, 1.0f, 1.0f, 200.0f);
        ImGui::DragFloat("Flow Speed", &editorStyle.FlowSpeed, 1.0f, 1.0f, 2000.0f);
        ImGui::DragFloat("Flow Duration", &editorStyle.FlowDuration, 0.001f, 0.0f, 5.0f);
        // ImVec2  PivotAlignment;
        // ImVec2  PivotSize;
        // ImVec2  PivotScale;
        // float   PinCorners;
        // float   PinRadius;
        // float   PinArrowSize;
        // float   PinArrowWidth;
        ImGui::DragFloat("Group Rounding", &editorStyle.GroupRounding, 0.1f, 0.0f, 40.0f);
        ImGui::DragFloat("Group Border Width", &editorStyle.GroupBorderWidth, 0.1f, 0.0f, 15.0f);

        ImGui::Separator();

        static ImGuiColorEditFlags edit_mode = ImGuiColorEditFlags_DisplayRGB;
        ImGui::BeginHorizontal("Color Mode", ImVec2(paneWidth, 0), 1.0f);
        ImGui::TextUnformatted("Filter Colors");
        ImGui::Spring();
        ImGui::RadioButton("RGB", &edit_mode, ImGuiColorEditFlags_DisplayRGB);
        ImGui::Spring(0);
        ImGui::RadioButton("HSV", &edit_mode, ImGuiColorEditFlags_DisplayHSV);
        ImGui::Spring(0);
        ImGui::RadioButton("HEX", &edit_mode, ImGuiColorEditFlags_DisplayHex);
        ImGui::EndHorizontal();

        static ImGuiTextFilter filter;
        filter.Draw("", paneWidth);

        ImGui::Spacing();

        ImGui::PushItemWidth(-160);
        for (int i = 0; i < ed::StyleColor_Count; ++i)
        {
            auto name = ed::GetStyleColorName((ed::StyleColor)i);
            if (!filter.PassFilter(name))
                continue;

            ImGui::ColorEdit4(name, &editorStyle.Colors[i].x, edit_mode);
        }
        ImGui::PopItemWidth();

        ImGui::End();
    }

    void render_editor()
    {
        auto cursorTopLeft = ImGui::GetCursorScreenPos();

        util::BlueprintNodeBuilder builder{};

        for (auto& node : m_Nodes)
        {
            if (node.Type != NodeType::Blueprint)
                continue;

            bool hasOutputDelegates = false;
            for (auto& output : node.Outputs)
                if (output.Type == PinType::Delegate)
                    hasOutputDelegates = true;

            builder.Begin(node.ID);

            builder.Header(node.Color);
            ImGui::Spring(0);
            ImGui::TextUnformatted(node.Name.c_str());
            ImGui::Spring(1);
            ImGui::Dummy(ImVec2(0, 28));
            if (hasOutputDelegates)
            {
                ImGui::BeginVertical("delegates", ImVec2(0, 28));
                ImGui::Spring(1, 0);
                for (auto& output : node.Outputs)
                {
                    if (output.Type != PinType::Delegate)
                        continue;

                    auto alpha = ImGui::GetStyle().Alpha;
                    if (newLinkPin && !CanCreateLink(newLinkPin, &output) && &output != newLinkPin)
                        alpha = alpha * (48.0f / 255.0f);

                    ed::BeginPin(output.ID, ed::PinKind::Output);
                    ed::PinPivotAlignment(ImVec2(1.0f, 0.5f));
                    ed::PinPivotSize(ImVec2(0, 0));
                    ImGui::BeginHorizontal(output.ID.AsPointer());
                    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
                    if (!output.Name.empty())
                    {
                        ImGui::TextUnformatted(output.Name.c_str());
                        ImGui::Spring(0);
                    }
                    DrawPinIcon(output, IsPinLinked(output.ID), (int)(alpha * 255));
                    ImGui::Spring(0, ImGui::GetStyle().ItemSpacing.x / 2);
                    ImGui::EndHorizontal();
                    ImGui::PopStyleVar();
                    ed::EndPin();

                    // DrawItemRect(ImColor(255, 0, 0));
                }
                ImGui::Spring(1, 0);
                ImGui::EndVertical();
                ImGui::Spring(0, ImGui::GetStyle().ItemSpacing.x / 2);
            }
            else
                ImGui::Spring(0);
            builder.EndHeader();

            for (auto& input : node.Inputs)
            {
                auto alpha = ImGui::GetStyle().Alpha;
                if (newLinkPin && !CanCreateLink(newLinkPin, &input) && &input != newLinkPin)
                    alpha = alpha * (48.0f / 255.0f);

                builder.Input(input.ID);
                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
                DrawPinIcon(input, IsPinLinked(input.ID), (int)(alpha * 255));
                ImGui::Spring(0);
                if (!input.Name.empty())
                {
                    ImGui::TextUnformatted(input.Name.c_str());
                    ImGui::Spring(0);
                }
                if (input.Type == PinType::Bool)
                {
                    ImGui::Button("Hello");
                    ImGui::Spring(0);
                }
                ImGui::PopStyleVar();
                builder.EndInput();
            }

            for (auto& output : node.Outputs)
            {
                if (output.Type == PinType::Delegate)
                    continue;

                auto alpha = ImGui::GetStyle().Alpha;
                if (newLinkPin && !CanCreateLink(newLinkPin, &output) && &output != newLinkPin)
                    alpha = alpha * (48.0f / 255.0f);

                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, alpha);
                builder.Output(output.ID);
                if (output.Type == PinType::String)
                {
                    static char buffer[128] = "Edit Me\nMultiline!";
                    static bool wasActive   = false;

                    ImGui::PushItemWidth(100.0f);
                    ImGui::InputText("##edit", buffer, 127);
                    ImGui::PopItemWidth();
                    if (ImGui::IsItemActive() && !wasActive)
                    {
                        ed::EnableShortcuts(false);
                        wasActive = true;
                    }
                    else if (!ImGui::IsItemActive() && wasActive)
                    {
                        ed::EnableShortcuts(true);
                        wasActive = false;
                    }
                    ImGui::Spring(0);
                }
                if (!output.Name.empty())
                {
                    ImGui::Spring(0);
                    ImGui::TextUnformatted(output.Name.c_str());
                }
                ImGui::Spring(0);
                DrawPinIcon(output, IsPinLinked(output.ID), (int)(alpha * 255));
                ImGui::PopStyleVar();
                builder.EndOutput();
            }

            builder.End();
        }

        for (auto& node : m_Nodes)
        {
            if (node.Type != NodeType::Comment)
                continue;

            const float commentAlpha = 0.75f;

            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, commentAlpha);
            ed::PushStyleColor(ed::StyleColor_NodeBg, ImColor(255, 255, 255, 64));
            ed::PushStyleColor(ed::StyleColor_NodeBorder, ImColor(255, 255, 255, 64));
            ed::BeginNode(node.ID);
            ImGui::PushID(node.ID.AsPointer());
            ImGui::BeginVertical("content");
            ImGui::BeginHorizontal("horizontal");
            ImGui::Spring(1);
            ImGui::TextUnformatted(node.Name.c_str());
            ImGui::Spring(1);
            ImGui::EndHorizontal();
            ed::Group(node.Size);
            ImGui::EndVertical();
            ImGui::PopID();
            ed::EndNode();
            ed::PopStyleColor(2);
            ImGui::PopStyleVar();

            if (ed::BeginGroupHint(node.ID))
            {
                // auto alpha   = static_cast<int>(commentAlpha * ImGui::GetStyle().Alpha * 255);
                auto bgAlpha = static_cast<int>(ImGui::GetStyle().Alpha * 255);

                // ImGui::PushStyleVar(ImGuiStyleVar_Alpha, commentAlpha * ImGui::GetStyle().Alpha);

                auto min = ed::GetGroupMin();
                // auto max = ed::GetGroupMax();

                ImGui::SetCursorScreenPos(min - ImVec2(-8, ImGui::GetTextLineHeightWithSpacing() + 4));
                ImGui::BeginGroup();
                ImGui::TextUnformatted(node.Name.c_str());
                ImGui::EndGroup();

                auto drawList = ed::GetHintBackgroundDrawList();

                auto hintBounds      = ImGui_GetItemRect();
                auto hintFrameBounds = ImRect_Expanded(hintBounds, 8, 4);

                drawList->AddRectFilled(
                    hintFrameBounds.GetTL(),
                    hintFrameBounds.GetBR(),
                    IM_COL32(255, 255, 255, 64 * bgAlpha / 255), 4.0f
                );

                drawList->AddRect(
                    hintFrameBounds.GetTL(),
                    hintFrameBounds.GetBR(),
                    IM_COL32(255, 255, 255, 128 * bgAlpha / 255), 4.0f
                );

                // ImGui::PopStyleVar();
            }
            ed::EndGroupHint();
        }

        for (auto& link : m_Links)
            ed::Link(link.ID, link.StartPinID, link.EndPinID, link.Color, 2.0f);

        if (!createNewNode)
        {
            if (ed::BeginCreate(ImColor(255, 255, 255), 2.0f))
            {
                auto showLabel = [](const char* label, ImColor color) {
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - ImGui::GetTextLineHeight());
                    auto size = ImGui::CalcTextSize(label);

                    auto padding = ImGui::GetStyle().FramePadding;
                    auto spacing = ImGui::GetStyle().ItemSpacing;

                    ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(spacing.x, -spacing.y));

                    auto rectMin = ImGui::GetCursorScreenPos() - padding;
                    auto rectMax = ImGui::GetCursorScreenPos() + size + padding;

                    auto drawList = ImGui::GetWindowDrawList();
                    drawList->AddRectFilled(rectMin, rectMax, color, size.y * 0.15f);
                    ImGui::TextUnformatted(label);
                };

                ed::PinId startPinId = 0, endPinId = 0;
                if (ed::QueryNewLink(&startPinId, &endPinId))
                {
                    auto startPin = FindPin(startPinId);
                    auto endPin   = FindPin(endPinId);

                    newLinkPin = startPin ? startPin : endPin;

                    if (startPin->Kind == PinKind::Input)
                    {
                        std::swap(startPin, endPin);
                        std::swap(startPinId, endPinId);
                    }

                    if (startPin && endPin)
                    {
                        if (endPin == startPin)
                        {
                            ed::RejectNewItem(ImColor(255, 0, 0), 2.0f);
                        }
                        else if (endPin->Kind == startPin->Kind)
                        {
                            showLabel("x Incompatible Pin Kind", ImColor(45, 32, 32, 180));
                            ed::RejectNewItem(ImColor(255, 0, 0), 2.0f);
                        }
                        // else if (endPin->Node == startPin->Node)
                        //{
                        //     showLabel("x Cannot connect to self", ImColor(45, 32, 32, 180));
                        //     ed::RejectNewItem(ImColor(255, 0, 0), 1.0f);
                        // }
                        else if (endPin->Type != startPin->Type)
                        {
                            showLabel("x Incompatible Pin Type", ImColor(45, 32, 32, 180));
                            ed::RejectNewItem(ImColor(255, 128, 128), 1.0f);
                        }
                        else
                        {
                            showLabel("+ Create Link", ImColor(32, 45, 32, 180));
                            if (ed::AcceptNewItem(ImColor(128, 255, 128), 4.0f))
                            {
                                m_Links.emplace_back(Link(GetNextId(), startPinId, endPinId));
                                m_Links.back().Color = GetIconColor(startPin->Type);
                            }
                        }
                    }
                }

                ed::PinId pinId = 0;
                if (ed::QueryNewNode(&pinId))
                {
                    newLinkPin = FindPin(pinId);
                    if (newLinkPin)
                        showLabel("+ Create Node", ImColor(32, 45, 32, 180));

                    if (ed::AcceptNewItem())
                    {
                        createNewNode  = true;
                        newNodeLinkPin = FindPin(pinId);
                        newLinkPin     = nullptr;
                        ed::Suspend();
                        ImGui::OpenPopup("Create New Node");
                        ed::Resume();
                    }
                }
            }
            else
                newLinkPin = nullptr;

            ed::EndCreate();

            if (ed::BeginDelete())
            {
                ed::LinkId linkId = 0;
                while (ed::QueryDeletedLink(&linkId))
                {
                    if (ed::AcceptDeletedItem())
                    {
                        auto id = std::find_if(m_Links.begin(), m_Links.end(), [linkId](auto& link) { return link.ID == linkId; });
                        if (id != m_Links.end())
                            m_Links.erase(id);
                    }
                }

                ed::NodeId nodeId = 0;
                while (ed::QueryDeletedNode(&nodeId))
                {
                    if (ed::AcceptDeletedItem())
                    {
                        auto id = std::find_if(m_Nodes.begin(), m_Nodes.end(), [nodeId](auto& node) { return node.ID == nodeId; });
                        if (id != m_Nodes.end())
                            m_Nodes.erase(id);
                    }
                }
            }
            ed::EndDelete();
        }

        ImGui::SetCursorScreenPos(cursorTopLeft);
    }

    void OnFrame()
    {
            ed::SetCurrentEditor(m_Editor);

            Splitter(true, 0.0f, &leftPaneWidth, &rightPaneWidth, 0.0f, 0.0f); // TODO(JF) Remove this. (But atm when removing it the view gets clipped when zooming)

            ed::Begin("Node editor");
            {
                render_editor();
            }

            auto openPopupPosition = ImGui::GetMousePos();
            ed::Suspend();
            if (ed::ShowBackgroundContextMenu())
            {
                ImGui::OpenPopup("Create New Node");
                newNodeLinkPin = nullptr;
            }
            ed::Resume();

            ed::Suspend();
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));

            if (ImGui::BeginPopup("Create New Node"))
            {
                auto newNodePostion = openPopupPosition;
                // ImGui::SetCursorScreenPos(ImGui::GetMousePosOnOpeningCurrentPopup());

                // auto drawList = ImGui::GetWindowDrawList();
                // drawList->AddCircleFilled(ImGui::GetMousePosOnOpeningCurrentPopup(), 10.0f, 0xFFFF00FF);

                Node* node = nullptr;
                if (ImGui::MenuItem("Input Action"))
                    node = SpawnInputActionNode();
                if (ImGui::MenuItem("Output Action"))
                    node = SpawnOutputActionNode();
                if (ImGui::MenuItem("Branch"))
                    node = SpawnBranchNode();
                if (ImGui::MenuItem("Do N"))
                    node = SpawnDoNNode();
                ImGui::Separator();
                if (ImGui::MenuItem("Comment"))
                    node = SpawnComment();
                ImGui::Separator();

                if (node)
                {
                    BuildNodes();

                    createNewNode = false;

                    ed::SetNodePosition(node->ID, newNodePostion);

                    if (auto startPin = newNodeLinkPin)
                    {
                        auto& pins = startPin->Kind == PinKind::Input ? node->Outputs : node->Inputs;

                        for (auto& pin : pins)
                        {
                            if (CanCreateLink(startPin, &pin))
                            {
                                auto endPin = &pin;
                                if (startPin->Kind == PinKind::Input)
                                    std::swap(startPin, endPin);

                                m_Links.emplace_back(Link(GetNextId(), startPin->ID, endPin->ID));
                                m_Links.back().Color = GetIconColor(startPin->Type);

                                break;
                            }
                        }
                    }
                }

                ImGui::EndPopup();
            }
            else
                createNewNode = false;
            ImGui::PopStyleVar();
            ed::Resume();

            ed::End();

            auto editorMin = ImGui::GetItemRectMin();
            auto editorMax = ImGui::GetItemRectMax();

            if (m_ShowOrdinals)
            {
                int                     nodeCount = ed::GetNodeCount();
                std::vector<ed::NodeId> orderedNodeIds;
                orderedNodeIds.resize(static_cast<size_t>(nodeCount));
                ed::GetOrderedNodeIds(orderedNodeIds.data(), nodeCount);

                auto drawList = ImGui::GetWindowDrawList();
                drawList->PushClipRect(editorMin, editorMax);

                int ordinal = 0;
                for (auto& nodeId : orderedNodeIds)
                {
                    auto p0 = ed::GetNodePosition(nodeId);
                    auto p1 = p0 + ed::GetNodeSize(nodeId);
                    p0      = ed::CanvasToScreen(p0);
                    p1      = ed::CanvasToScreen(p1);

                    ImGuiTextBuffer builder;
                    builder.appendf("#%d", ordinal++);

                    auto textSize   = ImGui::CalcTextSize(builder.c_str());
                    auto padding    = ImVec2(2.0f, 2.0f);
                    auto widgetSize = textSize + padding * 2;

                    auto widgetPosition = ImVec2(p1.x, p0.y) + ImVec2(0.0f, -widgetSize.y);

                    drawList->AddRectFilled(widgetPosition, widgetPosition + widgetSize, IM_COL32(100, 80, 80, 190), 3.0f, ImDrawFlags_RoundCornersAll);
                    drawList->AddRect(widgetPosition, widgetPosition + widgetSize, IM_COL32(200, 160, 160, 190), 3.0f, ImDrawFlags_RoundCornersAll);
                    drawList->AddText(widgetPosition + padding, IM_COL32(255, 255, 255, 255), builder.c_str());
                }

                drawList->PopClipRect();
            }
    }

    int                                     m_NextId      = 1;
    const int                               m_PinIconSize = 24;
    std::vector<Node>                       m_Nodes;
    std::vector<Link>                       m_Links;
    ImTextureID                             m_HeaderBackground = nullptr;
    ImTextureID                             m_SaveIcon         = nullptr;
    ImTextureID                             m_RestoreIcon      = nullptr;
    const float                             m_TouchTime        = 1.0f;
    std::map<ed::NodeId, float, NodeIdLess> m_NodeTouchTime;
    bool                                    m_ShowOrdinals = false;


    
             ed::NodeId contextNodeId  = 0;
             ed::LinkId contextLinkId  = 0;
             ed::PinId  contextPinId   = 0;
             bool       createNewNode  = false;
             Pin*       newNodeLinkPin = nullptr;
             Pin*       newLinkPin     = nullptr;

             float leftPaneWidth  = 000.0f;
             float rightPaneWidth = 800.0f;
};

void blueprints_example()
{
    static Example example;
    static bool    b = true;
    if (b)
    {
        example.OnStart();
        b = false;
    }
    example.OnFrame();
}