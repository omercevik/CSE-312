#       CSE 312 Operating Systems
#       Homework 01
#       Shell.asm
#       Omer CEVIK
#       161044004

.data

prompt:         .asciiz "f.. shell > "
input_command:  .space 256

.text
.globl main

main:

    infinite_loop:

        type_prompt:    #   Prints prompt
            li $v0, 4
            la $a0, prompt
            syscall

        read_command:   #   Reads Commands
            li $v0, 8
            la $a0, input_command
            syscall

        run_command:
            li $v0, 20
            la $a0, input_command
            syscall

    j infinite_loop

exit:
    li $v0, 10
    syscall