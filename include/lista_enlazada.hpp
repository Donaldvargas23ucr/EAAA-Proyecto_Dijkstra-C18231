#ifndef lista_enlazada_HPP
#define lista_enlazada_HPP

#include <functional>
#include <cstddef>

using namespace std;

template<typename T> // Template general para cualquier tipo de dato
class lista_enlazada {
public:
    struct Node {
        T valor; //Dato almacenado
        Node *sig; // puntero al siguiente

        Node(const T& v) : valor(v), sig(nullptr) {} // constructor por copia
        Node(T&& v) : valor(move(v)), sig(nullptr) {} // constructor por movimiento (más optimización)
    };

private:
    Node* tope; // primer valor de la lista
    Node* cola; // último valor de la lista
    size_t tamanio; // cantidad de elementos

public:
    // Constructor general 
    lista_enlazada() : tope(nullptr), cola(nullptr), tamanio(0) {}
    //Destructor general
    ~lista_enlazada() { 
        clear(); 
    }
    //Constrcutor por copia
    lista_enlazada(const lista_enlazada& copia) : tope(nullptr), cola(nullptr), tamanio(0) {
        Node* temp = copia.tope;
        while(temp != nullptr) {
            push_back(temp->valor);
            temp = temp->sig;
        }
    }
    // Constructor por movimiento
    lista_enlazada(lista_enlazada&& copia) noexcept
        : tope(copia.tope), cola(copia.cola), tamanio(copia.tamanio)
    {
        // dejamos la original vacia
        copia.tope = nullptr;
        copia.cola = nullptr;
        copia.tamanio = 0;
    }
    // Operador de asignación,  limpia la lista con clear y la copia nodo por nodo
    lista_enlazada& operator=(const lista_enlazada& copia) {
        if (this == &copia) return *this;
        
        clear();
        Node* temp = copia.tope;
        while (temp) {
            push_back(temp->valor);
            temp = temp->sig;
        }
        return *this;
    }
    // dos pushbacks, uno por copia y otro por movimiento, crean un nodo y si la lista no está vacía, la copia empezando por el final
    void push_back(const T& v) {
        Node *n = new Node(v);
        if (tope == nullptr) {
            tope = n;
            cola = n;
        } else {
            cola->sig = n;
            cola = n;
        }
        tamanio++;
    }

    void push_back(T&& v) {
        Node* n = new Node(move(v));
        
        // caso lista vacia
        if (!tope) 
            tope = cola = n;
        else {
            cola->sig = n;
            cola = n;
        }
        ++tamanio;
    }
    // Recorre la lista y elimina el nodo que cumpla la condición dada
    bool remover(const function<bool(const T&)>& pred) {
        Node* temp = tope;
        Node* prev = nullptr;
        
        while (temp != nullptr) {
            if (pred(temp->valor)) {
                // encontramos el elemento
                if (prev) {
                    prev->sig = temp->sig;
                } else {
                    tope = temp->sig;
                }

                if (temp == cola) {
                    cola = prev;
                }
                
                delete temp;
                this->tamanio--;
                return true;
            }
            prev = temp;
            temp = temp->sig;
        }
        return false;
    }
    // Recorre la lista nodo a nodo
    void recorrer(const function<void(const T&)>& funcion) const {
        Node* temp = tope;
        while(temp) {
            funcion(temp->valor);
            temp = temp->sig;
        }
    }

    Node* head() const { return tope; } //puntero al primer nodo
    
    size_t size() const { 
        return this->tamanio; 
    }
    
    bool empty() const { return tamanio == 0; }
    // libera los nodos de la lista
    void clear() {
        Node* temp = tope;
        while (temp) {
            Node *borrar = temp;
            temp = temp->sig;
            delete borrar;
        }
        tope = nullptr;
        cola = nullptr;
        tamanio = 0;
    }
};

#endif