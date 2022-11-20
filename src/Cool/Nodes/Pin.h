#pragma once

#include <imnodes/imnodes.h>
#include <reg/cereal.hpp>
#include <reg/reg.hpp>

namespace Cool {

using PinId = reg::AnyId;

class Pin {
public:
    Pin() = default;
    explicit Pin(std::string_view name);
    virtual ~Pin() = default;

    auto name() const -> std::string const& { return _name; }
    auto id() const -> PinId const& { return _id; }

    virtual void show() const = 0;

protected:
    void show_impl() const;

private:
    std::string _name;
    PinId       _id;

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
    void show() const override;

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
    void show() const override;

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