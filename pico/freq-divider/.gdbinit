cd build
file app.elf
target remote localhost:3333
load
monitor reset init
b main:32
#b main.cc:50
echo Type c to continue...\n

