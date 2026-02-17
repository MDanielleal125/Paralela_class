#include <iostream>
#include <string>
#include <math.h>
#include <vector>
#include <algorithm>
#include <SFML/Graphics.hpp>

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
        this->e[0] = e0; this->e[1] = e1; this->e[2] = e2;
        this->e[3] = e3; this->e[4] = e4; this->e[5] = e5;
        this->e[6] = e6;
    }
};

int main()
{
    int N_ASTEROIDS = 0;
    OrbitalEntity* orbital_entities;
    orbital_entities = (OrbitalEntity*)malloc(sizeof(OrbitalEntity) * (9 + N_ASTEROIDS));

    orbital_entities[0] = { 0.0,0.0,0.0,        0.0,0.0,0.0,        1.989e30   };
    orbital_entities[1] = { 57.909e9,0.0,0.0,   0.0,47.36e3,0.0,    0.33011e24 };
    orbital_entities[2] = { 108.209e9,0.0,0.0,  0.0,35.02e3,0.0,    4.8675e24  };
    orbital_entities[3] = { 149.596e9,0.0,0.0,  0.0,29.78e3,0.0,    5.9724e24  };
    orbital_entities[4] = { 227.923e9,0.0,0.0,  0.0,24.07e3,0.0,    0.64171e24 };
    orbital_entities[5] = { 778.570e9,0.0,0.0,  0.0,13e3,0.0,       1898.19e24 };
    orbital_entities[6] = { 1433.529e9,0.0,0.0, 0.0,9.68e3,0.0,     568.34e24  };
    orbital_entities[7] = { 2872.463e9,0.0,0.0, 0.0,6.80e3,0.0,     86.813e24  };
    orbital_entities[8] = { 4495.060e9,0.0,0.0, 0.0,5.43e3,0.0,     102.413e24 };

    double dt    = 86400;
    double t_end = 86400.0 * 365.0 * 10;
    double BIG_G = 6.67e-11;
    double t     = 0;

    std::vector<std::vector<Vector3>> trajectories(9 + N_ASTEROIDS);

    // ── Simular primero ──
    std::cout << "Simulando 10 anios..." << std::endl;
    while (t < t_end)
    {
        for (size_t m1_idx = 0; m1_idx < 9 + N_ASTEROIDS; m1_idx++)
        {
            Vector3 a_g = {0,0,0};
            for (size_t m2_idx = 0; m2_idx < 9 + N_ASTEROIDS; m2_idx++)
            {
                if (m1_idx != m2_idx)
                {
                    Vector3 r_vector;
                    r_vector.e[0] = orbital_entities[m1_idx].e[0] - orbital_entities[m2_idx].e[0];
                    r_vector.e[1] = orbital_entities[m1_idx].e[1] - orbital_entities[m2_idx].e[1];
                    r_vector.e[2] = orbital_entities[m1_idx].e[2] - orbital_entities[m2_idx].e[2];
                    double r_mag = sqrt(r_vector.e[0]*r_vector.e[0] + r_vector.e[1]*r_vector.e[1] + r_vector.e[2]*r_vector.e[2]);
                    double acceleration = -1.0 * BIG_G * orbital_entities[m2_idx].e[6] / pow(r_mag, 2.0);
                    Vector3 r_unit = { r_vector.e[0]/r_mag, r_vector.e[1]/r_mag, r_vector.e[2]/r_mag };
                    a_g.e[0] += acceleration * r_unit.e[0];
                    a_g.e[1] += acceleration * r_unit.e[1];
                    a_g.e[2] += acceleration * r_unit.e[2];
                }
            }
            orbital_entities[m1_idx].e[3] += a_g.e[0] * dt;
            orbital_entities[m1_idx].e[4] += a_g.e[1] * dt;
            orbital_entities[m1_idx].e[5] += a_g.e[2] * dt;
        }
        for (size_t i = 0; i < 9 + N_ASTEROIDS; i++)
        {
            orbital_entities[i].e[0] += orbital_entities[i].e[3] * dt;
            orbital_entities[i].e[1] += orbital_entities[i].e[4] * dt;
            orbital_entities[i].e[2] += orbital_entities[i].e[5] * dt;
        }
        for (size_t i = 0; i < 9 + N_ASTEROIDS; i++)
        {
            Vector3 pos;
            pos.e[0] = orbital_entities[i].e[0];
            pos.e[1] = orbital_entities[i].e[1];
            pos.e[2] = orbital_entities[i].e[2];
            trajectories[i].push_back(pos);
        }
        t += dt;
    }
    std::cout << "Listo! Abriendo animacion..." << std::endl;

    // ── Configuracion SFML 3.0 ──
    const unsigned int W = 1000;
    const unsigned int H = 1000;
    sf::RenderWindow window(sf::VideoMode({W, H}), "Sistema Solar - N-Body Simulation");
    window.setFramerateLimit(60);

    double scale = 2.0e-10; // zoom out para ver todo el sistema solar
    double cx = W / 2.0;
    double cy = H / 2.0;

    std::vector<sf::Color> colores = {
        sf::Color(255, 220,  50),  // Sol      - amarillo
        sf::Color(180, 180, 180),  // Mercurio - gris
        sf::Color(220, 160,  60),  // Venus    - naranja
        sf::Color( 50, 120, 255),  // Tierra   - azul
        sf::Color(200,  80,  50),  // Marte    - rojo
        sf::Color(200, 160, 100),  // Jupiter  - beige
        sf::Color(210, 190, 120),  // Saturno  - dorado
        sf::Color(100, 200, 220),  // Urano    - celeste
        sf::Color( 50,  80, 200),  // Neptuno  - azul oscuro
    };

    std::vector<float> tamanios = { 18, 4, 5, 5, 4, 10, 9, 7, 7 };

    int total_steps = (int)trajectories[0].size();
    int frame = 0;
    int speed = 3;

    std::cout << "Controles:\n";
    std::cout << "  Flecha Arriba/Abajo -> velocidad\n";
    std::cout << "  + / -               -> zoom in / zoom out\n";
    std::cout << "  R                   -> reiniciar\n";
    std::cout << "  ESC                 -> salir\n";

    // ── Loop de animacion ──
    while (window.isOpen())
    {
        // Manejo de eventos SFML 3.0
        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto* key = event->getIf<sf::Event::KeyPressed>())
            {
                if (key->code == sf::Keyboard::Key::Up)
                    speed = std::min(speed + 1, 60);
                if (key->code == sf::Keyboard::Key::Down)
                    speed = std::max(speed - 1, 1);
                if (key->code == sf::Keyboard::Key::Equal)
                    scale *= 1.3;
                if (key->code == sf::Keyboard::Key::Hyphen)
                    scale /= 1.3;
                if (key->code == sf::Keyboard::Key::R)
                    frame = 0;
                if (key->code == sf::Keyboard::Key::Escape)
                    window.close();
            }
        }

        window.clear(sf::Color(8, 8, 20));

        // Dibujar estelas
        for (size_t p = 1; p < 9 + N_ASTEROIDS; p++)
        {
            int trail_length = 365;
            int trail_start  = std::max(0, frame - trail_length);

            for (int f = trail_start; f < frame && f < total_steps - 1; f++)
            {
                float x1 = (float)(cx + trajectories[p][f].e[0]   * scale);
                float y1 = (float)(cy - trajectories[p][f].e[1]   * scale);
                float x2 = (float)(cx + trajectories[p][f+1].e[0] * scale);
                float y2 = (float)(cy - trajectories[p][f+1].e[1] * scale);

                // Alpha desvanecido — SFML 3.0: aggregate initialization para Vertex
                float alpha = (float)(f - trail_start) / (float)(frame - trail_start + 1);
                sf::Color c = colores[p];
                c.a = static_cast<uint8_t>(alpha * 200);

                // SFML 3.0: sf::Vertex usa aggregate init {{pos}, color}
                sf::Vertex line[2] = {
                    {{x1, y1}, c},
                    {{x2, y2}, c}
                };
                window.draw(line, 2, sf::PrimitiveType::Lines);
            }
        }

        // Dibujar planetas
        if (frame < total_steps)
        {
            for (size_t p = 0; p < 9 + N_ASTEROIDS; p++)
            {
                float x = (float)(cx + trajectories[p][frame].e[0] * scale);
                float y = (float)(cy - trajectories[p][frame].e[1] * scale);

                sf::CircleShape circle(tamanios[p]);
                circle.setFillColor(colores[p]);
                circle.setPosition(sf::Vector2f(x - tamanios[p], y - tamanios[p]));
                window.draw(circle);
            }
        }

        window.display();

        frame += speed;
        if (frame >= total_steps) frame = 0;
    }

    free(orbital_entities);
    return 0;
}
