#include <cstring>		// memset
#include <queue>
#include <algorithm>	// min, max
#include <cmath>		// abs

#include "CONSTS.h"

using namespace std;

/*
    An O(v^3) weighted bipartite matching algorithm, where v is the number of nodes
    input:
        - a complete bipartite graph given as a cost matrix c[n][m]
    output:
        - a matching that maximizes the overall cost: mate_l (mate_r) will contain
          the matching right (left) node of all the left (right) nodes
*/
void hungarian() {
	fill_n(u, MAXN, INF);
	memset(v, 0, sizeof v);

	// INVARIANT: u[i] + v[j] >= c[i][j]

	// find feasible potentials for left
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < m; ++j)
			u[i] = max(u[i], c[i][j]);

	memset(mate_r, -1, sizeof mate_r);
	memset(mate_l, -1, sizeof mate_l);

	matings = 0;

	if (matings == n) return;

	int s, t;
	while (1) {
		// find exposed left node
		for (s = 0; s < n; ++s)
			if (mate_l[s] == -1) break;

		queue<int> q; q.push(s);
		memset(path_r, -1, sizeof path_r);
		memset(seen_r, 0, sizeof seen_r);
		memset(seen_l, 0, sizeof seen_l);
		seen_l[s] = true;

		bool ok = false;
		while (!q.empty()) {					// build a maximal M-alternating tree using tight edges
			s = q.front(); q.pop();				// s is in the left

			for (t = 0; t < m; ++t) if (!seen_r[t]) {				// t is in the right
				if (u[s] + v[t] != c[s][t]) continue;				// not tight
				if (mate_l[s] == t) continue;						// choose unmatched edge

				path_r[t] = s;
				seen_r[t] = true;
				if (mate_r[t] == -1) {				// t is an exposed right node!
					ok = true; break; }

				if (!seen_l[mate_r[t]]) {			// mate_r[t] is in left
					seen_l[mate_r[t]] = true;
					q.push(mate_r[t]);
				}
			}
			if (ok) break;
		}

		if (ok) {							// found augmenting path
			while (t != -1) {				// augment along path
				mate_r[t] = path_r[t];
				swap(mate_l[path_r[t]], t);
			}
			matings++;
			if (matings == n) break;	// perfect matching!

		}
		else {		// update potentials
			int delta = INF;
			for (int i = 0; i < n; ++i)
				for (int j = 0; j < m; ++j) {
					if (!seen_l[i] or seen_r[j]) continue;
        			delta = min(delta, u[i] + v[j] - c[i][j]);
        		}

			for (int i = 0; i < n; ++i)
				if (seen_l[i]) u[i] -= delta;
			for (int j = 0; j < m; ++j)
				if (seen_r[j]) v[j] += delta;
		}
	}
}