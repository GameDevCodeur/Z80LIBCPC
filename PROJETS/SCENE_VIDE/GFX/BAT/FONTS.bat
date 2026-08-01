






echo off

set outf=-o BIN\FONTS.BIN
set mode=-m 0

REM TAILLE PIXELS SPRITE
set size=-size 04x07
set asm=-asmdump -o ASM\FONTS.ASM

REM NOMBRE DE SPRITES
set nbsp=-c 26
set offset=-offset 0,0 -cpccolor -noalpha

set file=GFX\FONTS.png
set pal=-impal GFX\FONTS_.pal

if exist ASM\FONTS.ASM erase ASM\FONTS.ASM
if exist BIN\FONTS.BIN erase BIN\FONTS.BIN

exe\convgeneric.exe %file% %outf% %asm% %size% %nbsp% %mode% %offset%

pause
