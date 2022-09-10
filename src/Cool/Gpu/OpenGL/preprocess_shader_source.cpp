#if defined(COOL_OPENGL)

#include "preprocess_shader_source.h"
#include <Cool/File/File.h>
#include <Cool/Path/Path.h>
#include <Cool/RegExp/RegExp.h>
#include <Cool/String/String.h>
#include <sstream>

namespace Cool::OpenGL {

/// If line is a `#include` line, returns the content of the included file
/// (or an error if the file couldn't be read).
/// Otherwise just returns the line that was passed in.
static tl::expected<std::string, std::string> line_or_include(const std::string& line)
{
    const auto path = RegExp::file_path_to_include(line);
    if (path.has_value())
    {
        const auto file_content = File::to_string(*path);
        if (!file_content)
        {
            return tl::make_unexpected(fmt::format(
                "Failed to #include file \"{}\".\n{}",
                *path, file_content.error()
            ));
        }
        return preprocess_shader_source(*file_content);
    }
    else
    {
        return line;
    }
}

tl::expected<std::string, std::string> preprocess_shader_source(std::string_view source)
{
    std::istringstream stream{std::string(source)};
    std::ostringstream output;

    std::string line;
    while (std::getline(stream, line))
    {
        String::replace_all(line, "_COOL_RES_", Path::cool_res());
        String::replace_all(line, "_ROOT_FOLDER_", Path::root());
        const auto content = line_or_include(line);
        if (!content)
            return content;
        output << *content << '\n';
    }

    return output.str();
}

} // namespace Cool::OpenGL

#endif