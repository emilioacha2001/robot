#include <iostream>
#include <armadillo>

using namespace std;
using namespace arma;

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

    D.swap_cols(0, 2);

    A.zeros(g, g);
    y.zeros(g);

    // aqui se cambia accu(D.col(x)) de acuerdo a la columna que se quiera para el motor x
    for (int i = 0; i < g; i++) {
        for (int j = 0; j < g; j++)
            A(i, j) = accu(arma::pow(D.col(0), (i + j)));
        y(i) = accu(D.col(2) % arma::pow(D.col(0), i));
    }

    coeficientes = inv(A) * y;
    cout << "Coeficientes del regresor: " << coeficientes.t() << endl;

    ofstream outFile("m_config/m2_regresor_inversor.txt");
    if (!outFile) {
        cerr << "Error al abrir el archivo para escritura\n";
        return 1;
    }

    vec x = linspace(min(D.col(0)), max(D.col(0)), 100);
    vec y_pred(x.n_elem, fill::zeros);

    for (size_t i = 0; i < g; i++) {
        y_pred += coeficientes(i) * arma::pow(x, i);
    }

    for (size_t i = 0; i < D.n_rows; i++) {
        outFile << D(i, 0) << " " << D(i, 2) << " NaN\n"; // Original data
    }
    for (size_t i = 0; i < x.n_elem; i++) {
        outFile << x(i) << " NaN " << y_pred(i) << "\n"; // Polynomial fit
    }

    outFile.close();
    cout << "Data written to output.dat\n";

    ofstream coefFile("m_config/m2_f_speed_delta.txt");
    for (size_t i = 0; i < coeficientes.n_elem; i++) {
        coefFile << coeficientes(i);
        if (i < coeficientes.n_elem - 1)
            coefFile << ",";
    }
    coefFile.close();

    // cpi regresor.cpp "graphs/mapeo1.txt" 3
    // plot "regresores/mapeo3_simple_m1_tiempo.txt" using 1:2 with lines title "Original Data" pointtype 7, "regresores/mapeo3_simple_m1_tiempo.txt" using 1:3 with lines title "Polynomial Fit" linecolor "red"
    // plot "output2.dat" using 1:2 with lines title "Original Data" pointtype 7, "output2.dat" using 1:3 with lines title "Polynomial Fit" linecolor "red"

    return 0;
}

// CompileOptions: -std=c++17 -O1 -larmadillo