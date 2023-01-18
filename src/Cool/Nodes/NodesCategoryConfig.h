#include <cereal/archives/json.hpp>
#include "Cool/StrongTypes/Color.h"

struct NodesCategoryConfig {
public:
    auto set_color(std::string category_name) -> void;
    auto get_color() -> Cool::Color; 
    auto serialize_from_json(std::filesystem::path url) -> void;
    auto popup() -> bool;

private:
    Cool::Color color;
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Color", color)
        ); // serialize things by passing them to the archive
    }
};