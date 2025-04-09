#!/usr/bin/python3

'''
discards the parts of pioasm's output we do not need
'''

from fileinput import input
from re import sub, split

lines = [split(' ', x) for x in list(input())]

i = 0
while(lines[i][0] != '#define'):
    i += 1
j = i
while(lines[j][0] != '#if'):
    j += 1

for x in lines[i:j]:
    print(' '.join(x), end = '')
