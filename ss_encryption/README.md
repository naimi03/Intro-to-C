# Project - Public Key Cryptography

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
$ ./encrypt [-:i:o:n:vh] [-i input file] [-o output file] [-n public key file]
'''

To decrypt the file run:

'''
$ ./decrypt [-:i:o:n:vh] [-i input file] [-o output file] [-n private key file]
'''


## Notes

This was an assignment for Professor Darell Long at UCSC in Winter '23 for CSE 13S. For help when running the file, please run


'''
$ ./keygen -h  (or)   $ ./encrypt -h (or)  $ ./decrypt -h
'''
