# Algoritmo Genético Paralelo para el Problema del Agente Viajero (TSP)

Proyecto desarrollado para el curso de **Programación Paralela**.

El objetivo consiste en implementar un **Algoritmo Genético** para resolver el **Problema del Agente Viajero (Traveling Salesman Problem - TSP)** utilizando **C++17** y paralelizando las etapas más costosas mediante **OpenMP**.

---

# Características

- Implementación modular en C++17.
- Resolución del Traveling Salesman Problem (TSP).
- Tournament Selection.
- Ordered Crossover (OX).
- Mutación por intercambio.
- Elitismo.
- Matriz de distancias precalculada.
- Generador aleatorio seguro para múltiples hilos (`thread_local`).
- Paralelización mediante OpenMP.
- Exportación automática de estadísticas en CSV.
- Medición del tiempo de ejecución.

---

# Tecnologías utilizadas

- C++17
- OpenMP
- MSYS2 UCRT64
- Visual Studio Code
- Git
- GitHub

---

# Estructura del proyecto

```text
include/
src/
data/
results/
informe/
```

---

# Compilación

Compilar utilizando g++ con soporte OpenMP.

```bash
g++ -std=c++17 -fopenmp -I include src/*.cpp -o genetic.exe
```

---

# Ejecución

```bash
./genetic.exe
```

---

# Resultados experimentales

Se realizaron pruebas utilizando una instancia de 100 ciudades.

| Hilos | Tiempo (ms) |
|-------:|------------:|
| 1 | 19763 |
| 2 | 11522 |
| 4 | 8447 |
| 8 | 5820 |

---

## Speedup

| Hilos | Speedup |
|-------:|---------:|
|1|1.000|
|2|1.715|
|4|2.339|
|8|3.396|

---

## Eficiencia

| Hilos | Eficiencia |
|-------:|-----------:|
|1|100%|
|2|85.77%|
|4|58.48%|
|8|42.45%|

---

# Paralelización

Las siguientes etapas fueron paralelizadas mediante OpenMP:

- Evaluación del Fitness
- Crossover
- Mutación

Directiva utilizada:

```cpp
#pragma omp parallel for schedule(static)
```

---

# Autores

Espinoza Benavente, Hector
Inga Chavez, Jorly

Curso: Programación Paralela

Universidad Nacional de Ingenieria 

2026