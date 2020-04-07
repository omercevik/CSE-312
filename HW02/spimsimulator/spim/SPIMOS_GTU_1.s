#       CSE 312 Operating Systems
#       Homework 02
#       SPIMOS_GTU_1.s
#       Omer CEVIK
#       161044004

.data

programLinear: .asciiz "LinearSearch.asm"
programBinary: .asciiz "BinarySearch.asm"
programCollatz: .asciiz "Collatz.asm"
newline: .asciiz "\n"

registers1: .space 136
registers2: .space 136
registers3: .space 136

programCounter1: .word 0
programCounter2: .word 0
programCounter3: .word 0

flagProgram1: .word 0
flagProgram2: .word 0
flagProgram3: .word 0

programFinish1: .word 0
programFinish2: .word 0
programFinish3: .word 0

programIndex: .word 0

.text
.globl main

main:

readLinear:
    li $v0, 20
    la $a0, programLinear
    syscall

    sw $a3, programCounter1

readCollatz:
    li $v0, 20
    la $a0, programCollatz
    syscall

    sw $a3, programCounter2

readBinary:
    li $v0, 20
    la $a0, programBinary
    syscall

    sw $a3, programCounter3




loadProgram1:
        li $s0, 1
        sw $s0, programIndex

        la $t6, registers1

        lw $t0, flagProgram1
        beqz $t0, runProgram1

        li $a0, 22
        lw $v0, programFinish1
        beq $a0, $v0, loadProgram2


        j loadAll

runProgram1:

        lw $t0, 32($t6)
        lw $t1, 36($t6)
        lw $t2, 40($t6)

        lw $a3, programCounter1
        li $v0, 21
        syscall

        sw $a3, programCounter1
        sw $v0, programFinish1

        j storeAll

storeBack1:

        lw $t0, flagProgram1
        addi $t0, $t0, 1
        sw $t0, flagProgram1



loadProgram2:

        li $s0, 2
        sw $s0, programIndex

        la $t6, registers2

        lw $t0, flagProgram2
        beqz $t0, runProgram2


        li $a0, 22
        lw $v0, programFinish2
        beq $a0, $v0, loadProgram3

        j loadAll

runProgram2:

        lw $t0, 32($t6)
        lw $t1, 36($t6)
        lw $t2, 40($t6)

        lw $a3, programCounter2
        li $v0, 21
        syscall

        sw $a3, programCounter2
        sw $v0, programFinish2

        j storeAll

storeBack2:

        lw $t0, flagProgram2
        addi $t0, $t0, 1
        sw $t0, flagProgram2

loadProgram3:

        li $s0, 3
        sw $s0, programIndex

        la $t6, registers3

        lw $t0, flagProgram3
        beqz $t0, runProgram3

        li $a0, 22
        lw $v0, programFinish3
        beq $a0, $v0, isFinishedPrograms


        j loadAll

runProgram3:

        lw $t0, 32($t6)
        lw $t1, 36($t6)
        lw $t2, 40($t6)

        lw $a3, programCounter3
        li $v0, 21
        syscall

        sw $a3, programCounter3
        sw $v0, programFinish3

        j storeAll

storeBack3:

        lw $t0, flagProgram3
        addi $t0, $t0, 1
        sw $t0, flagProgram3


isFinishedPrograms:

        lw $t0, programFinish1
        lw $t1, programFinish2
        lw $t2, programFinish3

        li $t3, 22

        bne $t0, $t3, loadProgram1

        bne $t1, $t3, loadProgram2

        bne $t2, $t3, loadProgram3


halt:
        li $v0, 23
        syscall

storeAll:

        #sw $zero, 0($t6)
        #sw $at, 4($t6)
        sw $v0, 8($t6)
        sw $v1, 12($t6)
        sw $a0, 16($t6)
        sw $a1, 20($t6)
        sw $a2, 24($t6)
        sw $a3, 28($t6)
        sw $t0, 32($t6)
        sw $t1, 36($t6)
        sw $t2, 40($t6)
        sw $t3, 44($t6)
        sw $t4, 48($t6)
        sw $t5, 52($t6)
        sw $s0, 56($t6)
        sw $t7, 60($t6)
        sw $s1, 64($t6)
        sw $s2, 68($t6)
        sw $s3, 72($t6)
        sw $s4, 76($t6)
        sw $s5, 80($t6)
        sw $s6, 84($t6)
        sw $s7, 88($t6)
        sw $t8, 92($t6)
        sw $t9, 96($t6)
        sw $k0, 100($t6)
        sw $k1, 104($t6)
        sw $gp, 108($t6)
        sw $sp, 112($t6)
        sw $s8, 116($t6)
        sw $ra, 120($t6)
        mfhi $t0
        sw $t0, 0($t6)
        mflo $t0
        sw $t0, 4($t6)

        li $t1, 1
        li $t2, 2

        lw $t0, programIndex
        beq $t0, $t1, storeBack1
        beq $t0, $t2, storeBack2
        j storeBack3

loadAll:

        lw $t0, 0($t6)
        mthi $t0
        lw $t0, 4($t6)
        mtlo $t0

        #lw $zero, 0($t6)
        #lw $at, 4($t6)
        lw $v0, 8($t6)
        lw $v1, 12($t6)
        lw $a0, 16($t6)
        lw $a1, 20($t6)
        lw $a2, 24($t6)
        lw $a3, 28($t6)
        lw $t0, 32($t6)
        lw $t1, 36($t6)
        lw $t2, 40($t6)
        lw $t3, 44($t6)
        lw $t4, 48($t6)
        lw $t5, 52($t6)
        lw $s0, 56($t6)
        lw $t7, 60($t6)
        lw $s1, 64($t6)
        lw $s2, 68($t6)
        lw $s3, 72($t6)
        lw $s4, 76($t6)
        lw $s5, 80($t6)
        lw $s6, 84($t6)
        lw $s7, 88($t6)
        lw $t8, 92($t6)
        lw $t9, 96($t6)
        lw $k0, 100($t6)
        lw $k1, 104($t6)
        lw $gp, 108($t6)
        lw $sp, 112($t6)
        lw $s8, 116($t6)
        lw $ra, 120($t6)

        li $t1, 1
        li $t2, 2

        lw $t0, programIndex
        beq $t0, $t1, runProgram1
        beq $t0, $t2, runProgram2

        j runProgram3