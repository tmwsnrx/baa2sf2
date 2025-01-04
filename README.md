# baa2sf2

Converts Twilight Princess Audio Archive files (BAA) to SoundFonts. This *may* work for other games as well, but it's only been tested with *Z2Sound.baa*.

⚠️ **Despite already working quite well, this is still a work in progress and may not yield perfect results.** 

## Credits

This project is the result of various reverse engineering efforts and the work of many people, especially from the [HCS forum](https://hcs64.com/mboard/forum.php).

## Dependencies

- [CMake](https://cmake.org/)
- [POCO](https://pocoproject.org/)

## Building

First, you need to download and install the POCO library. See [here](https://pocoproject.org/download.html) for instructions.

Then, you can clone the project and build it with CMake:

```bash
git clone https://github.com/tmwsnrx/baa2sf2 --recursive
cd baa2sf2
mkdir build
cd build
cmake ..
cmake --build .
```

## Usage

You need the game's extracted *Audiores* folder that contains `Z2Sound.baa` and the `Waves` folder. Then run:

```bash
baa2sf2 <YourPath>/Audiores/Z2Sound.baa <YourPath>/path/to/Audiores/Waves <OutputPath>
```

Since the game has two sound banks this will create two files in the output folder: `Z2Sound_0.sf2` (SFX) and `Z2Sound_1.sf2` (BGM).

## Unimplemented Features

- Attack envelopes