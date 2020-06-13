#       CSE 312 Operating Systems
#       Homework 03
#       Palindrome.asm
#       Omer CEVIK
#       161044004

.data

palindromePrompt: .asciiz "Palindrome.s:\n"
continue: .asciiz "Do you want to continue (y/n)?\n"
lastWordPrompt: .asciiz "Please enter the last word:\n"
yes: .asciiz "y"
newwl: .asciiz "\n"
palindromeInfo: .asciiz ": Palindrome\n"
notPalindromeInfo: .asciiz ": Not Palindrome\n"
dspace: .asciiz ": "
goodBye: .asciiz "Goodbye...\n"
answer: .space 4
wordDict: .space 64
lastWordInput: .space 64

.text
.globl mainPalindrome

mainPalindrome:

    # Prints prompt and inits loop
    la $a0, palindromePrompt
    li $v0, 4
    syscall

    lb $t0, newwl
    li $s4, 1
    li $t5, 101

    # Starts a loop and reads string from dictionary using syscall
loop100:
    li $t3, 0

    la $a0, wordDict
    li $v0, 27
    syscall

    la $t1, wordDict

    # Finds length of current string
findLength:

    addi $t3, $t3, 1
    addi $t1, $t1, 1

    lb $t4, 0($t1)

    bne $t4, $t0, findLength

    # Finds middle index of current string
    li $a0, 2
    div $t3, $a0
    mflo $t1

    la $t7, wordDict
    la $s1, wordDict

    # Finds last character of current string
    add $s1, $s1, $t3
    addi $s1, $s1, -1

    # Compares first and last characters of current string
palindromeSearch:

    lb $s2, 0($t7)
    lb $t2, 0($s1)

    # If characters are not equal then not palindrome
    bne $s2, $t2, notPalindrome

    # Iterates next characters of current string
    addi $t7, $t7, 1
    addi $s1, $s1, -1
    addi $t1, $t1, -1

    # Checks if the all characters are checked
    bnez $t1, palindromeSearch

    # If all characters are checked and there is no error then it is Palindrome
foundPalindrome:

    # Prints index of string
    li $v0, 1
    move $a0, $s4
    syscall

    # Prints prompt format
    la $a0, dspace
    li $v0, 4
    syscall

    # Loads the palindrome string address
    la $t1, wordDict
    lb $t0, newwl

    # Prints byte by byte of string iterating
printWordFound:

    lb $t4, 0($t1)

    li $v0, 11
    move $a0, $t4
    syscall

    addi $t1, $t1, 1

    lb $t4, 0($t1)

    bne $t4, $t0, printWordFound

    la $a0, palindromeInfo
    li $v0, 4
    syscall

    # Jumps to isHundred to check the last string is became
    j isHundred

    # Prints the not palindrome string information
notPalindrome:

    # Prints index of string
    li $v0, 1
    move $a0, $s4
    syscall

    # Prints prompt format
    la $a0, dspace
    li $v0, 4
    syscall

    # Loads the not palindrome string address
    la $t1, wordDict
    lb $t0, newwl

    # Prints byte by byte of string iterating
printWordNotFound:

    lb $t4, 0($t1)

    li $v0, 11
    move $a0, $t4
    syscall

    addi $t1, $t1, 1

    lb $t4, 0($t1)

    bne $t4, $t0, printWordNotFound

    la $a0, notPalindromeInfo
    li $v0, 4
    syscall

    # Counts the strings and if it is 100 then goes to get last word
isHundred:

    addi $s4, $s4, 1

    li $t4, 102
    beq $t4, $s4, finalPalindrome

    beq $s4, $t5, isLastWord
    j loop100

    # If 100 loop is over then asks to read last word from user
isLastWord:
    la $a0, continue
    li $v0, 4
    syscall

    # Reads yes or no with syscall
    li $v0, 28
    syscall

    sb $s0, answer

    lb $a0, answer

    lb $t7, yes

    # Checks if answer is yes then gets last word
    beq $a0, $t7, getLastWord

    # If answer is no then prints goodbye and exits with syscall
finalPalindrome:
    la $a0, goodBye
    li $v0, 4
    syscall

    li $v0, 22
    syscall

    # If yes is answer then reads last string using syscall
getLastWord:
    la $a0, lastWordPrompt
    li $v0, 4
    syscall

    la $a0, lastWordInput
    li $v0, 29
    syscall

    la $t0, lastWordInput
    la $t1, wordDict
    lb $t4, newwl

    # Loads the wordDict variable the read string and stores byte by byte to it
storeInput:

    lb $t2, 0($t0)
    sb $t2, 0($t1)

    addi $t0, $t0, 1
    addi $t1, $t1, 1

    lb $t2, 0($t0)

    bne $t2, $t4, storeInput

    sb $t4, 0($t1)

    # Refreshes the begin initial variables to evaluate the last word
    la $t1, wordDict

    li $t3, 0
    li $s4, 101
    li $t5, 102
    lb $t0, newwl

    # Jumps to get length of last word and evaluate palindrome
    j findLength