#include <stdio.h>
#include <limits.h>

// Structure to store one flight connection
// src = source city, dest = destination city, weight = flight cost
struct Edge {
    int src, dest, weight;
};

// Function to find the cheapest flight cost using Bellman-Ford Algorithm
void BellmanFord(struct Edge edges[], int V, int E, int src) {
    int dist[V];  // dist[] array stores the minimum cost to reach each city
    
    // Step 1: Initialize distances
    // Initially, set all city distances to infinity (INT_MAX)
    // The source city distance is 0 (distance from itself is zero)
    for (int i = 0; i < V; i++)
        dist[i] = INT_MAX;
    dist[src] = 0;

    // Step 2: Relax all edges |V| - 1 times
    // Relaxing means updating the shortest known distance if a cheaper route is found
    // Repeat (V-1) times because the shortest path can have at most (V-1) edges
    for (int i = 1; i <= V - 1; i++) {
        for (int j = 0; j < E; j++) {
            int u = edges[j].src;
            int v = edges[j].dest;
            int weight = edges[j].weight;

            // If there is a shorter path to v through u, update it
            if (dist[u] != INT_MAX && dist[u] + weight < dist[v])
                dist[v] = dist[u] + weight;
        }
    }

    // Step 3: Check for negative weight cycle
    // If we can still relax an edge, that means a negative cycle exists
    for (int j = 0; j < E; j++) {
        int u = edges[j].src;
        int v = edges[j].dest;
        int weight = edges[j].weight;
        if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
            printf("\n⚠ Graph contains a negative weight cycle!\n");
            return;
        }
    }

    // Step 4: Print the result table
    printf("\n📍 Cheapest Flight Costs from Source City %d:\n", src);
    printf("----------------------------------------\n");

    int cheapestCity = -1;   // Variable to store city with the cheapest flight
    int cheapestCost = INT_MAX;

    // Print cost from source to every other city
    for (int i = 0; i < V; i++) {
        if (dist[i] == INT_MAX)
            printf("City %d -> City %d : No Connection\n", src, i);
        else {
            printf("City %d -> City %d : %d\n", src, i, dist[i]);

            // Update cheapest city if a lower cost is found
            if (i != src && dist[i] < cheapestCost) {
                cheapestCost = dist[i];
                cheapestCity = i;
            }
        }
    }

    // Step 5: Display the cheapest available flight
    if (cheapestCity != -1)
        printf("\n✅ Cheapest flight is from City %d to City %d with cost %d.\n",
               src, cheapestCity, cheapestCost);
    else
        printf("\nNo reachable city found from source.\n");
}

int main() {
    int V, E, src;

    // Step 1: Take input for number of cities and flight connections
    printf("Enter number of cities: ");
    scanf("%d", &V);

    printf("Enter number of flight connections: ");
    scanf("%d", &E);

    struct Edge edges[E]; // Array to store all flight connections

    // Step 2: Input each flight connection data (Source, Destination, Cost)
    printf("\nEnter each flight connection (Source Destination Cost):\n");
    for (int i = 0; i < E; i++) {
        printf("Connection %d: ", i + 1);
        scanf("%d %d %d", &edges[i].src, &edges[i].dest, &edges[i].weight);
    }

    // Step 3: Input the source city (starting point)
    printf("\nEnter source city: ");
    scanf("%d", &src);

    // Step 4: Call the Bellman-Ford function to calculate the cheapest flight costs
    BellmanFord(edges, V, E, src);

    return 0;
}