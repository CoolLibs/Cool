#include "ErrorLogger.h"
#include <Cool/Constants/Constants.h>

namespace Cool {

void ErrorLogger::clear(const ErrorId& id)
{
    _errors.destroy(id);
}

void ErrorLogger::refresh(ErrorId& id, const Error& error)
{
    if (id.underlying_uuid().is_nil() ||
        !_errors.contains(id))
    {
        id = _errors.create({
            error,
            std::chrono::system_clock::now(),
            0,
        });
    }
    else
    {
        _errors.with_mutable_ref(id, [&](internal::ErrorWithMetadata& err) {
            err.error = error;
            err.count++;
            err.timestamp = std::chrono::system_clock::now();
        });
    }
}

void ErrorLogger::imgui_show() const
{
    ImGui::Begin("Errors");
    {
        _selected_error = ErrorId{};
        for (const auto& [id, err] : _errors)
        {
            const auto time = std::chrono::system_clock::to_time_t(err.timestamp);
            ImGui::BeginGroup();

#if !defined(__GNUC__)
#pragma warning(push)
#pragma warning(disable : 4996)
#endif
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif
            const auto local_time = localtime(&time);
#if defined(__clang__)
#pragma clang diagnostic pop
#endif
#if !defined(__GNUC__)
#pragma warning(pop)
#endif
            if (local_time)
            {
                ImGui::TextColored(
                    Cool::Constants::imvec4_red,
                    "[%d:%d'%d\"%lld] [#%lld] [%s]",
                    local_time->tm_hour,
                    local_time->tm_min,
                    local_time->tm_sec,
                    std::chrono::duration_cast<std::chrono::milliseconds>(err.timestamp.time_since_epoch()).count() % 1000,
                    err.count,
                    "YOLO" // message.category.c_str()
                );
            }
            ImGui::SameLine();
            ImGui::Text("%s", err.error.name.c_str());

            ImGui::EndGroup();
            if (ImGui::IsItemHovered())
            {
                _selected_error = id;
            }
        }
    }
    ImGui::End();
}

auto ErrorLogger::should_highlight(const ErrorId& id) -> bool
{
    return !id.underlying_uuid().is_nil() &&
           id == _selected_error;
}

} // namespace Cool
