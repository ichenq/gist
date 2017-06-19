#!/usr/bin/python
# -*-coding:utf-8-*-

import os
import argparse
import codecs
import chardet


# 枚举出所有的文件
def enum_files(rootpath, suffix):
    filelist = []
    for root, dirs, files in os.walk(rootpath):
        for filename in files:
            if filename.endswith(suffix):
                if root != './':
                    filename = root + '/' + filename
                filelist.append(filename)
    return filelist


# 转换文件编码
def convert_encoding(filename, target_encoding):
    content = codecs.open(filename, 'r').read()
    source_encoding = chardet.detect(content)['encoding']
    if source_encoding is None:
        print '%s detect encoding failed' % (filename)
        return
    if source_encoding != target_encoding:
        print '%s from %s to %s' % (filename, source_encoding, target_encoding)
        content = content.decode(source_encoding)
        codecs.open(filename, 'w', encoding=target_encoding).write(content)


def run(args):
    filelist = enum_files(args.dir, '.csv')
    for filename in filelist:
        convert_encoding(filename, args.encoding)


def main():
    defdir = './'
    defencoding = 'utf-8'

    parser = argparse.ArgumentParser(description="Load configurations")
    parser.add_argument("-d", "--dir", help="file directory", default=defdir)
    parser.add_argument("-e", "--encoding", help="target encoding", default=defencoding)
    args = parser.parse_args()
    run(args)


if __name__ == '__main__':
    main()
