cmake_minimum_required(VERSION 3.5)



idf_component_register(
                       INCLUDE_DIRS "."
                       SRC_DIRS "."
                       PUBLIC_HEADERS "ColorUtil.h"
                       REQUIRES arduino Adafruit-GFX-Library
                       )
