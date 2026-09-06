@echo off

REM EFFACE LA DISQUETTE
REM if exist DSK\30.DSK erase DSK\dgc.DSK

REM COMPILE & CRE UNE DISQUETTE
REM -eo : Ecraser les fichiers présents sur la disquette en modification.
REM -s  : Exporter les symboles.
REM -sl : Exporter les labels.
REM -sq : Exporter les equivalences.
REM -fq : UTF8 

ASM\rasm_w64.exe MAIN.ASM -eo -s -sl -sq -fq
EMU\CFE26_4_7\Caprice.exe ..\..\DSK\PONG.DSK
