#include <stdio.h>
#include <limits.h>

#define MAXV 100
#define MAXE 1000

int V, E;

// For Bellman-Ford
struct Edge { int src, dest, weight; };
struct Edge edges[MAXE];

// For storing Bellman-Ford results
int dist[MAXV];
int parent[MAXV];

// For DFS route listing
int adjHead[MAXV], to[MAXE], wght[MAXE], nxt[MAXE], edgeIdx = 0;
int visited[MAXV], path[MAXV], pathCount;

// Add edge to adjacency list (for DFS only)
void addAdjEdge(int u, int v, int wt) {
    to[edgeIdx] = v;
    wght[edgeIdx] = wt;
    nxt[edgeIdx] = adjHead[u];
    adjHead[u] = edgeIdx++;
}

// Print one DFS route
void printRoute(int length, int cost) {
    for (int i = 0; i < length; i++) {
        if (i) printf(" -> ");
        printf("%d", path[i]);
    }
    printf(" (Cost = %d)\n", cost);
}

// DFS to list all possible routes
void dfs(int current, int destination, int cost) {
    if (current == destination) {
        printRoute(pathCount, cost);
        return;
    }

    visited[current] = 1;

    for (int ei = adjHead[current]; ei != -1; ei = nxt[ei]) {
        int nextNode = to[ei];
        if (!visited[nextNode]) {
            path[pathCount++] = nextNode;
            dfs(nextNode, destination, cost + wght[ei]);
            pathCount--;
        }
    }

    visited[current] = 0;
}

// Print shortest path using parent[]
void printShortestPath(int dest) {
    if (parent[dest] == -1) {
        printf("%d", dest);
        return;
    }
    printShortestPath(parent[dest]);
    printf(" -> %d", dest);
}

// Bellman-Ford to compute shortest paths
void bellmanFord(int src) {
    for (int i = 0; i < V; i++) {
        dist[i] = INT_MAX;
        parent[i] = -1;
    }
    dist[src] = 0;

    for (int i = 1; i <= V - 1; i++) {
        for (int j = 0; j < E; j++) {
            int u = edges[j].src;
            int v = edges[j].dest;
            int w = edges[j].weight;
            if (dist[u] != INT_MAX && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                parent[v] = u;
            }
        }
    }
}

int main() {
    printf("Enter number of cities: ");
    scanf("%d", &V);

    printf("Enter number of flight connections: ");
    scanf("%d", &E);

    for (int i = 0; i < V; i++) adjHead[i] = -1;
    edgeIdx = 0;

    printf("\nEnter each flight connection (Source Destination Cost):\n");
    for (int i = 0; i < E; i++) {
        int s, d, w;
        printf("Connection %d: ", i + 1);
        scanf("%d %d %d", &s, &d, &w);
        edges[i].src = s;
        edges[i].dest = d;
        edges[i].weight = w;
        addAdjEdge(s, d, w);
    }

    int src;
    printf("\nEnter source city: ");
    scanf("%d", &src);

    // Compute shortest paths using Bellman-Ford
    bellmanFord(src);

    // For each destination print all routes + shortest
    for (int dest = 0; dest < V; dest++) {
        if (dest == src) continue;

        printf("\n====================================================\n");
        printf("All Possible Routes from City %d to City %d:\n\n", src, dest);

        for (int i = 0; i < V; i++) visited[i] = 0;

        pathCount = 0;
        path[pathCount++] = src;
        dfs(src, dest, 0);

        if (dist[dest] == INT_MAX) {
            printf("\nNo route available.\n");
        } else {
            printf("\n✅ Shortest Route (Bellman-Ford): ");
            printShortestPath(dest);
            printf(" (Cost = %d)\n", dist[dest]);
        }
    }

    return 0;
}
