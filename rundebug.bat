@echo off
cd asm
echo Start In Assembler
g++ assembler.cpp reading.cpp writing.cpp -Wall -Wextra
.\a.exe
cd ..
cd calc
echo Moved To Processor
g++ calc.cpp stack.cpp commands.cpp readswitch.cpp -Wall -Wextra
.\a.exe
echo Finish
pause
