Set-Location -Path F:\Code\C\Breakout\controls_test_suite
gcc controls_test_suite.c -o controlexample.exe -C -Wall -ggdb3 -O0 -std=c99 -Wno-missing-braces -I include/ -L lib/ -lraylib -lopengl32 -lgdi32 -lwinmm
& "F:\Code\C\Breakout\controls_test_suite\controlexample.exe" -recurse -force  /run /exit
Start-Sleep -s 3