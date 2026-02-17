#include <iostream>
#include <string>
#include <math.h>
#include <vector>
#include "gplot++.h"

struct Vector3
{
    double e[3] = { 0 };

    Vector3() {}
    ~Vector3() {}

    inline Vector3(double e0, double e1, double e2)
    {
        this->e[0] = e0;
        this->e[1] = e1;
        this->e[2] = e2;
    }
};

struct OrbitalEntity
{
    double e[7] = { 0 };

    OrbitalEntity() {}
    ~OrbitalEntity() {}

    inline OrbitalEntity(double e0, double e1, double e2, double e3, double e4, double e5, double e6)
    {
        this->e[0] = e0;
        this->e[1] = e1;
        this->e[2] = e2;
        this->e[3] = e3;
        this->e[4] = e4;
        this->e[5] = e5;
        this->e[6] = e6;
    }
};

int main()
{
    // Se establecen las condiciones iniciales de la simulación, las "Semillas"
    OrbitalEntity* orbital_entities;

    // ── CAMBIO 1: un vector de trayectorias por planeta (9 planetas) ──
    int N_ASTEROIDS = 0;
    std::vector<std::vector<Vector3>> trajectories(9 + N_ASTEROIDS);

    // N_ASTEROIDS es para agregar más cuerpos.
    orbital_entities = (OrbitalEntity*)malloc(sizeof(OrbitalEntity) * (9 + N_ASTEROIDS));

    orbital_entities[0] = { 0.0,0.0,0.0,        0.0,0.0,0.0,        1.989e30 }; // a star similar to the sun
    orbital_entities[1] = { 57.909e9,0.0,0.0,   0.0,47.36e3,0.0,    0.33011e24 }; // a planet similar to mercury
    orbital_entities[2] = { 108.209e9,0.0,0.0,  0.0,35.02e3,0.0,    4.8675e24 }; // a planet similar to venus
    orbital_entities[3] = { 149.596e9,0.0,0.0,  0.0,29.78e3,0.0,    5.9724e24 }; // a planet similar to earth
    orbital_entities[4] = { 227.923e9,0.0,0.0,  0.0,24.07e3,0.0,    0.64171e24 }; // a planet similar to mars
    orbital_entities[5] = { 778.570e9,0.0,0.0,  0.0,13e3,0.0,       1898.19e24 }; // a planet similar to jupiter
    orbital_entities[6] = { 1433.529e9,0.0,0.0, 0.0,9.68e3,0.0,     568.34e24 }; // a planet similar to saturn
    orbital_entities[7] = { 2872.463e9,0.0,0.0, 0.0,6.80e3,0.0,     86.813e24 }; // a planet similar to uranus
    orbital_entities[8] = { 4495.060e9,0.0,0.0, 0.0,5.43e3,0.0,     102.413e24 }; // a planet similar to neptune

    // más condiciones iniciales, tiempo inicial, final, paso de tiempo, etc
    double t_0 = 0;
    double t = t_0;

    std::cout << "Estado inicial:" << std::endl;
    for (size_t entity_idx = 0; entity_idx < 9 + N_ASTEROIDS; entity_idx++)
    {
        std::cout << "Particula " << entity_idx << ", tiempo " << t << 
        " : (" << orbital_entities[entity_idx].e[0] << "," << orbital_entities[entity_idx].e[1] << "," << orbital_entities[entity_idx].e[2] << ")" << '\n';
    }

    // más condiciones iniciales, tiempo inicial, final, paso de tiempo, etc
    double dt = 86400;
    double t_end = 86400 * 365 * 10; // approximately a decade in seconds
    double BIG_G = 6.67e-11; // gravitational constant

    // Propagación de la simulación
    while (t < t_end) // se repite hasta que se alcanza el tiempo máximo, que es una década.
    {
        for (size_t m1_idx = 0; m1_idx < 9 + N_ASTEROIDS; m1_idx++)  // se toma una partícula de referencia, m1_idx es el índice de esa partícula
        {
            Vector3 a_g = { 0,0,0 }; // vector aceleración para una partícula

            for (size_t m2_idx = 0; m2_idx < 9 + N_ASTEROIDS; m2_idx++)
            /* se suman las fuerzas para cada partícula, m2_idx representa la partícula que afecta a la de referencia para cada ocasión */
            {
                if (m1_idx != m2_idx) // la partícula no se afecta a ella misma.
                {
                    // vector posición generado. La distancia entre las dos partículas
                    Vector3 r_vector;
                    // acá se calcula el vector diferencia de posición.
                    r_vector.e[0] = orbital_entities[m1_idx].e[0] - orbital_entities[m2_idx].e[0];
                    r_vector.e[1] = orbital_entities[m1_idx].e[1] - orbital_entities[m2_idx].e[1];
                    r_vector.e[2] = orbital_entities[m1_idx].e[2] - orbital_entities[m2_idx].e[2];
                    
                    // se calcula la magnitud del vector r.
                    double r_mag = sqrt(r_vector.e[0] * r_vector.e[0] + r_vector.e[1] * r_vector.e[1] + r_vector.e[2] * r_vector.e[2]);
                    // se calcula la aceleración generada por la fuerza entre el par de partículas
                    double acceleration = -1.0 * BIG_G * (orbital_entities[m2_idx].e[6]) / pow(r_mag, 2.0);
                    // acá básicamente se calculó la magnitud, ahora se calcula el conjunto de vectores unitarios de la dirección
                    Vector3 r_unit_vector = { r_vector.e[0] / r_mag,r_vector.e[1] / r_mag,r_vector.e[2] / r_mag };
                    // aceleración por vectores unitarios.
                    a_g.e[0] += acceleration * r_unit_vector.e[0];
                    a_g.e[1] += acceleration * r_unit_vector.e[1];
                    a_g.e[2] += acceleration * r_unit_vector.e[2];
                }
            }

            // se acumulan las velocidades: vxi = vxi-1 + axi * dt
            // todas las aceleraciones generadas sobre m1_idx
            orbital_entities[m1_idx].e[3] += a_g.e[0] * dt;
            orbital_entities[m1_idx].e[4] += a_g.e[1] * dt;
            orbital_entities[m1_idx].e[5] += a_g.e[2] * dt;
        // acá finalizan las operaciones para el conjunto de partículas sobre una partícula m1_idx
        }

    // acá se calculan las posiciones para todas las partículas, a partir de las velocidades generadas.
        for (size_t entity_idx = 0; entity_idx < 9 + N_ASTEROIDS; entity_idx++)
        {
            orbital_entities[entity_idx].e[0] += orbital_entities[entity_idx].e[3] * dt;
            orbital_entities[entity_idx].e[1] += orbital_entities[entity_idx].e[4] * dt;
            orbital_entities[entity_idx].e[2] += orbital_entities[entity_idx].e[5] * dt;
            if (t == 0 || t == t_end - dt) // solo imprimir las posiciones iniciales y finales y comparar
            {
                std::cout << "Particula " << entity_idx << ", tiempo " << t + dt << " : (" << orbital_entities[entity_idx].e[0] << "," << orbital_entities[entity_idx].e[1] << "," << orbital_entities[entity_idx].e[2] << ")" << '\n';
            }    
        }

        // ── CAMBIO 2: guardar posición de TODOS los planetas en cada paso ──
        // Va DESPUÉS de actualizar posiciones y ANTES de avanzar t
        for (size_t i = 0; i < 9 + N_ASTEROIDS; i++)
        {
            Vector3 pos;
            pos.e[0] = orbital_entities[i].e[0];
            pos.e[1] = orbital_entities[i].e[1];
            pos.e[2] = orbital_entities[i].e[2];
            trajectories[i].push_back(pos);
        }

        // una vez terminado esto, se avanza en el tiempo.
        t += dt;
    }

    // ── CAMBIO 3: graficar todas las trayectorias con gplot++ ──
    std::vector<std::string> nombres = {
        "Sol", "Mercurio", "Venus", "Tierra",
        "Marte", "Jupiter", "Saturno", "Urano", "Neptuno"
    };

    Gnuplot plt{};
    plt.set_title("Simulacion Sistema Solar - 10 anios");
    plt.set_xlabel("X (m)");
    plt.set_ylabel("Y (m)");

    for (size_t i = 0; i < 9 + N_ASTEROIDS; i++)
    {
        std::vector<double> x, y;
        for (auto& p : trajectories[i])
        {
            x.push_back(p.e[0]);
            y.push_back(p.e[1]);
        }
        plt.plot(x, y, nombres[i]);
    }

    plt.show();

    return 0;
}