#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <windows.h>   // Para activar UTF-8 en Windows

using namespace std;

using State = string;
const std::string GOAL = "123456780";

int dx[4] = {-1, 1, 0, 0};
int dy[4] = {0, 0, -1, 1};

int posToIndex(int x, int y) { return x * 3 + y; }

// ======================= IMPRESIÓN =======================
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
// ==========================================================

// ================ HEURÍSTICA MANHATTAN ====================
int heuristic(const State& s) {
    int dist = 0;

    for (int i = 0; i < 9; ++i) {

        int val = s[i] - '0';
        int cur_x = i / 3;
        int cur_y = i % 3;

        int target_x, target_y;

        if (val == 0) {
            target_x = 1; target_y = 1; // posición del 0 en GOAL
        } else {
            target_x = (val - 1) / 3;
            target_y = (val - 1) % 3;
        }

        dist += abs(cur_x - target_x) + abs(cur_y - target_y);
    }
    return dist;
}
// ==========================================================

// ================ GENERACIÓN DE VECINOS ===================
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
// ==========================================================

// ================ RECONSTRUCCIÓN DE CAMINO ================
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
// ==========================================================

// ================ VERIFICADOR DE SOLUBILIDAD ==============
bool esSoluble(const string& s) {
    vector<int> v;

    for (char c : s)
        if (c != '0')
            v.push_back(c - '0');

    int inv = 0;

    for (int i = 0; i < v.size(); i++)
        for (int j = i + 1; j < v.size(); j++)
            if (v[i] > v[j])
                inv++;

    return (inv % 2 == 0);
}
// ==========================================================

// ================ ALGORITMO A* ============================
bool solvePuzzle(const State& start) {

    auto cmp = [](const pair<int, State>& a, const pair<int, State>& b) {
        return a.first > b.first;
    };

    priority_queue<pair<int, State>, vector<pair<int, State>>, decltype(cmp)> pq(cmp);

    unordered_map<State, int> g;
    unordered_map<State, State> parent;

    g[start]     = 0;
    parent[start] = start;
    pq.push({heuristic(start), start});

    int nodos = 0;

    while (!pq.empty()) {
        State curr = pq.top().second; 
        pq.pop();
        nodos++;

        if (curr == GOAL) {
            cout << "\n✓ Solución encontrada en " << g[curr] 
                 << " movimientos (" << nodos << " nodos explorados)\n\n";

            vector<State> path = reconstructPath(parent, curr);

            for (int i = 0; i < path.size(); ++i) {
                cout << "Paso " << i << ":\n";
                printState(path[i]);
                if (i < path.size() - 1)
                    cout << "   ↓\n";
            }
            return true;
        }

        for (const State& next : getNeighbors(curr)) {
            int tg = g[curr] + 1;

            if (!g.count(next) || tg < g[next]) {
                g[next] = tg;
                parent[next] = curr;
                pq.push({tg + heuristic(next), next});
            }
        }
    }

    cout << "\nNo hay solución.\n";
    return false;
}
// ==========================================================

int main() {

    // ACTIVAR UTF-8 EN WINDOWS PARA TILDES
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    cout << "=====================================\n";
    cout << "        RESOLVEDOR DE 8-PUZZLE\n";
    cout << "=====================================\n\n";

    cout << "Ingresa los 9 números (0 = espacio vacío):\n";
    cout << "Ejemplo: 5 7 2 4 1 0 3 8 6\n\n> ";

    vector<int> valores;
    int num;

    while (valores.size() < 9 && cin >> num)
        valores.push_back(num);

    vector<bool> usado(9, false);
    State inicio = "";

    for (int v : valores) {
        if (v < 0 || v > 8 || usado[v]) {
            cout << "\n✗ ERROR: Deben ser los números del 0 al 8 sin repetir.\n";
            return 1;
        }
        usado[v] = true;
        inicio += char('0' + v);
    }

    cout << "\nTablero inicial:\n";
    printState(inicio);

    // ====================== VERIFICAR SOLUBILIDAD ===========================
    if (!esSoluble(inicio)) {
        cout << "✗ Este rompecabezas NO tiene solución posible.\n";
        return 0;
    }
    // ========================================================================

    cout << "✓ El rompecabezas es soluble. Buscando solución...\n";

    solvePuzzle(inicio);

    return 0;
}
