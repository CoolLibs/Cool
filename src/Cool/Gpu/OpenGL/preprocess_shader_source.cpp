#if defined(COOL_OPENGL)

#include "preprocess_shader_source.h"
#include <Cool/File/File.h>
#include <Cool/Path/Path.h>
#include <Cool/RegExp/RegExp.h>
#include <Cool/String/String.h>
#include <sstream>

namespace Cool::OpenGL {

static std::string line_or_include(const std::string& line)
{
    const auto path = RegExp::file_path_to_include(line);
    if (path.has_value()) {
        return preprocess_shader_source(File::to_string(*path));
    }
    else {
        return line;
    }
}

std::string preprocess_shader_source(const std::string& source)
{
    std::istringstream stream{source};
    std::ostringstream output;

    std::string line;
    while (std::getline(stream, line)) {
        String::replace_all(line, "_COOL_RES_", Path::cool_res());
        output << line_or_include(line) << '\n';
    }

    return output.str();
}

} // namespace Cool::OpenGL

#endif