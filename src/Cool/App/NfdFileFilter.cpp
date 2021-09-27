#pragma once

#include "NfdFileFilter.h"

namespace Cool {

const std::vector<nfdfilteritem_t> NfdFileFilter::Image = {
    {"Image", "png,jpg,jpeg,tif,tiff"},
    {"png", "png"},
    {"jpeg", "jpg,jpeg"},
    {"tiff", "tif,tiff"},
};
const std::vector<nfdfilteritem_t> NfdFileFilter::Png  = {NfdFileFilter::Image[1]};
const std::vector<nfdfilteritem_t> NfdFileFilter::Jpeg = {NfdFileFilter::Image[2]};
const std::vector<nfdfilteritem_t> NfdFileFilter::Tiff = {NfdFileFilter::Image[3]};

const std::vector<nfdfilteritem_t> NfdFileFilter::Shader = {
    {"Shader", "frag,fs,vert,vs,comp,cs,glsl,hlsl"},
    {"Fragment Shader", "frag,fs,glsl,hlsl"},
    {"Vertex Shader", "vert,vs,glsl,hlsl"},
    {"Compute Shader", "comp,cs,glsl,hlsl"},
};
const std::vector<nfdfilteritem_t> NfdFileFilter::FragmentShader = {NfdFileFilter::Shader[1]};
const std::vector<nfdfilteritem_t> NfdFileFilter::VertexShader   = {NfdFileFilter::Shader[2]};
const std::vector<nfdfilteritem_t> NfdFileFilter::ComputeShader  = {NfdFileFilter::Shader[3]};

} // namespace Cool