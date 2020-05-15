#!/usr/bin/env python2

f = open('rewrite', 'rb').read()

#Loading of encoded int array
"""
0x000012ea      c745a0750000.  mov dword [var_60h], 0x75   ; 'u'
0x000012f1      c745a4820000.  mov dword [var_5ch], 0x82
0x000012f8      c745a87f0000.  mov dword [var_58h], 0x7f
0x000012ff      c745ac7b0000.  mov dword [var_54h], 0x7b   ; '{'
0x00001306      c745b08a0000.  mov dword [var_50h], 0x8a
0x0000130d      c745b4770000.  mov dword [var_4ch], 0x77   ; 'w'
0x00001314      c745b81a0000.  mov dword [var_48h], 0x1a
0x0000131b      c78560ffffff.  mov dword [var_a0h], 0x49   ; 'I'
0x00001325      c78564ffffff.  mov dword [var_9ch], 0x8a
0x0000132f      c78568ffffff.  mov dword [var_98h], 0x8c
0x00001339      c7856cffffff.  mov dword [var_94h], 0x89
0x00001343      c78570ffffff.  mov dword [var_90h], 0x7d   ; '}'
0x0000134d      c78574ffffff.  mov dword [var_8ch], 0x49   ; 'I'
0x00001357      c78578ffffff.  mov dword [var_88h], 0x3f   ; '?'
0x00001361      c7857cffffff.  mov dword [var_84h], 0x7e   ; '~'
0x0000136b      c74580490000.  mov dword [var_80h], 0x49   ; 'I'
0x00001372      c74584870000.  mov dword [var_7ch], 0x87
0x00001379      c745887b0000.  mov dword [var_78h], 0x7b   ; '{'
0x00001380      c7458c8a0000.  mov dword [var_74h], 0x8a
0x00001387      c745908d0000.  mov dword [var_70h], 0x8d
0x0000138e      c745941a0000.  mov dword [var_6ch], 0x1a
0x00001395      c78520ffffff.  mov dword [var_e0h], 0x49   ; 'I'
0x0000139f      c78524ffffff.  mov dword [var_dch], 0x8a
0x000013a9      c78528ffffff.  mov dword [var_d8h], 0x8c
0x000013b3      c7852cffffff.  mov dword [var_d4h], 0x89
0x000013bd      c78530ffffff.  mov dword [var_d0h], 0x7d   ; '}'
0x000013c7      c78534ffffff.  mov dword [var_cch], 0x49   ; 'I'
0x000013d1      c78538ffffff.  mov dword [var_c8h], 0x3f   ; '?'
0x000013db      c7853cffffff.  mov dword [var_c4h], 0x7e   ; '~'
0x000013e5      c78540ffffff.  mov dword [var_c0h], 0x49   ; 'I'
0x000013ef      c78544ffffff.  mov dword [var_bch], 0x87
0x000013f9      c78548ffffff.  mov dword [var_b8h], 0x7f
0x00001403      c7854cffffff.  mov dword [var_b4h], 0x87
0x0000140d      c78550ffffff.  mov dword [var_b0h], 0x1a
"""


#Split opcodes with '\xc7' (mov), then return mov-ed character (last character before '\x00's)
def decode(data, key):
    return ''.join([chr(ord(i.replace('\x00','')[-1]) - key) for i in data.split('\xc7') if i])


print("Decoding strings from binary...")

heap_dat = f[0x12ea:0x131b]
heap = decode(heap_dat, 0x1A)

maps_dat = f[0x131b:0x1395]
maps = decode(maps_dat, 0x1A)

mem_dat = f[0x1395:0x1417]
mem = decode(mem_dat, 0x1A)

print("Heap: {}\nProcMaps: {}\nProcMem: {}\n".format(heap, maps, mem))


#Loading of encoded flag
"""
0x00001417      c78590feffff.  mov dword [var_170h], 0x8b
0x00001421      c78594feffff.  mov dword [var_16ch], 0x89
0x0000142b      c78598feffff.  mov dword [var_168h], 0x87
0x00001435      c7859cfeffff.  mov dword [var_164h], 0xbc
0x0000143f      c785a0feffff.  mov dword [var_160h], 0xb5
0x00001449      c785a4feffff.  mov dword [var_15ch], 0xc3
0x00001453      c785a8feffff.  mov dword [var_158h], 0x86
0x0000145d      c785acfeffff.  mov dword [var_154h], 0xba
0x00001467      c785b0feffff.  mov dword [var_150h], 0x87
0x00001471      c785b4feffff.  mov dword [var_14ch], 0xbc
0x0000147b      c785b8feffff.  mov dword [var_148h], 0xcf
0x00001485      c785bcfeffff.  mov dword [var_144h], 0x87
0x0000148f      c785c0feffff.  mov dword [var_140h], 0xc4
0x00001499      c785c4feffff.  mov dword [var_13ch], 0xbd
0x000014a3      c785c8feffff.  mov dword [var_138h], 0xb5
0x000014ad      c785ccfeffff.  mov dword [var_134h], 0x8a
0x000014b7      c785d0feffff.  mov dword [var_130h], 0x87
0x000014c1      c785d4feffff.  mov dword [var_12ch], 0x8d
0x000014cb      c785d8feffff.  mov dword [var_128h], 0x89
0x000014d5      c785dcfeffff.  mov dword [var_124h], 0xc8
0x000014df      c785e0feffff.  mov dword [var_120h], 0xc4
0x000014e9      c785e4feffff.  mov dword [var_11ch], 0x8a
0x000014f3      c785e8feffff.  mov dword [var_118h], 0x8d
0x000014fd      c785ecfeffff.  mov dword [var_114h], 0x87
0x00001507      c785f0feffff.  mov dword [var_110h], 0xcc
0x00001511      c785f4feffff.  mov dword [var_10ch], 0x89
0x0000151b      c785f8feffff.  mov dword [var_108h], 0xb5
0x00001525      c785fcfeffff.  mov dword [var_104h], 0x89
0x0000152f      c78500ffffff.  mov dword [var_100h], 0x8f
0x00001539      c78504ffffff.  mov dword [var_fch], 0x8a
0x00001543      c78508ffffff.  mov dword [var_f8h], 0x8d
0x0000154d      c7850cffffff.  mov dword [var_f4h], 0x8b
0x00001557      c78510ffffff.  mov dword [var_f0h], 0x8e
0x00001561      c78514ffffff.  mov dword [var_ech], 0x88
0x0000156b      c78518ffffff.  mov dword [var_e8h], 0x8f
0x00001575      c7851cffffff.  mov dword [var_e4h], 0x56   ; 'V'
"""


print("Decoding flag from binary...")

flag_dat = f[0x1417:0x157f]
flag = decode(flag_dat, 0x56)

print("Flag: {}".format(flag))

