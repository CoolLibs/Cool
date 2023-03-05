#include "File.h"
#include <Cool/Log/ToUser.h>
#include <Cool/String/String.h>
#include <sys/stat.h>
#include <fstream>
#include <streambuf>
#include "Cool/File/File.h"

namespace Cool {

auto File::exists(std::filesystem::path const& file_path) -> bool
{
    return std::filesystem::exists(file_path);
}

auto File::file_name(std::filesystem::path const& file_path) -> std::filesystem::path
{
    return file_path.filename();
}

auto File::file_name_without_extension(std::filesystem::path const& file_path) -> std::filesystem::path
{
    return file_path.stem();
}

auto File::extension(std::filesystem::path const& file_path) -> std::filesystem::path
{
    return file_path.extension();
}

auto File::without_extension(std::filesystem::path file_path) -> std::filesystem::path
{
    return file_path.replace_extension();
}

auto File::without_file_name(std::filesystem::path const& file_path) -> std::filesystem::path
{
    if (!file_path.has_filename())
        return file_path;
    else
        return file_path.parent_path();
}

auto File::to_string(std::filesystem::path const& file_path) -> tl::expected<std::string, std::string>
{
    // Thanks to https://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
    std::ifstream stream(file_path.string());
    if (!stream.is_open())
    {
        return tl::make_unexpected(
            fmt::format("Failed to open file \"{}\"", file_path)
        );
    }
    stream.seekg(0, std::ios::end);
    std::string str;
    str.reserve(static_cast<size_t>(stream.tellg()));
    stream.seekg(0, std::ios::beg);
    str.assign(
        (std::istreambuf_iterator<char>(stream)),
        std::istreambuf_iterator<char>()
    );
    return str;
}

auto File::create_folders_if_they_dont_exist(std::filesystem::path const& folder_path) -> bool
{
    if (!exists(folder_path))
    {
        try
        {
            std::filesystem::create_directories(folder_path);
            return true;
        }
        catch (const std::exception& e)
        {
            Log::ToUser::warning("File::create_folders_if_they_dont_exist", fmt::format("Failed:\n{}", e.what()));
            return false;
        }
    }
    return true;
}

auto File::create_folders_for_file_if_they_dont_exist(std::filesystem::path const& file_path) -> bool
{
    return create_folders_if_they_dont_exist(without_file_name(file_path));
}

auto File::find_available_name(std::filesystem::path const& folder_path, std::filesystem::path const& file_name, std::filesystem::path const& extension) -> std::filesystem::path
{
    std::string const name = Cool::File::without_extension(file_name).string();
    // Split file_name into a number in parenthesis and the base_name that is before those parenthesis
    auto [k, base_name] = [&]() {
        if (auto const pos = name.find_last_of('(');
            pos != std::string::npos)
        {
            auto const end_pos = name.find_last_of(')');
            try
            {
                return std::make_pair(
                    std::stoi(Cool::String::substring(name, pos + 1, end_pos + 1)),
                    Cool::String::substring(name, 0, pos)
                );
            }
            catch (std::exception const&)
            {
            }
        }
        return std::make_pair(0, name);
    }();

    // Find an available name
    auto out_name = file_name;
    out_name.replace_extension(extension);
    while (File::exists(folder_path / out_name))
    {
        out_name = base_name + "(" + std::to_string(k) + ")";
        out_name.replace_extension(extension);
        k++;
    }
    return out_name;
}

void File::set_content(std::filesystem::path const& file_path, std::string_view content)
{
    if (!create_folders_for_file_if_they_dont_exist(file_path))
        return;

    auto file = std::ofstream{file_path};
    if (!file.is_open())
        return;

    file << content;
}

} // namespace Cool