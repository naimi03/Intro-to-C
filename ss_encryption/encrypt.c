#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <stdbool.h>
#include <time.h>
//import necessary files
#include "numtheory.h"
#include "randstate.h"
#include "ss.h"

//define options
#define OPTIONS ":i:o:n:vh"

int main(int argc, char **argv) {
    //initializing variables
    int opt = 0;
    FILE *infile = stdin;
    FILE *outfile = stdout;
    FILE *pub;
    char *pub_key = "ss.pub";
    int v_called = 0;

    //while there are commands to be read
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {

        switch (opt) {
        case 'i': //open optarg file
            infile = fopen(optarg, "r");
            break;
        case 'o': //open optarg file
            outfile = fopen(optarg, "w");
            break;
        case 'n': //assigns file name to public key
            pub_key = optarg;
            break;
        case 'v': //made true if called
            v_called = 1;
            break;
        case 'h': //print help message
            fprintf(stderr, "SYNOPSIS\n");
            fprintf(stderr, "   Encrypts data using SS encryption.\n");
            fprintf(stderr, "   Encrypted data is decrypted by the decrypt program.\n\n");
            fprintf(stderr, "USAGE\n");
            fprintf(stderr, "   ./encrypt [OPTIONS]\n\n");
            fprintf(stderr, "OPTIONS\n");
            fprintf(stderr, "   -h              Display program help and usage.\n");
            fprintf(stderr, "   -v              Display verbose program output.\n");
            fprintf(stderr, "   -i infile       Input file of data to encrypt (default: stdin).\n");
            fprintf(
                stderr, "   -o outfile      Output file for encrypted data (default: stdout).\n");
            fprintf(stderr, "   -n pbfile       Public key file (default: ss.pub).\n");
            return 0;
        default: //any errors, print help message
            fprintf(stderr, "SYNOPSIS\n");
            fprintf(stderr, "   Encrypts data using SS encryption.\n");
            fprintf(stderr, "   Encrypted data is decrypted by the decrypt program.\n\n");
            fprintf(stderr, "USAGE\n");
            fprintf(stderr, "   ./encrypt [OPTIONS]\n\n");
            fprintf(stderr, "OPTIONS\n");
            fprintf(stderr, "   -h              Display program help and usage.\n");
            fprintf(stderr, "   -v              Display verbose program output.\n");
            fprintf(stderr, "   -i infile       Input file of data to encrypt (default: stdin).\n");
            fprintf(
                stderr, "   -o outfile      Output file for encrypted data (default: stdout).\n");
            fprintf(stderr, "   -n pbfile       Public key file (default: ss.pub).\n");
            return 1;
        }
    }

    //if errors in opening file, print error message
    pub = fopen(pub_key, "r");
    if (!pub) {
        fprintf(stderr, "%s", "Error in opening public key file\n");
        return 1;
    }

    //initializing temporary variables
    mpz_t n;
    mpz_init(n);

    char user_name[1024];

    //read public key file
    ss_read_pub(n, user_name, pub);

    if (v_called == 1) { //print if verbose is enabled
        gmp_fprintf(stderr, "user = %s\n", user_name);
        gmp_fprintf(stderr, "n (%lu bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
    }

    //encrpt the file
    ss_encrypt_file(infile, outfile, n);

    //clear temporary variables and close files
    fclose(pub);
    fclose(infile);
    fclose(outfile);
    mpz_clear(n);
    return 0;
}
