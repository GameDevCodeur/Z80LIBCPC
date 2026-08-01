
general options:
-o <file>        full output filename
-flat            output one file for all extractions
-split <size>    split output files with size ex: 4K or 4096
-m <mode>        output mode (0,1,2)
-g               sort palette from darkest to brightest color
-asmdump         output assembly dump
-exnfo <file>    export assembly informations about extracted zones
-expal <file>    export palette in a text file
-impal <file>    import palette from a text file
-cpccolor        round a little colorz for CPC
-noalpha         do not extract transparency map

demomaking options:
-rotoffset  <file> export rototexture for rotoffset
-rotoheight <file> export rotoheightmap for rotoffset
-heightmap  <file> export heightmap

sprite options: (default progressive output)
-scan             scan sprites inside border
-fontscan         font extraction
-single           only one pixel per byte to the right side
-size <geometry>  set sprite dimensions in pixels. Ex: -size 16x16
-scrz <geometry>  set screen dimensions in pixels. Ex: -size 320x200
-splitLowHigh     export low bytes then high bytes for tilemap
-tilexclude <n>   exclude tile number n for tilemap
-offset <pos>     set start offset from top/left ex: 20,2
-mask             add mask info to data (see doc)
-c <maxsprite>    maximum number of sprites to extract
-meta <geometry>  gather sprites when scanning. Ex: -meta 3x2
-tiles            extract uniques sprites + map
-metatiles <size> create metatiles translation table

screen options:
-scr             enable screen output (interlaced data)
-old             enable overscan screen output (interlaced data)
-lb <nblines>    number of lines per block for screen output (for CPC+ split)
-ls <nblines>    number of lines of 1st screen in extended screen output
-w <width>       force output screen width in bytes
-splitraster     split-raster analysis (unfinished)
-rastaline       allow more color in global image and generate raster info

hardware sprite options:
-hsp             enable hardware sprite mode
-meta <nxn>      extraction of meta sprites
-scan            scan sprites inside border
-b               black is transparency (keep real transparency)
-st              search transparency color
-p 4             store data 4bits+4bits in reverse order into a single byte
-p 2             store data 2+2+2+2bits in logical order into a single byte
-force           force extraction of incomplete sprites
-k               keep empty sprites
