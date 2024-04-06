#include "RenderTarget.h"
#include "Cool/Gpu/WebGPUContext.h"
#include "Cool/WebGPU/ComputePipeline.h"
#include "Cool/WebGPU/ShaderModule.h"
#include "Cool/WebGPU/Texture.h"
#include "webgpu/webgpu.hpp"

namespace Cool {

void RenderTarget::render(std::function<void(wgpu::RenderPassEncoder render_pass)> const& render_fn)
{
    resize_if_necessary();
    assert(_texture.handle() != nullptr);

    wgpu::RenderPassDescriptor renderPassDesc{};
    TextureView const&         nextTexture{_texture.entire_texture_view()};

    wgpu::RenderPassColorAttachment renderPassColorAttachment{};
    renderPassColorAttachment.view          = nextTexture;
    renderPassColorAttachment.resolveTarget = nullptr;
    renderPassColorAttachment.loadOp        = wgpu::LoadOp::Clear; // TODO(WebGPU) bool to choose if we want to clear or not
    renderPassColorAttachment.storeOp       = wgpu::StoreOp::Store;
    renderPassColorAttachment.clearValue    = wgpu::Color{0., 0., 0., 0.};
    renderPassDesc.colorAttachmentCount     = 1;
    renderPassDesc.colorAttachments         = &renderPassColorAttachment;

    // TODO(WebGPU) bool to choose if we want depth buffer or not
    //     wgpu::RenderPassDepthStencilAttachment depthStencilAttachment;
    //     depthStencilAttachment.view              = webgpu_context().depthTextureView;
    //     depthStencilAttachment.depthClearValue   = 1.0f;
    //     depthStencilAttachment.depthLoadOp       = wgpu::LoadOp::Clear;
    //     depthStencilAttachment.depthStoreOp      = wgpu::StoreOp::Store;
    //     depthStencilAttachment.depthReadOnly     = false;
    //     depthStencilAttachment.stencilClearValue = 0;
    // #ifdef WEBGPU_BACKEND_WGPU
    //     depthStencilAttachment.stencilLoadOp  = wgpu::LoadOp::Clear;
    //     depthStencilAttachment.stencilStoreOp = wgpu::StoreOp::Store;
    // #else
    //     depthStencilAttachment.stencilLoadOp  = wgpu::LoadOp::Undefined;
    //     depthStencilAttachment.stencilStoreOp = wgpu::StoreOp::Undefined;
    // #endif
    //     depthStencilAttachment.stencilReadOnly = true;

    renderPassDesc.depthStencilAttachment = nullptr; //&depthStencilAttachment;

    renderPassDesc.timestampWriteCount = 0;
    renderPassDesc.timestampWrites     = nullptr;

    wgpu::RenderPassEncoder render_pass = webgpu_context().encoder.beginRenderPass(renderPassDesc);

    render_fn(render_pass); // TODO(WebGPU) What happens if this function throws ?

    // Render was successful, _texture has changed, so _texture_straight_alpha will need to update
    _texture_straight_alpha.reset();

    render_pass.end();
    render_pass.release();
}

auto RenderTarget::texture_straight_alpha() const -> Texture const&
{
    // TODO(WebGPU)
    // if (_texture.alpha_space == AlphaSpace::Straight())
    //     return _texture;

    if (!_texture_straight_alpha.has_value())
        make_texture_straight_alpha();
    assert(_texture_straight_alpha.has_value());
    return *_texture_straight_alpha;
}

static auto make_compute_pipeline_that_converts_to_straight_alpha() -> ComputePipeline
{
    // Create bind group layout
    std::vector<wgpu::BindGroupLayoutEntry> bindings(2, wgpu::Default);

    // Input texture
    bindings[0].binding               = 0;
    bindings[0].texture.sampleType    = wgpu::TextureSampleType::Float;
    bindings[0].texture.viewDimension = wgpu::TextureViewDimension::_2D;
    bindings[0].visibility            = wgpu::ShaderStage::Compute;

    // Output texture
    bindings[1].binding                      = 1;
    bindings[1].storageTexture.access        = wgpu::StorageTextureAccess::WriteOnly;
    bindings[1].storageTexture.format        = wgpu::TextureFormat::RGBA8Unorm;
    bindings[1].storageTexture.viewDimension = wgpu::TextureViewDimension::_2D;
    bindings[1].visibility                   = wgpu::ShaderStage::Compute;

    wgpu::BindGroupLayoutDescriptor bind_group_layout_desc;
    bind_group_layout_desc.entryCount = (uint32_t)bindings.size();
    bind_group_layout_desc.entries    = bindings.data();

    return ComputePipeline{{
        .label                    = "[RenderTarget] Convert premultiplied to straight alpha",
        .bind_group_layout_desc   = bind_group_layout_desc,
        .workgroup_size           = glm::uvec3{8, 8, 1}, // "I suggest we use a workgroup size of 8x8: this treats both X and Y axes symmetrically and sums up to 64 threads, which is a reasonable multiple of a typical warp size." from https://eliemichel.github.io/LearnWebGPU/basic-compute/image-processing/mipmap-generation.html#dispatch
        .wgsl_compute_shader_code = R"wgsl(
@group(0) @binding(0) var in_tex_premultiplied: texture_2d<f32>;
@group(0) @binding(1) var out_tex_straight: texture_storage_2d<rgba8unorm,write>;

@compute
fn main(@builtin(global_invocation_id) id: vec3<u32>) {
    if(any(id.xy >= textureDimensions(in_tex_premultiplied)))
    {
        return;
    }
    let color = textureLoad(in_tex_premultiplied, id.xy, 0);
    textureStore(out_tex_straight, id.xy, 
        vec4(
            select(color.rgb / color.a, vec3(0.), abs(color.a) < 0.0000001f),
            color.a
        )
    );
}
)wgsl",
    }};
}

void RenderTarget::make_texture_straight_alpha() const
{
    { // Recreate texture
        auto texture_desc = _texture.descriptor();
        texture_desc.usage |= wgpu::TextureUsage::StorageBinding; // We need to write to the texture in the shader
        _texture_straight_alpha = Texture{texture_desc};
    }

    { // Run compute pass to convert texture to straight alpha
        _compute_pipeline_that_converts_to_straight_alpha.compute({
            .invocation_count_x = _texture.size().width(),
            .invocation_count_y = _texture.size().height(),
            .bind_group         = {
                /* @binding(0) = */ _texture.entire_texture_view(),
                /* @binding(1) = */ _texture_straight_alpha->entire_texture_view()
            },
        });
    }
}

RenderTarget::RenderTarget()
    : _compute_pipeline_that_converts_to_straight_alpha{make_compute_pipeline_that_converts_to_straight_alpha()}
{}

void RenderTarget::set_size(img::Size size)
{
    _desired_size = size;
}

void RenderTarget::set_size_immediately(img::Size size)
{
    set_size(size);
    resize_if_necessary();
}

auto RenderTarget::needs_resizing() const -> bool
{
    return _texture.handle() == nullptr || _texture.size() != desired_size();
}

void RenderTarget::resize_if_necessary()
{
    if (!needs_resizing())
        return;

    wgpu::TextureDescriptor texture_desc;
    texture_desc.dimension       = wgpu::TextureDimension::_2D;
    texture_desc.size            = {_desired_size.width(), _desired_size.height(), 1};
    texture_desc.mipLevelCount   = 1;
    texture_desc.sampleCount     = 1;
    texture_desc.format          = wgpu::TextureFormat::RGBA8Unorm;
    texture_desc.usage           = wgpu::TextureUsage::TextureBinding | wgpu::TextureUsage::RenderAttachment /*  | TextureUsage::CopyDst */;
    texture_desc.viewFormatCount = 0;
    texture_desc.viewFormats     = nullptr;

    _texture = Texture{texture_desc};
    _texture_straight_alpha.reset();
}

} // namespace Cool