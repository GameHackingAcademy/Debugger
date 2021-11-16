# Debugger
Referenced in https://gamehacking.academy/lesson/7/5.

An example of a Windows debugger that will attach to a running Assault Cube 1.2.0.2 process, change a specific instruction to an int 3 instruction (0xCC), and then restore the original instruction when the breakpoint is hit. The instruction modified only executes when the player is firing, allowing us to verify that the debugger is working as intended.
