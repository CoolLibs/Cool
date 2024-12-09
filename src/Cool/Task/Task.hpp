#pragma once
#include <reg/src/AnyId.hpp>

namespace Cool {

class Task {
public:
    // Passing an owner_id allows to cancel all tasks with a given owner_id at once. This is optional
    explicit Task(reg::AnyId const& owner_id = {})
        : _owner_id{owner_id}
    {}
    virtual ~Task()                  = default;
    Task(Task const&)                = delete;
    Task& operator=(Task const&)     = delete;
    Task(Task&&) noexcept            = delete;
    Task& operator=(Task&&) noexcept = delete;

    virtual void do_work() = 0;
    /// Request to finish the task as quickly as possible
    virtual void cancel() = 0;
    /// When the user tries to close the app, should we warn them that this task is in progress and would be cancelled?
    virtual auto needs_user_confirmation_to_cancel_when_closing_app() const -> bool = 0;

    auto owner_id() const -> reg::AnyId const& { return _owner_id; }

private:
    reg::AnyId _owner_id;
};

} // namespace Cool