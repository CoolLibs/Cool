#include "RenderTarget_Base.h"

#if defined(__COOL_APP_VULKAN)
#include "RenderTarget_ImplVulkan.h"
template class Cool::RenderTarget_Base<Cool::RenderTarget_ImplVulkan>;
#elif defined(__COOL_APP_OPENGL)
#include "RenderTarget_ImplOpenGL.h"
template class Cool::RenderTarget_Base<Cool::RenderTarget_ImplOpenGL>;
#endif

namespace Cool {

template<typename T>
void RenderTarget_Base<T>::render(typename T::RenderFuncType render_fn)
{
    resize_if_necessary();
    _impl.render(render_fn);
}

template<typename T>
void RenderTarget_Base<T>::resize_if_necessary()
{
    if (_impl.size() != desired_size()) {
        _impl.resize(desired_size());
    }
}

} // namespace Cool