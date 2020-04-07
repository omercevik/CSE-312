#       CSE 312 Operating Systems
#       Homework 01
#       BinarySearch.asm
#       Omer CEVIK
#       161044004


.data
    search_list: .word 0
    type_prompt: .asciiz "Enter the length of the list then the list elements : "
    search_value_prompt: .asciiz "Enter the searching value : "
    search_value: .word 0
    success: .asciiz "It is in the List!\n"
    fail: .asciiz "It is NOT in the List!\n"

.text
.globl BinarySearch

BinarySearch:
    jal beginToCreate
    sw $v0, search_list

    la $a0, search_value_prompt
    li $v0, 4
    syscall

    li $v0, 5
    syscall

    sw $v0, search_value

    addi $sp, $sp, -4
    lw $t0, search_list
    sw $t0, 0($sp)
    jal sortTheList
    addi $sp, $sp, 4

    addi $sp, $sp, -16
    lw $t0, search_list
    sw $t0, 0($sp)
    lw $t1, search_value
    sw $t1, 4($sp)
    sw $0, 8($sp)

    lw $t0, search_list
    lw $t2, 0($t0)
    sw $t2, 12($sp)

    jal searchLabel
    addi $sp, $sp, 16
    beq $v0, $0, failureLabel

    la $a0, success
    li $v0, 4
    syscall

    j main

failureLabel:

    la $a0, fail
    li $v0, 4
    syscall

    j main

beginToCreate:

    addi $sp, $sp, -8
    sw $fp, 0($sp)
    sw $ra, 4($sp)

    addi $fp, $sp, 0
    addi $sp, $sp, -12

    sw $0, -12($fp)
    sw $0, -8($fp)
    sw $0, -4($fp)

    la $t0, type_prompt
    sw $t0, -12($fp)
    lw $a0, -12($fp)
    li $v0, 4
    syscall
    li $v0, 5
    syscall
    sw $v0, -12($fp)

    lw $t0, -12($fp)
    addi $t1, $0, 4
    mul $t2, $t1, $t0
    add $a0, $t2, $t1
    li $v0, 9
    syscall

    sw $v0, -8($fp)
    sw $t0, 0($v0)

startLoop1:

    lw $t0, -4($fp)
    lw $t2, -8($fp)
    lw $t1, 0($t2)
    bge $t0, $t1, stopLoop1
    li $v0, 5
    syscall
    lw $t2, -8($fp)
    lw $t0, -4($fp)
    addi $t3, $0, 4
    mul $t4, $t3, $t0
    add $t4, $t4, $t3
    add $t4, $t4, $t2
    sw $v0, 0($t4)
    lw $t0, -4($fp)
    addi $t0, $t0, 1
    sw $t0, -4($fp)
    j startLoop1


stopLoop1:

    lw $v0, -8($fp)
    addi $sp, $sp, 12
    lw $fp, 0($sp)
    lw $ra, 4($sp)
    addi $sp, $sp, 8
    jr $ra


sortTheList:

    addi $sp, $sp, -8
    sw $fp, 0($sp)
    sw $ra, 4($sp)

    addi $fp, $sp, 0
    addi $sp, $sp, -12

    sw $0, -12($fp)

startLoop2:

    lw $t0, -12($fp)
    lw $t2, 8($fp)
    lw $t1, 0($t2)
    sub $t1, $t1, 1
    bge $t0, $t1, stopLoop2
    lw $t3, -12($fp)
    addi $t3, $t3, 1
    sw $t3, -8($fp)

startLoop3:

    lw $t3, -8($fp)
    lw $t2, 8($fp)
    lw $t1, 0($t2)
    bge $t3, $t1, countIndex1

    lw $t0, -12($fp)
    addi $t4, $0, 4
    mul $t5, $t4, $t0
    add $t5, $t5, $t4
    lw $t2, 8($fp)
    add $t5, $t5, $t2
    lw $t8, 0($t5)

    lw $t3, -8($fp)
    addi $t4, $0, 4
    mul $t6, $t4, $t3
    add $t6, $t6, $t4
    lw $t2, 8($fp)
    add $t6, $t6, $t2
    lw $t7, 0($t6)

    ble $t8, $t7, countIndex2

    lw $t2, 8($fp)
    addi $t4, $0, 4
    lw $t0, -12($fp)
    mul $t5, $t4, $t0
    add $t5, $t5, $t4
    add $t5, $t5, $t2
    lw $t8, 0($t5)
    sw $t8, -4($fp)

    lw $t2, 8($fp)
    addi $t4, $0, 4
    lw $t0, -12($fp)
    mul $t5, $t4, $t0
    add $t5, $t5, $t4
    add $t5, $t5, $t2

    lw $t2, 8($fp)
    addi $t4, $0, 4
    lw $t3, -8($fp)
    mul $t6, $t4, $t3
    add $t6, $t6, $t4
    add $t6, $t6, $t2
    lw $t8, 0($t6)
    sw $t8, 0($t5)

    lw $t2, 8($fp)
    addi $t4, $0, 4
    lw $t3, -8($fp)
    mul $t6, $t4, $t3
    add $t6, $t6, $t4
    add $t6, $t6, $t2
    lw $t7, -4($fp)
    sw $t7, 0($t6)
    j countIndex2

countIndex1:

    lw $t0, -12($fp)
    addi $t0, $t0, 1
    sw $t0, -12($fp)
    j startLoop2

countIndex2:

    lw $t3, -8($fp)
    addi $t0, $t3, 1
    sw $t0, -8($fp)
    j startLoop3

stopLoop2:

    lw $v0, 8($fp)
    addi $sp, $sp, 12
    lw $fp, 0($sp)
    lw $ra, 4($sp)
    addi $sp, $sp, 8
    jr $ra

searchLabel:

    addi $sp, $sp, -8
    sw $fp, 0($sp)
    sw $ra, 4($sp)

    addi $fp, $sp, 0
    addi $sp, $sp, -4

firstLabel:

    lw $t1, 16($fp)
    lw $t2, 20($fp)
    beq $t1, $t2, stopFalseSearch
    lw $t1, 16($fp)
    lw $t2, 20($fp)
    addi $t3, $0, 2
    lw $t6, -4($fp)
    add $t4, $t1, $t2
    div $t4, $t3
    mflo $t5
    sw $t5, -4($fp)
    lw $t2, 8($fp)
    lw $t5, -4($fp)
    addi $t6, $0, 4
    mul $t5, $t5, $t6
    add $t5, $t5, $t6
    add $t2, $t2, $t5
    lw $t2, 0($t2)
    lw $t8, 12($fp)
    beq $t2, $t8, stopTrueSearch
    ble $t2, $t8, otherWise

leftlist:

    lw $t1, 16($fp)
    lw $t2, -4($fp)
    lw $t3, 8($fp)
    lw $t4, 12($fp)

    addi $sp, $sp, -16
    sw $t3, 0($sp)
    sw $t4, 4($sp)
    sw $t1, 8($sp)
    sw $t2, 12($sp)
    jal searchLabel
    addi $sp, $sp, 16

    addi $sp, $sp, 4
    lw $fp, 0($sp)
    lw $ra, 4($sp)
    addi $sp, $sp, 8
    jr $ra

otherWise:
    lw $t1, -4($fp)
    addi $t1, $t1, 1
    sw $t1, -4($fp)
    lw $t2, 20($fp)
    lw $t3, 8($fp)
    lw $t4, 12($fp)

    addi $sp, $sp, -16
    sw $t3, 0($sp)
    sw $t4, 4($sp)
    sw $t1, 8($sp)
    sw $t2, 12($sp)
    jal searchLabel
    addi $sp, $sp, 16

    addi $sp, $sp, 4
    lw $fp, 0($sp)
    lw $ra, 4($sp)
    addi $sp, $sp, 8
    jr $ra

stopFalseSearch:

    li $v0, 0
    addi $sp, $sp, 4
    lw $fp, 0($sp)
    lw $ra, 4($sp)
    addi $sp, $sp, 8
    jr $ra

stopTrueSearch:

    li $v0, 1
    addi $sp, $sp, 4
    lw $fp, 0($sp)
    lw $ra, 4($sp)
    addi $sp, $sp, 8
    jr $ra