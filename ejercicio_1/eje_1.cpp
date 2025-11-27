#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <cmath>
using namespace std;

// Representamos el tablero como un string de 9 caracteres (más eficiente para hash)
using State = string;

// Posición objetivo (como en la Fig. 2 del enunciado)
const State GOAL = "123804765";  // 1 2 3
                                 // 8 0 4
                                 // 7 6 5

// Direcciones: arriba, abajo, izquierda, derecha
int dx[4] = {-1, 1, 0, 0};
int dy[4] = {0, 0, -1, 1};

// Convierte posición (x,y) a índice en el string (0-8)
int posToIndex(int x, int y) { return x * 3 + y; }

// Heurística: distancia de Manhattan
int heuristic(const State& s) {
    int dist = 0;
    for (int i = 0; i < 9; ++i) {
        if (s[i] == '0') continue;           // el vacío no cuenta
        int value = s[i] - '0';
        int goalX = (value == 0) ? 1 : (value - 1) / 3;  // ajustamos porque 0 está en (1,1)
        int goalY = (value == 0) ? 1 : (value - 1) % 3;
        if (value == 0) { goalX = 1; goalY = 1; }  // posición del 0 en GOAL
        
        int currX = i / 3;
        int currY = i % 3;
        dist += abs(currX - goalX) + abs(currY - goalY);
    }
    return dist;
}

// Obtiene todos los vecinos posibles moviendo el 0
vector<State> getNeighbors(const State& s) {
    vector<State> neighbors;
    int pos = -1;
    for (int i = 0; i < 9; ++i) if (s[i] == '0') { pos = i; break; }
    
    int x = pos / 3;
    int y = pos % 3;
    
    for (int d = 0; d < 4; ++d) {
        int nx = x + dx[d];
        int ny = y + dy[d];
        if (nx >= 0 && nx < 3 && ny >= 0 && ny < 3) {
            State next = s;
            swap(next[pos], next[posToIndex(nx, ny)]);
            neighbors.push_back(next);
        }
    }
    return neighbors;
}

// Reconstruye el camino desde el padre
vector<State> reconstructPath(unordered_map<State, State>& parent, State current) {
    vector<State> path;
    while (current != parent[current]) {
        path.push_back(current);
        current = parent[current];
    }
    path.push_back(current);  // estado inicial
    reverse(path.begin(), path.end());
    return path;
}

// Imprime bonito un estado
void printState(const State& s) {
    for (int i = 0; i < 9; ++i) {
        cout << " " << (s[i] == '0' ? ' ' : s[i]);
        if (i % 3 == 2) cout << "\n";
    }
    cout << "\n";
}

// Algoritmo A*
bool solvePuzzle(const State& start) {
    if (start == GOAL) {
        cout << "¡Ya está resuelto!\n";
        printState(start);
        return true;
    }

    auto cmp = [](const pair<int, State>& a, const pair<int, State>& b) {
        return a.first > b.first;
    };
    priority_queue<pair<int, State>, vector<pair<int, State>>, decltype(cmp)> pq(cmp);

    unordered_map<State, int> gScore;
    unordered_map<State, State> parent;
    unordered_map<State, int> fScore;

    gScore[start] = 0;
    parent[start] = start;
    fScore[start] = heuristic(start);
    pq.push({fScore[start], start});

    while (!pq.empty()) {
        State current = pq.top().second;
        pq.pop();

        if (current == GOAL) {
            cout << "¡Solución encontrada en " << gScore[current] << " movimientos!\n\n";
            vector<State> path = reconstructPath(parent, current);
            for (int i = 0; i < path.size(); ++i) {
                cout << "Paso " << i << ":\n";
                printState(path[i]);
            }
            return true;
        }

        for (const State& neighbor : getNeighbors(current)) {
            int tentative_g = gScore[current] + 1;

            if (gScore.find(neighbor) == gScore.end() || tentative_g < gScore[neighbor]) {
                parent[neighbor] = current;
                gScore[neighbor] = tentative_g;
                fScore[neighbor] = tentative_g + heuristic(neighbor);
                pq.push({fScore[neighbor], neighbor});
            }
        }
    }

    cout << "No tiene solución (aunque con 8-puzzle siempre debería tenerla si es paridad correcta).\n";
    return false;
}

int main() {
    cout << "=== Resolutor de 8-Puzzle (3x3) ===\n";
    cout << "Ingresa el tablero inicial fila por fila (9 números del 0 al 8, sin repetir)\n";
    cout << "Usa 0 para la casilla vacía. Ejemplo para el estado inicial de tu imagen:\n";
    cout << "5 7 2 4 1 0 3 8 6\n\n-> ";

    State start = "";
    vector<int> nums;
    int x;
    while (nums.size() < 9 && cin >> x) {
        nums.push_back(x);
    }

    // Validación básica
    vector<bool> used(9, false);
    for (int n : nums) {
        if (n < 0 || n > 8 || used[n]) {
            cout << "Error: números deben ser de 0 a 8 sin repetir.\n";
            return 1;
        }
        used[n] = true;
        start += ('0' + n);
    }

    cout << "\nTablero inicial:\n";
    printState(start);

    solvePuzzle(start);

    return 0;
}