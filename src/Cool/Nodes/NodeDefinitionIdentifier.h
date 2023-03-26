#pragma once
#include <string>

namespace Cool {

struct NodeDefinitionIdentifier {
    std::string definition_name;
    std::string category_name;

private:
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
#if COOL_SERIALIZATION
        archive(
            cereal::make_nvp("Definition name", definition_name),
            cereal::make_nvp("Category name", category_name)
        );
#else
        (void)archive;
#endif
    }
};

} // namespace Cool