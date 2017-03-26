#!/usr/bin/python
# -*-coding:utf-8-*-

import argparse
import json

def parse(filename):
    result_lines = []
    lines = open(filename).readlines()
    into = False
    for line in lines:
        if line.startswith('request'): # next line is JSON string as we need
            into = True
            continue
        if into :
            into = False
            assert json.loads(line) != None
            result_lines.append(line)
    return  ''.join(result_lines)
        

def main():
    parser = argparse.ArgumentParser(description="Parse battle output JSON")
    parser.add_argument("-i", "--input", help="input battle stdout file")
    parser.add_argument("-o", "--output", help="output JSON file", default="battlerequest.json")
    args = parser.parse_args()

    context = parse(args.input)
    open(args.output,'a+').write(context)
    print('writed to ', args.output)


if __name__ == '__main__':
    main()
