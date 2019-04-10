#ifndef _CONSTS_
/*

    Description: This header file contains global constants that are accessed through out each file

*/
#define _CONSTS_
#define MAXN 350
#define MAXD 1100
#define INF 1<<30

extern int c[MAXN][MAXN];					            // cost matrix
extern int u[MAXN], v[MAXN];					            // potential matrix
extern int path_r[MAXN], mate_l[MAXN], mate_r[MAXN];		// prev node in path && matched pairs
extern bool seen_l[MAXN], seen_r[MAXN];                     // a matrix that keeps track of seen pairs
extern int n, m, matings;
extern constexpr int sqrsum(int x, int y, int z) { return x*x + y*y + z*z; }    // a function that calculates the magnitude of a given vector

#endif
