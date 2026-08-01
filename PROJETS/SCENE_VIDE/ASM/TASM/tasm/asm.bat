echo Syntax: asm [NAME (w/o extension)] [PATH]

@echo off
echo ----- Assembling %1 for the TI-83 Plus...
echo #define TI83P >temp.z80
cd "..\source"
if exist %1.z80 type %1.z80 >>temp.z80
if exist %1.asm type %1.asm >>temp.z80
move /y temp.z80 "../tasm"
cd "..\tasm"
tasm -80 -i -b -l temp.z80 %1.bin %2%1.xlt
if errorlevel 1 goto ERRORS
devpac8x %1
copy %1.8xp %2%1.8xp >nul
echo TI-83 Plus version is %1.8xp
move %1.8xp "..\exec"
goto DONE
:ERRORS
echo ----- There were errors.
:DONE
del temp.z80 >nul
del %1.bin >nul
del %1.xlt >nul