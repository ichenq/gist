#!/usr/bin/python
# -*-coding:utf-8-*-
#
# unity build cpp files, see:
#   https://buffered.io/posts/the-magic-of-unity-builds/
#

from __future__ import unicode_literals
from __future__ import print_function


import os
import argparse
import codecs
import shutil
from chardet.universaldetector import UniversalDetector

cpp_ext = ['.cc', '.cpp', '.cxx']

detector = UniversalDetector()

def to_ipp_filename(filename):
    for ext in cpp_ext:
        if filename.endswith(ext):
            idx = len(filename) - len(ext)
            return filename[:idx] + '.ipp'


def find_last_include(text_lines):
    last_index = 0
    idx = 0
    for line in text_lines:
        if line.startswith('#'):
            last_index = idx
        if line.startswith('{'):
            break
        idx += 1
    return last_index + 1


def make_unqiue_line(lines):
    newlines = []
    for line in lines:
        if line not in newlines:
            newlines.append(line)
    return newlines
   

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

                
def unity_build(args):
    source_lines = ['#include "stdafx.h"\n']
    allfiles = get_filename_list(args.dir)
    for filename in allfiles:
        if filename.endswith('unitybuild.cpp'):
            continue
        newfilename = to_ipp_filename(filename)
        shutil.move(filename, newfilename)
        line = '#include "%s"\n' % os.path.basename(newfilename)
        source_lines.append(line)
    
    #include_header_lines = make_unqiue_line(include_header_lines)
    filename = args.dir + '/unitybuild.cpp'
    f = codecs.open(filename, 'w', 'utf-8-sig')
    f.writelines(source_lines)
    f.close()
    print('wrote to ', filename)
    
    
def get_filename_list(rootdir):
    all_files = []
    for root, dirs, files in os.walk(rootdir):
        for filename in files:
            ext = os.path.splitext(filename)[1]
            if ext in cpp_ext:
                filename = root + '/' + filename
                all_files.append(filename)
        break
    return all_files
        

def main():
    parser = argparse.ArgumentParser(description='unity build cpp files')
    parser.add_argument('-d', '--dir', help='source file directory', default='./')
    args = parser.parse_args()
    unity_build(args)


if __name__ == '__main__':
    main()