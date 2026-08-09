echo off

set outf=-o MENU_ZX0_DATA.BIN
set mode=-m 0
set set asm=-asmdump -o MENU_ZX0_DATA.ASM

set file=MENU.png
set pal=-impal MENU.pal

if exist MENU_ZX0_DATA.ASM erase MENU_ZX0_DATA.ASM
if exist MENU_ZX0_DATA.BIN erase MENU_ZX0_DATA.BIN

convgeneric.exe %outf% %mode% %file% -scr

pause
