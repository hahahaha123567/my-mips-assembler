addi $s0, $zero, 0
addi $t0, $zero, 15
loop:
add $s0, $s0, $t0
addi $t0, $t0, -1
bne $t0, $zero, loop