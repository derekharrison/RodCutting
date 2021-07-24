/*
 * main.cpp
 *
 *  Created on: Jun 6, 2021
 *      Author: d-w-h
 */

#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <time.h>

typedef struct optimum_cut_info {
    int curr_cut_index;
    int max_rev;
} opt_cut_info;

typedef struct table_element {
    bool is_set = false;
    opt_cut_info optimum_cut_info;
} t_elem;

int num_calls = 0;

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

opt_cut_info cut_rod(int n, int A[], t_elem **f_table) {

    num_calls++;

    int curr_cut_index = n;
    int val = A[n-1];
    opt_cut_info optimim_cut_info = {curr_cut_index, val};

    if(n == 0) {
        f_table[n]->is_set = true;
        f_table[n]->optimum_cut_info = {0, 0};

        return {0, 0};
    }

    if(n == 1) {
        f_table[n]->is_set = true;
        f_table[n]->optimum_cut_info = {1, A[0]};

        return {1, A[0]};
    }

    for(int i = 1; i <= n - 1; ++i) {
        if(i <= (n - i)) {
            opt_cut_info dum1, dum2;
            if(f_table[i]->is_set) { dum1 = f_table[i]->optimum_cut_info; }
            else { dum1 = cut_rod(i, A, f_table); }
            if(f_table[n-i]->is_set) { dum2 = f_table[n-i]->optimum_cut_info; }
            else { dum2 = cut_rod(n-i, A, f_table); }

            int dum_int1 = dum1.max_rev;
            int dum_int2 = dum2.max_rev;
            int dum = max(dum_int1 + dum_int2, A[n-1]);

            if(val < dum) {
                val = dum;
                curr_cut_index = i;
            }
        }
    }

    f_table[n]->is_set = true;
    f_table[n]->optimum_cut_info = {curr_cut_index, val};

    optimim_cut_info = {curr_cut_index, val};

    return optimim_cut_info;
}

void delete_memo_table(t_elem **f_table, int n) {

    for(int i = 0; i < n + 1; ++i) {
        delete f_table[i];
    }

    delete [] f_table;
}

int get_optimum_solution(int n, int A[], bool *r, int *cut_counter) {

    //Initialize rod cutting tracker and memo table
    t_elem **f_table = new t_elem*[n+1];

    for(int i = 0; i < n + 1; ++i) {
        r[i] = false;
        f_table[i] = new t_elem;
        f_table[i]->is_set = false;
        cut_counter[i] = 0;
    }

    int NN = n;

    //Cut rod
    while(NN > 0) {
        opt_cut_info cut_info = cut_rod(NN, A, f_table);
        int curr_cut_index = cut_info.curr_cut_index;
        r[curr_cut_index] = true;
        cut_counter[curr_cut_index]++;
        NN = NN - curr_cut_index;
    }

    //Check to see if cut is optimal
    for(int i = 0; i < n + 1; ++i) {
        if(r[i] == true) {
            opt_cut_info cut_info = cut_rod(i, A, f_table);
            int max_rev_loc = cut_info.max_rev;
            if(max_rev_loc != A[i-1]) {
                printf("error, cut not optimal\n");
            }
        }
    }

    //Get max revenue from memo table
    int max_rev = f_table[n]->optimum_cut_info.max_rev;

    delete_memo_table(f_table, n);

    return max_rev;
}

int main(int argc, char* argv[]) {

    int N = 100; //Total length of the rod
    int *A = new int[N]; //Cost array for the rod

    //Arrays to store solution
    bool *rod_cut_at = new bool[N+1];
    int *cut_counter = new int[N+1];

    //Initialize cost array with random numbers
    srand(time(NULL));
    for(int i = 0; i < N; ++i) {
        A[i] = rand() % 4 + (5*i)/2;
    }

    //Get optimum solution
    int max_rev = get_optimum_solution(N, A, rod_cut_at, cut_counter);

    //Print results
    printf("total length of rod n: %d\n", N);
    printf("total revenue: %d\n", max_rev);
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
