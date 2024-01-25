# computer-graphics-university

[![Build](https://github.com/YarikRevich/computer-graphics-university/actions/workflows/build.yml/badge.svg)](https://github.com/YarikRevich/computer-graphics-university/actions/workflows/build.yml)
[![StandWithUkraine](https://raw.githubusercontent.com/vshymanskyy/StandWithUkraine/main/badges/StandWithUkraine.svg)](https://github.com/vshymanskyy/StandWithUkraine/blob/main/docs/README.md)

## General Information

Media codec with the possibility of result representation.

![](./docs/detailed-design.png)

## Setup

### Prerequisites

In the case of the usage with MacOS operational systems it's important to remember to allow the usage of SDL2 library on your local machine using privacy settings. 

### Installation
All setup related operations are processed via **Makefile**(which operates **CMake** configurations) placed in the root directory.

In order to build the application into project local **bin** library it's required to execute the following command.
```shell
make build
```

## Specification

### Metadata

All the output images of CGU type have **metadata** part, which is located after **EOF** flag. It's used for internal CGU properties
interpretation and its further usage as parameters for different kind of operations.

**It's important to remember, that all the metadata values are defined with the new line flag!**

All the available **metadata** flags:

| Name      | Size | Description | Mandatory |
| ----------- | ----------- | ----------- | ----------- |
| Compatibility      |  2 byte  |       Compatibility flag, which is required to be present, <br />so CGU can perform decode operation   |    <center>true</center>     |
| Conversion   |  4 byte  |      Describes conversion type used for image encoding       |   <center>true</center>    |
| Data size   |    4 byte     |    Represents size of the given data(**indeces** or **compounds**)          |   <center>false</center>   | 
| Indeces   |  4 byte * N        |    Represents indeces generated for palette conversions          |   <center>false</center>   | 
| Compounds   | 1 byte * N        |     Represents compounds generated for native conversions        |   <center>false</center>   | 

### Native conversion

Every pixel has **7-bit** color. That color consists of the following compounds:
* 

> #### RGB

> #### BW

### Palette conversion

In order to perform color conversion it's required to have generated reduced bit color map of the colors
of the given image.

> #### Detected palette conversion

Detected palette conversion uses previously generate reduced bit color map, but, first of all, it checks the number of unique colors. It should not exceed **128** colors. 

The next step is detected palette rendering. Each color will be placed in a rectangle, which are evenly spaced in the whole image surface.

// TODO: place the example photo here.

> #### MedianCut

**MedianCut** is the algorithm used for the color quantization operation. With the help of the generated **7-bit** color palette it can process the given image and return the generated one.

// TODO: place the example photo here. 

## Use cases

### Encoding

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

### Decoding

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

### Viewer

Viewer mode should be used in order to open **CGU** encoded images. Also it can be used for 
the view of the following image extensions:
* **jpeg**
* **jpg**
* **bmp**

The viewer command for CGU encoded image looks like this:

```shell
./bin/cgu view --from=<input>.cgu
```