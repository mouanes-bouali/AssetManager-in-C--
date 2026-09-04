@echo off
setlocal

if not exist build mkdir build
cd build

cmake ..
cmake --build . --config Release

cd ..
echo.
echo Build complete.
pause
