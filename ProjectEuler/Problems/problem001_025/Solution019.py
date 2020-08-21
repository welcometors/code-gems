# https://projecteuler.net/problem=19

# You are given the following information, but you may prefer to do some research for yourself.
# 
# 1 Jan 1900 was a Monday.
# Thirty days has September,
# April, June and November.
# All the rest have thirty-one,
# Saving February alone,
# Which has twenty-eight, rain or shine.
# And on leap years, twenty-nine.
# A leap year occurs on any year evenly divisible by 4, but not on a century unless it is divisible by 400.
# How many Sundays fell on the first of the month during the twentieth century (1 Jan 1901 to 31 Dec 2000)?

from datetime import date
from datetime import timedelta

def adMonth(today):
	if today.month < 12:
		return date(today.year, today.month+1, 1)
	else:
		return date(today.year+1, 1, 1)

def getDate(data, process=True):
	d = date(data[0], data[1], 1)
	if process and data[2] != 1:
		d = adMonth(d)
	return d

for i in range(1):
	d0 = getDate([1901, 1, 1])
	d1 = getDate([2000, 12, 1], False)
	sum = 0
	while d0 <= d1:
		sum += 1 if d0.weekday() == 6 else 0
		d0 = adMonth(d0)
	print(sum)