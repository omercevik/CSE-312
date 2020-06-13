#       CSE 312 Operating Systems
#       Homework 03
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
    # Prints prompt
    li $v0, 4
    la $a0, binarySearchPrompt
    syscall

    # Reads input from user with syscall
    li $v0, 24
    syscall
    sw $t0, input

    # Inits iterating loop
    la $t7, arr
    lw $t1, size
    li $s2, 0
    lw $s3, input
    move $s4, $t1

    # Finds middle index of array
    li $t0, 2
    div $t1, $t0
    mflo $t2
    move $t3, $t2

    # Finds first middle element of array using index
firstMid:

    addi $t7, $t7, 4
    addi $t2, $t2, -1

    bnez $t2, firstMid

    move $t2, $t3

    # Finds all middle elements searching as Binary search
findMid:
    lw $t5, 0($t7)

    # Checks the middle element is equal to element then prints found
    beq $t5, $s3, printFound

    # Checks if the end of the element of array then print not found
    addi $s4, $s4, -1
    beqz $s4, printNotFound

    # Checks which side should go left or right
    slt $t4, $t5, $s3

    # If element is smaller than middle element then go left
    beqz $t4, goLeft

    # If element is greater than middle element then go right
goRight:

    # Find the new middle element at right
    move $s2, $t2           # s2 is mid
    add $t2, $t1, $s2       # t2 = end + mid / 2
    li $t0, 2
    div $t2, $t0
    mflo $t2
    move $t3, $t2

    # Iterate to right in array
right:
    # Check if it is the element then print it
    beq $t2, $s2, b1

    addi $t7, $t7, 4
    addi $t2, $t2, -1

    # Keep iterate
    bne $t2, $s2, right

b1:
    move $t2, $t3

    # Keep find another middle in right
    j findMid

    # If element is smaller than middle element then go right
goLeft:

    # Find the new middle element at left
    move $t1, $t2
    add $t2, $t1, $s2
    li $t0, 2
    div $t2, $t0
    mflo $t2
    move $t3, $t2

    # Iterate to left in array
left:
    # Check if it is the element then print it
    beq $t2, $t1, b2

    addi $t7, $t7, -4
    addi $t2, $t2, 1

    # Keep iterate
    bne $t2, $t1, left

b2:
    move $t2, $t3

    # Keep find another middle in left
    j findMid


printNotFound:

    # Prints prompt not found and exit
    li $v0, 4
    la $a0, notFoundBinary
    syscall

    j halt

printFound:

    # Prints prompt found
    li $v0, 4
    la $a0, foundBinary
    syscall

    # Prints index of found element
    li $v0, 1
    move $a0, $t2
    syscall

    # Prints newline
    li $v0, 4
    la $a0, newl
    syscall

    # Exits
halt:
    li $v0, 22
    syscall