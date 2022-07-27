#include "File.h"
#include <Cool/Log/ToUser.h>
#include <sys/stat.h>
#include <fstream>
#include <streambuf>

namespace Cool {

bool File::exists(std::string_view file_path)
{
    struct stat buffer; // NOLINT
    return (stat(file_path.data(), &buffer) == 0);
}

std::string File::absolute_path(std::string_view file_path)
{
    return std::filesystem::absolute(file_path).string();
}

std::string File::file_name(std::string_view file_path)
{
    auto pos = file_path.find_last_of("/\\") + 1;
    return std::string{file_path.substr(pos, file_path.size() - pos)};
}

std::string File::file_name_without_extension(std::string_view file_path)
{
    return whithout_extension(file_name(file_path));
}

std::string File::extension(std::string_view file_path)
{
    auto pos = file_path.find_last_of('.');
    if (pos < file_path.size())
    {
        return std::string{file_path.substr(pos, file_path.size())};
    }
    else
    {
        return "";
    }
}

std::string File::whithout_extension(std::string_view file_path)
{
    auto pos = file_path.find_last_of('.');
    if (pos < file_path.size())
    {
        return std::string{file_path.substr(0, pos)};
    }
    else
    {
        return std::string{file_path};
    }
}

std::string File::whithout_file_name(std::string_view file_path)
{
    if (file_path.find_last_of('.') < file_path.size()) // There is a "." of an extension, so the thing after the last "/" must be a file name
    {
        auto pos = file_path.find_last_of("/\\");
        return std::string{file_path.substr(0, pos)};
    }
    else
    {
        return std::string{file_path};
    }
}

std::string File::to_string(std::string_view file_path)
{
    // Thanks to https://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
    std::ifstream stream(file_path.data());
    if (!stream.is_open())
    {
        Log::ToUser::warning(
            "File::to_string",
            fmt::format("Failed to open file: \"{}\"", file_path)
        );
        return "";
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

bool File::create_folders_if_they_dont_exist(std::string_view folder_path)
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

bool File::create_folders_for_file_if_they_dont_exist(std::string_view file_path)
{
    return create_folders_if_they_dont_exist(whithout_file_name(std::string(file_path)));
}

std::string File::find_available_name(std::string_view folder_path, std::string_view file_name, std::string_view extension)
{
    // Split file_name into a number in parenthesis and the base_name that is before those parenthesis
    auto [k, base_name] = [&]() {
        if (auto pos = file_name.find_last_of('(');
            pos != std::string::npos)
        {
            auto end_pos = file_name.find_last_of(')');
            try
            {
                return std::make_pair(
                    std::stoi(std::string{file_name.substr(pos + 1, end_pos - pos)}),
                    std::string{file_name.substr(0, pos)}
                );
            }
            catch (std::exception&)
            {
            }
        }
        return std::make_pair(0, std::string{file_name});
    }();

    // Find an available name
    auto out_name = std::string{file_name};
    while (File::exists(std::string{folder_path} + "/" + out_name + std::string{extension}))
    {
        out_name = base_name + "(" + std::to_string(k) + ")";
        k++;
    }
    return out_name;
}

} // namespace Cool