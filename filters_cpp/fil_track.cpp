// *ROOT*
// Version: ROOT 6.30/08
// Descripcion; Filtra y combina las ramas no filtradas en un mismo archivo .root
//
// Creado:  15 Dec 2024
// Last Modified: 15 Dec 2024
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
    TTree *tree_track = (TTree*)file->Get("mytracks/Events");
    if (!tree_track) {
        std::cerr << "Error: No se encontró el árbol 'Events'." << std::endl;
        return 1;}
    TFile *outputFile = TFile::Open(argv[2], "RECREATE");
    if (!outputFile || outputFile->IsZombie()) {
        std::cerr << "Error al abrir el archivo de salida ROOT." << std::endl;
        return 1; }
    std::vector<float>* track_pt = new std::vector<float>();
    std::vector<float>* track_eta= new std::vector<float>();    
    

    tree_track->SetBranchAddress("track_pt", &track_pt);  
    tree_track->SetBranchAddress("track_eta", &track_eta); 

    TTree *filteredTree = tree_track->CloneTree(0);


    Long64_t nEntries = tree_track->GetEntries();

    for (Long64_t i = 0; i < nEntries; ++i) {
        tree_track->GetEntry(i);


        bool passFilter = true;
       for (size_t j = 0; j < track_pt->size(); ++j) {
            //if (track_pt->at(j) < 150.0)
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
