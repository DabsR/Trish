# What is Trish?
Trish, the name, is an intelligently put-together acronym meaning, "The Rogue-like Idea Some Had." It is a small community project maintained by members of the One Lone Coder Discord server (https://discordapp.com/invite/7tjEznK) aimed at creating an ASCII rogue-like. 

# Setting (Work in Progress)
Set in a futuristic jazzy setting akin to the 1920's, you are a detective investigating an interstellar mafia's headquarters.

# Building for Windows
Before you can build, you must have the Developer Command Prompt installed via some recent version of Visual Studio (2015-2019) as well as Python 3. Run the python-win-x86.py file from the Developer Command Prompt. You dont need compat_stupid.h and comapat_stupid.c so you may replace them with empty versions of themselves.

# Building for MacOS
1) Install libtcod's dependencies via [homebrew](https://www.brew.sh)
1) Clone or download this (obviously)
1) Navigate to the master directory and run this horrendous abomination of a command in a terminal
1) `clang src/*.c Vendor/Libtcod/lib/MacOS/libtcod.dylib -I'Vendor/Libtcod/include/libtcod'`
1) You'll get a a.out run that with ./a.out
1) Play.

# Building for Linux
The mac procedure should work although you'll have to provide the library yourself
and change the -I path to matc (and remove the dylib from the command ofc)
