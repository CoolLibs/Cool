#pragma once
#include "RenderTarget.h"

namespace Cool {

class RenderTargetWindow {
public:
    RenderTarget&       operator*() { return _render_target; }
    RenderTarget*       operator->() { return &_render_target; }
    const RenderTarget& operator*() const { return _render_target; }
    const RenderTarget* operator->() const { return &_render_target; }

private:
    RenderTarget _render_target;
};

} // namespace Cool