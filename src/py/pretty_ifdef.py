#!/usr/bin/python
# -*-coding:utf-8-*-
#
# replace_ifdef.py
#
#   Convert #ifdef guard to #pragma once
#
#   On most compilers, #pragma once will speed up compilation (of one cpp)
#   because the compiler need not reopen the file containing this instruction
#
#   See http://stackoverflow.com/questions/1143936/pragma-once-vs-include-guards


import argparse
import sys
import os


cpp_header_ext = ['.h', '.hpp']


# replace ifdef guard to `#pragma once`
def replace_ifdef_guard(text_lines):
    guard_line = -1
    end_line = -1
    for i in range(len(text_lines)):
        line = text_lines[i].strip()
        if line.startswith("#ifndef"):
            guard = line[8:].strip()
            nextline = text_lines[i+1]
            if nextline.startswith("#define"):
                name = nextline[8:].strip()
                if guard == name:
                    guard_line = i
            break

    if guard_line >= 0:
        for i in reversed(range(len(text_lines))):
            line = text_lines[i].strip()
            if line.startswith("#endif"):
                end_line = i

                break
    if guard_line >= 0 and end_line >= 0:
        assert end_line > guard_line
        text_lines[guard_line] = "#pragma once\n"
        del text_lines[guard_line+1]
        del text_lines[end_line-1]
        return text_lines, True

    return text_lines, False


#
def pretty_file(filename):
    print(filename)
    fp = open(filename, 'r')    # TO-DO: dectect file encoding 
    text_lines = fp.readlines()
    fp.close()

    if len(text_lines) < 3:
        return False

    content, modified = replace_ifdef_guard(text_lines)
    if modified:
        f = open(filename, 'w')
        f.writelines(content)
        f.close()

    return modified


def pretty_dir(root_dir):
    process_num = 0
    for root, sub_folders, files in os.walk(root_dir):
        # print(root.split('/')[-1])
        for file_name in files:
            file_ext = os.path.splitext(file_name)[1]
            if file_ext in cpp_header_ext:
                if pretty_file(root + '/' + file_name):
                    print('\t' + file_name)
                    process_num = process_num + 1

    print(process_num, 'file processed.')


def main():
    parser = argparse.ArgumentParser(description="")
    parser.add_argument("-d", "--dir", help="project directory", default="./")
    args = parser.parse_args()
    pretty_dir(args.dir)


if __name__ == '__main__':
    main()
