#include "randstate.h"
#include "numtheory.h"
#include <stdlib.h>
#include <time.h>

#define EPSILON 1e-14

void ss_make_pub(mpz_t p, mpz_t q, mpz_t n, uint64_t nbits, uint64_t iters) {

    //init temp vars
    mpz_t p_sq, p_min, q_min;
    mpz_inits(p_sq, p_min, q_min, NULL);

    //uint64_t p_bits = (rand() % (upper - lower + 1)) + lower
    //logic is from geeks for geeks
    uint64_t p_bits = (random() % (((2 * nbits) / 5) - (nbits / 5) + 1) + (nbits / 5));
    uint64_t q_bits = nbits - 2 * p_bits;

    //create prime numbers

    uint64_t ln_n;

    while (true) {

        //create two prime numbers
        make_prime(p, p_bits, iters);
        make_prime(q, q_bits, iters);

        //get p-1 and q-1
        mpz_sub_ui(p_min, p, 1);
        mpz_sub_ui(q_min, q, 1);

        //n = p*p*q
        mpz_mul(p_sq, p, p);
        mpz_mul(n, p_sq, q);

        //getting log of n
        ln_n = mpz_sizeinbase(n, 2);

        //if p-1 != q or q-1 != p and log n is >= nbits, p and q have been found satisfying the requirements
        if ((mpz_cmp(p_min, q) != 0 || mpz_cmp(q_min, p) != 0) && (ln_n >= nbits)) {
            break;
        }
    }
    mpz_clears(p_sq, p_min, q_min, NULL);
}

void ss_make_priv(mpz_t d, mpz_t pq, const mpz_t p, const mpz_t q) {

    //initializing temporary variables
    mpz_t result, lambda, mul, p_min, q_min, n;
    mpz_inits(result, lambda, mul, p_min, q_min, n, NULL);

    // p-1 and q-1 are set
    mpz_sub_ui(p_min, p, 1);
    mpz_sub_ui(q_min, q, 1);

    //getting lcm(p-1, q-1)
    gcd(result, p_min, q_min);
    mpz_mul(mul, p_min, q_min);
    mpz_div(lambda, mul, result);

    mpz_mul(pq, p, q);
    mpz_mul(n, pq, p);
    // computing d as inverse of pq modulo lambda(n)
    mod_inverse(d, n, lambda);

    //clear temporary variables
    mpz_clears(result, lambda, mul, p_min, q_min, n, NULL);
}

void ss_write_pub(const mpz_t n, const char username[], FILE *pbfile) {
    //write to pbfile with trailing newlines
    gmp_fprintf(pbfile, "%Zx\n%s\n", n, username);
}

void ss_write_priv(const mpz_t pq, const mpz_t d, FILE *pvfile) {
    //write to pvfile with trailing newlines
    gmp_fprintf(pvfile, "%Zx\n%Zx\n", pq, d);
}

void ss_read_pub(mpz_t n, char username[], FILE *pbfile) {
    gmp_fscanf(pbfile, "%Zx", n);
    gmp_fscanf(pbfile, "%s", username);
}

void ss_read_priv(mpz_t pq, mpz_t d, FILE *pvfile) {
    //read from pvfile
    gmp_fscanf(pvfile, "%Zx", pq);
    gmp_fscanf(pvfile, "%Zx", d);
}

void ss_encrypt(mpz_t c, const mpz_t m, const mpz_t n) {
    //c = m^n(%n)
    pow_mod(c, m, n, n);
}

void ss_encrypt_file(FILE *infile, FILE *outfile, const mpz_t n) {
    //initializing temporary variables
    mpz_t m, out, n_sqt;
    mpz_inits(m, out, n_sqt, NULL);

    mpz_sqrt(n_sqt, n);
    size_t k = ((mpz_sizeinbase(n_sqt, 2) - 1) / 8); // = [(log(root n)-1)/8];

    //dynamically allocating an array
    uint8_t *arr = (uint8_t *) calloc(k, sizeof(uint8_t));

    //0th element is left as zero
    arr[0] = 0xFF;
    size_t j;

    //while the end of file has not been encountered
    while (!feof(infile)) {

        // j is actual number of bytes
        j = fread(&arr[1], 1, k - 1, infile);
        //import message into m
        mpz_import(m, j + 1, 1, 1, 1, 0, arr);
        //encrypt m
        ss_encrypt(out, m, n);
        //write encrypted message to file
        gmp_fprintf(outfile, "%Zx\n", out);
    }

    //clear temporary variables
    mpz_clears(m, out, n_sqt, NULL);
    free(arr);
}

void ss_decrypt(mpz_t m, const mpz_t c, const mpz_t d, const mpz_t pq) {
    //m = c^d(%pq)
    pow_mod(m, c, d, pq);
}

void ss_decrypt_file(FILE *infile, FILE *outfile, const mpz_t d, const mpz_t pq) {
    //initializing temporary variables
    mpz_t c, m, out;
    mpz_inits(c, m, out, NULL);

    size_t k = ((mpz_sizeinbase(pq, 2) - 1) / 8); // = [(log(pq)-1)/8];

    //dynamically allocating an array
    uint8_t *arr = calloc(k, sizeof(uint8_t));

    //0th element is left as zero
    arr[0] = 0xFF;
    size_t j;

    //while the end of file has not been encountered
    while (!feof(infile)) {
        //scan a line from the enrypted file
        gmp_fscanf(infile, "%Zx\n", c);
        //decrypt each line
        ss_decrypt(m, c, d, pq);
        //export the decrypted message
        mpz_export(arr, &j, 1, 1, 1, 0, m);

        uint64_t j_re = j - 1; //converting j from size_t to uint64_t for frwite

        //write the decrypted output to file
        fwrite(arr + 1, 1, j_re, outfile);
    }

    //clear temporary variables
    mpz_clears(c, m, out, NULL);
    free(arr);
}
