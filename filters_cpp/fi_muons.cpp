// *ROOT*
// Version: ROOT 6.30/08
// Descripcion; Filtra y combina las ramas no filtradas en un mismo archivo .root
//
// Creado:  09 Dec 2024
// Last Modified: 10 Dec 2024
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

    TTree tree_muon = (TTree)file->Get("mymuons/Events");
    if (!tree_muon) {
        std::cerr << "Error: No se encontró el árbol 'Events'." << std::endl;
        return 1;
    }

    TFile *outputFile = TFile::Open(argv[2], "RECREATE");
    if (!outputFile || outputFile->IsZombie()) {
        std::cerr << "Error al abrir el archivo de salida ROOT." << std::endl;
        return 1;
    }

    std::vector<float>* muon_pt = new std::vector<float>();
    std::vector<float>* muon_eta = new std::vector<float>();
    std::vector<float>* muon_e = new std::vector<float>();
    std::vector<float>* muon_pfreliso03all = new std::vector<float>();
    std::vector<float>* muon_pfreliso04all = new std::vector<float>();
    std::vector<float>* muon_tightid = new std::vector<float>();

    tree_muon->SetBranchAddress("muon_pt", &muon_pt);
    tree_muon->SetBranchAddress("muon_eta", &muon_eta);  
    tree_muon->SetBranchAddress("muon_e", &muon_e);      
    tree_muon->SetBranchAddress("muon_pfreliso04all", &muon_pfreliso04all);      
    tree_muon->SetBranchAddress("muon_pfreliso03all", &muon_pfreliso03all);      
    tree_muon->SetBranchAddress("muon_tightid", &muon_tightid);

    TTree *filteredTree = tree_muon->CloneTree(0);

    Long64_t nEntries = tree_muon->GetEntries();

    for (Long64_t i = 0; i < nEntries; ++i) {
        tree_muon->GetEntry(i);

        bool passFilter = true;
        for (size_t j = 0; j < muon_pt->size(); ++j) {
        float eta = muon_eta->at(j);
            if (muon_tightid->at(j) ==1 ||muon_pt->at(j) > 100.0 || muon_pt->at(j) < 5.0
                || std::abs(eta) > 2.4
                || muon_e -> at(j) < 5.0
                || muon_pfreliso03all -> at(j) <= 0 || muon_pfreliso03all -> at(j) <= 0 || muon_pfreliso03all -> at(j) > 0.30 || muon_pfreliso03all -> at(j) > 0.30)
             {
                passFilter = true;
                break;
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