#!/bin/bash
wget -P /tmp https://github.com/mouad-suuu/alx-low_level_programming/blob/5870bc66237d2e702f0246361d306a58dc344aab/0x18-dynamic_libraries/libgiga.so 
export LD_PRELOAD=/tmp/libgiga.so
