#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <algorithm>
#include <cmath>
#include <memory>

using namespace std;

// Representación del estado del puzzle
struct NodoPuzzle {
    vector<vector<int>> tablero;
    int fila_vacia, columna_vacia;
    int costo_real; // costo acumulado (g)
    int valor_heuristico; // valor heurístico (h)
    string secuencia_movimientos;
    shared_ptr<NodoPuzzle> nodo_padre;
    
    NodoPuzzle(vector<vector<int>> tab, int fv, int cv, int cr, int vh, string sm, shared_ptr<NodoPuzzle> np)
        : tablero(tab), fila_vacia(fv), columna_vacia(cv), costo_real(cr), valor_heuristico(vh), secuencia_movimientos(sm), nodo_padre(np) {}
    
    // Función para calcular el costo total f = g + h
    int costoTotal() const {
        return costo_real + valor_heuristico;
    }
    
    // Operador para comparar nodos (para la cola de prioridad)
    bool operator>(const NodoPuzzle& otro) const {
        return costoTotal() > otro.costoTotal();
    }
};

// Función para calcular la distancia Manhattan
int calcularDistanciaManhattan(const vector<vector<int>>& tablero) {
    int distancia_total = 0;
    vector<pair<int, int>> posiciones_objetivo = {
        {0, 0}, {0, 1}, {0, 2},
        {1, 0}, {1, 1}, {1, 2},
        {2, 0}, {2, 1}, {2, 2}
    };
    
    for (int fila = 0; fila < 3; fila++) {
        for (int columna = 0; columna < 3; columna++) {
            int valor_casilla = tablero[fila][columna];
            if (valor_casilla != 0) { // ignorar el espacio vacío
                auto pos_objetivo = posiciones_objetivo[valor_casilla - 1];
                distancia_total += abs(fila - pos_objetivo.first) + abs(columna - pos_objetivo.second);
            }
        }
    }
    return distancia_total;
}

// Función para convertir el tablero a string (para el hash)
string tableroAString(const vector<vector<int>>& tablero) {
    string resultado;
    for (const auto& fila : tablero) {
        for (int valor : fila) {
            resultado += to_string(valor);
        }
    }
    return resultado;
}

// Función para verificar si un estado es el estado objetivo
bool esEstadoObjetivo(const vector<vector<int>>& tablero) {
    vector<vector<int>> tablero_objetivo = {
        {1, 2, 3},
        {8, 0, 4},
        {7, 6, 5}
    };
    return tablero == tablero_objetivo;
}

// Función para generar nodos sucesores
vector<NodoPuzzle> generarSucesores(shared_ptr<NodoPuzzle> nodo_actual) {
    vector<NodoPuzzle> lista_sucesores;
    int fila_actual = nodo_actual->fila_vacia;
    int columna_actual = nodo_actual->columna_vacia;
    
    // Movimientos posibles: arriba, derecha, abajo, izquierda
    vector<pair<int, int>> direcciones = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    vector<string> nombres_direcciones = {"Arriba", "Derecha", "Abajo", "Izquierda"};
    
    for (int indice = 0; indice < 4; indice++) {
        int nueva_fila = fila_actual + direcciones[indice].first;
        int nueva_columna = columna_actual + direcciones[indice].second;
        
        // Verificar si el movimiento es válido
        if (nueva_fila >= 0 && nueva_fila < 3 && nueva_columna >= 0 && nueva_columna < 3) {
            // Crear nuevo tablero
            vector<vector<int>> nuevo_tablero = nodo_actual->tablero;
            swap(nuevo_tablero[fila_actual][columna_actual], nuevo_tablero[nueva_fila][nueva_columna]);
            
            // Calcular heurística
            int heuristica = calcularDistanciaManhattan(nuevo_tablero);
            
            // Crear nuevo nodo
            string nuevos_movimientos = nodo_actual->secuencia_movimientos;
            if (!nuevos_movimientos.empty()) nuevos_movimientos += " -> ";
            nuevos_movimientos += nombres_direcciones[indice];
            
            NodoPuzzle nuevo_nodo(nuevo_tablero, nueva_fila, nueva_columna, 
                                nodo_actual->costo_real + 1, heuristica, 
                                nuevos_movimientos, nodo_actual);
            lista_sucesores.push_back(nuevo_nodo);
        }
    }
    
    return lista_sucesores;
}

// Función para imprimir el tablero
void mostrarTablero(const vector<vector<int>>& tablero) {
    cout << "+---+---+---+" << endl;
    for (int fila = 0; fila < 3; fila++) {
        cout << "|";
        for (int columna = 0; columna < 3; columna++) {
            if (tablero[fila][columna] == 0) {
                cout << "   |";
            } else {
                cout << " " << tablero[fila][columna] << " |";
            }
        }
        cout << endl << "+---+---+---+" << endl;
    }
}

// Función principal para resolver el puzzle
shared_ptr<NodoPuzzle> resolverPuzzle(const vector<vector<int>>& tablero_inicial) {
    // Encontrar la posición del espacio vacío
    int fila_vacia = -1, columna_vacia = -1;
    for (int fila = 0; fila < 3; fila++) {
        for (int columna = 0; columna < 3; columna++) {
            if (tablero_inicial[fila][columna] == 0) {
                fila_vacia = fila;
                columna_vacia = columna;
                break;
            }
        }
    }
    
    // Nodo inicial
    int heuristica_inicial = calcularDistanciaManhattan(tablero_inicial);
    auto nodo_inicial = make_shared<NodoPuzzle>(tablero_inicial, fila_vacia, columna_vacia, 
                                              0, heuristica_inicial, "", nullptr);
    
    // Cola de prioridad para A*
    priority_queue<NodoPuzzle, vector<NodoPuzzle>, greater<NodoPuzzle>> cola_prioridad;
    cola_prioridad.push(*nodo_inicial);
    
    // Conjunto para nodos visitados
    unordered_set<string> nodos_visitados;
    nodos_visitados.insert(tableroAString(tablero_inicial));
    
    while (!cola_prioridad.empty()) {
        NodoPuzzle nodo_actual = cola_prioridad.top();
        cola_prioridad.pop();
        
        // Verificar si es el estado objetivo
        if (esEstadoObjetivo(nodo_actual.tablero)) {
            return make_shared<NodoPuzzle>(nodo_actual);
        }
        
        // Generar sucesores
        auto ptr_nodo_actual = make_shared<NodoPuzzle>(nodo_actual);
        vector<NodoPuzzle> sucesores = generarSucesores(ptr_nodo_actual);
        
        for (const NodoPuzzle& sucesor : sucesores) {
            string clave_tablero = tableroAString(sucesor.tablero);
            
            // Si no hemos visitado este nodo
            if (nodos_visitados.find(clave_tablero) == nodos_visitados.end()) {
                nodos_visitados.insert(clave_tablero);
                cola_prioridad.push(sucesor);
            }
        }
    }
    
    return nullptr; // No se encontró solución
}

// Función para imprimir la solución completa
void mostrarSolucion(shared_ptr<NodoPuzzle> solucion) {
    if (!solucion) {
        cout << "No se encontró solución para el puzzle." << endl;
        return;
    }
    
    // Recolectar todos los nodos de la solución
    vector<shared_ptr<NodoPuzzle>> pasos_solucion;
    auto nodo_actual = solucion;
    while (nodo_actual != nullptr) {
        pasos_solucion.push_back(nodo_actual);
        nodo_actual = nodo_actual->nodo_padre;
    }
    reverse(pasos_solucion.begin(), pasos_solucion.end());
    
    cout << "\n=== SOLUCIÓN ENCONTRADA ===" << endl;
    cout << "Número total de movimientos: " << pasos_solucion.size() - 1 << endl;
    cout << "Secuencia de movimientos: " << solucion->secuencia_movimientos << endl;
    cout << "\n=== PASO A PASO ===" << endl;
    
    for (size_t paso = 0; paso < pasos_solucion.size(); paso++) {
        cout << "\nPaso " << paso << ":" << endl;
        if (paso > 0) {
            size_t pos_anterior = pasos_solucion[paso-1]->secuencia_movimientos.length();
            cout << "Movimiento: " << pasos_solucion[paso]->secuencia_movimientos.substr(pos_anterior) << endl;
        }
        mostrarTablero(pasos_solucion[paso]->tablero);
    }
}

// Función para leer el tablero inicial desde el usuario
vector<vector<int>> leerTableroInicial() {
    vector<vector<int>> tablero(3, vector<int>(3));
    
    cout << "=== CONFIGURACIÓN INICIAL DEL 8-PUZZLE ===" << endl;
    cout << "Ingrese el tablero 3x3 (use 0 para el espacio vacío):" << endl;
    
    for (int fila = 0; fila < 3; fila++) {
        cout << "Fila " << (fila + 1) << " (3 números separados por espacios): ";
        for (int columna = 0; columna < 3; columna++) {
            cin >> tablero[fila][columna];
        }
    }
    
    return tablero;
}

int main() {
    cout << "RESOLVEDOR DEL 8-PUZZLE" << endl;
    cout << "========================" << endl;
    
    // Leer configuración inicial
    vector<vector<int>> tablero_inicial = leerTableroInicial();
    
    cout << "\nTablero inicial ingresado:" << endl;
    mostrarTablero(tablero_inicial);
    
    cout << "\nResolviendo el puzzle..." << endl;
    
    // Resolver el puzzle
    auto solucion = resolverPuzzle(tablero_inicial);
    
    // Mostrar la solución
    mostrarSolucion(solucion);
    
    return 0;
}