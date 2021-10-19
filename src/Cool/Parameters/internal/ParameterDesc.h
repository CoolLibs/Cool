#pragma once

namespace Cool {

template<typename T>
struct ParameterDesc {
    std::string name{};
    T           default_value{};

private:
    //Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(cereal::make_nvp("Name", name),
                cereal::make_nvp("Default value", default_value));
    }
};

} // namespace Cool
