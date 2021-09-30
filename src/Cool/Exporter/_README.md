# Exporter

## Exporter

### image_export_window and video_export_window

You can programmatically ask to open or close these windows by calling `image_export_window().open()` for example. 

## Polaroid

I gave it this funny name because it kind of is a polaroid : it is ready to take a picture (a.k.a. render) at any time, and then you can get the result (a.k.a. the render_target) immediately.

Also this class is designed for single use (altough you could reuse it). It is meant to serve as a parameter and not be stored.
