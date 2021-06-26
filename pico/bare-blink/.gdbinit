# quit without confirmation
define hook-quit
    set confirm off 
end

file app.elf
target remote localhost:3333
load
monitor reset init
#b main.c:64
echo Type c to continue...\n

