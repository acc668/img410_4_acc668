# Project 4: Illumination

A ray caster written in C++ that renders 3D scenes with Phong illumination,
point lights, spotlights, and shadows.

# Author(s)

- Alexandra Curry <acc668@nau.edu>

# Usage

```
./illumination width height input.scene output.ppm
```

- `width` / `height` — output image dimensions in pixels
- `input.scene` — scene description file
- `output.ppm` — output image in PPM P3 format

Example:
```
./illumination 512 512 test.scene output.ppm
```