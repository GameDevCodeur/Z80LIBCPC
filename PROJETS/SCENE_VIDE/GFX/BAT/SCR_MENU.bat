echo off

set outf=-o BIN\SCR_MENU.BIN
set mode=-m 0
set size=-size 04x05
set asm=-asmdump -o ASM\SCR_MENU.ASM
set nbsp=-c 40
set offset=-offset 1,0 -cpccolor -noalpha
set file=GFX\SCR_MENU.png
set pal=-impal GFX\SCR_MENU_.pal

if exist ASM\SCR_MENU.ASM erase ASM\SCR_MENU.ASM
if exist BIN\SCR_MENU.BIN erase BIN\SCR_MENU.BIN

exe\convgeneric.exe %outf% %mode% %file% -scr

pause
