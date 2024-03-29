// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Fill.asm

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, 
// the screen should be cleared.

    @8192
    D=A
    @n
    M=D
(DETECT)
    @i
    M=0
    @SCREEN
    D=A
    @address
    M=D
    @KBD
    D=M
    @BLACKENLOOP
    D;JNE
    @WHITENLOOP
    D;JEQ
(BLACKENLOOP)
    @i
    D=M
    @n
    D=D-M
    @DETECT
    D;JGT
    @address
    A=M
    M=-1
    @i
    M=M+1
    @address
    M=M+1
    @BLACKENLOOP
    0;JMP
(WHITENLOOP)
    @i
    D=M
    @n
    D=D-M
    @DETECT
    D;JGT
    @address
    A=M
    M=0
    @i
    M=M+1
    @address
    M=M+1
    @WHITENLOOP
    0;JMP
