#pragma once
#include "View.h"

namespace Cool {

class ViewView : public View {
public:
    ViewView(View const& viewed_view, std::string_view name, bool is_closable = false)
        : View{name, is_closable}
        , _viewed_view{viewed_view}
    {}

private:
    auto get_image_texture_id() const -> ImTextureID override { return _viewed_view.get_image_texture_id(); }
    auto get_image_size() const -> img::Size override { return _viewed_view.get_image_size(); }

private:
    View const& _viewed_view;
};

} // namespace Cool
