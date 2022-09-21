#include "File.h"
#include <Cool/Log/ToUser.h>
#include <Cool/String/String.h>
#include <sys/stat.h>
#include <fstream>
#include <streambuf>

namespace Cool {

auto File::exists(std::filesystem::path file_path) -> bool
{
    return std::filesystem::exists(file_path);
}

auto File::file_name(std::filesystem::path file_path) -> std::filesystem::path
{
    return file_path.filename();
}

auto File::file_name_without_extension(std::filesystem::path file_path) -> std::filesystem::path
{
    return file_path.stem();
}

auto File::extension(std::filesystem::path file_path) -> std::filesystem::path
{
    return file_path.extension();
}

auto File::without_extension(std::filesystem::path file_path) -> std::filesystem::path
{
    return file_path.replace_extension();
}

auto File::without_file_name(std::filesystem::path file_path) -> std::filesystem::path
{
    if (!file_path.has_filename())
        return file_path;
    else
        return file_path.parent_path();
}

auto File::to_string(std::filesystem::path file_path) -> tl::expected<std::string, std::string>
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
    stream.close();
    return str;
}

bool File::create_folders_if_they_dont_exist(std::filesystem::path folder_path)
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

bool File::create_folders_for_file_if_they_dont_exist(std::filesystem::path file_path)
{
    return create_folders_if_they_dont_exist(without_file_name(file_path));
}

auto File::find_available_name(std::filesystem::path folder_path, std::filesystem::path file_name, std::filesystem::path extension) -> std::filesystem::path
{
    const std::string name = file_name.string();
    // Split file_name into a number in parenthesis and the base_name that is before those parenthesis
    auto [k, base_name] = [&]() {
        if (auto pos = name.find_last_of('(');
            pos != std::string::npos)
        {
            auto end_pos = name.find_last_of(')');
            try
            {
                return std::make_pair(
                    std::stoi(Cool::String::substring(name, pos + 1, end_pos + 1)),
                    Cool::String::substring(name, 0, pos)
                );
            }
            catch (std::exception&)
            {
            }
        }
        return std::make_pair(0, name);
    }();

    // Find an available name
    auto out_name = file_name;
    while (File::exists(folder_path / out_name.replace_extension(extension)))
    {
        out_name = base_name + "(" + std::to_string(k) + ")";
        k++;
    }
    return out_name;
}

} // namespace Cool