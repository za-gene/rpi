/* Inspired from
https://smist08.wordpress.com/2021/04/16/assembly-language-on-the-raspberry-pi-pico/
*/

.cpu cortex-m0plus
.thumb
/*
@ .syntax unified
 .fpu softvfp 
@ .thumb
*/

.section .boot_loader
Reset_Handler:
	bl main


/*
.thumb_func
.global main_asm
.align 4
main_asm:
BL main  
*/

.data
.align 4
