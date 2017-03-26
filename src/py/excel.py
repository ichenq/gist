#! /usr/bin/env python
# -*- coding: utf-8 -*-

import openpyxl


def main(path):
    wb = openpyxl.load_workbook(filename=path)
    table = wb["Sheet1"]
    cell = table.cell(column=1, row=1)
    print cell.value
    print table['A1'].value
    table['A1'].value = 1023
    #wb.save(filename=path)

if __name__ == "__main__":
    path = "a.xlsx"
    main(path)