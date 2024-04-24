#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <Eigen/Eigen>
#include <cmath>
//#include <iomanip>



using namespace std;
namespace PolygonalLibrary{

//funzione per importare la mesh
bool importMesh(const string& filepath,
                PolygonalMesh& mesh)
{

    if(!importCell0D(filepath + "/Cell0Ds.csv",
                      mesh))
    {
        return false;
    }
    else
    {
        cout << "Cell0D marker:" << endl;
        for(auto it = mesh.VerticesMarker.begin(); it != mesh.VerticesMarker.end(); it++)
        {
            cout << "key:\t" << it -> first << "\t values:";
            for(const unsigned int id : it -> second)
                cout << "\t" << id;

            cout << endl;
        }
    }

    if(!importCell1D(filepath + "/Cell1Ds.csv",
                      mesh))
    {
        return false;
    }
    else
    {
        cout << "Cell1D marker:" << endl;
        for(auto it = mesh.EdgesMarker.begin(); it != mesh.EdgesMarker.end(); it++)
        {
            cout << "key:\t" << it -> first << "\t values:";
            for(const unsigned int id : it -> second)
                cout << "\t" << id;

            cout << endl;
        }
    }

    if(!importCell2D(filepath + "/Cell2Ds.csv",
                      mesh))
    {
        return false;
    }
    else
    {
        // Test:
        for(unsigned int c = 0; c < mesh.NumberCell2D; c++)
        {
            VectorXi edges = mesh.EdgesCell2D[c];           // identifica i lati di un deterimato poligono
            unsigned int n = edges.size();

            for(unsigned int e = 0; e < n; e++)     // itero sul numero di lati
            {
                const unsigned int origin = mesh.VerticesCell1D[edges[e]][0];       // del mio lato prendo il vertice iniziale e finale
                const unsigned int end = mesh.VerticesCell1D[edges[e]][1];

                auto findOrigin = find(mesh.VerticesCell2D[c].begin(), mesh.VerticesCell2D[c].end(), origin);               // cerca l'elemento origin dentro VerticesCell2D[c]
                //cout << findOrigin[0] << endl;
                if(findOrigin == mesh.VerticesCell2D[c].end())
                {
                    cerr << "Wrong mesh" << endl;
                    return 2;
                }

                auto findEnd = find(mesh.VerticesCell2D[c].begin(), mesh.VerticesCell2D[c].end(), end);
                if(findEnd == mesh.VerticesCell2D[c].end())
                {
                    cerr << "Wrong mesh" << endl;
                    return 3;
                }

            }
        }
    }

    return true;

}
//seguono le funzioni che importano le varie celle
bool importCell0D(const string &filename,
                  PolygonalMesh& mesh)
{
    // fase di apertura del file
    ifstream file;
    file.open(filename);

    if(file.fail())
        return false;
    // fine fase lettura e apertura del file

    list<string> listLines;             // creo una lista per memorizzare le righe del file
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    file.close();

    listLines.pop_front();      // rimuovo il primo elemento della lista perché è l'intestazione

    mesh.NumberCell0D = listLines.size();           // ricavo la dimensione della lista

    if (mesh.NumberCell0D == 0)                 // caso degenere
    {
        cerr << "There is no cell 0D" << endl;
        return false;
    }

    mesh.IdCell0D.reserve(mesh.NumberCell0D);           // riservo la memoria necessaria a memorizzare quello che mi serve
    mesh.CoordinatesCell0D.reserve(mesh.NumberCell0D);

    for (const string& line : listLines)
    {
        istringstream converter(line);          // definisco l'oggetto converter

        unsigned int id;
        unsigned int marker;
        Vector2d coord;

        // converto gli elementi della mia stringa
        converter >> id ;
        converter.ignore(); // ignoro il ;
        converter >> marker;
        converter.ignore();
        converter >> coord[0];
        converter.ignore();
        converter >> coord[1];

        mesh.Coppia_coordinate_id.insert({id,{coord}});     // creo il dizionario dove salvo le coordinate e gli id
        mesh.IdCell0D.push_back(id);        // aggiungo l'id e le coordinate
        mesh.CoordinatesCell0D.push_back(coord);

        if( marker != 0)            // se non sono un vertice interno alla mesh!
        {
            //            if (mesh.Cell0DMarkers.find(marker) == mesh.Cell0DMarkers.end())
            //                mesh.Cell0DMarkers.insert({marker, {id}});
            //            else
            //                mesh.Cell0DMarkers[marker].push_back(id);


            auto ret = mesh.VerticesMarker.insert({marker, {id}});
            if(!ret.second)
                (ret.first)->second.push_back(id);
        }

    }
    file.close();
    return true;
}


//le due funzioni seguenti sono un mio tentetivo di soluzione
bool importCell1D(const string &filename,
                  PolygonalMesh& mesh)
{
    // apertura file
    ifstream file;
    file.open(filename);

    if(file.fail())
        return false;
    //

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    listLines.pop_front();

    mesh.NumberCell1D = listLines.size();

    if (mesh.NumberCell1D == 0)
    {
        cerr << "There is no cell 1D" << endl;
        return false;
    }

    mesh.IdCell1D.reserve(mesh.NumberCell1D);
    mesh.VerticesCell1D.reserve(mesh.NumberCell1D);

    for (const string& line : listLines)
    {
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        Vector2i vertices;

        converter >>  id;
        converter.ignore();
        converter >> marker;
        converter.ignore();
        converter >> vertices[0];
        converter.ignore();
        converter >> vertices[1];

        mesh.IdCell1D.push_back(id);
        mesh.VerticesCell1D.push_back(vertices);

        if( marker != 0)
        {
            //            if (mesh.Cell1DMarkers.find(marker) == mesh.Cell1DMarkers.end())
            //                mesh.Cell1DMarkers.insert({marker, {id}});
            //            else
            //                mesh.Cell1DMarkers[marker].push_back(id);


            auto ret = mesh.EdgesMarker.insert({marker, {id}});
            if(!ret.second)
                (ret.first)->second.push_back(id);
        }
    }

    file.close();

    return true;
}

bool importCell2D(const string &filename,
                  PolygonalMesh& mesh)
{
    // apro
    ifstream file;
    file.open(filename);

    if(file.fail())
        return false;
    //

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    listLines.pop_front();

    mesh.NumberCell2D = listLines.size();

    if (mesh.NumberCell2D == 0)
    {
        cerr << "There is no cell 2D" << endl;
        return false;
    }

    mesh.IdCell2D.reserve(mesh.NumberCell2D);
    mesh.VerticesCell2D.reserve(mesh.NumberCell2D);
    mesh.EdgesCell2D.reserve(mesh.NumberCell2D);

    for (const string& line : listLines)
    {
        istringstream converter(line);

        unsigned int id;
        unsigned int NumVertices;
        unsigned int NumEdges;
        unsigned int marker;

        converter >>  id;
        converter.ignore();
        converter >> marker;
        converter.ignore();
        converter >> NumVertices;
        converter.ignore();

        VectorXi vertices = VectorXi::Zero(NumVertices);
        for(unsigned int i = 0; i < NumVertices; i++)
        {
            converter >> vertices(i);
            converter.ignore();
        }
        converter >> NumEdges;
        converter.ignore();
        VectorXi edges = VectorXi::Zero(NumEdges);


        for(unsigned int i = 0; i < NumEdges-1; i++)
        {
            converter >> edges(i);
            converter.ignore();
        }
        converter >> edges(NumEdges-1);    // fatto per evitare casini perché il file non finisce con ;

        mesh.IdCell2D.push_back(id);
        mesh.VerticesCell2D.push_back(vertices);
        mesh.EdgesCell2D.push_back(edges);

        // TEST LUNGHEZZA E AREA
        double somma = 0;
        for (unsigned int i = 0; i<NumVertices-1; i++)
        {

            //cout << vertices(i)<< " "<< vertices(i+1) << endl;
            Vector2d coordinate_1 = mesh.Coppia_coordinate_id[vertices(i)];         // vettore contenente le coordinate dei vertici
            Vector2d coordinate_2 = mesh.Coppia_coordinate_id[vertices(++i)];

            //cout << coordinate_1[0] <<" " <<coordinate_1[1]<< endl;

            double lunghezza_12 = sqrt((coordinate_1[0]-coordinate_2[0])*(coordinate_1[0]-coordinate_2[0]) + (coordinate_1[1]-coordinate_2[1])*(coordinate_1[1]-coordinate_2[1]));


            if (lunghezza_12 <= mesh.tol_lunghezze)
            {
                cerr << "Distanza tra i vertici minore della tolleranza" << endl;
            }

            //cout <<  coordinate_1[0]<< " " <<  coordinate_1[1]<< endl;
            //cout <<  coordinate_2[0]<< " " <<  coordinate_2[1]<< endl;

            somma += ( coordinate_1[0]*coordinate_2[1] - coordinate_1[1]*coordinate_2[0]);
            //cout << somma <<endl;
        }
        Vector2d coordinate_1 = mesh.Coppia_coordinate_id[vertices(NumVertices-1)];         // gli ultimi termini li metto fuori dal for, considerando anche che y(n+1) = y(1)
        Vector2d coordinate_2 = mesh.Coppia_coordinate_id[vertices(0)];
        somma += ( coordinate_1[0]*coordinate_2[1] - coordinate_1[1]*coordinate_2[0]);

        if(somma < 0)
            somma = -0.5*somma;       // ne considero il valore assoluto
        else
            somma = 0.5*somma;
        //cout << somma << endl;

        if (mesh.tol_aree > mesh.tol_lunghezze)
        {
            if (somma <= mesh.tol_aree)
            {
                cerr << "Area di un poligono minore della tolleranza"<< endl;
            }
        }
        else
        {
            if ( somma <= mesh.tol_lunghezze)
            {
                cerr << "Area di un poligono minore della tolleranza"<< endl;

            }
        }

    }
    file.close();
    return true;
}
}
