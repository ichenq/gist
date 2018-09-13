#!/usr/bin/python
# -*-coding:utf-8-*-

import json
import codecs
import argparse
import os


def pretty_file(filename):
    f = open(filename, 'r')
    obj = json.loads(f.read())
    f.close()
    content = json.dumps(obj, sort_keys=False, indent=2, allow_nan=False, ensure_ascii=False)    
    f = codecs.open(filename, 'w', 'utf-8')
    f.write(content)
    f.close()
    print('Saved to ' + filename)
        

def run(args):
    rootdir = args.dir
    for root, dirs, files in os.walk(rootdir):
        for filename in files:
            if len(root) > 0:
                filename = root + os.sep + filename
            if filename.endswith('.json'):
                print(filename)
                pretty_file(filename)



def main():
    parser = argparse.ArgumentParser(description="Pretty print JSON text")
    parser.add_argument("-d", "--dir", help="Input JSON text file")
    parser.add_argument("-t", "--indent", help="Indent tab size", type=int, default=2)
    parser.add_argument("-o", "--output", help="Input JSON text file")
    args = parser.parse_args()
    run(args)

if __name__ == '__main__':
    main()
