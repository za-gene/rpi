.section rodata
.global mydata
.type mydata, @object
.align 4

mydata:
.incbin "data.bin"

.global mydata_size
.type mydata_size, @object
.align 4
mydata_size:
.int mydata_size - mydata
