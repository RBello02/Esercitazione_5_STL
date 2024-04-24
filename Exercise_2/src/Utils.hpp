#pragma once
//#include <iostream>
#include "PolygonalMesh.hpp"

using namespace std;

namespace PolygonalLibrary{

//invoco le funzioni necessarie ad importare i dati per costruire la mesh
bool importMesh (const string& path, PolygonalMesh& mesh);
bool importCell0D (const string& path, PolygonalMesh& mesh);
bool importCell1D (const string& path, PolygonalMesh& mesh);
bool importCell2D (const string& path, PolygonalMesh& mesh);

}


