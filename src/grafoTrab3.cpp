#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <climits>
#include <iomanip> 
#include <fstream>

using namespace std;

struct Grafico { 
    int V; 
    vector<vector<int>> adj; 
    int numAresta;  
};

// Adiciona aresta entre os vértices "u" e "v" no grafo(G)
void AdcAresta(Grafico& G, int u, int v) {
    G.adj[u].push_back(v);
    G.adj[v].push_back(u);
    G.numAresta++;
}

// Função para calcular e imprimir o gmin, gmax e diametro através da busca em largura (BFS)
void PrintDados(const Grafico& G, ofstream& outputFile) {
    int gmin = INT_MAX;
    int gmax = 0;
    double gmed = 0;
    int diam = 0;

    for (int v = 0; v < G.V; ++v) {
        int grau = G.adj[v].size();
        gmin = min(gmin, grau);
        gmax = max(gmax, grau);
        gmed += grau;
    }

    gmed /= G.V;

    // Início do BFS
    for (int s = 0; s < G.V; ++s) {
        vector<int> dist(G.V, -1);
        queue<int> q;
        q.push(s);
        dist[s] = 0;

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (int v : G.adj[u]) {
                if (dist[v] == -1) {
                    dist[v] = dist[u] + 1;
                    q.push(v);
                }
            }
        }

        for (int v = 0; v < G.V; ++v) {
            diam = max(diam, dist[v]);
        }
    }
    // Fim do BFS

    // Escreve os dados no terminal
    cout << setw(5) << G.V << setw(8) << G.numAresta << setw(8) << gmin
         << setw(8) << gmax << setw(8) << fixed << setprecision(1) << gmed
         << setw(8) << diam << endl;

    // Escreve os dados no arquivo
    outputFile << setw(5) << G.V << setw(8) << G.numAresta << setw(8) << gmin
               << setw(8) << gmax << setw(8) << fixed << setprecision(1) << gmed
               << setw(8) << diam << endl;
}

// Função para criar um grafo
Grafico GerarGrafico(int n, double p) {
    Grafico G;
    G.V = n;
    G.adj.resize(n);
    G.numAresta = 0;

    srand(time(0));

    for (int u = 0; u < n; ++u) {
        for (int v = u + 1; v < n; ++v) {
            // Usa uma moeda tendenciosa para decidir se inclui a aresta uv
            double prob = static_cast<double>(rand()) / RAND_MAX;
            if (prob < p) {
                AdcAresta(G, u, v);
            }
        }
    }

    return G;
}

int main() {
    int ini = 10;
    int fim = 200;
    int stp = 10;
    double p = 0.1;

    // Abre o arquivo para escrita
    ofstream outputFile("grafico_resultados.txt");
    if (!outputFile) {
        cerr << "Erro ao abrir o arquivo para escrita." << endl;
        return 1;
    }

    // Escreve o cabeçalho no terminal e no arquivo
    cout << setw(5) << "V" << setw(8) << "E" << setw(8) << "gmin"
         << setw(8) << "gmax" << setw(8) << "gmed" << setw(8) << "diam" << endl;
    cout << "-----------------------------------------------" << endl;

    outputFile << setw(5) << "V" << setw(8) << "E" << setw(8) << "gmin"
               << setw(8) << "gmax" << setw(8) << "gmed" << setw(8) << "diam" << endl;
    outputFile << "-----------------------------------------------" << endl;

    // Gera os grafos e escreve os resultados
    for (int n = ini; n <= fim; n += stp) {
        Grafico G = GerarGrafico(n, p);
        PrintDados(G, outputFile);
    }

    // Fecha o arquivo
    outputFile.close();

    return 0;
}
