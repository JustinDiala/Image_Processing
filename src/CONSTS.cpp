#include "CONSTS.h"

#define MAXN 350
#define MAXD 1100
#define INF 1<<30
int c[MAXN][MAXN];					    // cost matrix
int u[MAXN], v[MAXN];					// potentials
int path_r[MAXN], mate_l[MAXN], mate_r[MAXN];		// previous node in path && matched pairs
bool seen_l[MAXN], seen_r[MAXN];
int n, m, matings;                      // n is # of left nodes; m is # of right nodes

