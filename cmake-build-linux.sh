cmake -B build -S . -DCMAKE_BUILD_TYPE="Release"
cmake --build build -j $(nproc)
cp disk.bin build
cp vgafont.ttf build