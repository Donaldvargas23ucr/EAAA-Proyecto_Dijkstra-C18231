#ifndef DIJKSTRA_HPP
#define DIJKSTRA_HPP
//La finalidad del algortimo Dijsktra es que retorne la distancia minima entre 2 nodos y el camino que se recorrió, siguiendo un orden.
#include "Grafo.hpp" // LLama al grafo
#include <vector>
#include <limits>
#include <queue> 
#include <utility>
using namespace std;
inline pair<double, vector<int>> dijkstra(
    const Grafo& g, //inicializa el grafo
    int inicio, // nodo inicio
    int objetivo) // nodo final
{
    int n = g.vertices(); // cantidad de nodos del grafo 
    const double INF = numeric_limits<double>::infinity(); // INF representa el infinito "numéricamente"

    vector<double> dist(n, INF); // distancia minima encontrada desde el nodo inicial
    vector<int> parent(n, -1); // desde el nodo que se partió hasta llegar inicial

    using Par = pair<double,int>;  // {dist, nodo}
    priority_queue<Par, vector<Par>, greater<Par>> pq; // cola de prioridad básica, se usa greater para que siempre salga primero
    //el nodo con menor distancia

    dist[inicio] = 0.0; // distancia del nodo inicial hacia sí mismo
    pq.push({0.0, inicio});
    // bucle general de Dijkstra, extrae el nodo más cercano no procesado
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d > dist[u]) continue; // si el nodo ya tiene una distancia superada, se ignora
        if (u == objetivo) break; // Condición de parada 
        // Parte primordial del algortimo, por cada vecino, se calcula la distancia relativa, si esta distancia es menor dela que ya tenemos
        // se actualiza distan[v], se guarda el parent para la reconstrucción de la ruta y se agrega el nodo v a la cola de prioridad
        g.vecinos(u).recorrer([&](const Arista& a){
            int v = a.apunta_a;
            double nd = dist[u] + a.peso;
            if (nd < dist[v]) {
                dist[v] = nd;
                parent[v] = u;
                pq.push({nd, v});
            }
        });
    }

    vector<int> path;
    if (dist[objetivo] == INF) // Si no se llega al objetivo
        return {INF, path};

    for (int cur = objetivo; cur != -1; cur = parent[cur]) // Sí sí hay ruta
        path.push_back(cur);

    reverse(path.begin(), path.end());
    return {dist[objetivo], path};
}

#endif
