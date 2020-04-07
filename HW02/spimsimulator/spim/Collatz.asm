#       CSE 312 Operating Systems
#       Homework 02
#       Collatz.asm
#       Omer CEVIK
#       161044004

.data

user_input: .word 0
space: .asciiz " "
dotdot: .asciiz " :"
newline: .asciiz "\n"
collatzPrompt: .asciiz "Collatz.asm:\nEnter the Collatz number : "

.text
.globl mainCollatz

mainCollatz:

	li $v0, 4
	la $a0, collatzPrompt
	syscall

	li $v0, 24
	syscall
	sw $t0, user_input

	lw $t1, user_input

	li $v0, 1
	move $a0, $t1
	syscall

	li $v0, 4
	la $a0, dotdot
	syscall

	DIV_2:

		li $v0, 4
		la $a0, space
		syscall

		li $t3, 1
		beq $t1, $t3, EXIT

		li $t0, 2
		div $t1, $t0
		mfhi $t0

		bnez $t0, MUL_3_PLUS_1

	DIV_2_RES:
		mflo $t1

		li $v0, 1
		move $a0, $t1
		syscall

		j DIV_2

	MUL_3_PLUS_1:
		li $t0, 3
		mult $t1, $t0
		mflo $t1
		addi $t1, $t1, 1

		li $v0, 1
		move $a0, $t1
		syscall

		j DIV_2

EXIT:
	li $v0, 4
	la $a0, newline
	syscall

	li $v0, 22
	syscall