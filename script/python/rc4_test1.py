# Copyright (C) 2016-2017 Jurriaan Bremer.
# See the file 'LICENSE.txt' for copying permission.
import binascii

def rc4(data, key):
    """RC4 encryption and decryption method."""
    S, j, out = list(range(256)), 0, []

    for i in range(256):
        j = (j + S[i] + ord(key[i % len(key)])) % 256
        S[i], S[j] = S[j], S[i]

    i = j = 0
    for ch in data:
        i = (i + 1) % 256
        j = (j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        out.append(chr(ord(ch) ^ S[(S[i] + S[j]) % 256]))

    return "".join(out)

if __name__ == "__main__":
    buf = rc4("hello script, i love python and lua", "dc")
    print binascii.b2a_hex(buf)
    #print buf
    bff =rc4(buf,"dc");
    print bff
    #assert rc4(buf, "dc") == "Hello World"
    print("Ran 1 test..")
