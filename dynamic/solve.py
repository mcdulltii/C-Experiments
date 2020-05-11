#!/usr/bin/env python2

f = open('dynamic', 'rb').read().replace('\x00','')

#iciicicciciicicicciciiciicciiciciic
sequence = f[f.find('flag?')+len('flag?'):f.find('Correct')]

"""
ELF Strings REF
Guess the flag?
iciicicciciicicicciciiciicciiciciic
Correct
<35 Rows of (OFFSET)\x__ Characters>
"""

encoded_flag = f[f.find('Correct')+len('Correct'):][:35]
flag = ""

#Characters are encoded via the "ic..." sequence, where i remains the same, and c is offset by 0x47
for char in range(len(encoded_flag)):
    if sequence[char] == 'i':
        flag += encoded_flag[char]
    elif sequence[char] == 'c':
        flag += chr(ord(encoded_flag[char]) - 0x47)
    else:
        print 'Sequence is incorrect!\n'
print flag
