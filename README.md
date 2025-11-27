# 🧮 Taller 4 - Programación Avanzada
## Algoritmos Avanzados y Aplicaciones en C++

Este repositorio contiene los ejercicios del Taller 4: Algoritmos Avanzados y Aplicaciones, desarrollados como parte del curso Programación Avanzada de la Universidad Nacional de Colombia – Sede La Paz, semestre 2025-2.

El propósito del taller es implementar algoritmos avanzados de búsqueda, simulación de sistemas dinámicos y redes neuronales artificiales, aplicando técnicas de inteligencia artificial, procesamiento de señales y control automático.

---

## 🎓 Curso y Docente

- **Curso:** Programación Avanzada
- **Docente:** Mauro Alonso Baquero Suárez, M. Sc. en Automatización y Control

---

## 👥 Integrantes del grupo

| Nombre completo | Rol | Universidad |
|------------------|------|-------------|
| Carlos Alberto Gómez García | Estudiante | Universidad Nacional de Colombia – Sede La Paz |
| Cristian Enrique Vásquez López | Estudiante | Universidad Nacional de Colombia – Sede La Paz |
| Juan Tomás Dangond Ovalle | Estudiante | Universidad Nacional de Colombia – Sede La Paz |

---

## 🧩 Estructura general del taller

Cada ejercicio fue implementado en una carpeta independiente con sus archivos fuente, ejecutables y archivos de datos correspondientes.

```
📁 TALLER_4_PROGRAMACION_AVANZADA/
│
├── 📁 .vscode/
│   └── settings.json
│
├── 📁 ejercicio_1/
│   ├── 📁 output/
│   │   └── eje_1.exe
│   └── 📄 eje_1.cpp
│
├── 📁 ejercicio_2/
│   ├── 📁 output/
│   │   └── eje_2.exe
│   ├── 📄 datos_hil.txt (generado)
│   ├── 📄 datos_rampa.txt (generado)
│   ├── 📄 datos_senoidal.txt (generado)
│   └── 📄 eje_2.cpp
│
├── 📁 ejercicio_3/
│   ├── 📁 output/
│   │   └── eje_3.exe
│   ├── 📄 digitos.txt (entrada requerida)
│   └── 📄 eje_3.cpp
│
└── 📄 README.md
```

---

## ⚙️ Compilación y ejecución

### 🔹 En Windows (PowerShell o CMD)

```bash
# Navegar a la carpeta del ejercicio
cd ejercicio_1

# Compilar
g++ -std=c++17 eje_1.cpp -o output/eje_1.exe

# Ejecutar
.\output\eje_1.exe
```

### 🔹 En Linux o macOS

```bash
# Navegar a la carpeta del ejercicio
cd ejercicio_1

# Compilar
g++ -std=c++17 eje_1.cpp -o output/eje_1

# Ejecutar
./output/eje_1
```

### 🔹 Compilación desde VS Code

El proyecto incluye configuración en `.vscode/` para compilación directa:

1. Abrir el archivo `.cpp` que deseas compilar
2. Presionar `F5` o usar el botón **▶ Run** en la barra superior
3. O usar el menú: `Terminal > Run Build Task` (Ctrl+Shift+B)

---

## 💡 Requisitos

- **Compilador:** g++ con soporte C++11 o superior (MinGW, MSYS2 o GCC)
- **IDE:** Visual Studio Code (opcional pero recomendado)
- **Archivos de entrada:** 
  - `digitos.txt` para el ejercicio_3 (debe estar en la carpeta ejercicio_3)
- **Sistema operativo:** Windows, Linux o macOS
- **Extensiones de VS Code recomendadas:**
  - C/C++ (Microsoft)
  - C/C++ Extension Pack
  - Code Runner (opcional, para ejecución rápida)

---

## 📘 Ejercicio 1 – Resolvedor de 8-Puzzle con Algoritmo A*

### 🎯 Descripción

Implementación del algoritmo de búsqueda informada A* para resolver el clásico rompecabezas 8-puzzle, un problema fundamental en inteligencia artificial que consiste en ordenar fichas numeradas del 1 al 8 en un tablero de 3×3 con un espacio vacío.

### 📂 Ubicación
```
ejercicio_1/
├── eje_1.cpp
└── output/eje_1.exe
```

### 📊 Metodología

El programa implementa el algoritmo A* (A-Star), un algoritmo de búsqueda informada que encuentra el camino óptimo desde un estado inicial hasta el estado objetivo utilizando una función de evaluación heurística.

#### Componentes principales:

**1. Representación del Estado:**
- El tablero se representa como una cadena de texto (string) de 9 caracteres
- Cada carácter representa un número del 0 al 8
- El '0' representa el espacio vacío
- Estado objetivo: `"123456780"`

**2. Función Heurística - Distancia Manhattan:**
```
h(n) = Σ |x_actual - x_objetivo| + |y_actual - y_objetivo|
```
La heurística calcula la suma de distancias Manhattan de cada ficha desde su posición actual hasta su posición objetivo. Esta heurística es admisible (nunca sobreestima el costo real) y consistente, lo que garantiza que A* encuentre la solución óptima.

**3. Función de Evaluación:**
```
f(n) = g(n) + h(n)
```
Donde:
- `g(n)`: costo acumulado desde el estado inicial (número de movimientos)
- `h(n)`: estimación heurística del costo hasta el objetivo
- `f(n)`: estimación del costo total del camino

**4. Algoritmo A*:**
- Utiliza una cola de prioridad (priority_queue) ordenada por f(n)
- Mantiene un registro de estados visitados con sus costos g(n)
- Explora primero los estados con menor f(n)
- Reconstruye el camino óptimo al encontrar la solución

**5. Generación de Vecinos:**
- Para cada estado, se generan movimientos válidos del espacio vacío
- Movimientos posibles: arriba, abajo, izquierda, derecha
- Solo se generan movimientos dentro de los límites del tablero 3×3

**6. Verificación de Solubilidad:**
El programa verifica si un estado inicial tiene solución mediante el conteo de inversiones:
- Una inversión ocurre cuando un número mayor aparece antes que uno menor
- El puzzle es soluble si y solo si el número de inversiones es par

### 🔧 Funcionalidades

- Entrada interactiva de 9 números (0-8 sin repetir)
- Validación automática de entrada
- Verificación de solubilidad antes de buscar
- Visualización del tablero en formato gráfico
- Muestra la secuencia completa de movimientos
- Reporta el número de pasos y nodos explorados

### 📈 Salida del Programa

```
=====================================
        RESOLVEDOR DE 8-PUZZLE
=====================================

Ingresa los 9 números (0 = espacio vacío):
Ejemplo: 5 7 2 4 1 0 3 8 6

> 5 7 2 4 1 0 3 8 6

Tablero inicial:
+---+---+---+
| 5 | 7 | 2 |
+---+---+---+
| 4 | 1 |   |
+---+---+---+
| 3 | 8 | 6 |
+---+---+---+

✓ El rompecabezas es soluble. Buscando solución...

✓ Solución encontrada en 12 movimientos (345 nodos explorados)

Paso 0:
[tablero inicial]
   ↓
Paso 1:
[tablero después del primer movimiento]
   ↓
...
```

### 🎓 Conceptos Aplicados

- **Algoritmo A***: búsqueda informada con heurística admisible
- **Heurística Manhattan**: estimación de distancia en grilla
- **Cola de prioridad**: estructura para exploración eficiente
- **Hash map**: almacenamiento de estados visitados
- **Teoría de grafos**: búsqueda en espacio de estados
- **Backtracking**: reconstrucción del camino óptimo

**Archivo principal:** `ejercicio_1/eje_1.cpp`  
**Ejecutable:** `ejercicio_1/output/eje_1.exe`

---

## 📘 Ejercicio 2 – Simulación Hardware-in-the-Loop (HIL) con Derivador de Orden Superior

### 🎯 Descripción

Implementación de un sistema de simulación Hardware-in-the-Loop que combina una planta SISO (Single Input Single Output) con un derivador de orden superior basado en modos deslizantes, para el procesamiento y análisis de señales de control.

### 📂 Ubicación
```
ejercicio_2/
├── eje_2.cpp
├── datos_hil.txt (generado)
├── datos_rampa.txt (generado)
├── datos_senoidal.txt (generado)
└── output/eje_2.exe
```

### 📊 Metodología

El programa implementa un sistema HIL que simula el comportamiento de una planta dinámica real en conjunto con un algoritmo de derivación numérica de alto orden.

#### Componentes principales:

**1. Planta SISO (Sistema Dinámico):**

La planta se modela mediante la función de transferencia continua:
```
G(s) = (7s² - 28s + 21) / (s³ + 9.8s² + 30.65s + 30.1)
```

**Discretización:**
- Método: Transformación Tustin (Bilineal)
- Frecuencia de muestreo: 250 Hz (Ts = 0.004 s)
- Ecuación en diferencias de 3er orden resultante

La planta mantiene:
- Estados internos discretizados (x₁, x₂, x₃)
- Historial de entradas u[k-1], u[k-2], u[k-3]
- Historial de salidas y[k-1], y[k-2], y[k-3]

**2. Derivador de Orden Superior:**

Implementa un observador basado en modos deslizantes de orden superior para estimar derivadas de la señal de referencia.

**Estados del derivador:**
- z₀: estimación de la señal
- z₁: estimación de la primera derivada
- z₂: estimación de la segunda derivada
- z₃: estimación de la tercera derivada

**Ecuaciones de actualización:**
```
z₀[k+1] = z₀[k] + Ts·φ₀(e) + Ts·z₁ + (Ts²/2)·z₂ + (Ts³/6)·z₃
z₁[k+1] = z₁[k] + Ts·φ₁(e) + Ts·z₂ + (Ts²/2)·z₃
z₂[k+1] = z₂[k] + Ts·φ₂(e) + Ts·z₃
z₃[k+1] = z₃[k] + Ts·φ₃(e)
```

**Función φᵢ(e):**
```
φᵢ(e) = -λᵢ · L^(i+1) · |e|^((n-i)/(n+1)) · sign(e)
```

Donde:
- e = z₀ - f (error entre estimación y referencia)
- n = 3 (orden del derivador)
- L = 1.8 (constante de Lipschitz)
- λᵢ: constantes de ajuste (λ₀=1.3, λ₁=1.85, λ₂=2.79, λ₃=6.48)

**3. Generador de Señales:**

El sistema puede generar tres tipos de señales de referencia:

**a) Señal Escalón:**
```
f(t) = A
```

**b) Señal Rampa:**
```
f(t) = A · t
```

**c) Señal Senoidal:**
```
f(t) = A · sin(2πft)
```

Donde A es la amplitud y f es la frecuencia.

**4. Ciclo de Simulación HIL:**

```
Para cada instante de muestreo k:
1. Generar señal de referencia f(t)
2. Actualizar derivador con la referencia
3. Aplicar entrada a la planta
4. Obtener salida de la planta
5. Registrar: tiempo, referencia, entrada, salida, z₀, z₁, z₂, z₃
6. Avanzar tiempo: t = t + Ts
```

### 🔧 Características del Sistema

**Parámetros de Simulación:**
- Tiempo de muestreo (Ts): 0.004 s (250 Hz)
- Constante de Lipschitz (L): 1.8
- Orden del derivador: 3
- Discretización: Método Tustin

**Modos de Operación:**
1. Simulación con señal escalón
2. Simulación con señal rampa
3. Simulación con señal senoidal
4. Simulación múltiple (combina las tres señales)

### 📁 Formato de Archivos de Salida

Los archivos generados contienen datos en formato CSV:

```csv
Tiempo,Referencia,Entrada_Planta,Salida_Planta,z0,z1,z2,z3
0.000000,1.000000,1.000000,0.000023,0.000000,0.000000,0.000000,0.000000
0.004000,1.000000,1.000000,0.000069,0.005200,0.000000,0.000000,0.000000
0.008000,1.000000,1.000000,0.000138,0.010296,0.000024,0.000000,0.000000
...
```

### 📈 Menú Interactivo

```
========================================
   SIMULADOR HIL - PLANTA SISO
========================================

1. Simular con señal Escalón
2. Simular con señal Rampa
3. Simular con señal Senoidal
4. Simular múltiples señales
5. Salir

Seleccione una opción:
```

### 🎓 Conceptos Aplicados

- **Hardware-in-the-Loop (HIL)**: simulación híbrida de sistemas
- **Sistemas SISO**: planta con una entrada y una salida
- **Discretización Tustin**: transformación bilineal s ↔ z
- **Derivadores de orden superior**: estimación robusta de derivadas
- **Modos deslizantes**: control robusto ante perturbaciones
- **Procesamiento en tiempo discreto**: ecuaciones en diferencias
- **Constante de Lipschitz**: límite de variación de la señal

**Archivo principal:** `ejercicio_2/eje_2.cpp`  
**Ejecutable:** `ejercicio_2/output/eje_2.exe`

---

## 📘 Ejercicio 3 – Red Neuronal Artificial para Clasificación de Dígitos

### 🎯 Descripción

Implementación de una red neuronal artificial feedforward con aprendizaje por retropropagación para clasificación multi-etiqueta de dígitos manuscritos (0-9) en cuatro categorías: par/impar y primo/compuesto.

### 📂 Ubicación
```
ejercicio_3/
├── eje_3.cpp
├── digitos.txt (entrada requerida)
└── output/eje_3.exe
```

### 📊 Metodología

El programa implementa una red neuronal artificial multicapa que aprende a clasificar dígitos en múltiples categorías simultáneamente mediante el algoritmo de retropropagación del error.

#### Arquitectura de la Red:

**Topología:**
```
Capa de Entrada: 35 neuronas (7 filas × 5 columnas)
        ↓
Capa Oculta: 20 neuronas (función sigmoide)
        ↓
Capa de Salida: 4 neuronas (función sigmoide)
```

**Representación de Dígitos:**
- Cada dígito se representa como una matriz de 7×5 píxeles
- Total: 35 valores binarios (0 o 1)
- Los píxeles activos forman el patrón visual del número

**Categorías de Clasificación:**
- **Clase 0 (Par)**: números 0, 2, 4, 6, 8
- **Clase 1 (Impar)**: números 1, 3, 5, 7, 9
- **Clase 2 (Primo)**: números 2, 3, 5, 7
- **Clase 3 (Compuesto)**: números 4, 6, 8, 9

Nota: Un número puede pertenecer a múltiples categorías simultáneamente.

#### Componentes del Algoritmo:

**1. Función de Activación Sigmoide:**
```
σ(x) = 1 / (1 + e^(-x))
```
Propiedades:
- Rango de salida: (0, 1)
- Diferenciable en todo su dominio
- Derivada: σ'(x) = σ(x) · (1 - σ(x))

**2. Propagación Hacia Adelante (Forward Propagation):**

**Capa Oculta:**
```
Para cada neurona j en la capa oculta:
    suma_j = sesgo_j + Σ(entrada_i × peso_ij)
    activacion_oculta_j = σ(suma_j)
```

**Capa de Salida:**
```
Para cada neurona k en la capa de salida:
    suma_k = sesgo_k + Σ(activacion_oculta_j × peso_jk)
    activacion_salida_k = σ(suma_k)
```

**3. Retropropagación del Error (Backpropagation):**

**Error en la Capa de Salida:**
```
Para cada neurona k:
    error_k = (objetivo_k - salida_k) × σ'(salida_k)
```

**Error en la Capa Oculta:**
```
Para cada neurona j:
    error_j = Σ(error_k × peso_jk) × σ'(activacion_j)
```

**Actualización de Pesos:**
```
peso_nuevo = peso_anterior + η × error × activacion_previa
sesgo_nuevo = sesgo_anterior + η × error
```

Donde η = 0.1 (tasa de aprendizaje)

**4. Entrenamiento:**

```
Para cada época (iteración completa):
    Para cada patrón de entrenamiento:
        1. Forward propagation (calcular salida)
        2. Calcular error
        3. Backward propagation (ajustar pesos)
        4. Acumular error cuadrático medio
    
    Mostrar progreso cada 500 épocas
```

**Parámetros de Entrenamiento:**
- Épocas: 5000 iteraciones
- Tasa de aprendizaje: 0.1
- Inicialización de pesos: valores aleatorios en [-0.5, 0.5]
- Función de error: Error Cuadrático Medio (MSE)

**5. Codificación One-Hot Modificada:**

Para un número que pertenece a múltiples clases, el vector objetivo tiene múltiples valores en 1:

Ejemplo para el número 2 (par y primo):
```
objetivo = [1, 0, 1, 0]
           [↑  ↑  ↑  ↑]
          par impar primo compuesto
```

**6. Predicción y Clasificación:**

Durante la predicción, se aplica un umbral (por defecto 0.5):
```
Si salida_k > 0.5:
    El dígito pertenece a la clase k
```

Esto permite clasificación multi-etiqueta (un ejemplo puede tener múltiples clases activas).

### 🔧 Proceso de Ejecución

**1. Carga de Datos:**
- Lee el archivo `digitos.txt`
- Espera 10 dígitos (del 0 al 9)
- Cada dígito: 35 valores (matriz 7×5 aplanada)

**2. Preparación:**
- Crea pares (entrada, objetivo) para cada dígito
- Determina las clases de cada número
- Genera vectores objetivo con codificación multi-etiqueta

**3. Entrenamiento:**
- Inicializa pesos aleatoriamente
- Ejecuta 5000 épocas de entrenamiento
- Muestra progreso y error cada 500 épocas

**4. Evaluación:**
- Prueba la red con todos los dígitos
- Compara predicciones con clases reales
- Calcula precisión global

### 📈 Salida del Programa

```
============================================
  RED NEURONAL - CLASIFICACIÓN DE DÍGITOS
============================================

Cargados 10 dígitos desde digitos.txt

Entrenando la red neuronal...
Época 500/5000 - Error: 2.456789
Época 1000/5000 - Error: 1.234567
...
¡Entrenamiento completado!

============================================
        RESULTADOS DE CLASIFICACIÓN
============================================

Dígito 0:
█ █ █ █ █ 
█       █ 
█       █ 
█       █ 
█       █ 
█       █ 
█ █ █ █ █ 

Salida de la red: 0.98 0.02 0.01 0.03 
Clases predichas: Par
Clases correctas: Par
✓ CORRECTO
--------------------------------------------

...

============================================
              ESTADÍSTICAS
============================================
Clasificaciones correctas: 38/40
Precisión: 95.00%
============================================
```

### 📁 Formato del Archivo digitos.txt

El archivo debe contener 10 líneas de 35 valores cada una:

```
1 1 1 1 1 1 0 0 0 1 1 0 0 0 1 ... (35 valores para el 0)
0 1 1 0 0 0 0 1 0 0 0 0 1 0 0 ... (35 valores para el 1)
...
```

### 🎓 Conceptos Aplicados

- **Redes neuronales artificiales**: modelo computacional inspirado en el cerebro
- **Perceptrón multicapa (MLP)**: arquitectura feedforward
- **Retropropagación**: algoritmo de aprendizaje supervisado
- **Función sigmoide**: activación no lineal diferenciable
- **Descenso de gradiente**: optimización iterativa
- **Clasificación multi-etiqueta**: múltiples salidas activas
- **One-hot encoding**: representación categórica
- **Aprendizaje supervisado**: entrenamiento con datos etiquetados

**Archivo principal:** `ejercicio_3/eje_3.cpp`  
**Ejecutable:** `ejercicio_3/output/eje_3.exe`  
**Archivo de entrada:** `ejercicio_3/digitos.txt` (requerido)

---

## 🧱 Metodología General del Taller

### Características Comunes

| Aspecto | Implementación |
|---------|----------------|
| **Paradigma** | Programación Orientada a Objetos |
| **Estructuras de datos** | Vectores dinámicos, colas de prioridad, hash maps |
| **Algoritmos** | Búsqueda informada, procesamiento de señales, aprendizaje automático |
| **Persistencia** | Archivos de texto (.txt, .csv) |
| **Documentación** | Comentarios explicativos en código |
| **Organización** | Carpetas independientes por ejercicio |

### Comparativa de Enfoques

| Ejercicio | Tipo de Algoritmo | Complejidad Temporal |
|-----------|-------------------|----------------------|
| 1 - 8-Puzzle | Búsqueda informada (A*) | O(b^d) con poda heurística |
| 2 - Simulación HIL | Procesamiento en tiempo discreto | O(n) lineal con tiempo |
| 3 - Red Neuronal | Aprendizaje supervisado | O(épocas × patrones × conexiones) |

---

## 🚀 Compilación Rápida (Todos los Ejercicios)

### Desde la raíz del proyecto:

```bash
# Compilar Ejercicio 1
cd ejercicio_1
g++ -std=c++17 eje_1.cpp -o output/eje_1.exe
cd ..

# Compilar Ejercicio 2
cd ejercicio_2
g++ -std=c++17 eje_2.cpp -o output/eje_2.exe
cd ..

# Compilar Ejercicio 3
cd ejercicio_3
g++ -std=c++17 eje_3.cpp -o output/eje_3.exe
cd ..
```

### Script batch para Windows (compile_all.bat):

```batch
@echo off
cd ejercicio_1
g++ -std=c++17 eje_1.cpp -o output/eje_1.exe
cd ..

cd ejercicio_2
g++ -std=c++17 eje_2.cpp -o output/eje_2.exe
cd ..

cd ejercicio_3
g++ -std=c++17 eje_3.cpp -o output/eje_3.exe
cd ..

echo Compilacion completada!
pause
```

---

## 📁 Archivos de Entrada/Salida

### Entrada Requerida:
- `ejercicio_3/digitos.txt`: patrones de dígitos 0-9 (matriz 7×5 aplanada)

### Salida Generada:
- `ejercicio_1`: Solución paso a paso en consola
- `ejercicio_2/datos_hil.txt`: datos de simulación con señal escalón
- `ejercicio_2/datos_rampa.txt`: datos de simulación con señal rampa
- `ejercicio_2/datos_senoidal.txt`: datos de simulación con señal senoidal
- `ejercicio_3`: Resultados de clasificación en consola

---

## 🔍 Notas Técnicas

### Consideraciones de Memoria:
- Uso eficiente de contenedores STL (vector, queue, unordered_map)
- Gestión automática de memoria con contenedores estándar
- Inicialización correcta de estructuras de datos
- Liberación automática al salir del ámbito

### Compatibilidad:
- Código multiplataforma (Windows, Linux, macOS)
- Configuración UTF-8 para Windows incluida
- Compilación con C++11/17 estándar
- Estructura de carpetas compatible con VS Code

### Validación de Datos:
- Verificación de archivos antes de abrir
- Validación de entrada del usuario
- Manejo de casos especiales
- Mensajes de error descriptivos

### Organización del Proyecto:
- Carpetas separadas por ejercicio
- Subcarpetas `output/` para ejecutables
- Archivos de datos en la carpeta correspondiente
- Configuración de VS Code en `.vscode/`

---

## 🛠️ Configuración de VS Code

El proyecto incluye configuración básica en `.vscode/settings.json` para:
- Configuración de compilador
- Rutas de inclusión
- Estándares de C++
- Configuración de terminal

---

*Proyecto desarrollado como parte del programa académico de Ingeniería - 2025-2*