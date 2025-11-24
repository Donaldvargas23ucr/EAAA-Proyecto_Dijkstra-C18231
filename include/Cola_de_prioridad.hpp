
#ifndef cola_de_prioridad_HPP
#define cola_de_prioridad_HPP

#include "vector_dinamico.hpp" //Llama al vector dinámico el cual almacena el heap
#include <stdexcept> // Para control de out of ranges
#include <utility>

using namespace std;

template<typename T> //La template para cualquier tipo de variable
class cola_de_prioridad {
public:
    struct Elemento { // El struct con el valor y el numero de prioridad del dato para determinar el orden del heap
        T valor;
        double prioridad;

        // 3 Constructores; Uno vacío (general), uno que recibe la referencia dada y el último que recibe un valor movible
        Elemento() {}
        Elemento(const T& valor_temp, double priori_temp) : valor(valor_temp), prioridad(priori_temp) {}
        Elemento(T&& valor_temp, double priori_temp) : valor(move(valor_temp)), prioridad(priori_temp) {} 
    };

private: 
    vector_dinamico<Elemento> tope; // Estructura principal, se cambió la variable heap a -> tope. Acá se guarda el heap

    void swap(size_t a, size_t b) { // Intercambia dos elementos almacenados en el heap
        Elemento temp = move(tope[a]);
        tope[a] = move(tope[b]);
        tope[b] = move(temp);
    }
//Función general percolate_up y down para colas, cumple la función de subir o bajar elementos nuevos dependiendo su prioridad
    void percolate_up(size_t indice) {
        while (indice > 0) {
            size_t cambio = (indice - 1) / 2;
            if (tope[indice].prioridad < tope[cambio].prioridad) {
                swap(indice, cambio);
                indice = cambio;
            } 
            else {
                break;
            }
        }
    }
    // Garantiza que el valor de menor prioridad sea siempre la raíz
    void percolate_down(size_t indice) {
        size_t nuevo = tope.size();
        while(true) {
            size_t izq = 2*indice + 1;
            size_t der = 2*indice + 2;
            size_t mas_peq = indice;

            // revisar hijo izq
            if (izq < nuevo && tope[izq].prioridad < tope[mas_peq].prioridad) 
                mas_peq = izq;
            
            // revisar hijo der
            if (der < nuevo && tope[der].prioridad < tope[mas_peq].prioridad) 
                mas_peq = der;

            if (mas_peq != indice) {
                swap(indice, mas_peq);
                indice = mas_peq;
            } else {
                break; 
            }
        }
    }

public:
    cola_de_prioridad() = default;
//Agrega un elemento al final de la cola y llama a percolate_up para acomodarlo 
    void insertar(const T& valor, double prioridad) {
        tope.push_back(Elemento(valor, prioridad));
        percolate_up(tope.size() - 1);
    }
    void insertar(T&& valor, double prioridad) {
        tope.push_back(Elemento(move(valor), prioridad));
        percolate_up(tope.size() - 1);
    }
//Verefica si está vacío el heap
    bool vacia() const {
        return tope.size() == 0;
    }
// función que permite que no se salga de rango, si está vacía lanza el mensaje de advertencia, sino guarda a tope[0] como minimo
// También, mueve el ultimo elemento a la raiz y llama a percolate_down para reordenar
    Elemento extraer_min() {
        if (vacia()) 
            throw out_of_range("extraer minnimo de cola vacia");
        
        Elemento cima = tope[0];
        
        if (tope.size() == 1) {
            tope.pop_back();
            return cima;
        }

        // movemos el ultimo al principio y reordenamos
        tope[0] = move(tope[tope.size() - 1]);
        tope.pop_back();
        percolate_down(0);
        
        return cima;
    }
//Tamaño del heap
    size_t tam() const { return tope.size(); }
};

#endif