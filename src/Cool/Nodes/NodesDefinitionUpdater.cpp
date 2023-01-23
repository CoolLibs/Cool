#include "NodesDefinitionUpdater.h"

namespace Cool {

auto get_category_name(std::filesystem::path const& path, std::filesystem::path const& root) -> std::string
{
    std::string const category_name = File::without_file_name(std::filesystem::relative(path, root)).string();
    return category_name.empty() ? "Unnamed Category" : category_name;
}

}; // namespace Cool
