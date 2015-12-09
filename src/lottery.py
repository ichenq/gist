#!/usr/bin/python
# -*-coding:utf-8-*-

import sys
import string


#统计
def satisics(numbers):
  my_numbers = []
	last_row = []
	for row in numbers:
		assert len(row) == 7
		last_row.append(row[6])
		del row[6]
		my_numbers.extend(row)
	print '前6行(数字，次数):'
	print_frequency(my_numbers)
	print '第7行(数字，次数):'
	print_frequency(last_row)

#打印出现频率
def print_frequency(numbers):
	my_dict = {}
	unique_numbers = set(numbers)
	for n in unique_numbers:
		count = numbers.count(n)
		my_dict.setdefault(n, count)
	result = sorted(my_dict.items(), key = lambda item:item[1], reverse=True)
	for item in result: print item



# 读取文件最后50行
def load_file(file_name):
	total_numbers = []
	total_lines = open(file_name).readlines()
	start_line_count = 0
	if len(total_lines) > 50:
		start_line_count = len(total_lines) - 50
	
	line_count = 0
	for line in total_lines:
		if line_count >= start_line_count:
			total_numbers.append(map(string.atoi, line.split(' ')))
		line_count = line_count + 1
	return total_numbers

# 主函数
def main():
	##assert len(sys.argv) >= 2
	file_name = 'E:/workspace/num.txt' #数据文件的位置
	satisics(load_file(file_name))

    
if __name__ == '__main__':
	main()
    