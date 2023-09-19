#!/bin/bash
wget -P .. https://github.com/mouad-suuu/alx-low_level_programming/blob/1eba3143472f904cd9ade455c0a59fec6daa9a49/0x18-dynamic_libraries/libgiga.so
export LD_PRELOAD="$PWD/../libgiga.so"
