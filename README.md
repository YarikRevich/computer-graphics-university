# computer-graphics-university

[![Build](https://github.com/YarikRevich/computer-graphics-univeristy/actions/workflows/build.yml/badge.svg)](https://github.com/YarikRevich/computer-graphics-university/actions/workflows/build.yml)
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