cmake -B build
cmake --build build && robocopy "build/bin/Debug/" "dist/"  