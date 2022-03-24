Set-Location -Path F:\Code\C\FlappyClone
gcc main.c brkout.c -o flappyclone.exe -C -Wall -ggdb3 -O0 -std=c99 -Wno-missing-braces -I include/ -L lib/ -lraylib -lopengl32 -lgdi32 -lwinmm
& "F:\Code\C\FlappyClone\flappyclone.exe" -recurse -force  /run /exit
Start-Sleep -s 3