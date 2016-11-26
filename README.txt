@ Neil Chen

This project is based on the paper The RC6 Block Cipher
Link of the paper:  https://people.csail.mit.edu/rivest/pubs/RRSY98.pdf

Test:	Code was tested on remote.cs.binghamton.edu and Ubuntu, works good on the test vectors on page 20 of the paper, and other vectors.


Accoding to the paper, my project works for a plaintext or cyphertext of 16Bytes. Does not support larger blocks.

If the original text is less than 16 Bytes, then it will considered that the rest bits are all 0's.
If the original key is big than (N-1)*32 but less than N*32 bits, then it will be considered as  N*32 bits, and be fufilled with 0's.

The security key supports: 32, 64, 96, 128, 160, 192,  224,  256 bits, when there are more bits, it just ignores.

The length of the input and output file names should not longger than 29 characters.

There should be space or new line between plaintext/ciphertext  and  hexadecimal numbers.


eg.

Encryption
plaintext: 00 00 00 00 00 00 00 00 00 00 00 00 
userkey: 00 00 00 00 00 00 00 00 00 00 00 00 0

Decryption
ciphertext: 8f c3 a5 36 56 b1 f7 78 c1 29 df 4e 98 48 a4 1e 
userkey: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 

