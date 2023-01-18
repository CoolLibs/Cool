#include <cereal/archives/json.hpp>
#include <filesystem>
#include <utility>
#include "Cool/StrongTypes/Color.h"

class NodesCategoryConfig {
public:
    explicit NodesCategoryConfig(std::filesystem::path const& category_folder_path)
        : _path_to_json{category_folder_path / "category_config.json"}
    {
        load_from_json();
    }

    [[nodiscard]] auto get_color() const -> Cool::Color { return _color; }

    auto imgui_popup() -> bool;

private:
    void save_to_json() const;
    void load_from_json();

private:
    Cool::Color           _color{};
    std::filesystem::path _path_to_json{};

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            cereal::make_nvp("Color", _color)
        );
    }
};