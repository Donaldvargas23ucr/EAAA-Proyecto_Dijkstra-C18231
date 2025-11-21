#ifndef GRAFO_HPP
#define GRAFO_HPP

#include "vector_dinamico.hpp"
#include "lista_enlazada.hpp"

#include <string>
#include <sstream>
#include <algorithm>
#include <cmath>

using namespace std;

struct Arista {
    int apunta_a;
    double peso;
    // constructor arista
    Arista(int a = 0, double p = 0.0) : apunta_a(a), peso(p) {}
};

class Grafo {
private:
    vector_dinamico< lista_enlazada<Arista> > adj_;
    int n_;

public:
    Grafo(int n = 0) : adj_(0), n_(n) {
        for (int i = 0; i < n_; ++i) {
            adj_.push_back(lista_enlazada<Arista>());
        }
    }

    int vertices() const { return n_; }

    // funcion para redimensionar el grafo
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

    const lista_enlazada<Arista>& vecinos(int u) const {
        return adj_[u];
    }

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