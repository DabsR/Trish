import os
import sys
from shutil import copyfile

if os.name != 'nt':
    print("This script only supports Windows!")
    exit()

###############
### Options ###
###############

                      # Option Name
auto_run_game = False # --autorun
skip_compile  = False # --skip-compile

include_directories = [
    '../Vendor/Libtcod/include/'
]

library_directories = [
    '../Vendor/Libtcod/lib/Win x86/'
]

libraries = [
    'libtcod.lib'
]

source_files = [
    '../src/*.c'
]

resource_directories = [
    '../Vendor/Libtcod/res/'
]

output_directory = '../bin/Win x86/'
output_file_name = 'Trish.exe'
executable_path  = os.path.join(output_directory, output_file_name)



######################
### Compiler Setup ###
######################

if len(sys.argv) > 0:
    if '--autorun' in sys.argv:
        auto_run_game = True
    if '--skip-compile' in sys.argv:
        skip_compile = True

if not os.path.exists(output_directory):
    os.mkdir(output_directory)

include_directory_arguments = ''
library_directory_arguments = ''
library_arguments           = ''
source_file_arguments       = ''

for directory in include_directories:
    include_directory_arguments += f'/I "{directory}" '

for directory in library_directories:
    library_directory_arguments += f'/LIBPATH:"{directory}" '

for library in libraries:
    library_arguments += f'"{library}" '

for file in source_files:
    source_file_arguments += f'"{file}" '

if (os.path.exists(output_directory) == False):
    os.mkdir(output_directory)

import sys


success = True

if (skip_compile == False):
    command = f'cl.exe -DUNICODE -D_UNICODE /Fe"{output_directory}{output_file_name}" {include_directory_arguments} {source_file_arguments} /link {library_directory_arguments} {library_arguments}'
    success = (os.system(command) == 0)

    if success:
        print("Compilation successful!")

    # Clean files
    file_names = os.listdir()

    for file_name in file_names:
        if file_name.endswith('.obj'):
            os.remove(file_name)

if success:
    for directory in library_directories:
        file_names = os.listdir(directory)

        for file_name in file_names:
            if file_name.endswith('.dll'):
                copyfile(os.path.join(directory, file_name), os.path.join(output_directory, file_name))

    for directory in resource_directories:
        file_names = os.listdir(directory)

        for file_name in file_names:
            copyfile(os.path.join(directory, file_name), os.path.join(output_directory, file_name))
    
    if (auto_run_game):
        os.system(f'"{executable_path}"')