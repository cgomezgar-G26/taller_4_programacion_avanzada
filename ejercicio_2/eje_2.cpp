#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <string>
#include <chrono>
#include <thread>
#include <windows.h>
using namespace std;

// ==================== CONSTANTES ====================
const double TAU_S = 0.004;      // Tiempo de muestreo (250 Hz)
const double L = 1.8;            // Constante de Lipschitz
const double LAMBDA_0 = 1.3;
const double LAMBDA_1 = 1.85;
const double LAMBDA_2 = 2.79;
const double LAMBDA_3 = 6.48;

// ==================== FUNCIONES AUXILIARES ====================
double signo(double x) {
    if (x > 0) return 1.0;
    if (x < 0) return -1.0;
    return 0.0;
}

// ==================== CLASE: PLANTA SISO ====================
class PlantaG {
private:
    // Coeficientes de la función de transferencia
    // G(s) = (7s^2 - 28s + 21) / (s^3 + 9.8s^2 + 30.65s + 30.1)
    
    // Estados discretizados usando método Tustin (bilinear)
    vector<double> estados_x;  // Estados de la planta
    vector<double> historial_u;  // Historial de entradas
    vector<double> historial_y;  // Historial de salidas
    
    // Coeficientes discretizados (Tustin con Ts = 0.004)
    double a0, a1, a2, a3;  // Denominador
    double b0, b1, b2, b3;  // Numerador

public:
    PlantaG() {
        // Inicialización de estados
        estados_x.resize(3, 0.0);
        historial_u.resize(3, 0.0);
        historial_y.resize(3, 0.0);
        
        // Discretización usando Tustin (aproximación)
        // Estos coeficientes se calculan a partir de c2d en MATLAB/Python
        // Para simplificar, usamos una aproximación de la respuesta al escalón
        
        // Coeficientes aproximados para Ts = 0.004
        a0 = 1.0;
        a1 = -2.88232;
        a2 = 2.76606;
        a3 = -0.88371;
        
        b0 = 0.00002334;
        b1 = -0.00004668;
        b2 = 0.00002334;
        b3 = 0.0;
    }
    
    double actualizar(double entrada_u) {
        // Ecuación en diferencias de 3er orden
        // y[k] = -a1*y[k-1] - a2*y[k-2] - a3*y[k-3] + b0*u[k] + b1*u[k-1] + b2*u[k-2]
        
        double salida_y = -a1 * historial_y[0] - a2 * historial_y[1] - a3 * historial_y[2]
                          + b0 * entrada_u + b1 * historial_u[0] + b2 * historial_u[1];
        
        // Actualizar historiales
        historial_y[2] = historial_y[1];
        historial_y[1] = historial_y[0];
        historial_y[0] = salida_y;
        
        historial_u[2] = historial_u[1];
        historial_u[1] = historial_u[0];
        historial_u[0] = entrada_u;
        
        return salida_y;
    }
    
    void reiniciar() {
        fill(estados_x.begin(), estados_x.end(), 0.0);
        fill(historial_u.begin(), historial_u.end(), 0.0);
        fill(historial_y.begin(), historial_y.end(), 0.0);
    }
};

// ==================== CLASE: DERIVADOR DE ORDEN SUPERIOR ====================
class DerivadorSuperior {
private:
    double z0, z1, z2, z3;  // Estados del derivador
    
    double phi(int i, double error) {
        // φi(e) = -λ[i] * L^(i+1) * |e|^((n-i)/(n+1)) * sign(e)
        // donde n = 3 (orden del derivador)
        double n = 3.0;
        double lambda;
        
        switch(i) {
            case 0: lambda = LAMBDA_0; break;
            case 1: lambda = LAMBDA_1; break;
            case 2: lambda = LAMBDA_2; break;
            case 3: lambda = LAMBDA_3; break;
            default: lambda = 1.0;
        }
        
        double exponente = (n - i) / (n + 1.0);
        double potencia_L = pow(L, i + 1);
        
        return -lambda * potencia_L * pow(abs(error), exponente) * signo(error);
    }

public:
    DerivadorSuperior() : z0(0), z1(0), z2(0), z3(0) {}
    
    void actualizar(double referencia_f) {
        double error = z0 - referencia_f;
        
        // Calcular phi para cada orden
        double phi0 = phi(0, error);
        double phi1 = phi(1, error);
        double phi2 = phi(2, error);
        double phi3 = phi(3, error);
        
        // Actualizar estados según ecuaciones (2)
        double z0_nuevo = z0 + TAU_S * phi0 + TAU_S * z1 
                          + (TAU_S * TAU_S / 2.0) * z2 
                          + (TAU_S * TAU_S * TAU_S / 6.0) * z3;
        
        double z1_nuevo = z1 + TAU_S * phi1 + TAU_S * z2 
                          + (TAU_S * TAU_S / 2.0) * z3;
        
        double z2_nuevo = z2 + TAU_S * phi2 + TAU_S * z3;
        
        double z3_nuevo = z3 + TAU_S * phi3;
        
        z0 = z0_nuevo;
        z1 = z1_nuevo;
        z2 = z2_nuevo;
        z3 = z3_nuevo;
    }
    
    double obtenerZ0() const { return z0; }
    double obtenerZ1() const { return z1; }
    double obtenerZ2() const { return z2; }
    double obtenerZ3() const { return z3; }
    
    void reiniciar() {
        z0 = z1 = z2 = z3 = 0.0;
    }
};

// ==================== GENERADOR DE SEÑALES ====================
class GeneradorSenal {
public:
    enum TipoSenal { ESCALON, RAMPA, SENOIDAL };
    
    static double generar(TipoSenal tipo, double tiempo_t, double amplitud = 1.0, double frecuencia = 0.5) {
        switch(tipo) {
            case ESCALON:
                return amplitud;
            
            case RAMPA:
                return amplitud * tiempo_t;
            
            case SENOIDAL:
                return amplitud * sin(2.0 * M_PI * frecuencia * tiempo_t);
            
            default:
                return 0.0;
        }
    }
};

// ==================== SIMULACIÓN HIL ====================
class SimulacionHIL {
private:
    PlantaG planta;
    DerivadorSuperior derivador;
    ofstream archivo_datos;
    
    string nombre_archivo;
    double tiempo_actual;
    int numero_muestras;

public:
    SimulacionHIL(const string& nombre = "datos_hil.txt") 
        : nombre_archivo(nombre), tiempo_actual(0.0), numero_muestras(0) {
    }
    
    void iniciar() {
        archivo_datos.open(nombre_archivo);
        if (!archivo_datos.is_open()) {
            cerr << "Error al abrir el archivo " << nombre_archivo << endl;
            return;
        }
        
        // Escribir encabezado
        archivo_datos << fixed << setprecision(6);
        archivo_datos << "Tiempo,Referencia,Entrada_Planta,Salida_Planta,z0,z1,z2,z3\n";
        
        planta.reiniciar();
        derivador.reiniciar();
        tiempo_actual = 0.0;
        numero_muestras = 0;
    }
    
    void simular(GeneradorSenal::TipoSenal tipo, double duracion, 
                 double amplitud = 1.0, double frecuencia = 0.5) {
        
        cout << "\n========================================\n";
        cout << "   SIMULACIÓN HARDWARE IN THE LOOP\n";
        cout << "========================================\n\n";
        
        string nombre_senal;
        switch(tipo) {
            case GeneradorSenal::ESCALON: nombre_senal = "Escalón"; break;
            case GeneradorSenal::RAMPA: nombre_senal = "Rampa"; break;
            case GeneradorSenal::SENOIDAL: nombre_senal = "Senoidal"; break;
        }
        
        cout << "Tipo de señal: " << nombre_senal << endl;
        cout << "Amplitud: " << amplitud << endl;
        if (tipo == GeneradorSenal::SENOIDAL) {
            cout << "Frecuencia: " << frecuencia << " Hz" << endl;
        }
        cout << "Duración: " << duracion << " segundos" << endl;
        cout << "Frecuencia de muestreo: 250 Hz" << endl;
        cout << "\nSimulando";
        
        int total_muestras = static_cast<int>(duracion / TAU_S);
        auto inicio = chrono::high_resolution_clock::now();
        
        for (int k = 0; k < total_muestras; ++k) {
            // Generar señal de referencia
            double senal_referencia = GeneradorSenal::generar(tipo, tiempo_actual, amplitud, frecuencia);
            
            // Actualizar derivador con la referencia
            derivador.actualizar(senal_referencia);
            
            // La entrada a la planta puede ser la referencia o una señal de control
            // Para este ejemplo, usamos la referencia directamente
            double entrada_planta = senal_referencia;
            
            // Actualizar planta
            double salida_planta = planta.actualizar(entrada_planta);
            
            // Guardar datos
            archivo_datos << tiempo_actual << ","
                         << senal_referencia << ","
                         << entrada_planta << ","
                         << salida_planta << ","
                         << derivador.obtenerZ0() << ","
                         << derivador.obtenerZ1() << ","
                         << derivador.obtenerZ2() << ","
                         << derivador.obtenerZ3() << "\n";
            
            // Avanzar tiempo
            tiempo_actual += TAU_S;
            numero_muestras++;
            
            // Mostrar progreso cada 10%
            if (k % (total_muestras / 10) == 0) {
                cout << ".";
                cout.flush();
            }
            
            // Simulación en tiempo real (opcional, comentar para simulación rápida)
            // this_thread::sleep_for(chrono::microseconds(4000));
        }
        
        auto fin = chrono::high_resolution_clock::now();
        auto duracion_sim = chrono::duration_cast<chrono::milliseconds>(fin - inicio);
        
        cout << " ¡Completo!\n\n";
        cout << "Muestras procesadas: " << numero_muestras << endl;
        cout << "Tiempo de simulación: " << duracion_sim.count() / 1000.0 << " segundos" << endl;
        cout << "Datos guardados en: " << nombre_archivo << endl;
    }
    
    void finalizar() {
        if (archivo_datos.is_open()) {
            archivo_datos.close();
        }
    }
    
    ~SimulacionHIL() {
        finalizar();
    }
};

// ==================== MENÚ PRINCIPAL ====================
void mostrarMenu() {
    cout << "\n========================================\n";
    cout << "   SIMULADOR HIL - PLANTA SISO\n";
    cout << "========================================\n\n";
    cout << "1. Simular con señal Escalón\n";
    cout << "2. Simular con señal Rampa\n";
    cout << "3. Simular con señal Senoidal\n";
    cout << "4. Simular múltiples señales\n";
    cout << "5. Salir\n\n";
    cout << "Seleccione una opción: ";
}

int main() {

    SetConsoleOutputCP(CP_UTF8);

    int opcion;
    double duracion, amplitud, frecuencia;
    
    do {
        mostrarMenu();
        cin >> opcion;
        
        if (opcion >= 1 && opcion <= 4) {
            SimulacionHIL simulador;
            simulador.iniciar();
            
            switch(opcion) {
                case 1:
                    cout << "\nIngrese amplitud del escalón: ";
                    cin >> amplitud;
                    cout << "Ingrese duración (segundos): ";
                    cin >> duracion;
                    simulador.simular(GeneradorSenal::ESCALON, duracion, amplitud);
                    break;
                
                case 2:
                    cout << "\nIngrese pendiente de la rampa: ";
                    cin >> amplitud;
                    cout << "Ingrese duración (segundos): ";
                    cin >> duracion;
                    simulador.simular(GeneradorSenal::RAMPA, duracion, amplitud);
                    break;
                
                case 3:
                    cout << "\nIngrese amplitud: ";
                    cin >> amplitud;
                    cout << "Ingrese frecuencia (Hz): ";
                    cin >> frecuencia;
                    cout << "Ingrese duración (segundos): ";
                    cin >> duracion;
                    simulador.simular(GeneradorSenal::SENOIDAL, duracion, amplitud, frecuencia);
                    break;
                
                case 4: {
                    cout << "\nSimulación de múltiples señales (10 seg cada una)...\n";
                    
                    cout << "\n--- ESCALÓN ---\n";
                    simulador.simular(GeneradorSenal::ESCALON, 10.0, 1.0);
                    simulador.finalizar();
                    
                    SimulacionHIL sim_rampa("datos_rampa.txt");
                    sim_rampa.iniciar();
                    cout << "\n--- RAMPA ---\n";
                    sim_rampa.simular(GeneradorSenal::RAMPA, 10.0, 0.1);
                    sim_rampa.finalizar();
                    
                    SimulacionHIL sim_senoidal("datos_senoidal.txt");
                    sim_senoidal.iniciar();
                    cout << "\n--- SENOIDAL ---\n";
                    sim_senoidal.simular(GeneradorSenal::SENOIDAL, 10.0, 1.0, 0.5);
                    sim_senoidal.finalizar();
                    break;
                }
            }
            
            simulador.finalizar();
            
            cout << "\n¡Simulación completada exitosamente!\n";
            cout << "Presione Enter para continuar...";
            cin.ignore();
            cin.get();
        }
        
    } while (opcion != 5);
    
    cout << "\n¡Hasta luego!\n";
    return 0;
}