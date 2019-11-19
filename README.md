# Framebuffer Graphics
**This is a modified version of a really old project I build back in high school. I'm still in progress of making it more modern and fast, and at the moment, it compiles, but is most likely very buggy**

At it's heart, this project aims to allow programs running on a small device such as a raspberry pi to be able to draw graphics using a simple API and without needing the X window system.

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
