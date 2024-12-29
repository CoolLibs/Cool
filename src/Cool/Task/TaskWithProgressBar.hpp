#pragma once
#include "ImGuiNotify/ImGuiNotify.hpp"
#include "Task.hpp"

namespace Cool {

class TaskWithProgressBar : public Task {
public:
    TaskWithProgressBar();

protected:
    auto cancel_requested() const -> bool { return _data.cancel.load(); }
    void set_progress(float progress) { _data.progress.store(progress); }

    virtual auto notification_after_do_work_completes() const -> ImGuiNotify::Notification;

private:
    void on_submit() override;
    void just_before_work_starts() override;
    void on_cleanup(bool has_been_canceled) override;
    auto is_quick_task() const -> bool override { return false; }
    void cancel() override { _data.cancel.store(true); }

private:
    ImGuiNotify::NotificationId _notification_id{};

    struct DataSharedWithNotification {
        std::atomic<bool>  cancel{false}; // Needs to be an atomic because it will be used on several threads (by do_work() and by cancel())
        std::atomic<float> progress{0.f};
    };
    DataSharedWithNotification _data{};
};

} // namespace Cool