#pragma once

#include "MessageLogger.h"
#include "ToUser.h"

namespace Cool {

void MessageLogger::handle(const std::optional<Message>& maybe_message)
{
    if (maybe_message)
    {
        Cool::Log::ToUser::console().send(_id, *maybe_message);
    }
    else
    {
        Cool::Log::ToUser::console().clear(_id);
    }
}

} // namespace Cool