cmake_minimum_required(VERSION 3.5)

project(cw-gfx-engine)

idf_component_register(
                       INCLUDE_DIRS "."
                       SRC_DIRS "." ColorUtil.h
                       REQUIRES arduino Adafruit-GFX-Library
                       )
