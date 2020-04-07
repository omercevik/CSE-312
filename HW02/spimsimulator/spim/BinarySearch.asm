#       CSE 312 Operating Systems
#       Homework 02
#       BinarySearch.asm
#       Omer CEVIK
#       161044004

.data

binarySearchPrompt: .asciiz "BinarySearch.asm:\nArray : {10, 20, 30, 50, 60, 80, 100, 110, 130, 170}\nEnter a number to search: "
notFoundBinary: .asciiz "BinarySearch.asm returns -1\n"
foundBinary: .asciiz "BinarySearch.asm found index at : "
newl: .asciiz "\n"
arr: .word 10, 20, 30, 50, 60, 80, 100, 110, 130, 170
size: .word 10
input: .word 0

.text
.globl mainBinarySearch

mainBinarySearch:

    li $v0, 4
    la $a0, binarySearchPrompt
    syscall

    li $v0, 24
    syscall
    sw $t0, input

    la $t7, arr
    lw $t1, size
    li $s2, 0
    lw $s3, input
    move $s4, $t1

    li $t0, 2
    div $t1, $t0
    mflo $t2
    move $t3, $t2

firstMid:

    addi $t7, $t7, 4
    addi $t2, $t2, -1

    bnez $t2, firstMid

    move $t2, $t3


findMid:
    lw $t5, 0($t7)

    beq $t5, $s3, printFound

    addi $s4, $s4, -1
    beqz $s4, printNotFound

    slt $t4, $t5, $s3

    beqz $t4, goLeft


goRight:

    move $s2, $t2           # s2 is mid
    add $t2, $t1, $s2       # t2 = end + mid / 2
    li $t0, 2
    div $t2, $t0
    mflo $t2
    move $t3, $t2

right:
    beq $t2, $s2, b1

    addi $t7, $t7, 4
    addi $t2, $t2, -1

    bne $t2, $s2, right

b1:
    move $t2, $t3


    j findMid

goLeft:

    move $t1, $t2
    add $t2, $t1, $s2
    li $t0, 2
    div $t2, $t0
    mflo $t2
    move $t3, $t2

left:
    beq $t2, $t1, b2

    addi $t7, $t7, -4
    addi $t2, $t2, 1

    bne $t2, $t1, left

b2:
    move $t2, $t3

    j findMid


printNotFound:
    li $v0, 4
    la $a0, notFoundBinary
    syscall

    j halt

printFound:
    li $v0, 4
    la $a0, foundBinary
    syscall

    li $v0, 1
    move $a0, $t2
    syscall

    li $v0, 4
    la $a0, newl
    syscall

halt:
    li $v0, 22
    syscall