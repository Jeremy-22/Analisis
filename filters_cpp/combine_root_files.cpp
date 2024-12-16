// *ROOT*
// Version: ROOT 6.30/08
// Descripcion; combina todos los archivos .root que se generaron de cada filtro.
//
// Creado:  13 Dec 2024
// Last Modified: 16 Dec 2024
// Author: Jeremy Rangel
// Contacto: jeryrangmart@gmail.com

#include <iostream>
#include <iostream>
#include <vector>
#include <string>
#include <TFile.h>
#include <TTree.h>

int main(int argc, char** argv) {
    std::vector<std::string> input_files;
    input_files.push_back("data/fil_electrons.root");
    input_files.push_back("data/fi_muons.root");
    input_files.push_back("data/fil_photons.root");
    input_files.push_back("data/fil_met.root");
    input_files.push_back("data/fi_tau.root");
    input_files.push_back("data/fil_trigEvent.root");
    input_files.push_back("data/fil_track.root");
    input_files.push_back("data/fil_pvertex.root");
    input_files.push_back("data/fil_genparticle.root");

    std::string output_file_name = "data/combined.root";

    TFile* output_file = TFile::Open(output_file_name.c_str(), "RECREATE");
    if (!output_file || output_file->IsZombie()) {
        std::cerr << "Error al crear el archivo de salida: " << output_file_name << std::endl;
        return 1;
    }

    TDirectory* events_dir = output_file->mkdir("Events");
    if (!events_dir) {
        std::cerr << "Error al crear el directorio 'Events' en el archivo de salida" << std::endl;
        return 1;
    }

    TTree* combined_fil_electrons = nullptr;
    TTree* combined_fi_muons = nullptr;
    TTree* combined_fil_met = nullptr;
    TTree* combined_fil_photons = nullptr;
    TTree* combined_fi_tau = nullptr;
    TTree* combined_fil_trigEvent = nullptr;
    TTree* combined_fil_track = nullptr;
    TTree* combined_fil_pvertex = nullptr;
    TTree* combined_fil_genparticle = nullptr;

    for (size_t i = 0; i < input_files.size(); ++i) {
        const std::string& file_name = input_files[i];
        TFile* input_file = TFile::Open(file_name.c_str(), "READ");
        if (!input_file || input_file->IsZombie()) {
            std::cerr << "Error al abrir el archivo de entrada: " << file_name << std::endl;
            continue;
        }

        // Intentar obtener el árbol 'Events'
        TTree* events_tree = dynamic_cast<TTree*>(input_file->Get("Events"));
        if (events_tree) {
            std::cout << "Árbol 'Events' encontrado en el archivo: " << file_name << std::endl;

            // Si el archivo es de electrones (fil_electrons.root)
            if (file_name.find("fil_electrons") != std::string::npos) {
                if (!combined_fil_electrons) {
                    // Crear una copia inicial para el archivo de electrones
                    events_dir->cd();
                    combined_fil_electrons = events_tree->CloneTree(0);
                    if (!combined_fil_electrons) {
                        std::cerr << "Error al clonar el árbol 'Events' para electrones" << std::endl;
                        return 1;
                    }
                }
                // Agregar eventos del archivo actual al árbol combinado de electrones
                combined_fil_electrons->CopyEntries(events_tree);

            } 
            else if (file_name.find("fi_muons") != std::string::npos) {
                if (!combined_fi_muons) {
                    events_dir->cd();
                    combined_fi_muons = events_tree->CloneTree(0);
                    if (!combined_fi_muons) {
                        std::cerr << "Error al clonar el árbol 'Events' para muones" << std::endl;
                        return 1;
                    }
                }
                combined_fi_muons->CopyEntries(events_tree);
            }

            else if (file_name.find("fil_met") != std::string::npos) {
                if (!combined_fil_met) {
                    events_dir->cd();
                    combined_fil_met = events_tree->CloneTree(0);
                    if (!combined_fil_met) {
                        std::cerr << "Error al clonar el árbol 'Events' para met" << std::endl;
                        return 1;
                    }
                }
                combined_fil_met->CopyEntries(events_tree);
            }

            else if (file_name.find("fil_photons") != std::string::npos) {
                if (!combined_fil_photons) {
                    events_dir->cd();
                    combined_fil_photons = events_tree->CloneTree(0);
                    if (!combined_fil_photons) {
                        std::cerr << "Error al clonar el árbol 'Events' para photon" << std::endl;
                        return 1;
                    }
                }
                combined_fil_photons->CopyEntries(events_tree);
            }
      
            else if (file_name.find("fi_tau") != std::string::npos) {
                if (!combined_fi_tau) {
                    events_dir->cd();
                    combined_fi_tau = events_tree->CloneTree(0);
                    if (!combined_fi_tau) {
                        std::cerr << "Error al clonar el árbol 'Events' para tau" << std::endl;
                        return 1;
                    }
                }
                combined_fi_tau->CopyEntries(events_tree);
            }      
            else if (file_name.find("fil_trigEvent") != std::string::npos) {
                if (!combined_fil_trigEvent) {
                    events_dir->cd();
                    combined_fil_trigEvent = events_tree->CloneTree(0);
                    if (!combined_fil_trigEvent) {
                        std::cerr << "Error al clonar el árbol 'Events' para trigEvent" << std::endl;
                        return 1;
                    }
                }
                combined_fil_trigEvent->CopyEntries(events_tree);
            }

            else if (file_name.find("fil_track") != std::string::npos) {
                if (!combined_fil_track) {
                    events_dir->cd();
                    combined_fil_track = events_tree->CloneTree(0);
                    if (!combined_fil_track) {
                        std::cerr << "Error al clonar el árbol 'Events' para track" << std::endl;
                        return 1;
                    }
                }
                combined_fil_track->CopyEntries(events_tree);
            }

            else if (file_name.find("fil_pvertex") != std::string::npos) {
                if (!combined_fil_pvertex) {
                    events_dir->cd();
                    combined_fil_pvertex = events_tree->CloneTree(0);
                    if (!combined_fil_pvertex) {
                        std::cerr << "Error al clonar el árbol 'Events' para pvertex" << std::endl;
                        return 1;
                    }
                }
                combined_fil_pvertex->CopyEntries(events_tree);
            }

            else if (file_name.find("fil_genparticle") != std::string::npos) {
                if (!combined_fil_genparticle) {
                    events_dir->cd();
                    combined_fil_genparticle = events_tree->CloneTree(0);
                    if (!combined_fil_genparticle) {
                        std::cerr << "Error al clonar el árbol 'Events' para genparticle" << std::endl;
                        return 1;
                    }
                }
                combined_fil_genparticle->CopyEntries(events_tree);
            }

        } else {
            std::cerr << "No se encontró el árbol 'Events' en el archivo: " << file_name << std::endl;
        }

        input_file->Close();
        delete input_file; 
    }

    if (combined_fil_electrons) {
        events_dir->cd();
        combined_fil_electrons->Write("fil_electrons", TObject::kOverwrite);
    } else {
        std::cerr << "No se pudo escribir el árbol 'fil_electrons'." << std::endl;
    }

    if (combined_fi_muons) {
        events_dir->cd();
        combined_fi_muons->Write("fi_muons", TObject::kOverwrite);
    } else {
        std::cerr << "No se pudo escribir el árbol 'fi_muons'." << std::endl;
    }

    if (combined_fil_met) {
        events_dir->cd();
        combined_fil_met->Write("fil_met", TObject::kOverwrite);
    } else {
        std::cerr << "No se pudo escribir el árbol 'fil_met'." << std::endl;
    }
    if (combined_fil_photons) {
        events_dir->cd();
        combined_fil_photons->Write("fil_photons", TObject::kOverwrite);
    } else {
        std::cerr << "No se pudo escribir el árbol 'fil_photons'." << std::endl;
    }
   if (combined_fi_tau) {
        events_dir->cd();
        combined_fi_tau->Write("fi_tau", TObject::kOverwrite);
    } else {
        std::cerr << "No se pudo escribir el árbol 'fi_tau'." << std::endl;
    }    
    if (combined_fil_trigEvent) {
       events_dir->cd();
       combined_fil_trigEvent->Write("fil_trigEvent", TObject::kOverwrite);
    } else {
        std::cerr << "No se pudo escribir el árbol 'fil_trigEvent'." << std::endl;
    }
    if (combined_fil_track) {
       events_dir->cd();
       combined_fil_track->Write("fil_track", TObject::kOverwrite);
    } else {
        std::cerr << "No se pudo escribir el árbol 'fil_track'." << std::endl;
    }
    if (combined_fil_pvertex) {
       events_dir->cd();
       combined_fil_pvertex->Write("fil_pvertex", TObject::kOverwrite);
    } else {
        std::cerr << "No se pudo escribir el árbol 'fil_pvertex'." << std::endl;
    }    
    if (combined_fil_genparticle) {
       events_dir->cd();
       combined_fil_genparticle->Write("fil_genparticle", TObject::kOverwrite);
    } else {
        std::cerr << "No se pudo escribir el árbol 'fil_genparticle'." << std::endl;
    }    
    
    output_file->Close();
    delete output_file;

    std::cout << "Archivos combinados exitosamente en: " << output_file_name << std::endl;
    return 0;
}
