#pragma once

namespace Cool {

struct VideoExportParams {
    float fps       = 30.f;
    float beginning = 0.f;
    float end       = 10.f;

    void imgui();

private:
    // Serialization
    friend class cereal::access;
    template<class Archive>
    void serialize(Archive& archive)
    {
        // #if COOL_SERIALIZATION // This one is pretty useful to have all the time, don't disable it.
        archive(
            cereal::make_nvp("FPS", fps),
            cereal::make_nvp("Beginning", beginning),
            cereal::make_nvp("End", end)
        );
        // #else
        //         (void)archive;
        // #endif
    }
};

} // namespace Cool