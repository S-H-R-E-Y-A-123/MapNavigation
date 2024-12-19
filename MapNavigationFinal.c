#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_CITIES 5 // Maximum number of cities
#define INF 99999 // Constant for no direct path

// Graph structure with adjacency matrix for distances and array for city names
typedef struct { 
    int adjMatrix[MAX_CITIES][MAX_CITIES];
    char* cityNames[MAX_CITIES];
} Graph;

// Initialize the graph with city names and distances
void initializeGraph(Graph* g) { 
    g->cityNames[0] = "City 0";
    g->cityNames[1] = "City 1";
    g->cityNames[2] = "City 2";
    g->cityNames[3] = "City 3";
    g->cityNames[4] = "City 4";

    // Set initial distances; 0 for same city, INF for others
    for (int i = 0; i < MAX_CITIES; i++) {
        for (int j = 0; j < MAX_CITIES; j++) {
            g->adjMatrix[i][j] = (i == j) ? 0 : INF;
        }
    }

    // Define specific distances between cities
    g->adjMatrix[0][1] = 50; g->adjMatrix[1][0] = 50;
    g->adjMatrix[1][2] = 70; g->adjMatrix[2][1] = 70;
    g->adjMatrix[1][3] = 60; g->adjMatrix[3][1] = 60;
    g->adjMatrix[3][4] = 40; g->adjMatrix[4][3] = 40;
    g->adjMatrix[0][2] = 150; g->adjMatrix[2][0] = 150;
    g->adjMatrix[0][3] = 90; g->adjMatrix[3][0] = 90;
    g->adjMatrix[0][4] = 200; g->adjMatrix[4][0] = 200;
    g->adjMatrix[2][3] = 100; g->adjMatrix[3][2] = 100;
}

// Display a random weather condition
void displayWeather() {
    const char* weatherConditions[] = {"Sunny", "Rainy", "Windy", "Cloudy"};
    srand(time(0));
    int randomCondition = rand() % 4; // Random weather index
    printf("Weather condition is: %s\n", weatherConditions[randomCondition]);
}

// Display random traffic congestion level
void displayTrafficPattern() {
    srand(time(0));
    int trafficCongestion = rand() % 100; // Random congestion level
    printf("Traffic analysis: Current congestion is %d%%.\n", trafficCongestion);
}

// Remove a road between two cities by setting distance to INF
void removeRoad(Graph* g, int city1, int city2) {
    g->adjMatrix[city1][city2] = INF;
    g->adjMatrix[city2][city1] = INF;
    printf("Road between %s and %s has been removed due to road closure.\n", g->cityNames[city1], g->cityNames[city2]);
}

// Dijkstra's algorithm to find the shortest path between two cities
int dijkstra(Graph* g, int src, int dest, int* path) {
    int dist[MAX_CITIES], visited[MAX_CITIES], prev[MAX_CITIES];

    // Initializing distance, visited, and previous arrays
    for (int i = 0; i < MAX_CITIES; i++) {
        dist[i] = INF;
        visited[i] = 0; 
        prev[i] = -1; //no paths visited 
    }
    dist[src] = 0; // Starting city's distance is 0

    // Finding shortest path to each city
    for (int i = 0; i < MAX_CITIES - 1; i++) { //iterated over each city in the graph 
        int min = INF, u = -1;   //u:unvisited city; min=INF to ensure that any actual distance will be less than INF
        for (int j = 0; j < MAX_CITIES; j++) {
            if (!visited[j] && dist[j] <= min) {  //checking if city j hasnt been visited and dist less than min=INF
                min = dist[j]; //when unvisited j is found with smaller dist than INF, min is updated to new small dist
                u = j; 
            }
        }
        visited[u] = 1; 

        for (int v = 0; v < MAX_CITIES; v++) { // Updating distances for neighbouring cities(v)
            if (!visited[v] && g->adjMatrix[u][v] != INF && dist[u] + g->adjMatrix[u][v] < dist[v]) {
                dist[v] = dist[u] + g->adjMatrix[u][v];
                prev[v] = u; // Track path
            }
        }
    }

    int pathLength = 0; //Reconstructing path from destination to source
    for (int at = dest; at != -1; at = prev[at]) {
        path[pathLength++] = at;
    }

    for (int i = 0; i < pathLength / 2; i++) {  //Reversing path to display correctly
        int temp = path[i];
        path[i] = path[pathLength - i - 1];
        path[pathLength - i - 1] = temp;
    }

    return dist[dest];
}


// Displaying journey details (Dijkstra's algorithm)
void displayJourney(Graph* g, int city1, int city2) {
    printf("Journey from %s to %s:\n", g->cityNames[city1], g->cityNames[city2]);
    printf("------------------------------------------------\n");

    int path[MAX_CITIES];
    int distance = dijkstra(g, city1, city2, path); 

    // Displaying path if reachable
    if (distance == INF) {
        printf("No direct or indirect route available between %s and %s.\n", g->cityNames[city1], g->cityNames[city2]);
    } else {
        printf("Shortest Path (Dijkstra's Algorithm):\n");
        for (int i = 0; path[i] != city2; i++) {
            printf("%s --[%d]--> ", g->cityNames[path[i]], g->adjMatrix[path[i]][path[i + 1]]);
        }
        printf("%s\n", g->cityNames[city2]);
        printf("Total Distance: %d km\n", distance);
    }
    printf("------------------------------------------------\n");
}

// Calculating travel time based on transport mode
float calculateTravelTime(int distance, const char* transport) {
    float speed;
    if (strcmp(transport, "Car") == 0) speed = 60;
    else if (strcmp(transport, "Flight") == 0) speed = 800;
    else if (strcmp(transport, "Train") == 0) speed = 100;
    else return -1; // Invalid transport mode
    return (float)distance / speed;
}


int main() {
    Graph g;
    initializeGraph(&g);
    int city1 = -1, city2 = -1;
    int choice;

    do {
        // Selecting cities for navigation
        printf("Select starting city (0 to %d): ", MAX_CITIES - 1);
        scanf("%d", &city1);
        printf("Select destination city (0 to %d): ", MAX_CITIES - 1);
        scanf("%d", &city2);
        
        if (city1 < 0 || city1 >= MAX_CITIES || city2 < 0 || city2 >= MAX_CITIES) {
            printf("Invalid city selection. Please choose valid cities.\n");
            continue;
        }

        displayJourney(&g, city1, city2); 

        do {
            // Displaying options menu
            printf("\n--- Map Navigation System ---\n");
            printf("1. Change Cities to Navigate From/To\n");
            printf("2. Select Mode of Transport\n");
            printf("3. Display Weather Conditions\n");
            printf("4. Display Traffic Pattern Analysis\n");
            printf("5. Remove a Road (Real-Time Update)\n");
            printf("6. Display Graph\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    printf("Enter the cities to navigate from and to (0 to %d): ", MAX_CITIES - 1);
                    scanf("%d %d", &city1, &city2);
                    displayJourney(&g, city1, city2);
                    break;
                case 2: {
                    char transport[10];
                    printf("Select mode of transport (Car/Flight/Train): ");
                    scanf("%s", transport);
                    float travelTime = calculateTravelTime(g.adjMatrix[city1][city2], transport);
                    if (travelTime >= 0) {
                        printf("Estimated travel time from %s to %s by %s is %.2f hours.\n", g.cityNames[city1], g.cityNames[city2], transport, travelTime);
                    } else {
                        printf("Invalid mode of transport selected.\n");
                    }
                    break;
                }
                case 3:
                    displayWeather();
                    break;
                case 4:
                    displayTrafficPattern();
                    break;
                case 5: {
                    int rmCity1, rmCity2;
                    printf("Enter the two cities for road removal (0 to %d): ", MAX_CITIES - 1);
                    scanf("%d %d", &rmCity1, &rmCity2);
                    removeRoad(&g, rmCity1, rmCity2);
                    break;
                }
                case 6:
                    printf("Adjacency Matrix:\n");
                    for (int i = 0; i < MAX_CITIES; i++) {
                        for (int j = 0; j < MAX_CITIES; j++) {
                            if (g.adjMatrix[i][j] == INF) printf("INF\t");
                            else printf("%d\t", g.adjMatrix[i][j]);
                        }
                        printf("\n");
                    }
                    break;
                default:
                    printf("Invalid choice. Please try again.\n");
                    break;
            } 
        } while (1);
    } while (1);
    return 0;
}