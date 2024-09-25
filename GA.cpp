#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <ctime>

// Estructura para representar un producto
struct Producto {
    std::string nombre;
    double peso;
    int costo;
};

// Constantes del problema
const int CAPACIDAD_MOCHILA = 30;
const int TAMANO_POBLACION = 10;
const int NUM_GENERACIONES = 10;
const double PROB_CRUZA = 0.85;
const double PROB_MUTACION = 0.1;
const int MAX_ITEMS = 10;

// Vector de productos
std::vector<Producto> productos = {
    {"Decoy Detonators", 4, 10},
    {"Love Potion", 2, 8},
    {"Extendable Ears", 5, 12},
    {"Skiving Snackbox", 5, 6},
    {"Fever Fudge", 2, 3},
    {"Puking Pastilles", 1.5, 2},
    {"Nosebleed Nougat", 1, 2}
};

// Función para generar un número aleatorio entre 0 y 1
double random_double() {
    return static_cast<double>(rand()) / RAND_MAX;
}

// Función para generar un número entero aleatorio entre min y max
int random_int(int min, int max) {
    return min + rand() % (max - min + 1);
}

// Función para calcular el fitness de un individuo
double calcular_fitness(const std::vector<int>& individuo) {
    double peso_total = 0;
    int valor_total = 0;
    int love_potions = 0;
    int skiving_snackboxes = 0;

    for (size_t i = 0; i < individuo.size(); ++i) {
        peso_total += productos[i].peso * individuo[i];
        valor_total += productos[i].costo * individuo[i];

        if (productos[i].nombre == "Love Potion") {
            love_potions = individuo[i];
        }
        if (productos[i].nombre == "Skiving Snackbox") {
            skiving_snackboxes = individuo[i];
        }
    }

    // Penalización si no se cumplen las restricciones
    if (peso_total > CAPACIDAD_MOCHILA || love_potions < 3 || skiving_snackboxes < 2) {
        return 0;
    }

    return valor_total;
}

// Función para generar la población inicial
std::vector<std::vector<int>> generar_poblacion_inicial() {
    std::vector<std::vector<int>> poblacion;
    for (int i = 0; i < TAMANO_POBLACION; ++i) {
        std::vector<int> individuo;
        for (size_t j = 0; j < productos.size(); ++j) {
            individuo.push_back(random_int(0, MAX_ITEMS));
        }
        poblacion.push_back(individuo);
    }
    return poblacion;
}

// Función para seleccionar un padre usando el método de la ruleta
int seleccion_ruleta(const std::vector<double>& fitness_valores) {
    double suma_fitness = 0;
    for (double fitness : fitness_valores) {
        suma_fitness += fitness;
    }

    double valor_ruleta = random_double() * suma_fitness;
    double suma_acumulada = 0;

    for (size_t i = 0; i < fitness_valores.size(); ++i) {
        suma_acumulada += fitness_valores[i];
        if (suma_acumulada >= valor_ruleta) {
            return i;
        }
    }

    return fitness_valores.size() - 1;
}

// Función para realizar el cruce uniforme
std::vector<int> cruce_uniforme(const std::vector<int>& padre1, const std::vector<int>& padre2) {
    std::vector<int> hijo(padre1.size());
    for (size_t i = 0; i < padre1.size(); ++i) {
        if (random_double() <= 0.5) {
            hijo[i] = padre1[i];
        } else {
            hijo[i] = padre2[i];
        }
    }
    return hijo;
}

// Función para realizar la mutación
void mutacion(std::vector<int>& individuo) {
    for (int& gen : individuo) {
        if (random_double() < PROB_MUTACION) {
            gen = random_int(0, MAX_ITEMS);
        }
    }
}

// Función principal del algoritmo genético
std::vector<int> algoritmo_genetico() {
    std::vector<std::vector<int>> poblacion = generar_poblacion_inicial();
    std::vector<int> mejor_solucion;
    double mejor_fitness = 0;

    for (int generacion = 0; generacion < NUM_GENERACIONES; ++generacion) {
        std::vector<double> fitness_valores;
        for (const auto& individuo : poblacion) {
            double fitness = calcular_fitness(individuo);
            fitness_valores.push_back(fitness);

            if (fitness > mejor_fitness) {
                mejor_fitness = fitness;
                mejor_solucion = individuo;
            }
        }

        std::vector<std::vector<int>> nueva_poblacion;

        while (nueva_poblacion.size() < TAMANO_POBLACION) {
            int indice_padre1 = seleccion_ruleta(fitness_valores);
            int indice_padre2 = seleccion_ruleta(fitness_valores);

            std::vector<int> hijo = poblacion[indice_padre1];

            if (random_double() < PROB_CRUZA) {
                hijo = cruce_uniforme(poblacion[indice_padre1], poblacion[indice_padre2]);
            }

            mutacion(hijo);
            nueva_poblacion.push_back(hijo);
        }

        poblacion = nueva_poblacion;
    }

    return mejor_solucion;
}

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    std::vector<int> mejor_solucion = algoritmo_genetico();
    double mejor_fitness = calcular_fitness(mejor_solucion);

    std::cout << "Mejor solución encontrada:\n";
    for (size_t i = 0; i < mejor_solucion.size(); ++i) {
        std::cout << productos[i].nombre << ": " << mejor_solucion[i] << " unidades\n";
    }
    std::cout << "Fitness: " << mejor_fitness << std::endl;

    return 0;
}