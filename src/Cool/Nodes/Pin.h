#pragma once

#include <imnodes/imnodes.h>

namespace Cool {

using PinId = uuids::uuid;

class Pin {
public:
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
        archive(cereal::make_nvp("Name", _name));
        archive(cereal::make_nvp("ID", _id));
    }
};

class InputPin : public Pin {
public:
    using Pin::Pin;
    void show() const override;
};

class OutputPin : public Pin {
public:
    using Pin::Pin;
    void show() const override;
};

} // namespace Cool