echo off

set outf=-o BIN\SCR_TITTLE.BIN
set mode=-m 0
set size=-size 04x05
set asm=-asmdump -o ASM\SCR_TITTLE.ASM
set nbsp=-c 40
set offset=-offset 1,0 -cpccolor -noalpha
set file=GFX\SCR_TITTLE.png
set pal=-impal GFX\SCR_TITTLE_.pal

if exist ASM\SCR_TITTLE.ASM erase ASM\SCR_TITTLE.ASM
if exist BIN\SCR_TITTLE.BIN erase BIN\SCR_TITTLE.BIN

exe\convgeneric.exe %outf% %mode% %file% -scr

pause
