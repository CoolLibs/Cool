#pragma once

#include <reg/reg.hpp>
#include "Error.h"
#include "ErrorId.h"

namespace Cool {

class ErrorLogger {
public:
    /// Removes the error from the list, because it was resolved.
    void clear(const ErrorId& id);
    // void clear_all(); // TODO I don't think this is usefull, errors will pop again immediately, or they have been closed and would disappear automatically anyways
    /// If id is null, creates a new error and sets id to it, otherwise refresh the error referenced by id.
    void refresh(ErrorId& id, const Error& error);

    void imgui_show() const; // TODO should focus associated window wehn clicking (or hovering) on an error

    /// Returns true iff the error is currently selected / hovered by the user
    /// Allows us to focus the corresponding window
    auto should_highlight(const ErrorId& id) -> bool;

private:
    reg::Registry<internal::ErrorMessage> _errors;
    mutable ErrorId                       _selected_error;
};

} // namespace Cool