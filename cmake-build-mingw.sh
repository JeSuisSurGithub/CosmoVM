cmake -B build -S . -DCMAKE_BUILD_TYPE="Release" -G"MinGW Makefiles"
cmake --build build -j $(nproc)
cp disk.bin build
cp vgafont.ttf build