#ifndef GRAFO_HPP
#define GRAFO_HPP

#include "vector_dinamico.hpp" //Enlaza el vector dinámico
#include "lista_enlazada.hpp" // Enlaza la lista enlazada

#include <string>
#include <sstream>
#include <algorithm>
#include <cmath>

using namespace std;

struct Arista { // Aristas 
    int apunta_a; // índice del vértice que conecta al destino
    double peso; // distancia de las aristas, en este caso, en km
    // constructor arista
    Arista(int a = 0, double p = 0.0) : apunta_a(a), peso(p) {}
};

class Grafo {
private:
    vector_dinamico< lista_enlazada<Arista> > adj_; // Vector dinámico de una lista enlazada, se usa adj por adyacencia
    // cada valor en el vector es una lista enlazada, cada lista enlazada es una conexción del nodo y cada lista enlazada contiene las aristas.
    int n_; // número de vertices

public:
    Grafo(int n = 0) : adj_(0), n_(n) { // ]Constructor del grafo
        for (int i = 0; i < n_; ++i) {
            adj_.push_back(lista_enlazada<Arista>());
        } // A mi parecer, el corazón del código; crea un grafo con n nodos, inicializa adj_ con n cantidad de listas vacías, una por
        //cada vértice, si n= 0, queda vacío
    }

    int vertices() const { return n_; } // Retorna la cantidad de nodos en el grafo

    // funcion para redimensionar el grafo, cuando se añade una arista con nodos que no han sido creador, es decir, si n ingresado> n actual
    // se debe agrandar, para ello, se crea un vector dinámico nuevo con mayor espacio, copia todo lo existente y añade las listas nuevas 
    // con listas vacías y reemplaza el antiguo grafo, es decir, crece dinámicamente
    void resize(int nuevo_n) {
        vector_dinamico<lista_enlazada<Arista>> temp(nuevo_n);
        
        for (int i = 0; i < nuevo_n; ++i) temp.push_back(lista_enlazada<Arista>());

        int m = std::min(n_, nuevo_n);
        
        for (int i = 0; i < m; ++i) {
            adj_[i].recorrer([&](const Arista& a){ 
                temp[i].push_back(a); 
            });
        }
        
        adj_ = move(temp);
        n_ = nuevo_n;
    }
    // crea la conexión entre dos nodos, es decir, la arista
    void agregar_arista(int u, int v, double peso, bool undirected = true) {
        if (u < 0 || v < 0) return;
        
        if (u >= n_ || v >= n_) {
            int maxv = std::max(u,v);
            resize(maxv + 1);
        }

        adj_[u].push_back(Arista(v, peso));
        
        if (undirected) {
            adj_[v].push_back(Arista(u,peso));
        }
    }
    // retorna los vecinos de un nodo, es decir, su lista de aristas, importante para el algortimo Dijkstra recorra el grafo
    const lista_enlazada<Arista>& vecinos(int u) const {
        return adj_[u];
    }
    // Representación en texto del grafo, para pruebas del código sin aún graficar
    string to_string() const {
        std::ostringstream oss;
        for (int i = 0; i < n_; ++i) {
            oss << i << ": ";
            
            // lambda para imprimir
            adj_[i].recorrer([&](const Arista& a){
              oss << "(" << a.apunta_a << ", " << a.peso << ") ";
            });
            
            oss << "\n";
        }
        return oss.str();
    }
};

#endif