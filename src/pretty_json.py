#!/usr/bin/python
# -*-coding:utf-8-*-

import json
import codecs
import argparse


def run(args):
    assert len(args.input) > 0
    f = open(args.input, 'r')
    obj = json.loads(f.read())
    content = json.dumps(obj, sort_keys=True, indent=args.indent, allow_nan=False, ensure_ascii=False)
    if args.output is None:
        print content
    else:
        f = codecs.open(args.output, 'w', 'utf-8')
        f.write(content)
        f.close()
        print('Saved to ' + args.output)


def main():
    parser = argparse.ArgumentParser(description="Pretty print JSON text")
    parser.add_argument("-i", "--input", help="Input JSON text file")
    parser.add_argument("-t", "--indent", help="Indent tab size", type=int, default=2)
    parser.add_argument("-o", "--output", help="Input JSON text file")
    args = parser.parse_args()
    run(args)

if __name__ == '__main__':
    main()
