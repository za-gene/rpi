cd build
file app.elf
target remote localhost:3333
load
monitor reset init
echo Type c to continue

