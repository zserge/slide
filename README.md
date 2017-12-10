# Slide - a minimal presentation tool

[![Build Status](https://travis-ci.org/zserge/slide.svg?branch=master)](https://travis-ci.org/zserge/slide)

<div>
<img align="left" src="https://raw.githubusercontent.com/trikita/slide/master/src/main/res/mipmap-xxxhdpi/ic_launcher.png" alt="logo" width="96px" height="96px" />
<p>A pure and simple presentation tool for Linux/MacOS/Windows inspired by <a href="https://en.wikipedia.org/wiki/Takahashi_method">Takahashi</a> method.
With simple markup language you can easily make modern-looking text slides. The main idea of such a technique is to visualize speech keynote with no distracting details. Slides with only a few words shown can easily keep the audience aware of the speaker's point.
</p>
</div>

## Syntax

**General**

- Each paragraph is a slide. Blank lines separate slides
- Text is rendered using the largest possible font size to fit in the slide bounds
- Single-line text is centered, otherwise it's left-aligned
- Dot at the beginning of a line is ignored. It's helpful to "escape" blank lines or special symbols inside a slide

**Text style**

- Text surrounded with `*` is rendered as bold (emphasized). Use `*` without a matching end marker to render a normal `*` sign
| Text | Text on slide |
|---|---|
|`* hello` | `* hello` |
|`*hello`| `* hello` |
|`* hello*` | `* hello*` |
|`**hello**`| `**hello**` |
| `*hello*` | **hello** |
| `*hello *` | **hello**|
| `***hello***` | **`**hello**`** |

- Line starting with `#` is a header
- Line starting with `␣␣` (two spaces) is rendered as code (monospace).

**Images**

- Line starting with `@` inserts an image from the URL (web, dropbox, local device storage)
- Images may be scaled, e.g `@http://example.com/bg.png 30%`
- Images may have certain gravity, e.g. `@http://example.com/logo.png top`

## Download

See Github [Releases](https://github.com/zserge/slide/releases/latest).

## Dependency instructions

```bash
# Install Cairo
$ sudo apt-get install libcairo2-dev
or
$ brew install cairo
```

## Building 

### Debug
```bash
$ mkdir build
$ cd build
$ cmake ..
$ make
```
### Release
```bash
$ mkdir build
$ cd build
$ cmake -D CMAKE_BUILD_TYPE=Release ..
$ make
```

## Other versions

Originally Slide was written for Android: https://github.com/trikita/slide

There is also a single-page HTML that can be edited to make a presentation: https://github.com/trikita/slide-html

