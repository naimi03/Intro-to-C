# Assignment 5 - Public Key Cryptography

This program contains the implementation of RSA algorithm for encryption and decryption.

## Building 

Run the following to build the keygen program to generate public and private key files:


'''
$ make keygen
'''


Run the following to build the encrypt program to encrypt input file


'''
$ make encrypt 
'''


Run the following to build the encrypt program to decrypt input file


'''
$ make decrypt 
'''

Run just make to build all the above executables.

## Running

Run the following to generate the keys

'''
$ ./keygen [-:b:i:n:d:s:vh] [-b minimum number of bits] [-i iterations] [-n public key file] [-d private key file] [-s seed]
'''

To encrypt the file, run:

'''
$ ./encypt [-:i:o:n:vh] [-i input file] [-o output file] [-n public key file]
'''

To decrypt the file run:

'''
$ ./encypt [-:i:o:n:vh] [-i input file] [-o output file] [-n private key file]
'''

## Files

randstate.h - header file for randstate.c

randstate.c - contains the interface for creating and clearing the random number generator.

numtheory.h - header file for numtheory.c

numtheory.c - contains the implementation of number theoretic functions.

ss.h - header file for ss.c

ss.c - contains the implementation of function for rsa encryption and decryption.

keygen.c - the interface for creating public and private key files.

encrypt.c - the interface for encrypting files.

encrypt.c - the interface for decrypting files.

Makefile - file to make or build the program.

DESIGN.pdf - contains the intial and final design for all the above files

WRITEUP.pdf - contains discussion on my difficulties with this assignment
