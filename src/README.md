# Make commands

`make` builds everything
`make run` builds everything and runs the combined server/client program
`make test` builds everything+tests and runs all tests
`make test <test regex>` builds everything+tests and then runs tests with the provided regex

There are other make commands.  Look at the makefile to see what they do.

# Installation: Windows (MSYS2)

This project was primarily developed on Windows.  It should work on other systems though.

Requires MSYS2, (MINGW64 or UCRT64).  For Mingw use the following:

```
pacman -S base-devel vim cmake git
pacman -S mingw64/mingw-w64-x86_64-gcc mingw64/mingw-w64-x86_64-gdb
pacman -S mingw64/mingw-w64-x86_64-SDL2 mingw64/mingw-w64-x86_64-SDL2_image mingw64/mingw-w64-x86_64-SDL2_mixer mingw64/mingw-w64-x86_64-SDL2_ttf
# For some reason this isn't common knowledge (on google)? but the lld linker is insanely
# fast compared to the standard one installed with gcc in MinGW on windows.
# The following command installs this faster linker, and to use it, the makefile
# specifies '-fuse-ld=lld'.  If for whatever reason this doesn't work for you, then
# don't worry about the command and remove this argument from the compile commands.
# But seriously, as of the early parts of this project, my link time went from ~5s to ~200ms
# on a ryzen9 4900HS.
# see https://lld.llvm.org/
pacman -S mingw64/mingw-w64-x86_64-lld
```

Install enet for server/client.  Technically optional, you can compile without this, but it's not
that big and recommended.

```
pacman -S mingw64/mingw-w64-x86_64-enet
```

Gtest for testing.

```
pacman -S mingw64/mingw-w64-x86_64-gtest
```

Clang for formatting/linting/tooling

```
pacman -S mingw-w64-x86_64-clang
pacman -S mingw-w64-x86_64-clang-tools-extra
pacman -S mingw-w64-x86_64-include-what-you-use
```

_very optional_ Python2 For some lib dependencies

```
pacman -S mingw64/mingw-w64-x86_64-python2-pip mingw64/mingw-w64-x86_64-python2
```

## Using ucrt64 instead of mingw64

UCRT links the executable under the Universal C Run Time instead of the old Microsoft one
that mingw uses.  It's not really that different; there are some compatibility concerns,
but it's included here for some completeness since UCRT seems to be better for future concerns.

```
pacman -S base-devel vim cmake git
pacman -S ucrt64/mingw-w64-ucrt-x86_64-gcc ucrt64/mingw-w64-ucrt-x86_64-gdb
pacman -S ucrt64/mingw-w64-ucrt-x86_64-SDL2  ucrt64/mingw-w64-ucrt-x86_64-SDL2_image ucrt64/mingw-w64-ucrt-x86_64-SDL2_mixer ucrt64/mingw-w64-ucrt-x86_64-SDL2_ttf
pacman -S ucrt64/mingw-w64-ucrt-x86_64-lld
```

enet

```
pacman -S ucrt64/mingw-w64-ucrt-x86_64-enet
```

Gtest

```
pacman -S ucrt64/mingw-w64-ucrt-x86_64-gtest
```

Clang for formatting/linting/tooling

```
pacman -S ucrt64/mingw-w64-ucrt-x86_64-clang
pacman -S ucrt64/mingw-w64-ucrt-x86_64-clang-tools-extra
pacman -S ucrt64/mingw-w64-ucrt-x86_64-include-what-you-use
```

_optional_ Python2 For some lib dependencies

```
pacman -S ucrt64/mingw-w64-ucrt-x86_64-python2-pip ucrt64/mingw-w64-ucrt-x86_64-python2
```

# Installation Ubuntu:

WARNING this section is incomplete.

Install build env (if you don't have these already for some reason):

```
sudo apt-get update
sudo apt install build-essential
sudo apt install make cmake zip unzip
```

The version of SDL2 on ubuntu's apt is too old to work with imgui sdl impl, so you need to install
it manually.

```
sudo apt install cmake
cd /tmp
git clone https://github.com/libsdl-org/SDL
cd SDL
# main branch is in constant development for SDL3 (as of time of writing)
git checkout SDL2
mkdir -p build
cd build
../configure
make -j8
sudo make install
```

Other SDL libs are fine to use apt versions.

```
sudo apt update
sudo apt install libsdl2-ttf-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-gfx-dev -y
```

enet

```
sudo apt-get install enet-dev
```

Gtest

```
sudo apt-get install libgtest-dev -y
cd /usr/src/googletest/googletest
sudo mkdir build
cd build
sudo cmake ..
sudo make
sudo cp libgtest* /usr/lib/
cd ..
sudo rm -rf build
sudo mkdir /usr/local/lib/googletest
sudo ln -s /usr/lib/libgtest.a /usr/local/lib/googletest/libgtest.a
sudo ln -s /usr/lib/libgtest_main.a /usr/local/lib/googletest/libgtest_main.a
```

# Mac M1^ (brew)

```
brew install gcc@13
brew install brew install sdl2 sdl2_image sdl2_mixer sdl2_ttf
```

gtest
```
git clone https://github.com/google/googletest.git
cd googletest
mkdir build
cd build
cmake .. -DCMAKE_OSX_ARCHITECTURES=arm64
make
sudo make install
```

clangd and req tools
```
python3 -m pip install --upgrade setuptools
python3 -m pip install --upgrade pip
```

# 3rd Party Libs Included In Repo

Should you ever need to re-generate 3rd party libs:

duktape

```
# If you need python 2 in mingw64
pacman -S mingw64/mingw-w64-x86_64-python2-pip mingw64/mingw-w64-x86_64-python2
# or pacman -S ucrt64/mingw-w64-x86_64-python2-pip ucrt64/mingw-w64-x86_64-python2
pip2 install PyYAML

cd duktape/duktape-2.7.0
python2 tools/configure.py --output-directory duktape-src -UDUK_USE_ES6_PROXY
cp duktape-src/duk_config.h ../
cp duktape-src/duktape.h ../
cp duktape-src/duktape.c ../duktape.cpp
cp duktape-src/duk_source_meta.json ../
```

# Clangd In VsCode

Clangd language server is much faster and more efficient than the C++ tools extension in vscode.
This project has files that work with the default C++ extension, but I've found that the clangd,
extension is quite a lot a bit better.  Perhaps not for debugging though...

Requires Python.
Requires clang tools from above to be installed.

Use https://github.com/nickdiego/compiledb to generate the json file clangd needs to debug.

Install with 

```
pip install compiledb
OR
python3 -m pip install compiledb
```

Then run

```
compiledb make
OR
python3 -m compiledb make
```

NOTE: You do NOT need to compile every src file individually, just the make command with the executable works fine.

You should now have a `compile_commands.json` file.

This compile_commands.json file should NOT be added to git (it contains paths relative to your machine).

Finally you can install the clangd extension for vscode.

You'll likely need to generate the compile_commands.json file for every time you add a new file.

# Emscripten & WASM Browser Version

To build the wasm version, run `make js`

NOTE this command assumes there's a folder named `dist` in a directory above.  Typically the Emscripten is run from the package.json file in the above directory, so