#pragma once

namespace Cool {

static constexpr const char* OSC_EVERY_AVAILABLE_ADDRESS{"Every available address"};

struct OSCConnectionEndpoint {
    int         port{-1};
    std::string ip_address{OSC_EVERY_AVAILABLE_ADDRESS};

public:
    auto is_valid() const -> bool;

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Port", port),
            cereal::make_nvp("IP Address", ip_address)
        );
    }
};

} // namespace Cool