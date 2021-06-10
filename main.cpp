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

typedef struct table_element {
    bool is_set = false;
    int val;
} t_elem;

typedef struct optimum_cut_info {
    int curr_cut_index;
    int max_rev;
} opt_cut_info;

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

int fill_memo_table(int n, int A[], t_elem **f_table) {

    num_calls++;

    if(n == 0) {
        f_table[n]->is_set = true;
        f_table[n]->val = 0;
        return 0;
    }

    int val = A[n-1];

    if(n == 1) {
        f_table[n]->is_set = true;
        f_table[n]->val = val;
        return A[0];
    }

    if(n > 1) {
        for(int i = 1; i <= n - 1; ++i) {
            if(i <= (n - i)) {
                int dum1, dum2;
                if(f_table[i]->is_set == true) {
                    dum1 = f_table[i]->val;
                }
                else {
                    dum1 = fill_memo_table(i, A, f_table);
                }
                if(f_table[n-i]->is_set == true) {
                    dum2 = f_table[n-i]->val;
                }
                else {
                    dum2 = fill_memo_table(n-i, A, f_table);
                }

                int dum = max(dum1 + dum2, A[n-1]);

                if(val < dum) {
                    val = dum;
                }
            }
        }
    }

    f_table[n]->is_set = true;
    f_table[n]->val = val;

    return val;
}

opt_cut_info get_optimum_cut(int n, int A[], t_elem **f_table) {

    int curr_cut_index = n;

    if(n == 0) {
        return {0, 0};
    }

    int val = A[n-1];

    if(n == 1) {
        return {curr_cut_index, val};
    }

    if(n > 1) {
        for(int i = 1; i <= n - 1; ++i) {
            if(i <= (n - i)) {
                int dum1, dum2;
                if(f_table[i]->is_set != true) {
                    printf("something went wrong, table element not set\n");
                }

                dum1 = f_table[i]->val;
                dum2 = f_table[n-i]->val;

                int dum = max(dum1 + dum2, A[n-1]);

                if(val < dum) {
                    val = dum;
                    curr_cut_index = i;
                }
            }
        }
    }

    opt_cut_info cut_info = {curr_cut_index, val};

    return cut_info;
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
        f_table[i]->val = 0;
        f_table[i]->is_set = false;
        cut_counter[i] = 0;
    }

    (void) fill_memo_table(n, A, f_table);

    int NN = n;

    while(NN > 0) {
        opt_cut_info cut_info = get_optimum_cut(NN, A, f_table);
        int curr_cut_index = cut_info.curr_cut_index;
        r[curr_cut_index] = true;
        cut_counter[curr_cut_index]++;
        NN = NN - curr_cut_index;
    }

    //Check to see if optimal cut can be cut up further
    for(int i = 0; i < n + 1; ++i) {
        if(r[i] == true) {
            opt_cut_info cut_info = get_optimum_cut(i, A, f_table);
            int max_rev_loc = cut_info.max_rev;
            if(max_rev_loc != A[i-1]) {
                printf("error, cut not optimal\n");
            }
        }
    }

    int max_rev = get_optimum_cut(n, A, f_table).max_rev;

    delete_memo_table(f_table, n);

    return max_rev;
}

int main(int argc, char* argv[]) {

    int N = 100; //Total length of the rod
    int *A = new int[N]; //Cost array for the rod

    //Arrays to store solution
    bool *rod_cut_at = new bool[N+1];
    int *cut_counter = new int[N+1];

    srand(time(NULL));

    //Initialize cost array with random numbers
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
