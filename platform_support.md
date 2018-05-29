### Libaudioverse Build Instructions and Platform Support

This document outlines Libaudioverse's support for various platforms and how to build it on each.  If a platform isn't in this document, it's probably planned for the future (this project will run everywhere, one day).  Supported platforms are currently Windows (X86 only) and Linux (X64 and X86).

Instructions are intentionally as basic as possible.  If you know what you're doing, you don't need to follow them to the letter.  If something breaks, however, please follow this document closely before reporting it.  If you prefer something briefer (and more cryptic for new programmers) see `appveyor.yml` (Windows) or `.travis.yml` (Linux) in the root of this repository.

## Dependencies For All Platforms

You need the following on all platforms.  Installation instructions for each platform may be found below:

- Boost.  I test with 1.59.0 and 1.58.0.  Travis CI has 1.54.0 and compiles, but nothing runs the resulting binaries due to lack of the ability to listen to the output.
- Python 3.4 or later.
- CMake 2.8 or later.
- The python packages PyYAML, Pypandoc, Jinja2, pycparser, sphinx, wheel, numpy, and scipy.
- A working installation of pandoc (for some platforms; on Windows, pypandoc bundles it).
- A C++14 capable compiler.

In addition, building the manual requires asciidoctor and ruby.  The manual does not build by default, so lack of these packages should not be detrimental.

## Windows

Please see the [instructions from the official github repository](https://github.com/libaudioverse/libaudioverse/blob/master/platform_support.md). Here I solely focus on Linux since I lack a windows development machine.

## Linux

Linux supports both X86 and X64.  The compiler requirement is gcc 4.9 or later.

At the moment, the only supported audio output is ALSA.  This causes somewhat higher latency for Pulse users.  A Pulse backend will happen in the future.

You cannot currently reasonably run Libaudioverse on ARM.  It does not have Neon optimizations.  Unlike Windows, this configuration may compile, but it will be slow.  ARM platforms are not officially supported or tested and you are on your own if you opt to try it.

### Getting Dependencies

Some distros change the name of some packages, here are for Ubuntu, and Arch Linux

## For Ubuntu (16.04 and newer)

```
sudo apt-get update
sudo apt-get install gcc g++ make cmake python3 libboost-all-dev libsndfile-dev libasound2-dev pandoc python3-numpy python3-scipy python3-pip
sudo python3 --m pip install pycparser jinja2 wheel sphinx pyyaml pypandoc
```
## For Arch Linux
```
sudo pacman -S gcc make cmake python boost libsndfile alsa-lib pandoc python-numpy python-scipy python-pip python-pycparser python-jinja python-wheel python-sphinx python-pyaml python-pypandoc 
```

### Building/Installing

```
git clone https://github.com/mmxgn/libaudioverse.git --recursive
cd libaudioverse
mkdir build
cd build
cmake ..
make
sudo make install
```

For python buildings (in ```build``` directory):

```
cd bindings/python
sudo python3 setup.py build install
```

### Testing

In root libaudioverse directory:

```
./examples/automation
./utils/time_convolution default
```

As with Windows, the first should give audio output and the second should give at least 100 sources as an estimate (at least on Desktops).
