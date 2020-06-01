#!/usr/bin/env python2

f = open('corrupt','rb').read()

index = f.find('\x00'*10)

encoded_flag = f[index:].replace('\x00'*23,'')

def decode(ef):
    possible_flags = []
    for xor in range(0, 127):
        for sub in range(0, 127):
            try:
                flag = ''.join([chr((ord(i) ^ xor) - sub) for i in ef])
                if "ASM" in flag or "A5M" in flag or "asm" in flag or "a5m" in flag:
                    possible_flags.append(flag)
            except:
                continue
    return [flag for flag in possible_flags if flag[-1] == '\n']

decoded_flag = decode(encoded_flag)

assert len(decoded_flag) == 1

print decoded_flag[0]
