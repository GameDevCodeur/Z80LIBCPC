echo off

set outf=-o TITRE_ZX0_DATA.BIN
set mode=-m 0
set file=TITRE.png
set pal=-impal TITRE.pal

if exist TITRE_ZX0_DATA.BIN erase TITRE_ZX0_DATA.BIN

convgeneric.exe %outf% %mode% %file% -scr

pause
