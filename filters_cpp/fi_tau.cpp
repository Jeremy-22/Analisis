// *ROOT*
// Version: ROOT 6.30/08
// Descripcion; Filtra y combina las ramas no filtradas en un mismo archivo .root
//
// Creado:  14 Dec 2024
// Last Modified: 14 Dec 2024
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
    TTree *tree_tau = (TTree*)file->Get("mytaus/Events");
    if (!tree_tau) {
        std::cerr << "Error: No se encontró el árbol 'Events'." << std::endl;
        return 1;
    }
    TFile *outputFile = TFile::Open(argv[2], "RECREATE");
    if (!outputFile || outputFile->IsZombie()) {
        std::cerr << "Error al abrir el archivo de salida ROOT." << std::endl;
        return 1;
    }
    std::vector<float>* tau_e = new std::vector<float>();
    std::vector<float>* tau_pt = new std::vector<float>();
    std::vector<float>* tau_eta= new std::vector<float>();    
    
    tree_tau->SetBranchAddress("tau_e", &tau_e);
    tree_tau->SetBranchAddress("tau_pt", &tau_pt);  
    tree_tau->SetBranchAddress("tau_eta", &tau_eta); 
    TTree *filteredTree = tree_tau->CloneTree(0);
    Long64_t nEntries = tree_tau->GetEntries();
    for (Long64_t i = 0; i < nEntries; ++i) {
        tree_tau->GetEntry(i);
        bool passFilter = true;
       for (size_t j = 0; j < tau_pt->size(); ++j) {
        float eta = tau_eta ->at(j);
            if (tau_pt->at(j) > 150.0
            || eta > 2.47)
             {
                passFilter = false;
          //      break;
            }
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
