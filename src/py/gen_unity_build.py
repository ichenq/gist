#!/usr/bin/python
# -*-coding:utf-8-*-
#
# walk through directory and generate a UnityBuild.cpp
#

import os
import os.path
import argparse


platforms = ['win32', 'linux', 'ios', 'android']


def is_acceptable_source_file(filename, except_name):
    if filename.endswith(except_name):
        return False
    if filename.endswith('stdafx.cpp'):
        return False
    if filename.endswith('.cpp') or filename.endswith('.cc') or filename.endswith('.c'):
        return True
    return False


def is_target_platform_dir(rootpath, platform):
    dirs = rootpath.split(os.sep)
    for name in dirs:
        if name in platforms and name != platform:
            return False
    return True


def collect_source_files(rootdir, target_platform, outfile):
    filenames = []
    prefix = rootdir + os.sep
    for root, dirs, files in os.walk(rootdir):
        if not is_target_platform_dir(root, target_platform):
            continue
        if root == rootdir or root.startswith(prefix):
            root = root[len(prefix):]
        for filename in files:
            if len(root) > 0:
                filename = root + os.sep + filename
            if is_acceptable_source_file(filename, outfile):
                assert filename not in filenames
                filenames.append(filename)
                #print(filename)
    print('collected %d files' % len(filenames))
    return filenames

#
def write_source_file(filenames, outfile):
    f = open(outfile, 'w')
    f.write('#include "stdafx.h"\n\n')
    for name in filenames:
        if name.endswith('.c'):
            f.write('extern "C" {\n')
            f.write('#include "%s"\n' % name)
            f.write('}\n')
        else:
            f.write('#include "%s"\n' % name)
    f.close()
    print('saved to ', outfile)


def main():
    parser = argparse.ArgumentParser(description="")
    parser.add_argument("-d", "--dir", help="project root directory", default='./')
    parser.add_argument("-o", "--output", help="output source file name", default='UnityBuild.cpp')
    parser.add_argument("-p", "--platform", help="project root directory", default='win32')
    args = parser.parse_args()

    filenames = collect_source_files(args.dir, args.platform, args.output,)
    outfile = args.dir + os.sep + args.output
    write_source_file(filenames, outfile)


if __name__ == '__main__':
    main()
