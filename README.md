# Framebuffer Graphics
Write directly to the linux framebuffer. I use this from Raspbian Lite on multiple Raspberry Pi devices for displaying monitoring dashboards with pre-rendered graphs from AWS CloudWatch.

Note: you may need/want to `usermod` your current user to add it into the `video` group like so, to avoid needing to use `sudo` to run each of these commands. The `ttymode` command still requires sudo, however:
```bash
sudo usermod -G video $(whoami)
```

## Dependencies
Install `libpng-dev` and `libjpeg-dev` if you are using the PNG or JPEG functions. `make` is required to build.
```bash
sudo apt-get install libpng-dev libjpeg-dev make
```

## Building
```bash
make
```

## Usage
- `fb-describe`
  - outputs the device name, as well as dimensions. this can be used by drawing scripts to calculate sizes.
  - example:
  ```bash
  $ fb-describe
  /dev/fb0 1280 800
  ```
  
- `fb-clear`
  - clears the `/dev/fb0` framebuffer
  - options:
    - `-g` _(optional)_ - grayscale value from 0 (black) to 255 (white)
      - example: `fb-clear -g 255`
      
- `fb-ttymode`
  - sets the mode of the tty for graphics or text use
  - example:
  ```bash
  $ fb-ttymode graphics # (this will cause text to not work, so run over ssh!)
  $ fb-ttymode text
  ```
  
- `fb-pngdraw` / `fb-jpgdraw`
  - draws an image to the framebuffer
  - options:
    - `-f` - path to the image file to draw.
    - `-x` _(optional)_ default: 0 - the x position to draw
    - `-y` _(optional)_ default: 0 - the y position to draw
    - `-w` _(optional)_ default: display width - the width of the image
    - `-h` _(optional)_ default: display height - the height of the image
    - `-hue [mask]` _(optional)_ default: disabled - apply a hue-shift to the image, in the red, green, blue, cyan, yellow, magenta, or white hue. Valid characters in mask string are "rgbcymw".
    - `-hue-threshold` _(optional)_ default: 20 - when applying a hue shift, pixels with RGB values abs. differences less than this are ignored.
    - `-invert` _(optional)_ default: disabled - apply a `255 - x` transform to all pixels in the image, inverting it.
    - `-grayscale` _(optional)_ default: disabled - shortcut for the options `-hue w -hue-threshold 0`
  - example:
  ```bash
  $ fb-pngdraw -f foo.png -x 0 -y 0 -w 100 -h 100 -invert
  $ fb-jpgdraw -f foo.jpg -x 0 -y 0 -w 100 -h 100 -hue r
  ```

## TODO
- Add text rendering function with freetype
- Add compositing which allows for multiple draw commands to be rendered to an offscreen buffer, and then rendered
- Add alpha rendering ability

## Licence
MIT
