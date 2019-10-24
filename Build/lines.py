import os
import sys

print('Counting lines in source folder:')
print('==================================================')

os.chdir("../src/")

file_names  = os.listdir()
total_lines = 0

longest_name_length = 0
for file_name in file_names:
    if len(file_name) > longest_name_length:
        longest_name_length = len(file_name)

for file_name in file_names:
    file  = open(file_name, 'r')
    lines = file.readlines()
    line_count = len(lines)
    total_lines += line_count

    format_string = '{:>25} {:>24}'
    print(format_string.format(file_name, line_count))
    file.close()

print('==================================================')
print(f'Total: {total_lines}')