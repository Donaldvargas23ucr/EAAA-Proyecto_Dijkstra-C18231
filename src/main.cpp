#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

#include "../include/Grafo.hpp"
#include "../include/Dijkstra.hpp" // Solo se llaman a estos dos métodos ya que ellos implicitamente ya llaman a Cola de prioridad, al Vector dinámico y a lista enlazada
using namespace std;

// Cargar grafo desde archivo 
Grafo leer_grafo_desde_archivo(
    const string& filename,
    vector<sf::Vector2f>& posiciones)
{
    ifstream ifs(filename);
    Grafo g;
    posiciones.clear();
    // Verifica la existencia del archivo txt
    if (!ifs) {
        cerr << "No se pudo abrir archivo: " << filename << "\n";
        return g;
    }

    int n = 0, m = 0;
    ifs >> n >> m;
    g = Grafo(n);
    // Va leyendo el archivo y agregando las aristas
    for (int i = 0; i < m; ++i) {
        int u, v;
        double w;
        ifs >> u >> v >> w;
        g.agregar_arista(u, v, w, true);
    }

    string token;
    ifs >> token;
    if (token == "POS") {
        posiciones.resize(n);
        for (int i = 0; i < n; ++i) {
            float x, y;
            ifs >> x >> y;
            posiciones[i] = {x, y};
        }
    }

    return g;
}


// El main
int main(int argc, char** argv)
{
    cout << "Proyecto Dijkstra----Visualizador Interactivo\n";

    string infile;
    if (argc >= 2) infile = argv[1];

    vector<sf::Vector2f> posiciones;
    Grafo g;

    if (!infile.empty()) {
        g = leer_grafo_desde_archivo(infile, posiciones);
    } else {
        // Grafo por defecto para que no haya problemas de ejecución y también para hacer pruebas
        g = Grafo(6);
        g.agregar_arista(0,1,7);
        g.agregar_arista(0,2,9);
        g.agregar_arista(0,5,14);
        g.agregar_arista(1,2,10);
        g.agregar_arista(1,3,15);
        g.agregar_arista(2,3,11);
        g.agregar_arista(2,5,2);
        g.agregar_arista(3,4,6);
        g.agregar_arista(4,5,9);
    }

    int n = g.vertices();
    if (n == 0) {
        cerr << "Grafo vacío\n";
        return 1;
    }

    // Si no hay posiciones del archivo, generarlas en círculo
    if (posiciones.size() != (size_t)n) {
        posiciones.resize(n);
        float R = 220.f;
        sf::Vector2f center(400.f, 300.f);

        for (int i = 0; i < n; i++) {
            float ang = 2.f * 3.14159265f * i / n;
            posiciones[i] = center + sf::Vector2f(
                R * cos(ang),
                R * sin(ang)
            );
        }
    }

//Interacción del usuario con la interfaz del diagrama
    int inicio = -1;
    int objetivo = -1;
    bool seleccionandoInicio = true;

    vector<int> camino;
    // Ejecuciones del SFML
    sf::RenderWindow window(sf::VideoMode(800,600), // abre la ventana SFML
        "Dijkstra Interactivo");

    window.setFramerateLimit(60);

    sf::Font font;
    bool fontLoaded = font.loadFromFile("arial.ttf"); // Carga arial

    // Preparamos nodos
    vector<sf::CircleShape> nodes(n);
    for (int i=0;i<n;i++){
        nodes[i] = sf::CircleShape(18.f);
        nodes[i].setOrigin(18.f,18.f);
        nodes[i].setOutlineColor(sf::Color::Black);
        nodes[i].setOutlineThickness(2.f);
    }

    // Lista de aristas únicas
    struct DrawEdge { int u,v; double w; };
    vector<DrawEdge> edges;

    for (int u = 0; u < n; u++) {
        g.vecinos(u).recorrer([&](const Arista& a){
            if (u <= a.apunta_a) edges.push_back({u, a.apunta_a, a.peso});
        });
    }

 //Bucle principal para que la ventana funcione siempre 

    while (window.isOpen()) {
        sf::Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed)
                window.close();

            // Clicks del mouse
            if (ev.type == sf::Event::MouseButtonPressed &&
                ev.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mouse(ev.mouseButton.x, ev.mouseButton.y);

                int nodoCercano = -1;
                float best = 35.f;

                for (int i = 0; i < n; i++) {
                    float dx = posiciones[i].x - mouse.x;
                    float dy = posiciones[i].y - mouse.y;
                    float d = sqrt(dx*dx + dy*dy);

                    if (d < best) {
                        best = d;
                        nodoCercano = i;
                    }
                }

                if (nodoCercano != -1) {
                    if (seleccionandoInicio) {
                        inicio = nodoCercano;
                        objetivo = -1;
                        camino.clear();
                        seleccionandoInicio = false;
                        cout << "Inicio = " << inicio << "\n";
                    } else {
                        objetivo = nodoCercano;
                        seleccionandoInicio = true;
                        cout << "Objetivo = " << objetivo << "\n";

                        auto r = dijkstra(g, inicio, objetivo); // acá se comienza a ejecutar Dijkstra
                        camino = r.second;
                    }
                }
            }
        }

        window.clear(sf::Color::White);

        // dibuja las aristas sin camino
        for (const auto& ed : edges) {
            sf::Vertex line[] = {
                sf::Vertex(posiciones[ed.u], sf::Color::Black),
                sf::Vertex(posiciones[ed.v], sf::Color::Black)
            };
            window.draw(line, 2, sf::Lines);

            if (fontLoaded) {
                sf::Text wtxt(to_string((int)ed.w), font, 14);
                wtxt.setFillColor(sf::Color::Black);
                wtxt.setPosition(
                    (posiciones[ed.u].x + posiciones[ed.v].x)/2.f,
                    (posiciones[ed.u].y + posiciones[ed.v].y)/2.f
                );
                window.draw(wtxt);
            }
        }

        // redibuja las aristas que son las del camino en rojo
        for (int i = 1; i < (int)camino.size(); i++) {
            int u = camino[i-1], v = camino[i];
            sf::Vertex line[] = {
                sf::Vertex(posiciones[u], sf::Color::Red),
                sf::Vertex(posiciones[v], sf::Color::Red)
            };
            window.draw(line, 2, sf::Lines);
        }

        // dibuja los nodos, tanto los vacíos como el de inicio y de llegada con colores diferenciales 
        for (int i = 0; i < n; i++) {
            if (i == inicio) nodes[i].setFillColor(sf::Color(255,200,200));
            else if (i == objetivo) nodes[i].setFillColor(sf::Color(200,255,200));
            else nodes[i].setFillColor(sf::Color(200,200,200));

            nodes[i].setPosition(posiciones[i]);
            window.draw(nodes[i]);

            if (fontLoaded) {
                sf::Text t(to_string(i), font, 16);
                t.setFillColor(sf::Color::Black);
                t.setPosition(posiciones[i].x - 8.f, posiciones[i].y - 10.f);
                window.draw(t);
            }
        }

        window.display();
    }

    return 0;
}
