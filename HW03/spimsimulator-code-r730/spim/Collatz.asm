#       CSE 312 Operating Systems
#       Homework 03
#       Collatz.asm
#       Omer CEVIK
#       161044004

.data

space: .asciiz " "
dotdot: .asciiz " :"
newline: .asciiz "\n"
collatzPrompt: .asciiz "Collatz.asm:\n"

.text
.globl mainCollatz

mainCollatz:
    # Prints prompt
	li $v0, 4
	la $a0, collatzPrompt
	syscall

	li $t4, 25

loop25:
    beqz $t4, EXIT

    move $t1, $t4

    # Prints read number
	li $v0, 1
	move $a0, $t1
	syscall

    # Prints prompt information
	li $v0, 4
	la $a0, dotdot
	syscall

	DIV_2:

    # Prints space
		li $v0, 4
		la $a0, space
		syscall

    # Checks the base case is it equal to 1
		li $t3, 1
		beq $t1, $t3, equalTo1

    # Divides 2
		li $t0, 2
		div $t1, $t0
		mfhi $t0

    # Checks if divided value's remaining is not zero then multiply 3 and plus 1
		bnez $t0, MUL_3_PLUS_1

    # If divided value's remaining is zero then print the value
	DIV_2_RES:

    # Set the new value as divided value
		mflo $t1

		li $v0, 1
		move $a0, $t1
		syscall

    # Back to divide loop
		j DIV_2

    # If divided value's remaining is not zero then multiply by 3 and add 1
	MUL_3_PLUS_1:
		li $t0, 3
		mult $t1, $t0
		mflo $t1
		addi $t1, $t1, 1

    # Print the multiplied value
		li $v0, 1
		move $a0, $t1
		syscall

    # Back to divide loop
		j DIV_2

equalTo1:

    li $v0, 4
    la $a0, newline
    syscall

    addi $t4, $t4, -1
    beqz $t4, EXIT
    j loop25

    # If base condition became then print a newline and exit
EXIT:
	li $v0, 4
	la $a0, newline
	syscall

	li $v0, 22
	syscall