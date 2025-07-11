# mv – A Simple Unix `mv` Utility in C

This program is a basic implementation of the Unix mv command written in C.
It moves (or renames) a file from a source path to a destination path by copying the contents and then deleting the original file. It can also be used to rename a file without moving it to another location.


## 📁 File

- `mv.c`: Source code for the `mv` (move) utility.

## 🔧 How to Compile

Use `gcc`:

```bash
gcc -o mv mv.c

or

you can use the make command to generate the executable. 
