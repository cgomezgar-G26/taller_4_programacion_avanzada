// ============================================================================
// RED NEURONAL ARTIFICIAL - CLASIFICACIÓN DE DÍGITOS 0-9
// ============================================================================
// Clasificación múltiple de dígitos en 4 categorías:
// - Clase 0: Números pares (0, 2, 4, 6, 8)
// - Clase 1: Números impares (1, 3, 5, 7, 9)
// - Clase 2: Números primos (2, 3, 5, 7)
// - Clase 3: Números compuestos (4, 6, 8, 9)
// ============================================================================

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <string>
#include <iomanip>
#include <random>
#include <algorithm>
#include <windows.h>   // Para activar UTF-8 en Windows

using namespace std;

// ==================== CONSTANTES ====================
const int FILAS_DIGITO = 7;      // Altura de cada dígito (7 filas)
const int COLS_DIGITO = 5;       // Ancho de cada dígito (5 columnas)
const int ENTRADAS = 35;         // Total de píxeles (7x5 = 35)
const int NEURONAS_OCULTA = 20;  // Neuronas en la capa oculta
const int SALIDAS = 4;           // 4 categorías de clasificación
const double TASA_APRENDIZAJE = 0.1;
const int EPOCAS = 5000;         // Número de iteraciones de entrenamiento

// ==================== FUNCIÓN DE ACTIVACIÓN SIGMOIDE ====================
// Squash de valores al rango (0, 1)
double sigmoide(double x) {
    return 1.0 / (1.0 + exp(-x));
}

// Derivada de la sigmoide (para backpropagation)
double derivada_sigmoide(double x) {
    return x * (1.0 - x);
}

// ==================== CLASE: RED NEURONAL ====================
class RedNeuronal {
private:
    // Pesos de las conexiones entre capas
    vector<vector<double>> pesos_entrada_oculta;   // [ENTRADAS][NEURONAS_OCULTA]
    vector<vector<double>> pesos_oculta_salida;    // [NEURONAS_OCULTA][SALIDAS]
    
    // Sesgos (bias) de cada capa
    vector<double> sesgo_oculta;   // [NEURONAS_OCULTA]
    vector<double> sesgo_salida;   // [SALIDAS]
    
    // Activaciones de cada capa (valores de las neuronas)
    vector<double> activacion_entrada;   // [ENTRADAS]
    vector<double> activacion_oculta;    // [NEURONAS_OCULTA]
    vector<double> activacion_salida;    // [SALIDAS]
    
    // Generador de números aleatorios
    mt19937 generador;
    
    // Inicializa pesos con valores aleatorios pequeños
    void inicializar_pesos() {
        uniform_real_distribution<double> dist(-0.5, 0.5);
        
        // Inicializar pesos entre entrada y capa oculta
        pesos_entrada_oculta.resize(ENTRADAS, vector<double>(NEURONAS_OCULTA));
        for (int i = 0; i < ENTRADAS; ++i) {
            for (int j = 0; j < NEURONAS_OCULTA; ++j) {
                pesos_entrada_oculta[i][j] = dist(generador);
            }
        }
        
        // Inicializar pesos entre capa oculta y salida
        pesos_oculta_salida.resize(NEURONAS_OCULTA, vector<double>(SALIDAS));
        for (int i = 0; i < NEURONAS_OCULTA; ++i) {
            for (int j = 0; j < SALIDAS; ++j) {
                pesos_oculta_salida[i][j] = dist(generador);
            }
        }
        
        // Inicializar sesgos
        sesgo_oculta.resize(NEURONAS_OCULTA);
        sesgo_salida.resize(SALIDAS);
        for (int i = 0; i < NEURONAS_OCULTA; ++i) {
            sesgo_oculta[i] = dist(generador);
        }
        for (int i = 0; i < SALIDAS; ++i) {
            sesgo_salida[i] = dist(generador);
        }
    }

public:
    RedNeuronal() : generador(42) {  // Semilla fija para reproducibilidad
        activacion_entrada.resize(ENTRADAS);
        activacion_oculta.resize(NEURONAS_OCULTA);
        activacion_salida.resize(SALIDAS);
        inicializar_pesos();
    }
    
    // PROPAGACIÓN HACIA ADELANTE (Forward Propagation)
    // Calcula la salida de la red dada una entrada
    vector<double> forward(const vector<double>& entrada) {
        // Copiar entrada a la capa de entrada
        activacion_entrada = entrada;
        
        // Calcular activaciones de la capa oculta
        for (int j = 0; j < NEURONAS_OCULTA; ++j) {
            double suma = sesgo_oculta[j];
            for (int i = 0; i < ENTRADAS; ++i) {
                suma += activacion_entrada[i] * pesos_entrada_oculta[i][j];
            }
            activacion_oculta[j] = sigmoide(suma);
        }
        
        // Calcular activaciones de la capa de salida
        for (int k = 0; k < SALIDAS; ++k) {
            double suma = sesgo_salida[k];
            for (int j = 0; j < NEURONAS_OCULTA; ++j) {
                suma += activacion_oculta[j] * pesos_oculta_salida[j][k];
            }
            activacion_salida[k] = sigmoide(suma);
        }
        
        return activacion_salida;
    }
    
    // RETROPROPAGACIÓN (Backpropagation)
    // Ajusta los pesos de la red para minimizar el error
    void backward(const vector<double>& objetivo) {
        // Calcular error en la capa de salida
        vector<double> error_salida(SALIDAS);
        for (int k = 0; k < SALIDAS; ++k) {
            double error = objetivo[k] - activacion_salida[k];
            error_salida[k] = error * derivada_sigmoide(activacion_salida[k]);
        }
        
        // Calcular error en la capa oculta
        vector<double> error_oculta(NEURONAS_OCULTA);
        for (int j = 0; j < NEURONAS_OCULTA; ++j) {
            double error = 0.0;
            for (int k = 0; k < SALIDAS; ++k) {
                error += error_salida[k] * pesos_oculta_salida[j][k];
            }
            error_oculta[j] = error * derivada_sigmoide(activacion_oculta[j]);
        }
        
        // Actualizar pesos entre capa oculta y salida
        for (int j = 0; j < NEURONAS_OCULTA; ++j) {
            for (int k = 0; k < SALIDAS; ++k) {
                pesos_oculta_salida[j][k] += TASA_APRENDIZAJE * error_salida[k] * activacion_oculta[j];
            }
        }
        
        // Actualizar pesos entre entrada y capa oculta
        for (int i = 0; i < ENTRADAS; ++i) {
            for (int j = 0; j < NEURONAS_OCULTA; ++j) {
                pesos_entrada_oculta[i][j] += TASA_APRENDIZAJE * error_oculta[j] * activacion_entrada[i];
            }
        }
        
        // Actualizar sesgos de la capa de salida
        for (int k = 0; k < SALIDAS; ++k) {
            sesgo_salida[k] += TASA_APRENDIZAJE * error_salida[k];
        }
        
        // Actualizar sesgos de la capa oculta
        for (int j = 0; j < NEURONAS_OCULTA; ++j) {
            sesgo_oculta[j] += TASA_APRENDIZAJE * error_oculta[j];
        }
    }
    
    // Entrena la red con un conjunto de datos
    void entrenar(const vector<vector<double>>& datos, const vector<vector<double>>& objetivos) {
        cout << "\nEntrenando la red neuronal...\n";
        
        for (int epoca = 0; epoca < EPOCAS; ++epoca) {
            double error_total = 0.0;
            
            // Entrenar con cada patrón
            for (size_t i = 0; i < datos.size(); ++i) {
                vector<double> salida = forward(datos[i]);
                backward(objetivos[i]);
                
                // Calcular error cuadrático medio
                for (int k = 0; k < SALIDAS; ++k) {
                    double diff = objetivos[i][k] - salida[k];
                    error_total += diff * diff;
                }
            }
            
            // Mostrar progreso cada 500 épocas
            if ((epoca + 1) % 500 == 0) {
                cout << "Época " << (epoca + 1) << "/" << EPOCAS 
                     << " - Error: " << fixed << setprecision(6) << error_total << endl;
            }
        }
        
        cout << "¡Entrenamiento completado!\n\n";
    }
    
    // Predice la clase de un patrón de entrada
    vector<double> predecir(const vector<double>& entrada) {
        return forward(entrada);
    }
};

// ==================== FUNCIONES DE CLASIFICACIÓN ====================
// Determina las clases a las que pertenece un número
vector<int> obtener_clases(int numero) {
    vector<int> clases;
    
    // Clase 0: Pares
    if (numero % 2 == 0) clases.push_back(0);
    
    // Clase 1: Impares
    if (numero % 2 == 1) clases.push_back(1);
    
    // Clase 2: Primos (2, 3, 5, 7)
    if (numero == 2 || numero == 3 || numero == 5 || numero == 7) {
        clases.push_back(2);
    }
    
    // Clase 3: Compuestos (4, 6, 8, 9)
    if (numero == 4 || numero == 6 || numero == 8 || numero == 9) {
        clases.push_back(3);
    }
    
    return clases;
}

// Convierte las clases en un vector objetivo (one-hot encoding modificado)
vector<double> clases_a_vector(const vector<int>& clases) {
    vector<double> objetivo(SALIDAS, 0.0);
    for (int clase : clases) {
        objetivo[clase] = 1.0;  // Activar las clases correspondientes
    }
    return objetivo;
}

// Interpreta la salida de la red
vector<int> interpretar_salida(const vector<double>& salida, double umbral = 0.5) {
    vector<int> clases_predichas;
    for (int i = 0; i < SALIDAS; ++i) {
        if (salida[i] > umbral) {
            clases_predichas.push_back(i);
        }
    }
    return clases_predichas;
}

// ==================== CARGA DE DATOS ====================
// Lee los dígitos desde el archivo digitos.txt
vector<vector<double>> cargar_digitos(const string& nombre_archivo) {
    ifstream archivo(nombre_archivo);
    if (!archivo.is_open()) {
        cerr << "Error: No se pudo abrir el archivo " << nombre_archivo << endl;
        return {};
    }
    
    vector<vector<double>> digitos;
    vector<double> digito_actual;
    
    int valor;
    int contador = 0;
    
    // Leer todos los valores del archivo
    while (archivo >> valor) {
        digito_actual.push_back(static_cast<double>(valor));
        contador++;
        
        // Cada dígito tiene 35 valores (7 filas x 5 columnas)
        if (contador == ENTRADAS) {
            digitos.push_back(digito_actual);
            digito_actual.clear();
            contador = 0;
        }
    }
    
    archivo.close();
    cout << "Cargados " << digitos.size() << " dígitos desde " << nombre_archivo << endl;
    return digitos;
}

// ==================== VISUALIZACIÓN ====================
// Muestra un dígito en formato ASCII
void mostrar_digito(const vector<double>& digito) {
    for (int i = 0; i < FILAS_DIGITO; ++i) {
        for (int j = 0; j < COLS_DIGITO; ++j) {
            int idx = i * COLS_DIGITO + j;
            cout << (digito[idx] > 0.5 ? "█" : " ") << " ";
        }
        cout << endl;
    }
}

// Convierte clases en nombres descriptivos
string nombre_clase(int clase) {
    switch(clase) {
        case 0: return "Par";
        case 1: return "Impar";
        case 2: return "Primo";
        case 3: return "Compuesto";
        default: return "Desconocido";
    }
}

// ==================== FUNCIÓN PRINCIPAL ====================
int main() {

    // Activar UTF-8 en Windows para caracteres especiales  
    SetConsoleOutputCP(CP_UTF8);

    cout << "============================================\n";
    cout << "  RED NEURONAL - CLASIFICACIÓN DE DÍGITOS\n";
    cout << "============================================\n\n";
    
    // 1. CARGAR LOS DÍGITOS DEL ARCHIVO
    vector<vector<double>> digitos = cargar_digitos("digitos.txt");
    
    if (digitos.size() != 10) {
        cerr << "Error: Se esperaban 10 dígitos (0-9)\n";
        return 1;
    }
    
    // 2. PREPARAR DATOS DE ENTRENAMIENTO
    vector<vector<double>> datos_entrenamiento;
    vector<vector<double>> objetivos_entrenamiento;
    
    for (int numero = 0; numero < 10; ++numero) {
        datos_entrenamiento.push_back(digitos[numero]);
        vector<int> clases = obtener_clases(numero);
        objetivos_entrenamiento.push_back(clases_a_vector(clases));
    }
    
    // 3. CREAR Y ENTRENAR LA RED NEURONAL
    RedNeuronal red;
    red.entrenar(datos_entrenamiento, objetivos_entrenamiento);
    
    // 4. PROBAR LA RED CON TODOS LOS DÍGITOS
    cout << "============================================\n";
    cout << "        RESULTADOS DE CLASIFICACIÓN\n";
    cout << "============================================\n\n";
    
    int correctos = 0;
    int total = 0;
    
    for (int numero = 0; numero < 10; ++numero) {
        cout << "Dígito " << numero << ":\n";
        mostrar_digito(digitos[numero]);
        
        // Predecir
        vector<double> salida = red.predecir(digitos[numero]);
        vector<int> clases_predichas = interpretar_salida(salida, 0.5);
        vector<int> clases_reales = obtener_clases(numero);
        
        // Mostrar resultados
        cout << "\nSalida de la red: ";
        for (int i = 0; i < SALIDAS; ++i) {
            cout << fixed << setprecision(2) << salida[i] << " ";
        }
        cout << endl;
        
        cout << "Clases predichas: ";
        if (clases_predichas.empty()) {
            cout << "Ninguna";
        } else {
            for (size_t i = 0; i < clases_predichas.size(); ++i) {
                cout << nombre_clase(clases_predichas[i]);
                if (i < clases_predichas.size() - 1) cout << ", ";
            }
        }
        cout << endl;
        
        cout << "Clases correctas: ";
        for (size_t i = 0; i < clases_reales.size(); ++i) {
            cout << nombre_clase(clases_reales[i]);
            if (i < clases_reales.size() - 1) cout << ", ";
        }
        cout << endl;
        
        // Verificar si la predicción es correcta
        bool correcto = (clases_predichas == clases_reales);
        total += clases_reales.size();
        
        for (int clase : clases_reales) {
            if (find(clases_predichas.begin(), clases_predichas.end(), clase) 
                != clases_predichas.end()) {
                correctos++;
            }
        }
        
        cout << (correcto ? "✓ CORRECTO" : "✗ INCORRECTO") << "\n";
        cout << "--------------------------------------------\n\n";
    }
    
    // 5. MOSTRAR ESTADÍSTICAS FINALES
    double precision = (total > 0) ? (100.0 * correctos / total) : 0.0;
    cout << "============================================\n";
    cout << "              ESTADÍSTICAS\n";
    cout << "============================================\n";
    cout << "Clasificaciones correctas: " << correctos << "/" << total << endl;
    cout << "Precisión: " << fixed << setprecision(2) << precision << "%\n";
    cout << "============================================\n";
    
    return 0;
}