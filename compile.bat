@echo off
echo Compiling K-D Tree SDL Application...
g++ framework.cpp KD-Tree\kd_tree.cpp Quad-Tree\quadtree.cpp -o my_map_app.exe -Ilibs/include/SDL2 -Llibs/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf
if %ERRORLEVEL% EQU 0 (
    echo Compilation successful! 
) else (
    echo Compilation failed! Check errors above.
)