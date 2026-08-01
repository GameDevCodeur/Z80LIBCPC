@echo off

REM EFFACE LA DISQUETTE
REM if exist DSK\VIDE.DSK erase DSK\dgc.DSK

REM -eo : Ecraser les fichiers présents sur la disquette en modification.
REM -s  : Exporter les symboles.
REM -sl : Exporter les labels.
REM -sq : Exporter les equivalences.
REM -fq : UTF8 

ASM\RASM\rasm_w64.exe MAIN.ASM -eo -s -sl -sq -fq

REM LANCE LA DISQUETTE DANS L'EMULATEUR
EMU\CFE26_4_7\Caprice.exe ..\..\DSK\VIDE.DSK
