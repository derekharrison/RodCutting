/*
 * main.cpp
 *
 *  Created on: Jun 6, 2021
 *      Author: d-w-h
 */

#include <stdio.h>
#include <vector>
#include <stdlib.h>

typedef struct table_element {
    bool is_set = false;
    int val;
} t_elem;

t_elem **f_table;
int num_calls = 0;
int curr_cut_index;

int max(int a, int b) {
    int dum;
    if(a < b) {
        dum = b;
    }
    else {
        dum = a;
    }

    return dum;
}

int func(int n, int A[], bool *r) {

    num_calls++;

    if(n == 0) {
        return 0;
    }
    if(n == 1) {
        return A[0];
    }

    int val = A[n-1];
    curr_cut_index = n;

    if(n > 1) {
        for(int i = 1; i <= n - 1; ++i) {
            int dum1, dum2;
            if(f_table[i]->is_set == true) {
                dum1 = f_table[i]->val;
            }
            else {
                dum1 = func(i, A, r);
            }
            if(f_table[n-i]->is_set == true) {
                dum2 = f_table[n-i]->val;
            }
            else {
                dum2 = func(n-i, A, r);
            }

            int dum = max(dum1 + dum2, A[n-1]);

            if(val < dum) {
                val = dum;
                curr_cut_index = i;
            }
        }
    }

    f_table[n]->is_set = true;
    f_table[n]->val = val;

    return val;
}

void extract_optimum_cut(int n, int A[], bool *r, int *cut_counter) {

    int NN = n;

    while(NN > 0) {
        (void) func(NN, A, r);
        r[curr_cut_index] = true;
        cut_counter[curr_cut_index]++;
        NN = NN - curr_cut_index;
    }

    //Check to see if optimal cut can be cut up further
    for(int i = 0; i < n + 1; ++i) {
        if(r[i] == true) {
            int max_rev_loc = func(i, A, r);
            if(max_rev_loc != A[i-1]) {
                printf("error, cut not optimal\n");
            }
        }
    }
}

int main(int argc, char* argv[]) {

    int N = 25; //Total length of the rod
    int *A = new int[N]; //Cost array for the rod

    int *cut_counter = new int[N+1];
    curr_cut_index = N;

    //Initialize cost array with random numbers
    for(int i = 0; i < N; ++i) {
        A[i] = rand() % 20;
    }

    //Initialize rod cutting tracker and memo table
    bool *rod_cut_at = new bool[N+1];
    f_table = new t_elem*[N+1];

    for(int i = 0; i < N + 1; ++i) {
        rod_cut_at[i] = false;
        f_table[i] = new t_elem;
        f_table[i]->val = 0;
        f_table[i]->is_set = false;
        cut_counter[i] = 0;
    }

    //Compute omptimum revenue
    int rev = func(N, A, rod_cut_at);

    //Get optimum cut
    extract_optimum_cut(N, A, rod_cut_at, cut_counter);


    //Print results
    printf("total length of rod n: %d\n", N);
    printf("total revenue: %d\n", rev);
    for(int i = 0; i < N + 1; ++i) {
        if(rod_cut_at[i] == true) {
            printf("rod cut at %i a total of %i times\n", i, cut_counter[i]);
        }
    }

    //Verify computation
    int total_rev = 0;
    for(int i = 1; i < N + 1; ++i) {
        if(rod_cut_at[i] == true) {
            total_rev = total_rev + cut_counter[i]*A[i-1];
        }
    }

    printf("total revenue verification: %i\n", total_rev);
    printf("total number of calls: %i\n", num_calls);
    printf("done\n");

    return 0;
}
