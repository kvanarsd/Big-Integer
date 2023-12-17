//-----------------------------------------------------------------------------
// Katrina VanArsdale
// kvanarsd
// Pa6
//-----------------------------------------------------------------------------

#include <iostream>
#include<fstream>

#include "List.h" 
#include "BigInteger.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <input file> <output file>" << endl;
        return 1;
    }


    ifstream in(argv[1]);
    if(!in.is_open()) {
        cerr << "Unable to open file: " << argv[1] << "for reading" << endl;
        exit(1);
    }

    ofstream out(argv[2]);
    if(!out.is_open()) {
        cerr << "Unable to open file: " << argv[2] << "for writing" << endl;
        exit(1);
    }

    // read in first number
    string s;
    getline(in, s);
    BigInteger A = s;

    // skip second line and read second number
    getline(in, s);
    getline(in, s);
    BigInteger B = s;

    // Ints for math
    BigInteger C, D, E, F, G, H, I, J;

    C = A + B;
    D = A - B;
    E = A - A;
    F = 3*A - 2*B;
    G = A * B;
    H = A * A;
    I = B * B;
    J = 9*(A*A*A*A) + 16*(B*B*B*B*B);

    // print
    out << A << endl << endl;
    out << B << endl << endl;
    out << C << endl << endl;
    out << D << endl << endl;
    out << E << endl << endl;
    out << F << endl << endl;
    out << G << endl << endl;
    out << H << endl << endl;
    out << I << endl << endl;
    out << J << endl << endl;

    return 0;
}