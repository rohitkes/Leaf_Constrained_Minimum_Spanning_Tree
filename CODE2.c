/**********************************************************************************************************
Problem Description:
Let L be an integer, 2 <= L < no. of nodes-1.
A leaf constrained spanning tree (LCST) is a spanning tree on G
that has at least L leaves, and a leaf-constrained minimum
spanning tree (LCMST) is an LCST of minimum total
Nature Inspired Algorithm used in the solution is : Artificial Bee Colony Optimization
***********************************************************************************************************/
#include<stdio.h>
#include<stdlib.h> 
#include<math.h> 
#include<string.h>
    struct Bee_Employee {
        int Bee_source;
        int visit;
        int key;
        int Bee_distination;
    }
bee[10];
int L[10];
int best(struct Bee_Employee  bee[], int n) {
    int min, i, index = 0;
    min = 99999;
    for (i = 0; i < n; i++) {
        if (!bee[i].visit && min > bee[i].key) {
            min = bee[i].key;
            index = i;
        }
    }
    return index;
}
int * * neighbour_source(int employee_food_source[][10], int n, int source) {
    int i, f, r, cycle, u, j;
    /*** beeueue Initiliazation ***/
    for (i = 0; i < n; i++) {
        bee[i].Bee_source = i;
        bee[i].visit = 0;
        bee[i].Bee_distination = -1;
        bee[i].key = 99999;
    }
    /** Initiliazation of the source *******/
    bee[source].Bee_source = source;
    bee[source].key = 0;
    bee[source].Bee_distination = -1;
    f = 0;
    r = n - 1;
    cycle = 1;
    while (cycle < n) {
        u = best(bee, n);
        bee[u].visit = 1;
        for (i = 0; i < n; i++) {
            if (employee_food_source[u][i]) {
                if (!bee[i].visit && bee[i].key > employee_food_source[u][i]) {
                    bee[i].key = employee_food_source[u][i];
                    bee[i].Bee_distination = u;
                }
            }
        }
        cycle++;
    }
    int * * s;
    s = (int * * ) malloc(n * sizeof(int * ));
    for (i = 0; i < n; ++i) { * (s + i) = (int * ) malloc(n * sizeof(int));
    }
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++)
            s[i][j] = 0;
    }
    j = 0;
    for (i = 0; i < n; i++) {
        if (bee[i].Bee_distination != -1) {
            s[bee[i].Bee_distination][bee[i].Bee_source] = bee[i].key;
            s[bee[i].Bee_source][bee[i].Bee_distination] = bee[i].key;
        }
    }
    return s;
}
int Evaluate_fitness(int * * T, int n) {
    int i, j, w = 0;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++)
            w += T[i][j];
    }
    return w / 2;
}
void leaves(int * * a, int n) {
    int i, j, count = 0;
    for (i = 0; i < n; i++)
        L[i] = 0;
    for (i = 0; i < n; i++) {
        count = 0;
        for (j = 0; j < n; j++) {
            if (a[i][j] == 0)
                count++;
        }
        if (count == n - 1)
            L[i] = 1;
    }
}
int One_food_source_Bee(int L[10], int n) {
    int i, count = 0;
    for (i = 0; i < n; i++) {
        if (L[i] == 1)
            count++;
    }
    return count;
}
void LCMST(int employee_food_source[][10], int l, int n) {
    int * * T, One_food_source_Bee_Number, k, i, j, len, s, c, d, u, t, min;
    /*Produce new solutions Tij for the employed bees */
    T = neighbour_source(employee_food_source, n, 0);
    leaves(T, n);
    One_food_source_Bee_Number = One_food_source_Bee(L, n);
    do {
        if (One_food_source_Bee_Number >= l)
            break;
        len = 99999;
        for (k = 0; k < n; k++) {
            if (!L[k]) {
                int L1[10];
                for (i = 0; i < n; i++)
                    L1[i] = L[i];
                L1[k] = 1;
                int * * T1;
                int x[10][10];
                T1 = (int * * ) malloc(n * sizeof(int * ));
                for (i = 0; i < n; ++i) { * (T1 + i) = (int * ) malloc(n * sizeof(int));
                }
                for (i = 0; i < n; i++) {
                    if (L1[i]) {
                        for (j = 0; j < n; j++) {
                            x[i][j] = 0;
                        }
                    } else {
                        for (j = 0; j < n; j++) {
                            x[i][j] = employee_food_source[i][j];
                            if (L1[j]) {
                                x[i][j] = 0;
                            }
                        }
                        s = i;
                    }
                }
                T1 = neighbour_source(x, n, s);
                for (i = 0; i < n; i++) {
                    if (L1[i]) {
                        min = 99999;
                        for (t = 0; t < n; t++) {
                            if (employee_food_source[i][t] && !L1[t]) {
                                if (min > employee_food_source[i][t]) {
                                    min = employee_food_source[i][t];
                                    u = t;
                                }
                            }
                        }
                        T1[u][i] = min;
                        T1[i][u] = min;
                    }
                }
                int Evaluate_fitness_ = Evaluate_fitness(T1, n);
                /*Determine the abandoned feasible solution , if exists, and replace it with a new solution */
                if (Evaluate_fitness_ < len) {
                    len = Evaluate_fitness_;
                    T = T1;
                }
            }
        }
        leaves(T, n);
    } while (One_food_source_Bee(L, n) < l);
    /***********Evaluat the Evaluate_fitness  of the constrained minimum spanning tree ***********/
    printf("\n\tWeight of the constrained minimum spanning tree is : ");
    printf("%d\n", Evaluate_fitness(T, n));
    printf("\n\tAdjacency matrix of the Leaf constrained minimum spanning tree :\n\t\t");
    /******** If there is an edge in i to j then T[i][j]=Evaluate_fitness_ of the edge otherwise it is 0 */
    for (i = 0; i < n; i++) { 
        for (j = 0; j < n; j++)
            printf("%d\t", T[i][j]);
        printf("\n\t\t");
    }
}
int main() {
    int l, employee_bee_number, adj[10][10], i, j;
    printf("\n\n\t\t\t PROJECT Leaf Constrained Minimum Spanning Tree\n\t\t\t Uses Artificial Bee Colony as the Nature Inspired Algorithm\n\n\t\t\t Under the guidance of Dr.Manish Kumar Bajpai\n\n\t\t\t\t\t Submitted by- \n\t\t\t\t\t Rohit Kesarvani 2014142\n\t\t\t\t\t Praneet Bhatnagar 2014125\n\t\t\t\t\t Abhishek Kumar 2014007");
    printf("\n\n\t\tPlease input the number of vertices in Graph  : ");
    scanf("%d", & employee_bee_number);
    printf("\n\t\tPlease input the Adjacency matrix of the employee_food_source :\n\n");
    for (i = 0; i < employee_bee_number; i++) {
        printf("\t\t");
        for (j = 0; j < employee_bee_number; j++) {
            scanf("%d", & adj[i][j]);
        }
    }
    printf("\n\t\tPlease enter the constraint (The least number of leaves you want in the MST)\n\t\tNOTE: Constraint < Number of Vertices-1 : ");
    scanf("%d", & l);
    LCMST(adj, l, employee_bee_number);
}

