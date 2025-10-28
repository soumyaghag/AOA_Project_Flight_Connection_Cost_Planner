#include <stdio.h>
#include <limits.h>

#define MAXV 100    // max number of vertices (cities)
#define MAXE 1000   // max number of directed edges (flight connections)

// Forward-star adjacency list storage
int head[MAXV];
int to[MAXE];
int wght[MAXE];
int nxt[MAXE];
int edgeIdx = 0;

int V, E;

// DFS state
int visited[MAXV];
int path[MAXV];
int pathCount;
int bestPath[MAXV];
int bestPathCount;
int bestCost;

// Add directed edge u -> v with weight wt
void addEdge(int u, int v, int wt) {
    to[edgeIdx] = v;
    wght[edgeIdx] = wt;
    nxt[edgeIdx] = head[u];
    head[u] = edgeIdx;
    edgeIdx++;
}

// Print one route stored in path[]
void printRoute(int length, int cost) {
    for (int i = 0; i < length; i++) {
        if (i) printf(" ");
        printf("%d", path[i]);
    }
    printf(" (Cost = %d)\n", cost);
}

// DFS exploring all paths from current to destination
void dfs(int current, int destination, int currentCost) {
    if (current == destination) {
        // print this complete route
        printRoute(pathCount, currentCost);

        // update best if cheaper
        if (currentCost < bestCost) {
            bestCost = currentCost;
            bestPathCount = pathCount;
            for (int i = 0; i < pathCount; i++)
                bestPath[i] = path[i];
        }
        return;
    }

    visited[current] = 1;

    // iterate over all outgoing edges from 'current'
    for (int ei = head[current]; ei != -1; ei = nxt[ei]) {
        int nextNode = to[ei];
        int edgeWeight = wght[ei];

        if (!visited[nextNode]) {
            // push and explore
            path[pathCount++] = nextNode;
            dfs(nextNode, destination, currentCost + edgeWeight);
            pathCount--;
        }
        // Note: if nextNode is already visited, we skip to avoid cycles.
        // This still allows multiple parallel edges to the same unvisited node to be explored,
        // because after returning from dfs the visited[nextNode] will be cleared.
    }

    visited[current] = 0;
}

int main() {
    printf("Enter number of cities: ");
    if (scanf("%d", &V) != 1) return 0;

    printf("Enter number of flight connections: ");
    if (scanf("%d", &E) != 1) return 0;

    // initialize adjacency list
    for (int i = 0; i < V; i++) head[i] = -1;
    edgeIdx = 0;

    printf("\nEnter each flight connection (Source Destination Cost):\n");
    for (int i = 0; i < E; i++) {
        int s, d, wt;
        printf("Connection %d: ", i + 1);
        if (scanf("%d %d %d", &s, &d, &wt) != 3) return 0;
        if (s < 0 || s >= V || d < 0 || d >= V) {
            printf("Invalid city index. Cities are 0 .. %d\n", V - 1);
            return 0;
        }
        addEdge(s, d, wt);
    }

    int src;
    printf("\nEnter source city: ");
    if (scanf("%d", &src) != 1) return 0;
    if (src < 0 || src >= V) {
        printf("Invalid source city index.\n");
        return 0;
    }

    // For every destination, run DFS from src to dest and list all routes + shortest
    for (int dest = 0; dest < V; dest++) {
        if (dest == src) continue;

        printf("\n====================================================\n");
        printf("All Possible Routes from City %d to City %d:\n\n", src, dest);

        // reset DFS/best state
        bestCost = INT_MAX;
        bestPathCount = 0;
        pathCount = 0;
        for (int i = 0; i < V; i++) visited[i] = 0;

        // start path with source
        path[pathCount++] = src;

        dfs(src, dest, 0);

        if (bestCost == INT_MAX) {
            printf("\nNo route available.\n");
        } else {
            printf("\n✅ Shortest Route: ");
            for (int i = 0; i < bestPathCount; i++) {
                if (i) printf(" ");
                printf("%d", bestPath[i]);
            }
            printf(" (Cost = %d)\n", bestCost);
        }
    }

    return 0;
}
