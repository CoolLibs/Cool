#pragma once
#include "Cool/Gpu/RenderTarget.h"

namespace Cool {

class DoubleBufferedRenderTarget {
public:
    auto read_target() const -> Cool::RenderTarget const&;
    auto write_target() -> Cool::RenderTarget&;

    void swap_buffers();

    void clear_render_targets();
    void set_read_target_size_immediately(img::Size const&);

private:
    auto read_target() -> Cool::RenderTarget&;

private:
    Cool::RenderTarget _rt1{};
    Cool::RenderTarget _rt2{};
    bool               _rt1_is_read{};
};

} // namespace Cool