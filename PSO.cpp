#include <bits/stdc++.h>

#define b1 0.7
#define b2 1.2
#define a 0.8
#define fitnessFunction(x, y) (x * x + y * y + (25*(sin(x) + sin(y))))

using namespace std;

struct Particula {
    float x;
    float y;
    float vx;
    float vy;
    float pbest_x, pbest_y;
    float pbest_fitness;
    float fitness;
    float vx_at_pbest;  
    float vy_at_pbest;  
};

struct GBestInfo {
    float x;
    float y;
    float fitness;
};

float randomNumber(float max, float min) {
    return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (max - min)));
}

ostream& operator<<(ostream& os, const Particula& p) {
    os << "Posicion: (" << p.x << ", " << p.y << ") "
       << "Velocidad: (" << p.vx << ", " << p.vy << ") "
       << "pbest: (" << p.pbest_x << ", " << p.pbest_y << "), "
       << "pbest_fitness: " << p.pbest_fitness << ", "
       << "Fitness actual: " << p.fitness;

    return os;
}

int main() {
    int particles = 20;
    int iteraciones = 2;
    srand(time(NULL));
    int i, j;
    vector<Particula> particlesArray(particles);
    map<int, GBestInfo> gbestHistory; //Mapa para guardar el mejor global de cada iteración, guarda el fitness y la posición
    // Inicialización de variables globales
    float gbest_x = 0.0, gbest_y = 0.0, r1 = 0.0, r2 = 0.0;
    float lastVx = 0.0, lastVy = 0.0;
    float gbest_fitness = numeric_limits<float>::max();

    // Inicialización de partículas
    for(j = 0; j < particles; j++) {
        // Inicializar posiciones
        particlesArray[j].x = randomNumber(5.0, -5.0);
        particlesArray[j].y = randomNumber(5.0, -5.0);
        
        // Inicializar velocidades
        particlesArray[j].vx = 0.0;
        particlesArray[j].vy = 0.0;
        
        // Calcular fitness inicial
        particlesArray[j].fitness = fitnessFunction(particlesArray[j].x, particlesArray[j].y);
        
        // Inicializar pbest con valores actuales
        particlesArray[j].pbest_x = particlesArray[j].x;
        particlesArray[j].pbest_y = particlesArray[j].y;
        particlesArray[j].pbest_fitness = particlesArray[j].fitness;
        
        // Actualizar gbest inicial si es necesario
        if (particlesArray[j].fitness < gbest_fitness) {
            gbest_x = particlesArray[j].x;
            gbest_y = particlesArray[j].y;
            gbest_fitness = particlesArray[j].fitness;
        }
    }

    for(i = 0; i < iteraciones; i++) {
        cout << "\nIteracion " << i+1 << endl;
        
        float iterationBestFitness = numeric_limits<float>::max();
        GBestInfo iterationBest;

        // Primer paso: Actualizar posiciones y calcular nuevos fitness
        for (j = 0; j < particles; j++) {
            if (i > 0) {  // No actualizamos posiciones en la primera iteración
                particlesArray[j].x += particlesArray[j].vx;
                particlesArray[j].y += particlesArray[j].vy;
            }
            
            // Calcular nuevo fitness
            particlesArray[j].fitness = fitnessFunction(particlesArray[j].x, particlesArray[j].y);
            
            // Actualizar pbest si es necesario
            if (particlesArray[j].fitness < particlesArray[j].pbest_fitness) {
                particlesArray[j].pbest_x = particlesArray[j].x;
                particlesArray[j].pbest_y = particlesArray[j].y;
                particlesArray[j].pbest_fitness = particlesArray[j].fitness;
                particlesArray[j].vx_at_pbest = particlesArray[j].vx;  // Guardamos la posición donde se encontró
                particlesArray[j].vy_at_pbest = particlesArray[j].vy;
    }
            
            // Actualizar gbest si es necesario
            if (particlesArray[j].fitness < gbest_fitness) {
                gbest_x = particlesArray[j].x;
                gbest_y = particlesArray[j].y;
                gbest_fitness = particlesArray[j].fitness;
            }
            
            // Actualizar mejor de la iteración
            if (particlesArray[j].fitness < iterationBestFitness) {
                iterationBestFitness = particlesArray[j].fitness;
                iterationBest = {particlesArray[j].x, particlesArray[j].y, particlesArray[j].fitness};
            }
        }
        
        // Segundo paso: Actualizar velocidades usando gbest y pbest actualizados
        for (j = 0; j < particles; j++) {
            r1 = randomNumber(1.0, 0.0);
            r2 = randomNumber(1.0, 0.0);
            
            // Guardar velocidad anterior para el factor de inercia
            lastVx = particlesArray[j].vx;
            lastVy = particlesArray[j].vy;
            
            // Actualizar velocidades usando los valores actualizados de pbest y gbest
            particlesArray[j].vx = (a * lastVx) +
                                 (b1 * r1 * (particlesArray[j].pbest_x - particlesArray[j].x)) +
                                 (b2 * r2 * (gbest_x - particlesArray[j].x));
            
            particlesArray[j].vy = (a * lastVy) +
                                 (b1 * r1 * (particlesArray[j].pbest_y - particlesArray[j].y)) +
                                 (b2 * r2 * (gbest_y - particlesArray[j].y));
            
            cout << "Particula " << j + 1 << ": " << particlesArray[j] << endl;
        }

        // Guardar el mejor de la iteración en el historial
        gbestHistory[i] = iterationBest;
        
        cout << "\nMejor de la iteracion " << i+1 << ":"
             << "\nPosicion: (" << iterationBest.x << ", " << iterationBest.y << ")"
             << "\nFitness: " << iterationBest.fitness << endl;
    }

    cout << "\nHistorial de mejores globales por iteracion:" << endl;
for (const auto& it : gbestHistory) {
    cout << "Iteracion " << it.first + 1 
         << ": Posicion (" << it.second.x << ", " << it.second.y 
         << "), Fitness: " << it.second.fitness << endl;
}

    cout << "\nMejor global final:" 
         << "\nPosicion: (" << gbest_x << ", " << gbest_y << ")"
         << "\nFitness: " << gbest_fitness << endl;
if (particlesArray[j].fitness < particlesArray[j].pbest_fitness) {
    particlesArray[j].pbest_x = particlesArray[j].x;
    particlesArray[j].pbest_y = particlesArray[j].y;
    particlesArray[j].pbest_fitness = particlesArray[j].fitness;
    particlesArray[j].vx_at_pbest = particlesArray[j].vx;  // Guardamos la posición donde se encontró
    particlesArray[j].vy_at_pbest = particlesArray[j].vy;
}
cout <<"\n"<< "ENJAMBRE:" << endl;
// Al final, para evaluar:
for(j = 0; j < particles; j++) {
    float new_x = particlesArray[j].pbest_x + particlesArray[j].vx_at_pbest;
    float new_y = particlesArray[j].pbest_y + particlesArray[j].vy_at_pbest;
    float new_fitness = fitnessFunction(new_x, new_y);
    
    cout << "Particula " << j + 1 << ":"
         << "\nNueva posicion: (" << new_x << ", " << new_y << ")"
         << "\nNuevo fitness: " << new_fitness << endl;
}
    return 0;
}