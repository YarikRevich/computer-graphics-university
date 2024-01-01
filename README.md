# computer-graphics-university

[![Build](https://github.com/YarikRevich/computer-graphics-university/actions/workflows/build.yml/badge.svg)](https://github.com/YarikRevich/computer-graphics-university/actions/workflows/build.yml)
[![StandWithUkraine](https://raw.githubusercontent.com/vshymanskyy/StandWithUkraine/main/badges/StandWithUkraine.svg)](https://github.com/vshymanskyy/StandWithUkraine/blob/main/docs/README.md)

## General Information

Media codec with the possibility of result representation.

## Setup

All setup related operations are processed via **Makefile**(which operates **CMake** configurations) placed in the root directory.

First of all, it's important to install prerequisites. It can be done via the following command.
```shell
make prepare
```

In order to build the application into project local **bin** library it's required to execute the following command.
```shell
make build
```

### Prerequisites

In the case of the usage with MacOS operational systems it's important to remember to allow the usage of SDL2 library on your local machine using privacy settings. 

### Use cases

#### Encoding

In order to encode input image, it should be of the supported extension type:
* **jpeg**
* **jpg**
* **bmp**

Codec obliges to select **conversion** mode during encoding operation:
* **native_rgb**
* **native_bw**
* **palette_rgb**
* **palette_bw**

The encoding for **bmp-to-cgu** with **palette_rgb** command looks like this:

```shell
./bin/cgu encode --from=<input>.bmp --type=bmp --conversion=palette_rgb --to=<output>.cgu
```

#### Decoding

In order to decode input image, it should be of **CGU** extension.

Output image can be one of the following extensions:
* **jpeg**
* **jpg**
* **bmp**

Codec obliges to select **conversion** mode during decoding operation:
* **native_rgb**
* **native_bw**
* **palette_rgb**
* **palette_bw**

The encoding for **bmp-to-cgu** with **palette_rgb** command looks like this:

```shell
./bin/cgu decode --from=<input>.cgu --type=bmp --conversion=palette_rgb --to=<output>.bmp
```

#### Viewer

Viewer mode should be used in order to open **CGU** encoded images. Also it can be used for 
the view of the following image extensions:
* **jpeg**
* **jpg**
* **bmp**

The viewer command for CGU encoded image looks like this:

```shell
./bin/cgu view --from=<input>.cgu
```