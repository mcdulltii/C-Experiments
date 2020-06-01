#!/usr/bin/env python2

f = open('corrupt','rb').read()

index = f.find('\x00'*10)

encoded_flag = f[index:].replace('\x00'*23,'')

flag = ''.join([chr((ord(i) ^ ord('9')) - ord('d')) for i in encoded_flag])

print flag
