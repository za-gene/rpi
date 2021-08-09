cd build

# quit without confirmation
define hook-quit
    set confirm off 
end

set pagination off
file app.elf
target remote localhost:3333
load
monitor reset init
echo RUNNING...\n
c
