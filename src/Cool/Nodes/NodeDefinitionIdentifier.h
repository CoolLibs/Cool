#pragma once
#include <string>

namespace Cool {

struct NodeDefinitionIdentifier {
    std::string definition_name;
    std::string category_name;

private:
    friend class ser20::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        archive(
            ser20::make_nvp("Definition name", definition_name),
            ser20::make_nvp("Category name", category_name)
        );
    }
};

} // namespace Cool