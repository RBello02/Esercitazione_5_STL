#include "Utils.hpp"
#include "PolygonalMesh.hpp"
#include <iostream>
#include <Eigen/Eigen>

using namespace PolygonalLibrary;
using namespace std;
using namespace Eigen;



int main()
{
    PolygonalMesh mesh;

    //importo la mesh da file input e ritorno 1 se ci sono degli errori
    string path = "PolygonalMesh";
    if (!importMesh(path, mesh)){
        cerr << "Errore nell'imporrare la mesh." << endl;
        return 1;
    }


    return 0;
}
