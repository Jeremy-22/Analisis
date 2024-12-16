// *ROOT*
// Version: ROOT 6.30/08
// Descripcion; Filtra y combina las ramas no filtradas en un mismo archivo .root
//
// Creado:  11 Dec 2024
// Last Modified: 12 Dec 2024
// Author: Jeremy Rangel
// Contacto: jeryrangmart@gmail.com

#include <TInterpreter.h>
#include <iostream>
#include "TClass.h"
#include <cmath>
#include "TROOT.h"
#include "TSystem.h"
#include <vector>
#include "TFile.h"
#include "TTree.h"
#include "TClonesArray.h"
#include "TObjArray.h"

namespace std {
    template class vector<float>;
}

int main(int argc, char** argv) {
    gSystem->Load("libTree");
    gInterpreter->GenerateDictionary("vector<float>", "vector");
    gSystem->Load("libPhysics");    
    if (argc < 3) {
        std::cerr << "Uso: " << argv[0] << " <archivo_entrada.root> <archivo_salida.root>" << std::endl;
        return 1;
    }


    TFile *file = TFile::Open(argv[1]);
    if (!file || file->IsZombie()) {
        std::cerr << "Error al abrir el archivo ROOT." << std::endl;
        return 1;
    }

    TTree *tree_vertex = (TTree*)file->Get("mypvertex/Events");
    if (!tree_vertex) {
        std::cerr << "Error: No se encontró el árbol 'Events'." << std::endl;
        return 1;
    }

    TFile *outputFile = TFile::Open(argv[2], "RECREATE");
    if (!outputFile || outputFile->IsZombie()) {
        std::cerr << "Error al abrir el archivo de salida ROOT." << std::endl;
        return 1;
    }
   
    std::vector<float>* PV_chi2 = new std::vector<float>();
    std::vector<float>* PV_ndof = new std::vector<float>();

    tree_vertex->SetBranchAddress("PV_ndof", &PV_ndof);  
    tree_vertex->SetBranchAddress("PV_chi2", &PV_chi2); 

    TTree *filteredTree = tree_vertex->CloneTree(0);


    Long64_t nEntries = tree_vertex->GetEntries();

    for (Long64_t i = 0; i < nEntries; ++i) {
        tree_vertex->GetEntry(i);


        bool passFilter = true;
       for (size_t j = 0; j < PV_chi2->size(); ++j) {
            //if (PV_chi2->at(j)   )
             //{
             //   passFilter = false;  // Si alguno no pasa, el evento es descartado
          //      break;
            //}
        }
        if (passFilter) {
            filteredTree->Fill();
        }
    }
    filteredTree->Write();
    
    outputFile->Close();
    file->Close();

    std::cout << "Filtrado completado. Los datos se guardaron en " << argv[2] << std::endl;
    return 0;
}
