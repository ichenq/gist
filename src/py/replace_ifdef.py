#!/usr/bin/python
# -*-coding:utf-8-*-
#
# replace_ifdef.py
#   Convert #ifdef guard to #pragma once，reduce compiling file I/O operation.
#   On most compilers, #pragma once will speed up compilation (of one cpp),
#   because the compiler need not reopen the file containing this instruction  
#   See http://stackoverflow.com/questions/1143936/pragma-once-vs-include-guards
#

from __future__ import unicode_literals
from __future__ import print_function

import argparse
import sys
import os
import codecs
from chardet.universaldetector import UniversalDetector

cpp_header_ext = ['.h', '.hpp']

detector = UniversalDetector()

def trim_line_space(line):
    new_line = line.strip()
    if len(new_line) == 0:
        return line
    return new_line

def get_encoding(filename):
    detector.reset()
    content = open(filename, 'rb').read()
    detector.feed(content)
    detector.close()
    encoding = detector.result['encoding']
    confidence = detector.result['confidence']
    print(filename, encoding, confidence)
    if encoding.startswith('ascii') or encoding.startswith('UTF-8') or encoding.startswith('GB2312'):
        assert confidence >= 0.7
        return encoding
    assert confidence >= 0.9, filename
    return encoding
    
    
def pretty_file(filename):
    print(filename)
    should_replace = False
    new_text_lines = []
    encoding = get_encoding(filename)
    fp = codecs.open(filename, 'r', encoding)
    text_lines = fp.readlines()
    fp.close()
    if len(text_lines) < 3:
        return False
        
    #print(text_lines)
        
    i = 0
    # find #ifdef guar
    while i < len(text_lines):
        line = text_lines[i]
        if line.startswith('#ifndef'):
            next_line = text_lines[i+1]
            guard = line[8:].strip()
            if next_line.startswith('#define'):
                if guard == next_line[8:].strip():
                    should_replace = True
                    i += 2
                    continue
        i += 1
        new_text_lines.append(line)
           

    if should_replace:
        # find last #endif
        index = len(new_text_lines) - 1
        while index > 0:
            line = new_text_lines[index]
            if line.startswith("#endif"):
                new_text_lines = new_text_lines[:index]
                break
            index -= 1
                
        # write to file
        #print('-------------------------------------------')

        content = '#pragma once\n' + ''.join(new_text_lines)
        #print('--------------------------content:\n',content)
        #filename = filename + 'pp'
        f = codecs.open(filename, 'w', 'utf-8-sig')
        f.write(content)
        f.close()

    return should_replace


def pretty_dir(root_dir):
    process_num = 0
    for root, dirs, files in os.walk(root_dir):
        print(root.split('/')[-1])
        for filename in files:
            ext = os.path.splitext(filename)[1]
            if ext in cpp_header_ext:
                if pretty_file(root + '/' + filename):
                    print('\t' + filename)
                    process_num = process_num + 1
        for dir in dirs:
            pretty_dir(dir)
    print(process_num, 'file processed.')


def main():
    parser = argparse.ArgumentParser(description='replace #ifdef guard to #pragma once')
    parser.add_argument('-d', '--dir', help='project directory', default='./')
    args = parser.parse_args()
    pretty_dir(args.dir)


if __name__ == '__main__':
    main()
