#pragma once
#include <Cool/MessageConsole/MessageConsole.h>
#include "Dirty.h"

namespace Cool {

class UpdateContext_Ref {
public:
    auto message_console() -> Cool::MessageConsole& { return _data.message_console; }
    void set_clean(const DirtyFlag& dirty_flag) { _data.set_clean(dirty_flag); }

    struct Data { // We wrap our members in a struct to get a constructor automatically
        std::reference_wrapper<Cool::MessageConsole> message_console;
        SetClean_Ref                                 set_clean;
    };
    explicit UpdateContext_Ref(Data data)
        : _data{data} {}

private:
    Data _data;
};

} // namespace Cool