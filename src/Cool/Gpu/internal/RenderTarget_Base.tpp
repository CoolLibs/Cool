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
    if (needs_resizing()) {
        _impl.resize(desired_size());
    }
}

} // namespace Cool