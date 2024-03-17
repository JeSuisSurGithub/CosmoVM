#!/bin/bash

set -e

cd build

echo Building stage 1...
../../build/cosmoemu example ../boot.asm ../user_io.asm ../disk_io.asm

echo Creating blank disk of 4x512 bytes...
dd if=/dev/zero bs=512 count=4 of=disk.bin
echo Writing binary to boot sector...
dd if=example.bin of=disk.bin conv=notrunc

echo Building stage 2...
../../build/cosmoemu stage2 ../stage2.asm ../user_io.asm ../general.asm ../clock.asm
echo Writing binary to second sector...
dd if=stage2.bin of=disk.bin conv=notrunc bs=512 seek=1

echo Copying disk.bin
cp disk.bin ../../build/
cd ..