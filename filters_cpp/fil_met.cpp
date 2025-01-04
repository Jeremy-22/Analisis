// *ROOT*
// Version: ROOT 6.30/08
// Descripcion; Filtra y combina las ramas no filtradas en un mismo archivo .root
//
// Creado:  03 Enero 2025
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
    TTree* tree_met = (TTree*)file->Get("mymets/Events");
    if (!tree_met) {
        std::cerr << "Error: No se encontró el árbol 'Events'." << std::endl;
        file->Close();
        return 1;
    }
    TBranch* branch_met_pt = tree_met->GetBranch("met_pt");
    if (!branch_met_pt) {
        std::cerr << "Error: No se encontró la rama 'met_pt' en el árbol." << std::endl;
        file->Close();
        return 1;
    }
    TFile *outputFile = TFile::Open(argv[2], "RECREATE");
    if (!outputFile || outputFile->IsZombie()) {
        std::cerr << "Error al abrir el archivo de salida ROOT." << std::endl;
        file->Close();
        return 1;
    }
    float met_significance = 0.0f;
    float met_e = 0.0f;
    tree_met->SetBranchAddress("met_pt", &met_pt);
    tree_met->SetBranchAddress("met_significance", &met_significance);
    tree_met->SetBranchAddress("met_e", &met_e);    

    TTree* filteredTree = tree_met->CloneTree(0);

    float filtered_met_pt = 0.0f;
    float filtered_met_significance = 0.0f; 
    float filtered_met_e = 0.0f; 
    filteredTree->Branch("met_pt", &filtered_met_pt, "met_pt/F");
    filteredTree->Branch("met_significance", &filtered_met_significance, "met_significance/F");    
    filteredTree->Branch("met_e", &filtered_met_e, "met_e/F");    

    Long64_t nEntries = tree_met->GetEntries();

    for (Long64_t i = 0; i < nEntries; ++i) {
        tree_met->GetEntry(i);

        if (met_pt > 20.0 && met_significance > 5 && met_e > 20){
            filtered_met_pt = met_pt;
            filteredTree->Fill();
        }
    }


    filteredTree->Write();
    outputFile->Close();
    file->Close();

    std::cout << "Filtro aplicado. Archivo generado: " << argv[2] << std::endl;
    return 0;
}
