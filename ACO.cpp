#include <bits/stdc++.h>

#define p 0.5    
#define Q 1      
#define a 1.5     
#define b 0.8    
#define N 6      

using namespace std;

// Función para probabilidad de transición
double calcularProbabilidad(vector<vector<double>>& feromonas, vector<vector<int>>& matriz, int actual, int siguiente, vector<bool>& visitados) {
    if (visitados[siguiente]) return 0;
    // Cálculo del numerador
    double numerador = pow(feromonas[actual][siguiente], a) * pow(1.0/matriz[actual][siguiente], b);
    double denominador = 0;
    
    for (int i = 0; i < N; i++) {
        if (!visitados[i]) {
            // Cálculo del denominador
            denominador += pow(feromonas[actual][i], a) * pow(1.0/matriz[actual][i], b);
        }
    }
    return numerador / denominador;
}

int seleccionarSiguienteNodo(vector<vector<double>>& feromonas, vector<vector<int>>& matriz, int actual, vector<bool>& visitados){
    vector<double> probabilidades(N);
    
    // Calcular probabilidades para cada nodo no visitado
    for (int i = 0; i < N; i++) {
        probabilidades[i] = calcularProbabilidad(feromonas, matriz, actual, i, visitados);
    }
    
    // Aquí se hace ruleta
    double r = (double)rand() / RAND_MAX;
    double suma = 0;
    
    for (int i = 0; i < N; i++) {
        suma += probabilidades[i];
        if (r <= suma && !visitados[i]) {
            return i;
        }
    }
    
    // Si no se encuentra algún nodo, retornar el primer nodo no visitado
    for (int i = 0; i < N; i++) {
        if (!visitados[i]) return i;
    }
    
    return -1; // En caso de que todos los nodos estén visitados
}

int calcularLongitudRuta(vector<int>& ruta, vector<vector<int>>& matriz) {
    int longitud = 0;
    for (int i = 0; i < N-1; i++) {
        // Ida
        longitud += matriz[ruta[i]][ruta[i+1]];
    }
    // Vuelta
    longitud += matriz[ruta[N-1]][ruta[0]];
    return longitud;
}

int main() {
    int iteraciones = 50;
    srand(time(NULL));
    
    vector<vector<int>> matriz = {
        {0, 6, 9, 17, 13, 21},
        {6, 0, 19, 21, 12, 18},
        {9, 19, 0, 20, 23, 11},
        {17, 21, 20, 0, 15, 10},
        {13, 12, 23, 15, 0, 21},
        {21, 18, 11, 10, 21, 0}
    };
    
    // Matriz de feromonas 
    vector<vector<double>> feromonas(N, vector<double>(N, 0.1));

    // Matriz de la mejor ruta
    vector<int> mejorRuta;
    int mejorLongitud = INT_MAX;

    for (int iter = 0; iter < iteraciones; iter++) {
        vector<vector<int>> rutasHormigas(N, vector<int>()); // Declaramos en cada iteración para evitar la mezcla de datos
        vector<int> longitudesRutas(N);
        
        for (int hormiga = 0; hormiga < N; hormiga++) {
            vector<bool> visitados(N, false);
            vector<int> ruta;
            
            int nodoActual = hormiga; // Posición de partida por hormiga
            ruta.push_back(nodoActual); // Registro de ruta de la hormiga
            visitados[nodoActual] = true;

            // Construir el resto de la ruta
            for (int i = 1; i < N; i++) {
                nodoActual = seleccionarSiguienteNodo(feromonas, matriz, nodoActual, visitados);
                ruta.push_back(nodoActual);
                visitados[nodoActual] = true;
            }
            
            rutasHormigas[hormiga] = ruta;
            longitudesRutas[hormiga] = calcularLongitudRuta(ruta, matriz); // Sumatoria de rutas
            
            // Actualizar mejor solución si es necesario
            if (longitudesRutas[hormiga] < mejorLongitud) {
                mejorLongitud = longitudesRutas[hormiga];
                mejorRuta = ruta;
            }
        }
        // Evaporación de feromonas
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                feromonas[i][j] *= (1 - p);
            }
        }
        
        for (int k = 0; k < N; k++) {
            double cantFeromonas = Q / longitudesRutas[k];
            for (int i = 0; i < N-1; i++) {
                int ciudad1 = rutasHormigas[k][i];
                int ciudad2 = rutasHormigas[k][i+1];
                feromonas[ciudad1][ciudad2] += cantFeromonas;
                feromonas[ciudad2][ciudad1] += cantFeromonas;
            }
            // Asignar feromonas a los caminos entre la última y la primera ciudad
            int primera = rutasHormigas[k][0];
            int ultima = rutasHormigas[k][N-1];
            feromonas[ultima][primera] += cantFeromonas;
            feromonas[primera][ultima] += cantFeromonas;
        }
    }


    ofstream archivo("optimumSolve.txt");
    archivo << "Mejor ruta encontrada: ";
    for (int ciudad : mejorRuta) {
        archivo << ciudad + 1 << " "; 
    }
    archivo << mejorRuta[0] + 1 << endl; // Regresar a la ciudad inicial
    archivo << "Longitud de la mejor ruta: " << mejorLongitud << endl;
    
    archivo.close();
    cout << "Resultado guardado en optimumSolve.txt" << endl;
}
