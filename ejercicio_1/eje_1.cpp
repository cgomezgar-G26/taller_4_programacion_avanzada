#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <cmath>
using namespace std;
using namespace std::string_literals;

using State = string;
const State GOAL = "123804765";  // 1 2 3 \ 8 0 4 \ 7 6 5

int dx[4] = {-1, 1, 0, 0};
int dy[4] = {0, 0, -1, 1};

int posToIndex(int x, int y) { return x * 3 + y; }

// ------------------ IMPRESIÓN BONITA ------------------
void printState(const State& s) {
    cout << "+---+---+---+\n";
    for (int i = 0; i < 9; ++i) {
        cout << "| ";
        if (s[i] == '0')
            cout << " ";
        else
            cout << s[i];
        cout << " ";
        if (i % 3 == 2) {
            cout << "|\n";
            cout << "+---+---+---+\n";
        }
    }
    cout << "\n";
}
// -----------------------------------------------------

int heuristic(const State& s) {
    int dist = 0;
    for (int i = 0; i < 9; ++i) {
        if (s[i] == '0') continue;
        int val = s[i] - '0';
        int target_x = (val - 1) / 3;
        int target_y = (val - 1) % 3;
        if (val == 0) { target_x = 1; target_y = 1; } // posición del 0 en GOAL

        int cur_x = i / 3;
        int cur_y = i % 3;
        dist += abs(cur_x - target_x) + abs(cur_y - target_y);
    }
    return dist;
}

vector<State> getNeighbors(const State& s) {
    vector<State> neighbors;
    int pos = s.find('0');
    int x = pos / 3, y = pos % 3;

    for (int d = 0; d < 4; ++d) {
        int nx = x + dx[d], ny = y + dy[d];
        if (nx >= 0 && nx < 3 && ny >= 0 && ny < 3) {
            State nxt = s;
            swap(nxt[pos], nxt[posToIndex(nx, ny)]);
            neighbors.push_back(nxt);
        }
    }
    return neighbors;
}

vector<State> reconstructPath(unordered_map<State, State>& parent, State curr) {
    vector<State> path;
    while (curr != parent[curr]) {
        path.push_back(curr);
        curr = parent[curr];
    }
    path.push_back(curr);
    reverse(path.begin(), path.end());
    return path;
}

bool solvePuzzle(const State& start) {
    if (start == GOAL) {
        cout << "El puzzle ya está resuelto!\n";
        printState(start);
        return true;
    }

    auto cmp = [](const pair<int, State>& a, const pair<int, State>& b) {
        return a.first > b.first;
    };
    priority_queue<pair<int, State>, vector<pair<int, State>>, decltype(cmp)> pq(cmp);

    unordered_map<State, int> g;
    unordered_map<State, State> parent;
    unordered_map<State, int> f;

    g[start] = 0;
    parent[start] = start;
    f[start] = heuristic(start);
    pq.push({f[start], start});

    int nodos = 0;
    while (!pq.empty()) {
        State curr = pq.top().second; pq.pop();
        nodos++;

        if (curr == "123804765"s) {
            cout << "SOLUCION ENCONTRADA en " << g[curr] << " movimientos (" << nodos << " nodos explorados)\n\n";
            vector<State> camino = reconstructPath(parent, curr);
            for (int i = 0; i < camino.size(); ++i) {
                cout << "Paso " << i << ":\n";
                printState(camino[i]);
                if (i < camino.size()-1) {
                    cout << "   ↓\n";
                }
            }
            return true;
        }

        for (const State& vec : getNeighbors(curr)) {
            int tg = g[curr] + 1;
            if (g.find(vec) == g.end() || tg < g[vec]) {
                parent[vec] = curr;
                g[vec] = tg;
                f[vec] = tg + heuristic(vec);
                pq.push({f[vec], vec});
            }
        }
    }
    cout << "No hay solución.\n";
    return false;
}

int main() {
    cout << "=====================================\n";
    cout << "   RESOLVEDOR DE 8-PUZZLE (3x3)\n";
    cout << "=====================================\n\n";
    cout << "Ingresa los 9 numeros (0 = espacio vacio) separados por espacios o enter:\n";
    cout << "Ejemplo: 5 7 2 4 1 0 3 8 6\n\n> ";

    State inicio = "";
    vector<int> valores;
    int num;
    while (valores.size() < 9 && cin >> num) {
        valores.push_back(num);
    }

    // Validacion
    vector<bool> usado(9, false);
    for (int v : valores) {
        if (v < 0 || v > 8 || usado[v]) {
            cout << "\nERROR: Deben ser los numeros del 0 al 8 sin repetir.\n";
            return 1;
        }
        usado[v] = true;
        inicio += ('0' + v);
    }

    cout << "\nTablero inicial:\n";
    printState(inicio);

    solvePuzzle(inicio);

    return 0;
}