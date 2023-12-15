#pragma once
#include <exception>
#include "Cool/Log/OptionalErrorMessage.h"

namespace Cool {

class Exception : public std::exception {
public:
    explicit Exception(OptionalErrorMessage);
    auto what() const noexcept -> const char* override;
    auto error_message() const -> OptionalErrorMessage const&;

private:
    OptionalErrorMessage _error_message;
};

} // namespace Cool