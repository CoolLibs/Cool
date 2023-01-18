#include "NodesLibrary.h"
#include <wafl/wafl.hpp>
#include "imgui.h"

namespace Cool::internal {

auto name_matches_filter(std::string const& name, std::string const& filter) -> bool
{
    return wafl::similarity_match({.input = filter, .reference = name}) >= wafl::Matches::Strongly;
}

} // namespace Cool::internal