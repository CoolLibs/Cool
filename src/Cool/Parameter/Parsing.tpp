#include <cereal/archives/json.hpp>
#include <sstream>

namespace Cool {

template<ParameterDesc Desc>
Desc parse(std::string_view source)
{
    auto res = Desc{};
    auto ss  = std::stringstream{std::string{source}};
    {
        cereal::JSONInputArchive archive{ss};
        archive(res);
    }
    return res;
}

} // namespace Cool
