#include "ErrorLogger.h"
#include <Cool/Constants/Constants.h>
#include <stringify/stringify.hpp>

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
            ImGui::BeginGroup();

            ImGui::TextColored(
                Cool::Constants::imvec4_red,
                "[%s] [#%lld] [%s]",
                Cool::stringify(err.timestamp).c_str(),
                err.count,
                err.error.category.c_str()
            );
            ImGui::SameLine();
            ImGui::Text("%s", err.error.detailed_message.c_str());

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
