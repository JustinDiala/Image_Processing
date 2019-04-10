#include <iostream>
#include <iomanip>
#include <cstring>		// memset
#include <queue>
#include <algorithm>	// min, max
#include <cmath>		// abs
using namespace std;

#define MAXN 22
#define INF 1<<30
const double eps = 1e-5;
double c[MAXN][MAXN];					// cost matrix
double u[MAXN], v[MAXN];				// potentials
double slack[MAXN]; int slack_r[MAXN];  // slack[j] == min(slack[j], u[i] + v[i] - c[i][j]) for all 0 <= i < n
int path_r[MAXN], mate_l[MAXN], mate_r[MAXN];		// previous node in path && matched pairs
bool seen_l[MAXN], seen_r[MAXN];
int n, m, matings;                      // n is # of left nodes; m is # of right nodes

// input: the left-side node, i, that has an updated potential
// output: updated slacks for all adjacent nodes in the right side.
void update_slack(int i) {
    for (int j = 0; j < m; ++j) {
        if (u[i] + v[j] - c[i][j] > slack[j]) continue;
        slack[j] = u[i] + v[j] - c[i][j];
        slack_r[j] = i;
    }
}

/*
    An O(v^3) weighted bipartite matching algorithm, where v is the number of nodes
    input:
        - a complete bipartite graph given as a cost matrix c[n][m]
    output:
        - a matching that maximizes the overall cost: mate_l (mate_r) will contain
          the matching right (left) node of all the left (right) nodes
*/
void hungarian() {
	memset(u, 215, sizeof u);	// u[i] > INF for all 0 <= i < n
	memset(v, 0, sizeof v);

	// INVARIANT: u[i] + v[j] >= c[i][j]

	// find feasible potentials for left
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < m; ++j)
			u[i] = max(u[i], c[i][j]);

	memset(mate_r, -1, sizeof mate_r);
	memset(mate_l, -1, sizeof mate_l);

	matings = 0;

	if (n == 0 or m == 0) return;

	int s, t;
	while (1) {                          // increase the number of matchings, one by one
		// find exposed left node
		for (s = 0; s < n; ++s)
			if (mate_l[s] == -1) break;

		queue<int> q; q.push(s);
		memset(path_r, -1, sizeof path_r);
		memset(seen_r, 0, sizeof seen_r);
		memset(seen_l, 0, sizeof seen_l);
		seen_l[s] = true;

        for (int j = 0; j < m; ++j) {   // set the slacks of the right nodes
            slack[j] = u[s] + v[j] - c[s][j];
            slack_r[j] = s;
        }

		bool ok = false;
        for (;;) {                       // repeatedly relax potentials until an augmenting path is found
            while (!q.empty()) {
    			s = q.front(); q.pop();				                    // s is in the left

    			for (t = 0; t < m; ++t) if (!seen_r[t]) {				// t is in the right
    				if (abs(u[s] + v[t] - c[s][t]) > eps) continue;				// not tight
    				if (mate_l[s] == t) continue;						// choose unmatched edge

    				path_r[t] = s;
    				seen_r[t] = true;

    				if (mate_r[t] == -1) {				                // t is an exposed right node!
    					ok = true; break; }

                    // s -> t -> mate_r[t]
    				if (!seen_l[mate_r[t]]) {			                // mate_r[t] is in left
    					seen_l[mate_r[t]] = true;
                        update_slack(mate_r[t]);
    					q.push(mate_r[t]);
    				}
    			}
    			if (ok) break;
    		}

            if (ok) break;
            // update potentials and slacks
            double delta = INF;
            for (int j = 0; j < m; ++j)
                if (!seen_r[j]) delta = min(delta, slack[j]);   // find delta
            for (int j = 0; j < m; ++j)                         // update slack
                if (!seen_r[j]) slack[j] -= delta;
            for (int i = 0; i < n; ++i)
                if (seen_l[i]) u[i] -= delta;
            for (int j = 0; j < m; ++j)
                if (seen_r[j]) v[j] += delta;

            // add newly tight edges to bfs tree
            for (int j = 0; j < m; ++j) {
                if (seen_r[j] or slack[j] != 0) continue;
                if (mate_r[j] == -1) {                      // j is an exposed right node!
                    path_r[j] = slack_r[j];
                    ok = true; t = j; break;
                }

                seen_r[j] = true;
                path_r[j] = slack_r[j];                           // add j, a right node, to tree
                if (seen_l[mate_r[j]]) continue;
                q.push(mate_r[j]);                      // add the mate of j, a left node, to the queue

                // slack_r[j] -> j -> mate_r[j]
                seen_l[mate_r[j]] = true;
                update_slack(mate_r[j]);
            }
        }

		// found augmenting path
		while (t != -1) {				// augment along path
			mate_r[t] = path_r[t];
			swap(mate_l[path_r[t]], t);
		}
		matings++;
		if (matings == n) break;	    // perfect matching!
	}
}

int main () {
	while (cin >> n >> m) {
		if (n == 0 && m == 0) break;

		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < m; ++j) {
				cin >> c[i][j];
				c[i][j] *= -1;
			}
		}

		hungarian();

		double ans = 0;
		for (int i = 0; i < n; ++i)
			if (mate_l[i] >= 0) ans += -1.0 * c[i][mate_l[i]];
		cout << fixed << setprecision(2) << round(n > 0 ? 100 * ans / n : 0.0)/100 << endl;
	}
	return 0;
}
