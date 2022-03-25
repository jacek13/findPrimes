# **Project description**

The topic of my project was searching for prime numbers in a given range. The main goal of the project was to implement vector instructions related to the **SIMD** (Single Instruction, Multiple Data) architecture. It should have been done using x64 assembler.

---

## **Completed features**:
1. implementation of the graphic interface in C++ using the [Dear ImGui library](https://github.com/ocornut/imgui),
2. implementation of a DLL library written in C++,
3. implementation of the DLL library written in x86 / x64 assembly language,
4. support for switches entered from the command line (allows you to use the program from the level of scripts),
5. support for multithreading using the standard **thread** library,
6. adding the option of saving found numbers to files with the .CSV extension,
7. handling incorrect data entered by the user and informing him about it.

---
## **Unfinished elements:**
1. more effective division of the given range into individual threads,
2. ability to kill referenced threads (in the current version, the free threads button does not work
correct),
3. attempt to use registers larger than 128 bits.

---
## **Program specification**

The program can be started by clicking on its icon, then a version with a graphical interface appears.The second method is to invoke it from the command line. Then we can run the application with
graphical interface enabled or disabled.

---

## **Graphical interface Demo**

![zdjecie](https://s7.gifyu.com/images/primes_gui_demo.gif)

## **Script Demo**

![zdjecie](https://s7.gifyu.com/images/primes_script_demo.gif)
