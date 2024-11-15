#include <iostream>
#include <armadillo>
#include <sciplot/sciplot.hpp>

using namespace std;
using namespace arma;
using namespace sciplot;

int main(int argc, char *argv[]) {
    mat D, A;
    vec y, coeficientes;
    unsigned g;

    if (argc > 1) {
        D.load(argv[1]);
        unsigned N = D.n_rows;
        g = atoi(argv[2]) + 1;
    } else {
        cerr << "Faltan parametros datos.txt y grado\n ";
        exit(0);
    }

    A.zeros(g, g);
    y.zeros(g);

    // aqui se cambia accu(D.col(x)) de acuerdo a la columna que se quiera para el motor x
    for (int i = 0; i < g; i++) {
        for (int j = 0; j < g; j++)
            A(i, j) = accu(pow(D.col(0), (i + j)));
        y(i) = accu(D.col(1) % pow(D.col(0), i));
    }

    coeficientes = inv(A) * y;
    cout << "Coeficientes del regresor: " << coeficientes.t() << endl;

    ofstream outFile("output2.dat");
    if (!outFile) {
        cerr << "Error al abrir el archivo para escritura\n";
        return 1;
    }

    vec x = linspace(min(D.col(0)), max(D.col(0)), 100);
    vec y_pred(x.n_elem, fill::zeros);

    for (size_t i = 0; i < g; i++) {
        y_pred += coeficientes(i) * pow(x, i);
    }

    for (size_t i = 0; i < D.n_rows; i++) {
        outFile << D(i, 0) << " " << D(i, 1) << " NaN\n"; // Original data
    }
    for (size_t i = 0; i < x.n_elem; i++) {
        outFile << x(i) << " NaN " << y_pred(i) << "\n"; // Polynomial fit
    }

    outFile.close();
    cout << "Data written to output.dat\n";

    // cpi regresor.cpp "graphs/mapeo1.txt" 3
    // plot "output.dat" using 1:2 with lines title "Original Data" pointtype 7, "output.dat" using 1:3 with lines title "Polynomial Fit" linecolor "red"
    // plot "output2.dat" using 1:2 with lines title "Original Data" pointtype 7, "output2.dat" using 1:3 with lines title "Polynomial Fit" linecolor "red"


    return 0;
}

// CompileOptions: -std=c++17 -O1 -larmadillo -lsciplot