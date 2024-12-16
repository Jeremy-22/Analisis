// *ROOT*
// Version: ROOT 6.30/08
// Descripcion; Filtra y combina las ramas no filtradas en un mismo archivo .root
//
// Creado:  07 Dec 2024
// Last Modified: 10 Dec 2024
// Author: Jeremy Rangel
// Contacto: jeryrangmart@gmail.com

#include <iostream>
#include <cmath>
#include <vector>
#include "TFile.h"
#include "TTree.h"
#include "TH1F.h"
#include "TMath.h"

void mass() {
    TFile *file = TFile::Open("myoutput_21.root");
    if (!file || file->IsZombie()) {
        std::cerr << "Error al abrir el archivo ROOT." << std::endl;
        return ;
    }
    TTree *tree_electron = (TTree*)file->Get("myelectrons/Events");
    if (!tree_electron) {
        std::cerr << "Error al cargar el árbol de electrones." << std::endl;
        return ;
    }
    TTree *tree_mets = (TTree*)file->Get("mymets/Events");
    if (!tree_mets) {
        std::cerr << "Error al cargar el árbol de MET." << std::endl;
        return ;
    }
    std::vector<float> *electron_pt = nullptr;
    std::vector<float> *electron_phi = nullptr;
    float met_pt = 0;
    float met_phi = 0;
    tree_electron->SetBranchAddress("electron_pt", &electron_pt);
    tree_electron->SetBranchAddress("electron_phi", &electron_phi);
    tree_mets->SetBranchAddress("met_pt", &met_pt);
    tree_mets->SetBranchAddress("met_phi", &met_phi);

    TH1F *h_mt = new TH1F("h_mt", "Masa Transversa del W;M_{T} (GeV/c^{2});Eventos", 100, 0, 200);

    Long64_t nEntries = tree_electron->GetEntries();
    for (Long64_t i = 0; i < nEntries; ++i) {
        tree_electron->GetEntry(i);
        tree_mets->GetEntry(i);

        if (electron_pt->size() > 0) {
            float pt_electron = electron_pt->at(0);
            float phi_electron = electron_phi->at(0);

            float delta_phi = TMath::Abs(phi_electron - met_phi);
            if (delta_phi > TMath::Pi()) delta_phi = 2 * TMath::Pi() - delta_phi;

            float mt = TMath::Sqrt(2 * pt_electron * met_pt * (1 - TMath::Cos(delta_phi)));

            h_mt->Fill(mt);
        }
    }

    TFile *output = TFile::Open("output_mt5.root", "RECREATE");
    h_mt->Write();
    output->Close();
    delete h_mt;
    file->Close();
    delete file;

    std::cout << "Cálculo completado. Histograma guardado en output_mt.root." << std::endl;
    return ;
}
