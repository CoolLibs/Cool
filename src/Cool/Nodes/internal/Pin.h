#pragma once

#include <imnodes/imnodes.h>
#include "imnode_id.h"

namespace Cool {

enum class PinKind {
    Input,
    Output
};

using PinId = uuids::uuid;

class Pin {
public:
    PinKind kind() const
    {
        return PinKind::Input;
    }
    void show() const
    {
        ImNodes::BeginInputAttribute(id());
        ImGui::Dummy({0.f, 10.f}); // Makes sure that consecutive input pins are not too close to each other
        ImNodes::EndInputAttribute();
    }

    auto id() const -> const PinId& { return _id; }

private:
    PinId _id;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("id", _id));
    }
};

} // namespace Cool