REM @ECHO OFF
DEL a.exe
cls
g++-3 src/*.cpp -o a.exe -O0
a.exe