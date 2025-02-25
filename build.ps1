cd .\build\
cmake --% .. -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=../toolchain.cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
make
cd ..