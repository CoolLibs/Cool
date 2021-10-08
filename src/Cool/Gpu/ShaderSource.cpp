#include "ShaderSource.h"
#include <Cool/RegExp/RegExp.h>
#include <sstream>

namespace Cool {

static std::string preprocess_glsl(const std::string& glsl_source);

ShaderSource::ShaderSource(std::string_view glsl_source)
    : _glsl_source{preprocess_glsl(std::string{glsl_source})}
{
}

std::string check_for_include(const std::string& line)
{
    const auto path = RegExp::file_path_to_include(line);
    if (path.has_value()) {
        return ShaderSource{File::to_string(*path)}.glsl_source();
    }
    else {
        return line;
    }
}

std::string preprocess_glsl(const std::string& glsl_source)
{
    std::istringstream stream{glsl_source};
    std::ostringstream output;

    std::string line;
    while (std::getline(stream, line)) {
        output << check_for_include(line) << '\n';
    }

    return output.str();
}

} // namespace Cool
