#pragma once

namespace Cool {

template<typename WGPUObject>
concept WGPUObjectHasDestroyMethod = requires(WGPUObject handle) {
    handle.destroy();
};

template<typename WGPUObject>
class WGPUUnique {
public:
    explicit WGPUUnique(WGPUObject handle)
        : _handle{handle}
    {}
    ~WGPUUnique()
    {
        if (_handle == nullptr)
            return; // Object has been moved-from and doesn't need to be destroyed.

        if constexpr (WGPUObjectHasDestroyMethod<WGPUObject>)
            _handle.destroy(); // Some objects like Buffers also have a destroy() method that we need to call. See https://eliemichel.github.io/LearnWebGPU/basic-3d-rendering/input-geometry/playing-with-buffers.html#creating-a-buffer
        _handle.release();
    }

    WGPUUnique(WGPUUnique const&)                    = delete;
    auto operator=(WGPUUnique const&) -> WGPUUnique& = delete;
    WGPUUnique(WGPUUnique&& o) noexcept
        : _handle{o._handle}
    {
        o._handle = nullptr;
    }
    auto operator=(WGPUUnique&& o) noexcept -> WGPUUnique&
    {
        if (this != &o)
        {
            ~WGPUUnique();
            _handle   = o._handle;
            o._handle = nullptr;
        }
        return *this;
    }

    auto handle() const -> WGPUObject { return _handle; } // TODO(WebGPU) Is this indeed just an int that is cheap to copy around?

private:
    WGPUObject _handle;
};

} // namespace Cool