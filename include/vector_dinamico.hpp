
#ifndef vector_dinamico_HPP
#define vector_dinamico_HPP

#include <cstddef>
#include <new>
#include <stdexcept>
#include <utility>

using namespace std;

template<typename T>
class vector_dinamico {
private:
    T* dato;
    size_t tamanio;
    size_t capacidad;

    // funcion auxiliar para redimensionar
    void reallocate(size_t nueva_capacidad) {
        if(nueva_capacidad < tamanio) return;
        
        T* nuevo_dato = static_cast<T*>(::operator new(sizeof(T) * nueva_capacidad));
        
        for(size_t i=0; i<tamanio; ++i) {
            new (nuevo_dato + i) T(move(dato[i]));
            dato[i].~T();
        }
        ::operator delete(dato);
        dato = nuevo_dato;
        capacidad = nueva_capacidad;
    }

public:
    // constructor por defecto
    vector_dinamico(size_t initial = 4) : dato(nullptr), tamanio(0), capacidad(initial) {
        if (capacidad == 0) capacidad = 1;
        dato = static_cast<T*>(::operator new(sizeof(T) * capacidad));
    }

    // iteradores
    T* begin() { return dato; }
    const T* begin() const { return dato; }
    T* end() { return dato + tamanio; }
    const T* end() const { return dato + tamanio; }

    // Constructor de copia
    vector_dinamico(const vector_dinamico& copia)
        : dato(nullptr), tamanio(0), capacidad(copia.capacidad) 
    {
        dato = static_cast<T*>(::operator new(sizeof(T) * capacidad));
        for (size_t i = 0; i < copia.tamanio; ++i) {
            new (dato + i) T(copia.dato[i]);
        }
        tamanio = copia.tamanio;
    }

    // Move constructor
    vector_dinamico(vector_dinamico&& copia) noexcept 
        : dato(copia.dato), tamanio(copia.tamanio), capacidad(copia.capacidad) {
        copia.dato = nullptr;
        copia.tamanio = 0;
        copia.capacidad = 0;
    }

    ~vector_dinamico() {
        clear();
        ::operator delete(dato);
    }

    vector_dinamico& operator=(const vector_dinamico& copia) {
        if (this == &copia) return *this;
        clear();
        ::operator delete(dato);
        
        capacidad = copia.capacidad;
        dato = static_cast<T*>(::operator new(sizeof(T) * capacidad));
        for(size_t i = 0; i < copia.tamanio; ++i)
            new (dato + i) T(copia.dato[i]);
        
        tamanio = copia.tamanio;
        return *this;
    }

    vector_dinamico& operator=(vector_dinamico&& copia) noexcept {
        if(this == &copia) return *this;
        
        clear();
        ::operator delete(dato);
        dato = copia.dato;
        tamanio = copia.tamanio;
        capacidad = copia.capacidad;
        
        // dejar el otro vacio
        copia.dato = nullptr;
        copia.tamanio = 0;
        copia.capacidad = 0;
        return *this;
    }

    void push_back(const T& valor) {
        if (tamanio >= capacidad) {
            reallocate(capacidad * 2);
        }
        new (dato + tamanio) T(valor);
        ++tamanio;
    }

    void push_back(T&& valor) {
        if(tamanio >= capacidad) reallocate(capacidad * 2);
        new (dato + tamanio) T(move(valor));
        ++tamanio;
    }

    void pop_back() {
        if (tamanio == 0) 
            throw out_of_range("se intentó hacer pop_back en un vector_dinamico vacío");
        --tamanio;
        dato[tamanio].~T();
    }

    T& operator[](size_t indice) {
        if(indice >= tamanio) throw out_of_range("El valor está fuera de rango");
        return dato[indice];
    }

    const T& operator[](size_t indice) const {
        if (indice >= tamanio) throw out_of_range("El valor está fuera de rango");
        return dato[indice];
    }

    size_t size() const { return tamanio; }
    
    bool empty() const { 
        return tamanio == 0; 
    }

    void clear() {
        for(size_t i=0; i<tamanio; ++i) {
            dato[i].~T();
        }
        tamanio = 0;
    }
};

#endif