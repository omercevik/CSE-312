#       CSE 312 Operating Systems
#       Homework 01
#       LinearSearch.asm
#       Omer CEVIK
#       161044004

.data

prompt: .asciiz "Values : 100, 20, 50, 40, 45, 83, 6, 9\nEnter a number to search: "
resultPositive: .asciiz "Number found at : "
resultNegative: .asciiz "Number not found!\n"
datas: .word 100, 20, 50, 40, 45, 83, 6, 9
nl: .asciiz "\n"

.text
.globl LinearSearch

LinearSearch:
    la $s1, datas

    li $t1, 0
    li $t2, 1
    li $t3, 20

    li $v0, 4
    la $a0, prompt
    syscall

    li $v0, 5
    syscall

    move $t0, $v0


searchLoop:
    beq $t1, $t3, negativeRes
    lw $t4, 0($s1)

    beq $t0, $t4, positiveRes

continueLoop:

    addi $s1, $s1, 4
    addi $t1, $t1, 1

    j searchLoop

positiveRes:
    li $v0, 4
    la $a0, resultPositive
    syscall

    li $v0, 1
    move $a0, $t1
    syscall

    li $v0, 4
    la $a0, nl
    syscall

    li $t5, 1

    j continueLoop

negativeRes:
    beq $t5, $t2, final

    li $v0, 4
    la $a0, resultNegative
    syscall

    j final

final:
    j main
