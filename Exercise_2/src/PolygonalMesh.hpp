#pragma once
//#include <iostream>
#include <vector>
#include "Eigen/Eigen"

using namespace std;
using namespace Eigen;

namespace PolygonalLibrary{

struct PolygonalMesh{          // creo la struttura per memorizzare la mesh

    //strutture per la definzione delle celle 0-dim

    unsigned int NumberCell0D = 0; ///< number of Cell0D
    vector <unsigned int> IdCell0D = {};                     // salvo l'id per le celle 0d
    vector <Vector2d> CoordinatesCell0D = {};               // vettore 2d perchè ci memorizzo le coordinate x e y
    map <unsigned int, Vector2d> Coppia_coordinate_id = {};
    map <unsigned int, list<unsigned int>> VerticesMarker = {};      // creo un dizionario che ad ogni marker(che in questo caso è una chiave) associa la lista di vertici con quel marker


    //strutture per la definzione delle celle 1-dim
    unsigned int NumberCell1D = 0; ///< number of Cell1D
    vector <unsigned int> IdCell1D {};                      // salvo l'id delle celle 1d (spigoli)
    vector <Vector2i> VerticesCell1D = {};     //questa struttara ha dimensione fissa, e serve per memorizzare il punto iniziale e finale
    map <unsigned int, list<unsigned int>> EdgesMarker = {};         // analogo per gli spigoli


    //strutture per la definzione delle celle 2-dim
    unsigned int NumberCell2D = 0; ///< number of Cell2D
    vector <unsigned int> IdCell2D ={};                    // salvo l'id delle celle 2d (triangoli)
    vector <unsigned int> NumVertices ={};                 // salvo il numero di vertici del mio poligono
    vector <VectorXi> VerticesCell2D={};    // salvo i vertici delle celle 2d in un vettore di dimensione qualsiasi, perché non so a prescindere quanto è grande il poligono
    vector <unsigned int> NumEdges={};                 // salvo il numero di lati del mio poligono
    vector <VectorXi> EdgesCell2D={};       // salvo gli spigoli della mia cella in un vettore di dimensione qualsiasi perché non so quanto è grande il mio poligono
    //map <unsigned int, list<unsigned int>> PolygonalMarker={};  // non serve tanto i marker sono tutti 0

    //tolleranze
    double tol_lunghezze = numeric_limits<double>::epsilon()/10;            // definisco la tolleranza come 10* la precisione di macchina
    double tol_aree = (tol_lunghezze)*(tol_lunghezze)/2;

};
}

