#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <stdbool.h>
#include <time.h>

#include "numtheory.h"
#include "randstate.h"
#include "ss.h"

#define OPTIONS ":i:o:n:vh"

int main(int argc, char **argv) {
    //initializing variables
    int opt = 0;
    FILE *infile = stdin;
    FILE *outfile = stdout;
    FILE *priv;
    char *priv_key = "ss.priv";
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
        case 'n': //assigns file name to private key
            priv_key = optarg;
            break;
        case 'v': //made true if called
            v_called = 1;
            break;
        case 'h': //print help message
            fprintf(stderr, "SYNOPSIS\n");
            fprintf(stderr, "   Decrypts data using SS decryption.\n");
            fprintf(stderr, "   Encrypted data is encrypted by the encrypt program.\n\n");
            fprintf(stderr, "USAGE\n");
            fprintf(stderr, "   ./decrypt [OPTIONS]\n\n");
            fprintf(stderr, "OPTIONS\n");
            fprintf(stderr, "   -h              Display program help and usage.\n");
            fprintf(stderr, "   -v              Display verbose program output.\n");
            fprintf(stderr, "   -i infile       Input file of data to decrypt (default: stdin).\n");
            fprintf(
                stderr, "   -o outfile      Output file for decrypted data (default: stdout).\n");
            fprintf(stderr, "   -n pvfile       Private key file (default: ss.priv).\n");
            return 0;
        default: //any errors, print help message
            fprintf(stderr, "SYNOPSIS\n");
            fprintf(stderr, "   Decrypts data using SS decryption.\n");
            fprintf(stderr, "   Encrypted data is encrypted by the encrypt program.\n\n");
            fprintf(stderr, "USAGE\n");
            fprintf(stderr, "   ./decrypt [OPTIONS]\n\n");
            fprintf(stderr, "OPTIONS\n");
            fprintf(stderr, "   -h              Display program help and usage.\n");
            fprintf(stderr, "   -v              Display verbose program output.\n");
            fprintf(stderr, "   -i infile       Input file of data to decrypt (default: stdin).\n");
            fprintf(
                stderr, "   -o outfile      Output file for decrypted data (default: stdout).\n");
            fprintf(stderr, "   -n pvfile       Private key file (default: ss.priv).\n");
            return 1;
        }
    }
    //if errors in opening file, print error message
    priv = fopen(priv_key, "r");
    if (!priv) {
        fprintf(stderr, "%s", "Error in opening private key file\n");
        fclose(priv);
        fclose(infile);
        fclose(outfile);
        return 1;
    }

    //initializing temporary variables
    mpz_t pq, d;
    mpz_inits(pq, d, NULL);

    //read from private key file
    ss_read_priv(pq, d, priv);

    if (v_called == 1) { //if verbose enabled, print values
        gmp_fprintf(stderr, "pq (%lu bits): %Zd\n", mpz_sizeinbase(pq, 2), pq);
        gmp_fprintf(stderr, "d (%lu bits): %Zd\n", mpz_sizeinbase(d, 2), d);
    }

    //decrypt file
    ss_decrypt_file(infile, outfile, d, pq);

    //clear temporary variables and close files
    fclose(priv);
    fclose(infile);
    fclose(outfile);
    mpz_clears(pq, d, NULL);
    return 0;
}
