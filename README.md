# Framebuffer Graphics
Write directly to the linux framebuffer. I use this from Raspbian Lite on multiple Raspberry Pi devices for displaying monitoring dashboards with pre-rendered graphs from AWS CloudWatch.

## Dependencies
Install `libpng-dev` and `libjpeg-dev` if you are using the PNG or JPEG functions.
```bash
sudo apt-get install libpng-dev libjpeg-dev
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
    - `g` _(optional)_ - grayscale value from 0 (black) to 255 (white)
      - example: `fb-clear -g 255
      
- `fb-ttymode`
  - sets the mode of the tty for graphics or text use
  - example:
  ```bash
  $ fb-ttymode graphics # (this will cause text to not work, so run over ssh!)
  $ fb-ttymode text
  ```
  
- `fb-pngdraw`
  - draws a png image to the framebuffer
  - options:
    - `f` - path to the png file to draw.
    - `x` _(optional)_ default: 0 - the x position to draw
    - `y` _(optional)_ default: 0 - the y position to draw
    - `w` _(optional)_ default: display width - the width of the image
    - `h` _(optional)_ default: display height - the height of the image
## Licence
MIT
