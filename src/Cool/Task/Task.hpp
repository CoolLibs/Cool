#pragma once

namespace Cool {

class Task {
public:
    Task()                           = default;
    virtual ~Task()                  = default;
    Task(Task const&)                = delete;
    Task& operator=(Task const&)     = delete;
    Task(Task&&) noexcept            = delete;
    Task& operator=(Task&&) noexcept = delete;

    virtual void do_work() = 0;

private:
};

} // namespace Cool