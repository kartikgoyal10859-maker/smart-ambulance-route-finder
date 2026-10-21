
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define V 20
#define INF 1000000000

const char *places[V] = {
    "Mahavir Nagar", "Kandivali Station", "Charkop",
    "Borivali Station", "Eksar", "Dahisar",
    "Gorai", "Malad Station", "Kandivali East",
    "City Hospital", "Poisar", "IC Colony",
    "Thakur Village", "Mira Road", "Andheri Station",
    "Goregaon", "Jogeshwari", "Bhayandar",
    "Emergency Ward", "Shimpoli"
};

typedef struct {
    int u, v, w;
} Road;

Road roads[] = {
    {0,1,4}, {0,2,3}, {0,8,5}, {0,19,4},
    {1,2,5}, {1,3,6}, {1,8,4},
    {2,4,4}, {2,10,6}, {2,19,3},
    {3,4,3}, {3,11,5}, {3,13,8},
    {4,5,5}, {4,10,4}, {4,11,4},
    {5,6,6}, {5,13,5}, {5,17,7},
    {6,7,8}, {6,17,6},
    {7,8,5}, {7,15,4}, {7,16,6},
    {8,9,7}, {8,12,4},
    {9,12,3}, {9,14,9}, {9,18,2},
    {10,11,3}, {10,12,6},
    {11,13,7}, {11,19,4},
    {12,14,8}, {12,15,5},
    {13,17,4}, {13,18,6},
    {14,15,4}, {14,16,5},
    {15,16,3}, {16,18,7},
    {17,18,5}, {18,19,3}
};

#define E ((int)(sizeof(roads) / sizeof(roads[0])))

int graph[V][V];
int posX[V] = {
    100,220,180,360,300,460,570,550,310,690,
    250,430,410,600,800,700,850,760,900,150
};
int posY[V] = {
    180,100,280,80,190,200,320,420,40,100,
    350,300,30,430,220,350,450,520,340,430
};

void buildGraph(void) {
    int i;
    for (i = 0; i < V; i++) {
        int j;
        for (j = 0; j < V; j++)
            graph[i][j] = 0;
    }

    for (i = 0; i < E; i++) {
        int a = roads[i].u;
        int b = roads[i].v;
        graph[a][b] = roads[i].w;
        graph[b][a] = roads[i].w;
    }
}

void dijkstra(int source, int dist[], int parent[],
              int bannedA, int bannedB) {
    int visited[V] = {0};
    int i;

    for (i = 0; i < V; i++) {
        dist[i] = INF;
        parent[i] = -1;
    }
    dist[source] = 0;

    for (i = 0; i < V; i++) {
        int u = -1;
        int best = INF;
        int j;

        for (j = 0; j < V; j++) {
            if (!visited[j] && dist[j] < best) {
                best = dist[j];
                u = j;
            }
        }

        if (u == -1)
            break;

        visited[u] = 1;

        for (j = 0; j < V; j++) {
            if (graph[u][j] > 0 && !visited[j]) {
                if ((u == bannedA && j == bannedB) ||
                    (u == bannedB && j == bannedA))
                    continue;

                if (dist[u] + graph[u][j] < dist[j]) {
                    dist[j] = dist[u] + graph[u][j];
                    parent[j] = u;
                }
            }
        }
    }
}

int getPath(int source, int destination, int parent[],
            int path[]) {
    int reverse[V];
    int count = 0;
    int i = destination;

    while (i != -1 && count < V) {
        reverse[count++] = i;
        if (i == source)
            break;
        i = parent[i];
    }

    if (count == 0 || reverse[count - 1] != source)
        return 0;

    for (i = 0; i < count; i++)
        path[i] = reverse[count - 1 - i];

    return count;
}

int roadIsOnPath(int a, int b, int path[], int count) {
    int i;
    for (i = 0; i < count - 1; i++) {
        if ((path[i] == a && path[i + 1] == b) ||
            (path[i] == b && path[i + 1] == a))
            return 1;
    }
    return 0;
}

void drawNetwork(const char *filename, int path[], int pathCount) {
    FILE *f = fopen(filename, "w");
    int i;

    if (f == NULL) {
        printf("Could not create %s\n", filename);
        return;
    }

    fprintf(f, "<svg xmlns='http://www.w3.org/2000/svg' "
               "width='1000' height='620'>");
    fprintf(f, "<rect width='100%%' height='100%%' fill='#f4f7fb'/>");
    fprintf(f, "<text x='500' y='30' text-anchor='middle' "
               "font-size='22' font-family='Arial' font-weight='bold'>"
               "Simulated Mumbai Road Network</text>");

    for (i = 0; i < E; i++) {
        int a = roads[i].u, b = roads[i].v;
        int chosen = roadIsOnPath(a, b, path, pathCount);

        fprintf(f,
            "<line x1='%d' y1='%d' x2='%d' y2='%d' "
            "stroke='%s' stroke-width='%d'/>",
            posX[a], posY[a], posX[b], posY[b],
            chosen ? "#e63946" : "#b8c3d1",
            chosen ? 5 : 2);

        fprintf(f,
            "<text x='%d' y='%d' font-size='10' "
            "font-family='Arial' fill='#555'>%d</text>",
            (posX[a] + posX[b]) / 2,
            (posY[a] + posY[b]) / 2,
            roads[i].w);
    }

    for (i = 0; i < V; i++) {
        fprintf(f,
            "<circle cx='%d' cy='%d' r='7' fill='%s' "
            "stroke='white' stroke-width='2'/>",
            posX[i], posY[i],
            roadIsOnPath(i, i, path, pathCount) ?
                "#e63946" : "#2463a6");

        fprintf(f,
            "<text x='%d' y='%d' font-size='11' "
            "font-family='Arial' fill='#172033'>%s</text>",
            posX[i] + 9, posY[i] - 8, places[i]);
    }

    fprintf(f, "<text x='20' y='590' font-size='13' "
               "font-family='Arial'>Red roads: selected shortest route | "
               "Numbers: simulated road distance (km)</text>");
    fprintf(f, "</svg>");
    fclose(f);
}

void drawDistanceChart(const char *filename, int path[],
                       int count) {
    FILE *f = fopen(filename, "w");
    int i, cumulative = 0, maxDist = 1;
    int totals[V];

    if (f == NULL)
        return;

    totals[0] = 0;
    for (i = 1; i < count; i++) {
        cumulative += graph[path[i - 1]][path[i]];
        totals[i] = cumulative;
        if (cumulative > maxDist)
            maxDist = cumulative;
    }

    fprintf(f, "<svg xmlns='http://www.w3.org/2000/svg' "
               "width='900' height='500'>");
    fprintf(f, "<rect width='100%%' height='100%%' fill='#ffffff'/>");
    fprintf(f, "<text x='450' y='30' text-anchor='middle' "
               "font-size='22' font-family='Arial' font-weight='bold'>"
               "Cumulative Distance Along Route</text>");

    for (i = 0; i <= 5; i++) {
        int y = 400 - i * 65;
        fprintf(f, "<line x1='80' y1='%d' x2='850' y2='%d' "
                   "stroke='#dddddd'/>", y, y);
    }

    fprintf(f, "<line x1='80' y1='75' x2='80' y2='400' "
               "stroke='#333'/>");
    fprintf(f, "<line x1='80' y1='400' x2='850' y2='400' "
               "stroke='#333'/>");

    for (i = 0; i < count; i++) {
        int x = 100 + i * (720 / (count > 1 ? count - 1 : 1));
        int y = 400 - (totals[i] * 325 / maxDist);

        fprintf(f, "<circle cx='%d' cy='%d' r='5' fill='#2463a6'/>",
                x, y);

        fprintf(f, "<text x='%d' y='%d' font-size='11' "
                   "font-family='Arial' text-anchor='middle'>%d</text>",
                x, y - 12, totals[i]);

        fprintf(f, "<text transform='translate(%d,420) rotate(35)' "
                   "font-size='10' font-family='Arial'>%s</text>",
                x, places[path[i]]);

        if (i > 0) {
            int prevX = 100 + (i - 1) *
                (720 / (count > 1 ? count - 1 : 1));
            int prevY = 400 - (totals[i - 1] * 325 / maxDist);

            fprintf(f, "<line x1='%d' y1='%d' x2='%d' y2='%d' "
                       "stroke='#2463a6' stroke-width='3'/>",
                    prevX, prevY, x, y);
        }
    }

    fprintf(f, "<text x='450' y='485' text-anchor='middle' "
               "font-size='13' font-family='Arial'>"
               "Route progress (km) - simulated data</text>");
    fprintf(f, "</svg>");
    fclose(f);
}

void drawRouteComparison(const char *filename, int source,
                         int destination, int bestDistance,
                         int path[], int pathCount) {
    FILE *f = fopen(filename, "w");
    int first = INF, second = INF;
    int i, dist[V], parent[V];

    if (f == NULL)
        return;

    /* Find alternative routes by temporarily blocking each
       road on the shortest route. */
    for (i = 0; i < pathCount - 1; i++) {
        int a = path[i], b = path[i + 1];

        dijkstra(source, dist, parent, a, b);

        if (dist[destination] < first) {
            second = first;
            first = dist[destination];
        } else if (dist[destination] > bestDistance &&
                   dist[destination] < second) {
            second = dist[destination];
        }
    }

    fprintf(f, "<svg xmlns='http://www.w3.org/2000/svg' "
               "width='800' height='420'>");
    fprintf(f, "<rect width='100%%' height='100%%' fill='#fff'/>");
    fprintf(f, "<text x='400' y='35' text-anchor='middle' "
               "font-size='22' font-family='Arial' font-weight='bold'>"
               "Route Distance Comparison</text>");

    {
        int values[3];
        const char *labels[3] = {
            "Shortest route", "Alternative route 1",
            "Alternative route 2"
        };

        values[0] = bestDistance;
        values[1] = first;
        values[2] = second;

        for (i = 0; i < 3; i++) {
            int y = 100 + i * 90;
            int width;

            if (values[i] >= INF)
                width = 0;
            else
                width = values[i] * 8;

            if (width > 650)
                width = 650;

            fprintf(f, "<text x='30' y='%d' font-size='14' "
                       "font-family='Arial'>%s</text>",
                    y - 12, labels[i]);

            fprintf(f, "<rect x='30' y='%d' width='%d' height='30' "
                       "fill='%s'/>",
                    y, width,
                    i == 0 ? "#218838" : "#e6a23c");

            if (values[i] >= INF) {
                fprintf(f, "<text x='40' y='%d' font-size='13' "
                           "font-family='Arial'>No alternative found</text>",
                        y + 20);
            } else {
                fprintf(f, "<text x='%d' y='%d' font-size='13' "
                           "font-family='Arial'>%d km</text>",
                        40 + width, y + 20, values[i]);
            }
        }
    }

    fprintf(f, "<text x='30' y='390' font-size='12' "
               "font-family='Arial'>"
               "Alternatives are calculated by blocking roads on "
               "the shortest route. Distances are simulated.</text>");
    fprintf(f, "</svg>");
    fclose(f);
}

int main(void) {
    int source, destination;
    int dist[V], parent[V], path[V];
    int pathCount, i;
    int totalDistance, estimatedMinutes;

    buildGraph();

    printf("\n========================================\n");
    printf("   SMART AMBULANCE ROUTE FINDER\n");
    printf("   Dijkstra Shortest Path Algorithm\n");
    printf("========================================\n");
    printf("\nLocations (enter the number):\n");

    for (i = 0; i < V; i++)
        printf("%2d. %s\n", i, places[i]);

    printf("\nEnter starting location number (0-19): ");
    if (scanf("%d", &source) != 1 || source < 0 || source >= V) {
        printf("Invalid starting location.\n");
        return 1;
    }

    printf("Enter destination location number (0-19): ");
    if (scanf("%d", &destination) != 1 ||
        destination < 0 || destination >= V) {
        printf("Invalid destination location.\n");
        return 1;
    }

    dijkstra(source, dist, parent, -1, -1);

    if (dist[destination] == INF) {
        printf("No route could be found.\n");
        return 1;
    }

    pathCount = getPath(source, destination, parent, path);
    if (pathCount == 0) {
        printf("Unable to reconstruct the route.\n");
        return 1;
    }

    totalDistance = dist[destination];

    /* Approximate demonstration speed: 30 km/h.
       This is not a real-time traffic estimate. */
    estimatedMinutes = totalDistance * 2;

    printf("\n========== AMBULANCE ROUTE REPORT ==========\n");
    printf("Starting Point : %s\n", places[source]);
    printf("Destination    : %s\n", places[destination]);
    printf("Shortest Route : ");

    for (i = 0; i < pathCount; i++) {
        printf("%s", places[path[i]]);
        if (i < pathCount - 1)
            printf(" -> ");
    }

    printf("\nTotal Distance : %d km (simulated)\n", totalDistance);
    printf("Estimated Time : %d minutes (simulated)\n",
           estimatedMinutes);
    printf("============================================\n");

    drawNetwork("road_network.svg", NULL, 0);
    drawNetwork("best_route.svg", path, pathCount);
    drawDistanceChart("distance_line.svg", path, pathCount);
    drawRouteComparison("route_comparison.svg", source, destination,
                        totalDistance, path, pathCount);

    printf("\nVisual reports created:\n");
    printf("1. road_network.svg\n");
    printf("2. best_route.svg\n");
    printf("3. distance_line.svg\n");
    printf("4. route_comparison.svg\n");
    printf("\nOpen these SVG files in your browser to view them.\n");

    return 0;
}
