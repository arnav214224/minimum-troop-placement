#include <bits/stdc++.h>
using namespace std;

const long long INF = LLONG_MAX / 4;

vector<vector<int>> input(int n)
{
    vector<vector<int>> adj(n + 1);

    for(int i = 0; i < n - 1; i++)
    {
        int u, v;
        cout << "Enter edge " << i + 1 << ": ";
        cin >> u >> v;

        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    return adj;
}

void dfs(vector<vector<int>>& adj,
         vector<int>& parent,
         vector<int>& depth,
         vector<int>& order)
{
    stack<int> st;
    st.push(1);

    parent[1] = 0;
    depth[1] = 0;

    while(!st.empty())
    {
        int u = st.top();
        st.pop();

        order.push_back(u);

        for(int v : adj[u])
        {
            if(v == parent[u])
                continue;

            parent[v] = u;
            depth[v] = depth[u] + 1;
            st.push(v);
        }
    }
}

void dynamicProgramming(
    vector<vector<int>>& adj,
    vector<int>& parent,
    vector<int>& depth,
    vector<int>& order,
    vector<vector<array<long long, 3>>>& dp)
{
    reverse(order.begin(), order.end());

    for(int u : order)
    {
        // p = 0, 1, 2: parent's troop count
        // p = 3: u is the root
        for(int p = 0; p <= 3; p++)
        {
            // x = troop count at u
            for(int x = 0; x <= 2; x++)
            {
                long long best = INF;

                if(x == 0)
                {
                    // u has zero troops.
                    // Exactly one neighbor must have two troops.

                    if(p == 2)
                    {
                        // Parent already has two troops.
                        // No child can have two troops.

                        best = 0;

                        for(int v : adj[u])
                        {
                            if(v == parent[u])
                                continue;

                            best += min(dp[v][0][0],
                                        dp[v][0][1]);
                        }
                    }
                    else
                    {
                        // Parent does not have two troops.
                        // Exactly one child must have two troops.

                        for(int chosen : adj[u])
                        {
                            if(chosen == parent[u])
                                continue;

                            // Choose this child to have two troops.
                            long long cost = dp[chosen][0][2];

                            // Every other child must have 0 or 1 troops.
                            for(int v : adj[u])
                            {
                                if(v == parent[u] || v == chosen)
                                    continue;

                                cost += min(dp[v][0][0],
                                            dp[v][0][1]);
                            }

                            best = min(best, cost);
                        }
                    }
                }
                else
                {
                    // u has one or two troops.
                    // Its own condition is automatically satisfied.
                    // Each child can have 0, 1, or 2 troops.

                    best = 0;

                    for(int v : adj[u])
                    {
                        if(v == parent[u])
                            continue;

                        best += min({
                            dp[v][x][0],
                            dp[v][x][1],
                            dp[v][x][2]
                        });
                    }
                }

                // Add the troops stationed at u.
                if(best < INF)
                    dp[u][p][x] = x + best;
                else
                    dp[u][p][x] = INF;
            }
        }
    }
}

// Reconstruct an optimal troop allocation using the DP table.
void reconstruct(
    int u,
    int p,
    int x,
    vector<vector<int>>& adj,
    vector<int>& parent,
    vector<vector<array<long long, 3>>>& dp,
    vector<int>& troops)
{
    // Store the number of troops assigned to u.
    troops[u] = x;

    // Case 1: u has one or two troops.
    if(x > 0)
    {
        for(int v : adj[u])
        {
            if(v == parent[u])
                continue;

            // Choose the optimal troop count for child v.
            int chosenX = 0;

            for(int y = 1; y <= 2; y++)
            {
                if(dp[v][x][y] < dp[v][x][chosenX])
                    chosenX = y;
            }

            reconstruct(v, x, chosenX,
                        adj, parent, dp, troops);
        }
    }

    // Case 2: u has zero troops and its parent has two troops.
    else if(p == 2)
    {
        for(int v : adj[u])
        {
            if(v == parent[u])
                continue;

            // Children can have only zero or one troop.
            int chosenX = 0;

            if(dp[v][0][1] < dp[v][0][0])
                chosenX = 1;

            reconstruct(v, 0, chosenX,
                        adj, parent, dp, troops);
        }
    }

    // Case 3: u has zero troops and its parent does not
    // have two troops. Exactly one child must have two troops.
    else
    {
        int chosenChild = -1;
        long long bestCost = INF;

        // Find the child that should receive two troops.
        for(int chosen : adj[u])
        {
            if(chosen == parent[u])
                continue;

            long long cost = dp[chosen][0][2];

            for(int v : adj[u])
            {
                if(v == parent[u] || v == chosen)
                    continue;

                cost += min(dp[v][0][0],
                            dp[v][0][1]);
            }

            if(cost < bestCost)
            {
                bestCost = cost;
                chosenChild = chosen;
            }
        }

        // Reconstruct the allocation for each child.
        for(int v : adj[u])
        {
            if(v == parent[u])
                continue;

            int chosenX;

            if(v == chosenChild)
            {
                chosenX = 2;
            }
            else
            {
                chosenX = 0;

                if(dp[v][0][1] < dp[v][0][0])
                    chosenX = 1;
            }

            reconstruct(v, 0, chosenX,
                        adj, parent, dp, troops);
        }
    }
}

int main()
{
    cout << "Enter the number of vertices in your tree: ";

    int n;
    cin >> n;

    if(n <= 0)
    {
        cout << "The number of vertices must be positive.\n";
        return 0;
    }

    cout << "Enter the " << n - 1 << " edges (u v):\n";

    vector<vector<int>> adj = input(n);

    vector<int> parent(n + 1, 0);
    vector<int> depth(n + 1, 0);
    vector<int> order;

    dfs(adj, parent, depth, order);

    vector<vector<array<long long, 3>>> dp(
        n + 1, vector<array<long long, 3>>(4)
    );

    for(int u = 0; u <= n; u++)
    {
        for(int p = 0; p <= 3; p++)
            dp[u][p].fill(INF);
    }

    dynamicProgramming(adj, parent, depth, order, dp);

    // Root 1 has no parent, represented by p = 3.
    long long ans = min({
        dp[1][3][0],
        dp[1][3][1],
        dp[1][3][2]
    });

    cout << "\nMinimum number of troops: ";

    if(ans >= INF)
    {
        cout << "No valid assignment exists\n";
        return 0;
    }

    cout << ans << '\n';

    // Reconstruct an optimal allocation.
    int rootX = 0;

    for(int x = 1; x <= 2; x++)
    {
        if(dp[1][3][x] < dp[1][3][rootX])
            rootX = x;
    }

    vector<int> troops(n + 1, 0);

    reconstruct(1, 3, rootX,
                adj, parent, dp, troops);

    cout << "\nTroop allocation:\n";

    for(int u = 1; u <= n; u++)
    {
        cout << "Vertex " << u << ": "
             << troops[u]
             << (troops[u] == 1 ? " troop" : " troops")
             << '\n';
    }

    return 0;
}