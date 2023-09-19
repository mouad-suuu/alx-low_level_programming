#!/bin/bash
wget -q --output-document=$HOME/lib101-makemewin.so https://github.com/mouad-suuu/alx-low_level_programming/blob/5870bc66237d2e702f0246361d306a58dc344aab/0x18-dynamic_libraries/lib101-makemewin.so 
export LD_PRELOAD=$HOME/lib101-makemewin.so
