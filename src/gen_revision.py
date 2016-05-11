#!/usr/bin/python
# -*-coding:utf-8-*-

import os
import subprocess


svn_template = """package main

// `go:generate` produced code, DO NOT EDIT!
import (
    "fmt"
    "runtime"
)

func GetVersion() string {
    return fmt.Sprintf("rev %s, %s (built w/%%s)", runtime.Version())
}
"""

git_template = """package main

import (
    "fmt"
    "runtime"
)

// `go:generate` produced code, DO NOT EDIT!
func GetVersion() string {
    return fmt.Sprintf("rev %s %s (built w/%%s)", runtime.Version())
}
"""


# generate go source file
def generate_svn_source(info):
    revision = info['Revision']
    date = info['Last Changed Date']
    date = date[:date.index('(')]
    content = svn_template % (revision, date)
    open('version.go', 'w').write(content)


# current SVN version information
def get_svn_infomation():
    cmd = "svn info $GOPATH"
    output = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT).communicate()
    assert output[1] == None # stderr
    infomation = {}
    for line in output[0].split('\n'):
        items = line.split(': ')
        if len(items) == 2:
            infomation[items[0]] = items[1]
    return infomation


# last git commit info
def get_git_information():
    cmd = 'git log --date=iso --abbrev-commit -1'
    output = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT).communicate()
    assert output[1] == None # stderr
    lines = output[0].split('\n')
    assert len(lines) >= 3
    commit = ''
    date = ''
    if lines[0].startswith('commit '):
        commit = lines[0][7:].strip()
    if lines[2].startswith('Date:'):
        date = lines[2][6:].strip()
    assert len(commit) > 0
    assert len(date) > 0
    content = git_template % (commit, date)
    open('version.go', 'w').write(content)

if __name__ == '__main__':
    get_git_information()


