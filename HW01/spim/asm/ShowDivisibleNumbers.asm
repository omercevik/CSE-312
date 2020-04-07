#       CSE 312 Operating Systems
#       Homework 01
#       ShowDivisibleNumbers.asm
#       Omer CEVIK
#       161044004

.data

prompt_1: .asciiz "Enter first number : "
prompt_2: .asciiz "Enter second number : "
prompt_3: .asciiz "Enter third number : "
found: .asciiz "Divisible numbers are :\n"
error:  .asciiz "Exception: Divide by zero!\n"
newline: .asciiz "\n"

firstNumber: .word 1
secondNumber: .word 1
thirdNumber: .word 1

.text
.globl ShowDivisibleNumbers

ShowDivisibleNumbers:

    read_numbers:
        li $v0, 4           #   Print Prompt
        la $a0, prompt_1
        syscall

        li $v0, 5           #   Read integer first number
        syscall
        sw $v0, firstNumber           #   Store integer first number


        li $v0, 4           #   Print Prompt
        la $a0, prompt_2
        syscall

        li $v0, 5           #   Read integer second number
        syscall
        sw $v0, secondNumber           #   Store integer second number

        li $v0, 4           #   Print Prompt
        la $a0, prompt_3
        syscall

        li $v0, 5           #   Read integer third number
        syscall
        sw $v0, thirdNumber           #   Store integer third number

    findDivisible:

        la $s1, firstNumber
        la $s2, secondNumber
        la $s3, thirdNumber

        lw $t0, 0($s1)
        lw $t1, 0($s2)
        lw $t2, 0($s3)

        slt $t5, $t0, $t1
        beqz $t5, firstBigger
        j checkError

    firstBigger:

        lw $t0, 0($s2)
        lw $t1, 0($s1)
        lw $t2, 0($s3)

    checkError:
        beqz $t2, errorLabel

        la $a0, found
        li $v0, 4
        syscall

    increaseLoop:

        div $t0, $t2
        mfhi $t3

        beqz $t3, printDivisible
    backToLoop:

        add $t0, $t0, 1

        bne $t0, $t1, increaseLoop
        j exit

    printDivisible:
        li $v0, 1
        la $a0, 0($t0)
        syscall

        li $v0, 4
        la $a0, newline
        syscall

        j backToLoop


    errorLabel:
        li $v0, 4
        la $a0, error
        syscall

exit:
    j main