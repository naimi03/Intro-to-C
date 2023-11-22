#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <getopt.h>
#include <inttypes.h>
#include <gmp.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "numtheory.h"
#include "randstate.h"
#include "ss.h"

#define OPTIONS ":b:i:n:d:s:vh"

int main(int argc, char **argv) {
    //initializing variables
    int opt = 0;
    uint64_t bits = 256;
    uint64_t iters = 50;
    char *pbfile = "ss.pub";
    char *pvfile = "ss.priv";
    uint64_t seed = time(NULL);
    int v_called = 0;

    //while there are commands to be read
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {

        switch (opt) {
        case 'b': //bits = input
            if (atoi(optarg) < 5) {
                fprintf(stderr, "Floating point exception\n");
                return 1;
            }
            bits = atoi(optarg);
            break;
        case 'i': //iters = input
            if (atoi(optarg) < 0) {
                fprintf(stderr, "Number of iterations cannot be negative\n");
                return 1;
            }
            iters = atoi(optarg);
            break;
        case 'n': // pbfile = input
            pbfile = optarg;
            break;
        case 'd': // pvfile = input
            pvfile = optarg;
            break;
        case 's': // seed = input
            seed = atoi(optarg);
            break;
        case 'v': //made true if called
            v_called = 1;
            break;
        case 'h': //print help message
            fprintf(stderr, "SYNOPSIS\n");
            fprintf(stderr, "   Generates an SS public/private key pair.\n\n");
            fprintf(stderr, "USAGE\n");
            fprintf(stderr, "   ./keygen [OPTIONS]\n\n");
            fprintf(stderr, "OPTIONS\n");
            fprintf(stderr, "   -h              Display program help and usage.\n");
            fprintf(stderr, "   -v              Display verbose program output.\n");
            fprintf(stderr,
                "   -b bits         Minimum bits needed for public key n (default: 256).\n");
            fprintf(stderr,
                "   -i iterations   Miller-Rabin iterations for testing primes (default: 50).\n");
            fprintf(stderr, "   -n pbfile       Public key file (default: ss.pub).\n");
            fprintf(stderr, "   -d pvfile       Private key file (default: ss.priv).\n");
            fprintf(stderr, "   -s seed         Random seed for testing.\n");
            return 0;
        default: //any errors, print help message
            fprintf(stderr, "SYNOPSIS\n");
            fprintf(stderr, "   Generates an SS public/private key pair.\n\n");
            fprintf(stderr, "USAGE\n");
            fprintf(stderr, "   ./keygen [OPTIONS]\n\n");
            fprintf(stderr, "OPTIONS\n");
            fprintf(stderr, "   -h              Display program help and usage.\n");
            fprintf(stderr, "   -v              Display verbose program output.\n");
            fprintf(stderr,
                "   -b bits         Minimum bits needed for public key n (default: 256).\n");
            fprintf(stderr,
                "   -i iterations   Miller-Rabin iterations for testing primes (default: 50).\n");
            fprintf(stderr, "   -n pbfile       Public key file (default: ss.pub).\n");
            fprintf(stderr, "   -d pvfile       Private key file (default: ss.priv).\n");
            fprintf(stderr, "   -s seed         Random seed for testing.\n");
            return 1;
        }
    }
    //if errors in opening file, print error message
    FILE *pub = fopen(pbfile, "w");
    if (!pub) {
        fprintf(stderr, "%s", "Error in opening public key file\n");
        fclose(pub);
        return 1;
    }
    //if errors in opening file, print error message
    FILE *priv = fopen(pvfile, "w");
    if (!priv) {
        fprintf(stderr, "%s", "Error in opening private key file\n");
        fclose(priv);
        return 1;
    }

    fchmod(fileno(priv), 0600);

    randstate_init(seed);

    //initializing temporary variables
    mpz_t p, q, n, pq, d;
    mpz_inits(p, q, n, pq, d, NULL);

    ss_make_pub(p, q, n, bits, iters);
    ss_make_priv(d, pq, p, q);

    //get username
    char *usr = getenv("USER");

    //write both public and private keysto their respective files
    ss_write_pub(n, usr, pub);
    ss_write_priv(pq, d, priv);

    if (v_called == 1) { //if verbose enabled, print the values
        gmp_fprintf(stderr, "user = %s\n", usr);
        gmp_fprintf(stderr, "p (%lu bits) = %Zd\n", mpz_sizeinbase(p, 2), p);
        gmp_fprintf(stderr, "q (%lu bits) = %Zd\n", mpz_sizeinbase(q, 2), q);
        gmp_fprintf(stderr, "n (%lu bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
        gmp_fprintf(stderr, "pq (%lu bits) = %Zd\n", mpz_sizeinbase(pq, 2), pq);
        gmp_fprintf(stderr, "d (%lu bits) = %Zd\n", mpz_sizeinbase(d, 2), d);
    }

    //clear temporary variables and close files
    fclose(pub);
    fclose(priv);
    randstate_clear();
    mpz_clears(p, q, n, pq, d, NULL);
    return 0;
}
