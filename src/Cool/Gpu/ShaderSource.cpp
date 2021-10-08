#include "ShaderSource.h"
#include <Cool/RegExp/RegExp.h>
#include <sstream>

namespace Cool {

static std::string preprocess_glsl(const std::string& glsl_source);

ShaderSource::ShaderSource(std::string_view glsl_source)
    : _glsl_source{preprocess_glsl(std::string{glsl_source})}
{
}

std::string preprocess_glsl(const std::string& glsl_source)
{
    std::istringstream stream{glsl_source};
    std::ostringstream output;

    std::string line;
    while (std::getline(stream, line)) {
        const auto path = RegExp::file_path_to_include(line);
        if (path.has_value()) {
            output << ShaderSource{File::to_string(*path)}.glsl_source() << '\n';
        }
        else {
            output << line << '\n';
        }
    }

    return output.str();
}

} // namespace Cool
