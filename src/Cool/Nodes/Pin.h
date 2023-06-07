#pragma once

#include <reg/cereal.hpp>
#include <reg/reg.hpp>
#include "Cool/Nodes/utilities/drawing.h"
#include "utilities/drawing.h"

namespace Cool {

using PinId = reg::AnyId;

enum class PinKind {
    Input,
    Output,
};

class Pin {
public:
    Pin() = default;
    explicit Pin(std::string_view name, ax::Drawing::IconType = ax::Drawing::IconType::Circle);
    Pin(Pin const&)                    = default;
    Pin(Pin&&)                         = default;
    auto operator=(Pin const&) -> Pin& = default;
    auto operator=(Pin&&) -> Pin&      = default;
    virtual ~Pin()                     = default;

    [[nodiscard]] auto name() const -> std::string const& { return _name; }
    [[nodiscard]] auto icon() const -> ax::Drawing::IconType { return _icon; }
    [[nodiscard]] auto id() const -> PinId const& { return _id; }

    void set_id(PinId const& id) { _id = id; }
    void set_icon(ax::Drawing::IconType icon) { _icon = icon; }

    [[nodiscard]] virtual auto kind() const -> PinKind = 0;

protected:
    void show_impl() const;

private:
    std::string           _name{};
    ax::Drawing::IconType _icon{ax::Drawing::IconType::Circle};
    PinId                 _id{};

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Name", _name),
            cereal::make_nvp("ID", _id)
        );
    }
};

class InputPin : public Pin {
public:
    using Pin::Pin;
    [[nodiscard]] auto kind() const -> PinKind override { return PinKind::Input; }

    friend auto operator==(InputPin const& lhs, InputPin const& rhs) -> bool { return lhs.id() == rhs.id(); }

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Base Pin", cereal::base_class<Pin>(this))
        );
    }
};

class OutputPin : public Pin {
public:
    using Pin::Pin;
    [[nodiscard]] auto kind() const -> PinKind override { return PinKind::Output; }

    friend auto operator==(OutputPin const& lhs, OutputPin const& rhs) -> bool { return lhs.id() == rhs.id(); }

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Base Pin", cereal::base_class<Pin>(this))
        );
    }
};

} // namespace Cool