# Survey Designer

Survey Designer is a C++20 command-line tool for generating survey line and point schemes from polygon shapefiles.

## Build Support

The build is tested on Linux and Windows with the Ninja generator.

## Requirements

- CMake 3.25 or newer
- Ninja
- A C++20 compiler
- Git, for fetching the shapelib dependency during configuration

## Build

Configure and build with one of the provided CMake presets:

```sh
cmake --preset debug
cmake --build --preset debug
```

For a release build:

```sh
cmake --preset release
cmake --build --preset release
```

The executable is built as `survey-designer`.

## Usage

Conventional survey:

```sh
survey-designer -c -b -dl=50 -a=45 -ds=10 examples/input/polygon examples/output/scheme
```

Hexagonal survey:

```sh
survey-designer -h -ds=50 examples/input/polygon examples/output/points_hex
```

Input and output shapefile paths should be provided without the file extension.

## Options

- `-c`: create a conventional survey
- `-h`: create a hexagonal survey
- `-l`: save lines only
- `-p`: save points only
- `-b`: save lines and points
- `-dl=<value>`: line spacing for conventional surveys
- `-a=<degrees>`: line azimuth for conventional surveys
- `-ds=<value>`: station spacing
