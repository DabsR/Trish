# What is Trish?
Trish, the name, is an intelligently put-together acronym meaning, "The Rogue-like Idea Some Had." It is a small community project maintained by members of the One Lone Coder Discord server (https://discordapp.com/invite/7tjEznK) aimed at creating an ASCII rogue-like. 

# Setting (Work in Progress)
Set in a futuristic jazzy setting akin to the 1920's, you are a detective investigating an interstellar mafia's headquarters.

# Building for Windows
1) Ensure you have the Microsoft Visual Studio Developer Command Prompt installed. (https://docs.microsoft.com/en-us/dotnet/framework/tools/developer-command-prompt-for-vs)
1) Ensure you have Python 3 installed. (https://www.python.org/downloads/)
1) Launch the Developer Command Prompt in or navigate to the `Trish/Build/` directory.
1) Run the command `python build-win-x86.py --autorun`. The game will automatically run upon compiling successfuly.

# Building for MacOS
1) Install libtcod's dependencies via [homebrew](https://brew.sh)
1) Clone or download this (obviously)
1) Navigate to the master directory and run this horrendous abomination of a command in a terminal
1) `clang src/*.c Vendor/Libtcod/lib/MacOS/libtcod.dylib -I'Vendor/Libtcod/include/libtcod'`
1) You'll get a a.out run that with ./a.out
1) Play.

# Building for Linux
The mac procedure should work although you'll have to provide the library yourself
and change the -I path to matc (and remove the dylib from the command ofc)
