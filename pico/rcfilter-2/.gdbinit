cd build

# quit without confirmation
define hook-quit
    set confirm off 
end

file app.elf
target remote localhost:3333
load
monitor reset init
#b main.cc:48
echo Type c to continue...\n

