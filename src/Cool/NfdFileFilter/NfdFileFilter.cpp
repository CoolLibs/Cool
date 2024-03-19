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

const std::vector<nfdfilteritem_t> NfdFileFilter::Video = {
    {"Video", "*"}, // TODO(Video) List all the video formats that we support (programmatically get them from OpenCV ?)
};

const std::vector<nfdfilteritem_t> NfdFileFilter::Audio = {
    // Restricted to only the extensions that libnyquist can load.
    {"Audio", "mp3,wav,wave,wv,flac,ogg,opus,mpc,mpp"},
    {"mp3", "mp3"},
    {"wav", "wav,wave"},
    {"wv", "wv"},
    {"flac", "flac"},
    {"ogg", "ogg"},
    {"opus", "opus"},
    {"mpc", "mpc,mpp"},
};

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