# Debugger
An example of a Windows debugger that will attach to a running Assault Cube process, change a specific instruction to an int 3 instruction (0xCC), and then restore the original instruction when the breakpoint is hit. The instruction modified only executes when the player is firing, allowing us to verify that the debugger is working as intended.
    
The code and approach are discussed in the article at: https://gamehacking.academy/lesson/40
