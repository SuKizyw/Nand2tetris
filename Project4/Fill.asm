// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Fill.asm

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, 
// the screen should be cleared.

(INIT)
    @8192
    D=A
    @n
    M=D
(DETECT)
    @n
    M=M-1
    D=M
    @INIT
    D;JLT
    @KBD
    D=M
    @BLACKENLOOP
    D;JNE
    @WHITENLOOP
    0;JMP
(BLACKENLOOP)
    @SCREEN
    D=A
    @n
    A=D+M
    M=-1
    @DETECT
    0;JMP
(WHITENLOOP)
    @SCREEN
    D=A
    @n
    A=D+M
    M=0
    @DETECT
    0;JMP
