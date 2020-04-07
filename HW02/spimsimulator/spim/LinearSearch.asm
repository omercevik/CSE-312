#       CSE 312 Operating Systems
#       Homework 02
#       LinearSearch.asm
#       Omer CEVIK
#       161044004

.data

linearSearchPrompt: .asciiz "LinearSearch.asm:\nArray : {10, 20, 80, 30, 60, 50, 110, 100, 130, 170}\nEnter a number to search: "
notFoundLinear: .asciiz "LinearSearch.asm returns -1\n"
foundLinear: .asciiz "LinearSearch.asm found index at : "
nl: .asciiz "\n"
datas: .word 10, 20, 80, 30, 60, 50, 110, 100, 130, 170

.text
.globl mainLinearSearch

mainLinearSearch:

    la $t1, datas

    li $t7, 0
    li $t2, 1
    li $t3, 20

    li $v0, 4
    la $a0, linearSearchPrompt
    syscall

    li $v0, 24
    syscall

searchLoop:

    beq $t7, $t3, negativeRes
    lw $t4, 0($t1)

    beq $t0, $t4, positiveRes

continueLoop:

    addi $t1, $t1, 4
    addi $t7, $t7, 1

    j searchLoop

positiveRes:

    li $v0, 4
    la $a0, foundLinear
    syscall

    li $v0, 1
    move $a0, $t7
    syscall

    li $v0, 4
    la $a0, nl
    syscall

    li $t5, 1

    j continueLoop

negativeRes:
    beq $t5, $t2, final

    li $v0, 4
    la $a0, notFoundLinear
    syscall

    j final

final:
    li $v0, 22
    syscall