# Cherry

Cherry is an embeddable scripting language. This repo provides the SDK and compiler source code. It may be split into two separate repos in the future, as I want it to be embedded but it may also be nice to have a standalone compiler and virtual machine for it. In the future it may also support AOT compilation and native binaries. I'm still feeling out the actual requirements I have.

## Requirements

This project utilizes a few features from C++23, so you'll need a compatible compiler. GCC 14 currently supports the C++23 features I needed so that is the compiler that I'm using. Other than that, you'll need CMake 3.16 or higher for precompiled header support.

## Motivation

I'm not happy with current scripting language solutions, and I have a few uses for one, so I'm building one. Simple as that! I would however like this to be comparable, if not faster than, Lua. I'm not far enough along to have benchmarks but once I do, I'll make sure to provide those.