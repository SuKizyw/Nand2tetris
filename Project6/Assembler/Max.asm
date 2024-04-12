// This file is part of www.nand2tetris.org

// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/06/rect/Rect.asm
// Draws a rectangle at the top-left corner of the screen.
// The rectangle is 16 pixels wide and R0 pixels high.

 // D = R0 - R1
   @R0
   D=M
   @R1
   D=D-M
   // If (D > 0) goto ITSR0
   @ITSR0
   D;JGT
   // Its R1
   @R1
   D=M
   @SET_RESULT
   0;JMP
(ITSR0)
   @R0
   D=M
(SET_RESULT)
   @R2
   M=D
(END)
   @END
   0;JMP