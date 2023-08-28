# jawa
<br>
<p align=center>
<img src="icon/icon.png" alt="jawa" width="300"/>
</p><br>
Jawa is a minimalistic toy programming language made to make you feel like a little alien
in a deserted planet chaotically hacking away at a droid's internals. It features only 8 operations and the program 
memory lives along side data memory. Each line of jawa code is placed in memory and is 
accessable through it's line number. You can use this to alter data by line number or 
even the code itself during runtime.

### Operations
| Name   | bits | Description                                           |
|--------|------|-------------------------------------------------------|
| end    | 0    | Program's end. Empty lines will also end the program. |
| start  | 1    | Program's starting point.                             |
| add X  | 2    | Add the value in box to X. (X += box)                 |
| sub X  | 3    | Subtract the value in box from X. (X -= box)          |
| box X  | 4    | Put the value of X into box (box = X)                 |
| go X   | 5    | Move program pointer to X line.                       |
| if X   | 6    | If box == 0 move program to X line.                   |
| draw X | 7    | Prints character encoded by value at X.               |
