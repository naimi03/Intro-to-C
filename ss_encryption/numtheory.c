
#include "numtheory.h"
#include "randstate.h"

//gmp_randstate_t state;

void gcd(mpz_t g, const mpz_t a, const mpz_t b) {
    //initializing temporary variables
    mpz_t t, ain, bin;
    mpz_inits(t, ain, bin, NULL);

    //setting temporary variable values
    mpz_set(ain, a);
    mpz_set(bin, b);

    //while (bin != 0)
    while (mpz_cmp_ui(bin, 0)
           != 0) { // checks if b != 0 by checking if the comparison returns zero (means equal)

        //calc gcd
        mpz_set(t, bin); //t = bin
        mpz_mod(bin, ain, bin); // b = a%b
        mpz_set(ain, t); //ain = t
    }
    //set the answer d to ain
    mpz_set(g, ain);

    //clear temporary variables
    mpz_clears(t, ain, bin, NULL);
}

//void mod_inverse(mpz_t i, mpz_t a, mpz_t n){
void mod_inverse(mpz_t o, const mpz_t a, const mpz_t n) {
    //initializing temporary variables
    mpz_t r, r_dash, q, q_r, q_t, temp, o_temp, t, t_dash;
    mpz_inits(r, r_dash, q, q_r, q_t, temp, o_temp, t, t_dash, NULL);

    //setting temporary variables r and r'
    mpz_set(r, n);
    mpz_set(r_dash, a);

    //setting temporary variables t and t'
    mpz_set_ui(t, 0);
    mpz_set_ui(t_dash, 1);

    //while(r' != 0)
    while (mpz_cmp_ui(r_dash, 0) != 0) {

        //quoteint of r/r'
        mpz_fdiv_q(q, r, r_dash);

        //temp = r and r = r'
        mpz_set(temp, r);
        mpz_set(r, r_dash);

        //r' = temp - (q*r');
        mpz_mul(q_r, q, r_dash);
        mpz_sub(r_dash, temp, q_r);

        //temp = t and t = t'
        mpz_set(o_temp, t);
        mpz_set(t, t_dash);

        //t' = temp - (q*t');
        mpz_mul(q_t, q, t_dash);
        mpz_sub(t_dash, o_temp, q_t);
    }

    //if r > 1
    if (mpz_cmp_ui(r, 1) > 0) {

        //o = 0
        mpz_set_ui(o, 0);
        mpz_clears(r, r_dash, q, q_r, q_t, temp, o_temp, t, t_dash, NULL);
        return;
    }

    //if t<0
    if (mpz_cmp_ui(t, 0) < 0) {

        //o = t+n;
        mpz_add(t, t, n);
        mpz_set(o, t);
        mpz_clears(r, r_dash, q, q_r, q_t, temp, o_temp, t, t_dash, NULL);
        return;
    }

    //o = t
    mpz_set(o, t);

    //clear temporary variables
    mpz_clears(r, r_dash, q, q_r, q_t, temp, o_temp, t, t_dash, NULL);
}

void pow_mod(mpz_t o, const mpz_t a, const mpz_t d, const mpz_t n) {
    //initializing temporary variables
    mpz_t v, p, v_p, vp_mod_n, p_sq, ex;
    mpz_inits(v, p, v_p, vp_mod_n, p_sq, ex, NULL);

    //setting temporary variable values
    mpz_set_ui(v, 1); //mpz_t v = 1
    mpz_set(p, a); //mpz_t p = a
    mpz_set(ex, d);

    while (mpz_cmp_ui(ex, 0) > 0) { //while(d > 0)

        //mpz_mod_ui(d_odd, d, 2); // d_odd = d%2
        if (mpz_divisible_ui_p(ex, 2) == 0) { //if (d_odd == 1 (cmp gives 0 when equal))

            //v = (v*p)%n;
            mpz_mul(v_p, v, p);
            mpz_mod(vp_mod_n, v_p, n);
            mpz_set(v, vp_mod_n);
        }

        //p = (p*p)%n;
        mpz_mul(p_sq, p, p);
        mpz_mod(p, p_sq, n);

        //d = d/2;
        mpz_fdiv_q_ui(ex, ex, 2);
    }
    //o = v;
    mpz_set(o, v);

    //clear temporary variables
    mpz_clears(v, p, v_p, vp_mod_n, p_sq, ex, NULL);
}

bool is_prime(const mpz_t n, uint64_t iters) {
    //returning false if n is 1 or 0
    if (mpz_cmp_ui(n, 0) == 0 || mpz_cmp_ui(n, 1) == 0) {
        return false;
        //returning false if n is even
    } else if (mpz_cmp_ui(n, 2) == 0 || mpz_cmp_ui(n, 3) == 0) {
        return true;
    } else if (mpz_even_p(n) != 0) {
        return false;
        //returning true if n is 2 or 3
    }

    //initializing temporary variables
    mpz_t r, s, n_min, r_odd, a, y, j, s_min, two;
    mpz_inits(r, s, n_min, r_odd, a, y, j, s_min, two, NULL);

    //n_min, r, two, s, r_odd, s_min, a, y, j,

    // setting temporary variables
    mpz_sub_ui(n_min, n, 1);
    mpz_set(r, n_min);
    mpz_set_ui(two, 2); // done because pow_mod only takes mpz_t types
    mpz_set_ui(s, 0);
    mpz_mod(r_odd, r, two);

    //while (r%2 != 1)
    while (mpz_cmp_ui(r_odd, 1) != 0) {

        //r = r/2
        mpz_fdiv_q(r, r, two);

        //r_odd = r%2
        mpz_mod(r_odd, r, two);

        //s++
        mpz_add_ui(s, s, 1);
    }

    //s_min = s-1
    mpz_sub_ui(s_min, s, 1);

    for (uint64_t i = 0; i < iters; i++) {

        //random of a with n bits
        mpz_urandomm(a, state, n);
        mpz_add_ui(a, a, 2);

        //y = a^r(%n)
        pow_mod(y, a, r, n);

        //if( y != 1 && y != n-1)
        if ((mpz_cmp_ui(y, 1) != 0) && mpz_cmp(y, n_min) != 0) {
            //j = 1
            mpz_set_ui(j, 1);

            // while(j <= s-1 && y != n_min)
            while ((mpz_cmp(j, s_min) <= 0) && (mpz_cmp(y, n_min) != 0)) {

                //y = y^2(%n)
                pow_mod(y, y, two, n);

                //if (y == 1)
                if (mpz_cmp_ui(y, 1) == 0) {
                    mpz_clears(r, s, n_min, r_odd, a, y, j, s_min, two, NULL);
                    return false;
                }

                //j++
                mpz_add_ui(j, j, 1);
            }

            //if y != n_min)
            if (mpz_cmp(y, n_min) != 0) {
                mpz_clears(r, s, n_min, r_odd, a, y, j, s_min, two, NULL);
                return false;
            }
        }
    }

    //clear temporary variables
    mpz_clears(r, s, n_min, r_odd, a, y, j, s_min, two, NULL);
    return true;
}

void make_prime(mpz_t p, uint64_t bits, uint64_t iters) {
    // I think we generate a random number of bits length?
    // then if is_prime of number, items gives true, assign p this value and break out of loop

    //run forever (until exit condition within loop is met)
    while (true) {

        // random number p of <bits> bits
        mpz_urandomb(p, state, bits);

        mpz_setbit(p, bits);

        //if p is prime, end loop
        if (is_prime(p, iters)) {
            break;
        }
    }
}
