ldr r0, =0x20200004
ldr r1, =0x20200028
ldr r2, =0x2020001c
ldr r3, =0x39ffffff
ldr r4, =0x00040000
ldr r5, =0x0000ffff
str r3, [r0]
start:
str r4, [r1]
ldr r6, [r5]
loop1:
sub r6, r6, #1
cmp r6, #0
bne loop1
str r4, [r2]
ldr r6, [r5]
loop2:
sub r6, r6, #1
cmp r6, #0
bne loop2
bal start
