@echo off
echo Compiling...
g++ framework.cpp -o my_map_app.exe -Ilibs/include/SDL2 -Llibs/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf
echo Done!