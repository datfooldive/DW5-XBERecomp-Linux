# Dynasty Warriors 5 XBE Recomp

An unofficial native Linux port of **Dynasty Warriors 5** (Xbox, 2005) by static recompilation. The game's x86 XBE is translated to C++ ahead of time and the Xbox kernel and XDK libraries (Direct3D 8, DirectSound, XAPI) are implemented at the API level. It draws with Vulkan and plays sound and reads input through SDL3. There is no emulator, pushbuffer interpreter or software rasteriser.

Based on [TGP482/XboxXBERecomp](https://github.com/TGP482/XboxXBERecomp). See [Credits and licensing](#credits-and-licensing).

> [!NOTE]
> This repository does not include any game files or recompiled game code. You need your own legally acquired copy of the game to build and run it.

## Status

| | |
| --- | --- |
| title | Dynasty Warriors 5 (USA), title id `0x4B4F0007`, XDK 5849 |
| boots | being brought up |
| renderer | Vulkan 1.4 (shaders compiled to SPIR-V at run time by shaderc) |
| audio | SDL3, stereo, 48 kHz |
| input | SDL3 controllers, and the keyboard as controller 1 |

## Requirements

- Linux with Wayland or X11 and a Vulkan 1.3+ GPU
- `clang`, `lld`, `cmake`, `ninja`, `git`, `pkg-config`
- Vulkan headers and loader, and `shaderc`
- SDL3's build dependencies. CMake fetches SDL3, Zydis, toml++ and Dear ImGui.

Arch:

```bash
sudo pacman -S clang lld cmake ninja git pkgconf vulkan-headers vulkan-icd-loader shaderc
```

`build.sh` lists the Debian/Ubuntu and Fedora packages if something is missing.

## Building

1. Put the game in `assets/`: either the disc image (`.iso`), or `default.xbe` with its data files beside it.
2. Run:

   ```bash
   ./build.sh          # release
   ./build.sh debug    # same optimisation, full logging
   ```

   The first build finds the XDK symbols, recompiles `default.xbe` into `generated/`, and compiles it, which takes from a few minutes to an hour depending on the machine. Later builds are incremental.
3. Run `out/build/release/dw5`.

> [!IMPORTANT]
> `config/game.toml` carries the `[abi]` block mapped by hand for the link time code generated D3D8 library (`D3D8LTCG`), which takes its arguments in registers instead of on the stack. `build.sh` keeps the file once it has a `[symbols]` section. Do not delete or regenerate it, or those fixes are lost.

## Controls

A controller works as on the Xbox. The keyboard always plays as controller 1:

| Keyboard | Xbox controller |
| --- | --- |
| Arrow keys | D-pad |
| W A S D | Left stick |
| Numpad 8 4 2 6 | Right stick |
| Enter | Start |
| Esc / Backspace | Back |
| Space / J | A |
| K | B |
| U | X |
| I | Y |
| O | Black |
| L | White |
| Q / E | Left / right trigger |
| Left / right Shift | Left / right stick click |

Keys are set in `src/common/keyboard.cpp`.

## Layout

| path | |
| --- | --- |
| `build.sh` | toolchain check, symbols, cmake + ninja |
| `config/game.toml` | recompiler input: sections, entry points, XDK symbol map, `[abi]` |
| `src/d3d/` | Vulkan backend for Linux and its headers |
| `src/audio/sdl_linux.cpp` | audio output through SDL3 |
| `src/common/` | game info, keyboard mapping, capture signal, Linux versions of Windows-only SDK files |
| `src/game/hooks.cpp` | title-specific function overrides |
| `sdk/` | XboxXBERecomp, with this port's runtime fixes |

## Credits and licensing

- [TGP482](https://github.com/TGP482) for [XboxXBERecomp](https://github.com/TGP482/XboxXBERecomp), the recompiler and runtime SDK this port is built on, kept in `sdk/` with this port's runtime fixes.
- The Vulkan renderer came from the BLACK fork for Linux, [datfooldive/BLACK-XBERecomp-Linux](https://github.com/datfooldive/BLACK-XBERecomp-Linux), forked from [TGP482/BLACK-XBERecomp](https://github.com/TGP482/BLACK-XBERecomp).
- The libraries and references credited in `sdk/README.md`.

The SDK is MIT licensed, see `sdk/LICENSE` and `sdk/THIRD_PARTY.txt`.

This repository contains no game files or recompiled game code. Dynasty Warriors 5 is © 2005 Koei Co., Ltd. This project is not affiliated with or endorsed by Koei.
