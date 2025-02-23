/* SPIM S20 MIPS simulator.
   Execute SPIM syscalls, both in simulator and bare mode.
   Execute MIPS syscalls in bare mode, when running on MIPS systems.
   Copyright (c) 1990-2010, James R. Larus.
   All rights reserved.

   Redistribution and use in source and binary forms, with or without modification,
   are permitted provided that the following conditions are met:

   Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

   Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation and/or
   other materials provided with the distribution.

   Neither the name of the James R. Larus nor the names of its contributors may be
   used to endorse or promote products derived from this software without specific
   prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
   GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
   HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
   OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#ifndef _WIN32
#include <unistd.h>
#endif
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>

#ifdef _WIN32
#include <io.h>
#endif

#include "spim.h"
#include "string-stream.h"
#include "inst.h"
#include "reg.h"
#include "mem.h"
#include "sym-tbl.h"
#include "syscall.h"

/*

#       CSE 312 Operating Systems
#       Homework 03
#       syscall.cpp
#       Omer CEVIK
#       161044004

*/

#include <iostream>
using namespace std;

// Kernels program counter
mem_addr spimOSPC;
int endCounter = 0;
int whichKernel = 1;
int dictionaryIndex = 0;
// Dictionary
string dictionary[101] =
{
"aba\n",
"how\n",
"book\n",
"ada\n",
"arthas\n",
"abc\n",
"def\n",
"ghj\n",
"klm\n",
"nopr\n",
"stuv\n",
"wxyz\n",
"klmnp\n",
"word\n",
"clear\n",
"cristal\n",
"fake\n",
"one\n",
"two\n",
"three\n",
"turut\n",
"four\n",
"five\n",
"six\n",
"seven\n",
"eight\n",
"nine\n",
"ten\n",
"madam\n",
"eleven\n",
"twelve\n",
"thirteen\n",
"fourteen\n",
"fifteen\n",
"monom\n",
"twenty\n",
"thirty\n",
"clean\n",
"work\n",
"juuj\n",
"person\n",
"justice\n",
"feel\n",
"like\n",
"rebel\n",
"gtu\n",
"operating\n",
"system\n",
"homework\n",
"home\n",
"fear\n",
"score\n",
"point\n",
"breathe\n",
"babab\n",
"way\n",
"game\n",
"super\n",
"awesome\n",
"bad\n",
"licence\n",
"captain\n",
"computer\n",
"science\n",
"engineer\n",
"noon\n",
"omeremo\n",
"human\n",
"laugh\n",
"left\n",
"right\n",
"down\n",
"wasd\n",
"insert\n",
"delete\n",
"kelek\n",
"prtsc\n",
"istanbul\n",
"kocaeli\n",
"gebze\n",
"ankara\n",
"izmir\n",
"antalya\n",
"adana\n",
"sakarya\n",
"zonguldak\n",
"erzurum\n",
"sivas\n",
"diyarbakir\n",
"edirne\n",
"hatay\n",
"malatya\n",
"hakkari\n",
"erzincan\n",
"bayburt\n",
"giresun\n",
"trabzon\n",
"samsun\n",
"kilis\n",
"last\n"
};

#ifdef _WIN32
/* Windows has an handler that is invoked when an invalid argument is passed to a system
   call. https://msdn.microsoft.com/en-us/library/a9yf33zb(v=vs.110).aspx

   All good, except that the handler tries to invoke Watson and then kill spim with an exception.

   Override the handler to just report an error.
*/

#include <stdio.h>
#include <stdlib.h>
#include <crtdbg.h>


void myInvalidParameterHandler(const wchar_t* expression,
 const wchar_t* function,
 const wchar_t* file,
 unsigned int line,
 uintptr_t pReserved)
{
  if (function != NULL)
  {
      run_error ("Bad parameter to system call: %s\n", function);
  }
  else
  {
      run_error ("Bad parameter to system call\n");
  }
}

static _invalid_parameter_handler oldHandler;

void windowsParameterHandlingControl(int flag )
{
  static _invalid_parameter_handler oldHandler;
  static _invalid_parameter_handler newHandler = myInvalidParameterHandler;

  if (flag == 0)
  {
      oldHandler = _set_invalid_parameter_handler(newHandler);
      _CrtSetReportMode(_CRT_ASSERT, 0); // Disable the message box for assertions.
  }
  else
  {
      newHandler = _set_invalid_parameter_handler(oldHandler);
      _CrtSetReportMode(_CRT_ASSERT, 1);  // Enable the message box for assertions.
  }
}
#endif

int counter = 0;

/*You implement your handler here*/
void SPIM_timerHandler()
{
    // int i;
    // write_output (console_out, "\n! INTERRUPT !\n\n");
    // for (i = 0; i < 32; ++i)
    //     write_output(console_out,"REG[%d] -> %d\n",i,R[i]);

    // write_output(console_out,"HI -> %d\n",HI);
    // write_output(console_out,"LO -> %d\n",LO);
    // write_output(console_out,"PC -> %d\n",PC);
    // write_output(console_out,"Stack Pointer Address -> %d\n\n",R[29]);

    // Checks which strategy is running and finished
    if (whichKernel == 0)
    {
        if (R[REG_V0] == 22 && ++endCounter == 4)
            haltExe();
    }
    else if (whichKernel == 1)
    {
        if (R[REG_V0] == 22 && ++endCounter == 10)
            haltExe();
    }
    else if (whichKernel == 2)
    {
        if (R[REG_V0] == 22 && ++endCounter == 6)
            haltExe();
    }

    // Checks the interrupt size
    if (++counter >= 5)
    {
        // Stores the program counter and runs the next instruction in microkernel
        counter = 0;
        bool x;
        R[REG_A3] = PC + 4;
        run_program (spimOSPC, DEFAULT_RUN_STEPS , false, false, &x);
    }

}
/* Decides which syscall to execute or simulate.  Returns zero upon
   exit syscall and non-zero to continue execution. */
int do_syscall ()
{
    #ifdef _WIN32
    windowsParameterHandlingControl(0);
    #endif

      /* Syscalls for the source-language version of SPIM.  These are easier to
         use than the real syscall and are portable to non-MIPS operating
         systems. */

    switch (R[REG_V0])
    {
        case PRINT_INT_SYSCALL:
        write_output (console_out, "%d", R[REG_A0]);
        break;

        case PRINT_FLOAT_SYSCALL:
        {
           float val = FPR_S (REG_FA0);

           write_output (console_out, "%.8f", val);
           break;
       }

       case PRINT_DOUBLE_SYSCALL:
       write_output (console_out, "%.18g", FPR[REG_FA0 / 2]);
       break;

       case PRINT_STRING_SYSCALL:
       write_output (console_out, "%s", mem_reference (R[REG_A0]));
       break;

       case READ_INT_SYSCALL:
       {
        static char str [256];

        read_input (str, 256);
        R[REG_RES] = atol (str);
        break;
    }

    case READ_FLOAT_SYSCALL:
    {
        static char str [256];

        read_input (str, 256);
        FPR_S (REG_FRES) = (float) atof (str);
        break;
    }

    case READ_DOUBLE_SYSCALL:
    {
        static char str [256];

        read_input (str, 256);
        FPR [REG_FRES] = atof (str);
        break;
    }

    case READ_STRING_SYSCALL:
    {
        read_input ( (char *) mem_reference (R[REG_A0]), R[REG_A1]);
        data_modified = true;
        break;
    }

    case SBRK_SYSCALL:
    {
        mem_addr x = data_top;
        expand_data (R[REG_A0]);
        R[REG_RES] = x;
        data_modified = true;
        break;
    }

    case PRINT_CHARACTER_SYSCALL:
    write_output (console_out, "%c", R[REG_A0]);
    break;

    case READ_CHARACTER_SYSCALL:
    {
        static char str [2];

        read_input (str, 2);
        if (*str == '\0') *str = '\n';      /* makes xspim = spim */
        R[REG_RES] = (long) str[0];
        break;
    }

    case EXIT_SYSCALL:
    spim_return_value = 0;
    return (0);

    case EXIT2_SYSCALL:
        spim_return_value = R[REG_A0];	/* value passed to spim's exit() call */
    return (0);

    case OPEN_SYSCALL:
    {
        #ifdef _WIN32
        R[REG_RES] = _open((char*)mem_reference (R[REG_A0]), R[REG_A1], R[REG_A2]);
        #else
        R[REG_RES] = open((char*)mem_reference (R[REG_A0]), R[REG_A1], R[REG_A2]);
        #endif
        break;
    }

    case READ_SYSCALL:
    {
        /* Test if address is valid */
        (void)mem_reference (R[REG_A1] + R[REG_A2] - 1);
        #ifdef _WIN32
        R[REG_RES] = _read(R[REG_A0], mem_reference (R[REG_A1]), R[REG_A2]);
        #else
        R[REG_RES] = read(R[REG_A0], mem_reference (R[REG_A1]), R[REG_A2]);
        #endif
        data_modified = true;
        break;
    }

    case WRITE_SYSCALL:
    {
        /* Test if address is valid */
        (void)mem_reference (R[REG_A1] + R[REG_A2] - 1);
        #ifdef _WIN32
        R[REG_RES] = _write(R[REG_A0], mem_reference (R[REG_A1]), R[REG_A2]);
        #else
        R[REG_RES] = write(R[REG_A0], mem_reference (R[REG_A1]), R[REG_A2]);
        #endif
        break;
    }

    case CLOSE_SYSCALL:
    {
        #ifdef _WIN32
        R[REG_RES] = _close(R[REG_A0]);
        #else
        R[REG_RES] = close(R[REG_A0]);
        #endif
        break;
    }

    case LOAD_ASSEMBLY_KERNEL_1:
    {
        loadAssemblies();
        break;
    }

    case RUN_PROGRAM:
    {
        runProgram();
        break;
    }

    case END_ASSEMBLY:
    {
        endAssembly();
        break;
    }

    case HALT_EXE:
    {
        haltExe();
        break;
    }

    case READ_INTO_T0:
    {
        readIntoT0();
        break;
    }

    case READ_RANDOM_KERNEL_2:
    {
        loadAssemblies2();
        break;
    }

    case READ_RANDOM_KERNEL_3:
    {
        loadAssemblies3();
        break;
    }

    case READ_PALINDROME_WORD:
    {
        readPalindromeWord();
        break;
    }

    case READ_PALINDROME_YES:
    {
        readYesNo();
        break;
    }

    case READ_PALINDROME_LAST_WORD:
    {
        readLastWord();
        break;
    }

    default:
    run_error ("Unknown system call: %d\n", R[REG_V0]);
    break;
}

        #ifdef _WIN32
windowsParameterHandlingControl(1);
        #endif
return (1);
}


void
handle_exception ()
{
          if (!quiet && CP0_ExCode != ExcCode_Int)
            error ("Exception occurred at PC=0x%08x\n", CP0_EPC);

        exception_occurred = 0;
        PC = EXCEPTION_ADDR;

        switch (CP0_ExCode)
        {
            case ExcCode_Int:
            break;

            case ExcCode_AdEL:
            if (!quiet)
               error ("  Unaligned address in inst/data fetch: 0x%08x\n", CP0_BadVAddr);
            break;

           case ExcCode_AdES:
           if (!quiet)
            error ("  Unaligned address in store: 0x%08x\n", CP0_BadVAddr);
            break;

            case ExcCode_IBE:
            if (!quiet)
                error ("  Bad address in text read: 0x%08x\n", CP0_BadVAddr);
            break;

            case ExcCode_DBE:
            if (!quiet)
                error ("  Bad address in data/stack read: 0x%08x\n", CP0_BadVAddr);
            break;

            case ExcCode_Sys:
            if (!quiet)
                error ("  Error in syscall\n");
            break;

            case ExcCode_Bp:
            exception_occurred = 0;
            return;

            case ExcCode_RI:
            if (!quiet)
                error ("  Reserved instruction execution\n");
            break;

            case ExcCode_CpU:
            if (!quiet)
                error ("  Coprocessor unuable\n");
            break;

            case ExcCode_Ov:
            if (!quiet)
                error ("  Arithmetic overflow\n");
            break;

            case ExcCode_Tr:
            if (!quiet)
                error ("  Trap\n");
            break;

            case ExcCode_FPE:
            if (!quiet)
                error ("  Floating point\n");
            break;

            default:
            if (!quiet)
                error ("Unknown exception: %d\n", CP0_ExCode);
            break;
        }
}

// Reads assemblies in strategy 1 and stores the program counters
void loadAssemblies()
{
    char* command = (char *)mem_reference (R[REG_A0]);

    read_assembly_file(command);

    PC = PC + 4;
    spimOSPC = PC;
    whichKernel = 0;

    switch(command[0])
    {
        case 'L':
        R[REG_A3] = find_symbol_address("mainLinearSearch");
        break;

        case 'B':
        R[REG_A3] = find_symbol_address("mainBinarySearch");
        break;

        case 'C':
        R[REG_A3] = find_symbol_address("mainCollatz");
        break;

        case 'P':
        R[REG_A3] = find_symbol_address("mainPalindrome");
        break;

        default: cerr << "Unacceptable file to load!" << endl;
    }

    // Runs the current instruction in microkernel
    bool x;
    run_program (spimOSPC, DEFAULT_RUN_STEPS , false, false, &x);
}

// Second strategy to load assemblies and runs the microkernel
void loadAssemblies2()
{
    srand(time(0));

    PC = PC + 4;
    spimOSPC = PC;
    whichKernel = 1;

    int random = rand() % 3;

    readSwitch(random, &R[REG_A3]);

    bool x;
    run_program (spimOSPC, DEFAULT_RUN_STEPS , false, false, &x);
}

// Third strategt to load assemblies and runs the microkernel
void loadAssemblies3()
{
    srand(time(0));

    PC = PC + 4;
    spimOSPC = PC;
    whichKernel = 2;

    int random1 = rand() % 3;
    int random2 = rand() % 3;

    for (; random1 == random2; random2 = rand() % 3);

    readSwitch(random1, &R[8]);
    readSwitch(random2, &R[REG_A3]);

    bool x;
    run_program (spimOSPC, DEFAULT_RUN_STEPS , false, false, &x);
}

// Reads assemblies given random program
void readSwitch(int program, int* reg)
{
    switch(program)
    {
        case 0:
        {
            char* command = (char *)mem_reference (R[REG_A0]);

            read_assembly_file(command);

            *reg = find_symbol_address("mainLinearSearch");
            break;
        }

        case 1:
        {
            char* command = (char *)mem_reference (R[REG_A1]);

            read_assembly_file(command);

            *reg = find_symbol_address("mainCollatz");
            break;
        }

        case 2:
        {
            char* command = (char *)mem_reference (R[REG_A2]);

            read_assembly_file(command);

            *reg = find_symbol_address("mainBinarySearch");
            break;
        }

        case 3:
        {
            char* command = (char *)mem_reference (R[REG_S0]);

            read_assembly_file(command);

            *reg = find_symbol_address("mainPalindrome");
            break;
        }
    }
}

// Runs the test program
void runProgram()
{
    bool x;
    spimOSPC = PC + 4;
    run_program (R[REG_A3], DEFAULT_RUN_STEPS , false, false, &x);
}

// Back to microkernel from programs
void endAssembly()
{
    bool x;
    run_program(spimOSPC, DEFAULT_RUN_STEPS, false, false, &x);
}

// Exits the microkernel
void haltExe()
{
    exit(0);
}

// Reads integer into T0 register
void readIntoT0()
{
    static char str [256];
    read_input (str, 256);
    R[8] = atol (str);
}

// Reads palindrome words from dictionary
void readPalindromeWord()
{
    if (dictionaryIndex < 100)
    {
        int i = 0;
        char* setWord = (char *)mem_reference (R[REG_A0]);

        for (i = 0; i < (int)dictionary[dictionaryIndex].size(); ++i)
            setWord[i] = dictionary[dictionaryIndex][i];
        setWord[i] = '\0';

        ++dictionaryIndex;
    }
    else
        endAssembly();
}

// Reads yes or no character and stores to s0 register
void readYesNo()
{
    char str = ' ';
    cin >> str;
    R[REG_S0] = str;
}

// Reads last word ans stores in a0 address
void readLastWord()
{
    char* lastWord = (char *)mem_reference (R[REG_A0]);
    string input;

    cin >> input;

    int i = 0;
    for (i = 0; i < input.size(); ++i)
        lastWord[i] = input[i];

    lastWord[i] = '\0';
}