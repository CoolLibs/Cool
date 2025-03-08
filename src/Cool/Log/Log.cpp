
#include "Cool/DebugOptions/DebugOptions.h"
#include "Cool/Log/file_logger.hpp"
#include "message_console.hpp"

namespace Cool::Log {

void info(std::string category, std::string message, std::vector<ClipboardContent> clipboard_contents)
{
    message_console().send(Message{
        .category           = std::move(category),
        .message            = std::move(message),
        .severity           = MessageSeverity::Info,
        .clipboard_contents = std::move(clipboard_contents),
    });
}

void warning(std::string category, std::string message, std::vector<ClipboardContent> clipboard_contents)
{
    message_console().send(Message{
        .category           = std::move(category),
        .message            = std::move(message),
        .severity           = MessageSeverity::Warning,
        .clipboard_contents = std::move(clipboard_contents),
    });
}

void error(std::string category, std::string message, std::vector<ClipboardContent> clipboard_contents)
{
    message_console().send(Message{
        .category           = std::move(category),
        .message            = std::move(message),
        .severity           = MessageSeverity::Error,
        .clipboard_contents = std::move(clipboard_contents),
    });
}

// TODO(Logs) mettre log Coollab et Launcher au m^me endroit ?
// TODO(Logs) disable logging in Perform mode
// TODO(Logs) launcher command "reveal log file in explorer"
// TODO(Logs) Dump the system info at the beginning of the log file
// TODO(Logs) In Github issues tell people to send the log file (no need to specify version anymore since it is in the log file (+ no need for devs to give their commit number))
// TODO(Logs) In Github issues tell people to send the project file
// TODO(Logs) In webgpu check that the info dump shows the underlying chosen API
// TODO(Logs) Remove debug option to gen info dump since it is in log now
// TODO(Logs) replace many logs to console with sending a notification

// TODO(Logs) remove Debug console and switch to log_internal_warning?
// TODO(Logs) make a pass to replace logging to console with a Notification where it makes sense
// TODO(Logs) also check where i throw exceptions (eg when creating a Window fails)
// TODO(Logs) rename fields (and change order) of Message to match Notification

void internal_info(std::string category, std::string message, std::vector<ClipboardContent> clipboard_contents, std::optional<bool> should_log_to_user)
{
    if ((should_log_to_user.has_value() && *should_log_to_user) || DebugOptions::log_internal_warnings())
        Log::info(std::move(category), std::move(message), std::move(clipboard_contents));
    else
    {
        file_logger().info(fmt::format("[{}] {}", std::move(category), std::move(message)));
        file_logger().flush(); // We flush as soon as we log a message, to make sure that if the app crashes we won't lose any logs that haven't been flushed yet
    }
}

void internal_warning(std::string category, std::string message, std::vector<ClipboardContent> clipboard_contents, std::optional<bool> should_log_to_user)
{
    if ((should_log_to_user.has_value() && *should_log_to_user) || DebugOptions::log_internal_warnings())
        Log::warning(std::move(category), std::move(message), std::move(clipboard_contents));
    else
    {
        file_logger().warn(fmt::format("[{}] {}", std::move(category), std::move(message)));
        file_logger().flush(); // We flush as soon as we log a message, to make sure that if the app crashes we won't lose any logs that haven't been flushed yet
    }
}

void internal_error(std::string category, std::string message, std::vector<ClipboardContent> clipboard_contents, std::optional<bool> should_log_to_user)
{
    if ((should_log_to_user.has_value() && *should_log_to_user) || DebugOptions::log_internal_warnings())
        Log::error(std::move(category), std::move(message), std::move(clipboard_contents));
    else
    {
        file_logger().error(fmt::format("[{}] {}", std::move(category), std::move(message)));
        file_logger().flush(); // We flush as soon as we log a message, to make sure that if the app crashes we won't lose any logs that haven't been flushed yet
    }
}

} // namespace Cool::Log
