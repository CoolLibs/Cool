# ImageSize

This is a simple type that holds a width and a height, with one important invariant : ```width >= 1 && height >= 1```. Indeed them beeing negative would make no sense, and them beeing 0 would be problematic too, mostly because for most graphics APIs, an image with 0 pixels is an error resulting in a crash. Also, this is perfectly fine because if someone  wants to be able to represent an empty image, then ```std::optional<ImageSize>```  is the perfect fit for it.

Having this invariant removes the need for many checks, when computing the aspect ratio (no risk of dividing by 0) or when resizing a texture for example.

We have a templated version ```ImageSizeT``` which allow you to choose the type used to store ```width``` and ```height```, and we have a default ```ImageSize = ImageSizeT<uint32_t>```. You can ask for the type used to store ```width``` and ```height``` with ```ImageSizeT::DataType```.

```ImageSizeU::fit_into``` returns specifically a ```ImageSizeT<float>``` instead of matching the ```DataType``` of the input (```ImageSizeT<T>```) because this is what the computation gives us (since we multiply by the aspect ratio), and because ImGui expects the width and height to be floats and this function is mainly used to fit a texture in an ImGui window. Therefore we avoid extra conversions, and users can do them if they actually need them.