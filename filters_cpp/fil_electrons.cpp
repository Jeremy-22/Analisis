// *ROOT*
// Version: ROOT 6.30/08
// Descripcion; Filtra y combina las ramas no filtradas en un mismo archivo .root
//
// Creado:  09 Dec 2024
// Last Modified: 13 Dec 2024
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

    // Abrir el archivo ROOT original
    TFile *file = TFile::Open(argv[1]);
    //file->ls();
    if (!file || file->IsZombie()) {
        std::cerr << "Error al abrir el archivo ROOT." << std::endl;
        return 1;
    }

    // Obtener el árbol de electrones
    TTree *tree_electron = (TTree*)file->Get("myelectrons/Events");
    if (!tree_electron) {
        std::cerr << "Error: No se encontró el árbol 'Events'." << std::endl;
        return 1;
    }

    // Crear un archivo de salida ROOT
    TFile *outputFile = TFile::Open(argv[2], "RECREATE");
    if (!outputFile || outputFile->IsZombie()) {
        std::cerr << "Error al abrir el archivo de salida ROOT." << std::endl;
        return 1;
    }
    // Crear vectores para almacenar las variables de electrones
    std::vector<float>* electron_pt = new std::vector<float>();
    std::vector<float>* electron_eta = new std::vector<float>();
    std::vector<float>* electron_e = new std::vector<float>();
    std::vector<float>* electron_ch = new std::vector<float>();
    std::vector<float>* electron_iso = new std::vector<float>();
    std::vector<bool>* electron_isTight = new std::vector<bool>();

    tree_electron->SetBranchAddress("electron_pt", &electron_pt);
    tree_electron->SetBranchAddress("electron_eta", &electron_eta);  
    tree_electron->SetBranchAddress("electron_e", &electron_e);      
    tree_electron->SetBranchAddress("electron_iso", &electron_iso);
    tree_electron->SetBranchAddress("electron_isTight", &electron_isTight);

    TTree *filteredTree = tree_electron->CloneTree(0);

    // Obtener el número de entradas en el árbol
    Long64_t nEntries = tree_electron->GetEntries();
    // Procesar las entradas
    for (Long64_t i = 0; i < nEntries; ++i) {
        tree_electron->GetEntry(i);

        // Verificar si todos los electrones tienen pt > 50
        bool passFilter = true;
        for (size_t j = 0; j < electron_pt->size(); ++j) {
        float eta = electron_eta->at(j);
            if (electron_isTight -> at(j) || electron_pt->at(j) < 20.0 ||
                std::abs(eta) > 2.47 || (std::abs(eta) > 1.327 && std::abs(eta) < 1.52)
                || electron_e -> at(j) < 20
                || electron_iso -> at(j) > 2)
     {
                passFilter = false;  // Si alguno no pasa, el evento es descartado
                break;
                std::cout << "Evento " << i << " pasa el filtro con pt: " << electron_pt->at(j) << std::endl;
            }
        }
    
   // (std::abs(electron_eta->at(j)) > 1.37 && std::abs(electron_eta->at(j)) < 1.52) || (std::abs(electron_eta->at(j)) > 1.57 && std::abs(electron_eta->at(j)) < 2.4)
    
        // Si el evento pasa el filtro, añadirlo al árbol filtrado
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
