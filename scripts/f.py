#-*- coding: utf-8 -*-
# *ROOT*
# Version: ROOT 6.30/08
# Descripcion; combina todos los archivos .root que se generaron de cada filtro.
#
# Creado:  11 Dec 2024
# Last Modified: 16 Dec 2024
# Author: Jeremy Rangel
# Contacto: jeryrangmart@gmail.com

import os
import subprocess
import ROOT

input_file = "./data/myoutput20.root"

filters = [
    "fi_muons",
    "fil_electrons",
    "fil_photons",
    "fil_met",
    "fi_tau",
    "fil_trigEvent",
    "fil_track",
    "fil_pvertex",
    "fil_genparticle"
]

filters_dir = "./filters_cpp"

output_dir = "./data"

if not os.path.exists(output_dir):
    os.mkdir(output_dir)

for filter_name in filters:
    cpp_file = os.path.join(filters_dir, "{0}.cpp".format(filter_name))
    
    print("Compilando el archivo: {0}".format(cpp_file))
    if not os.path.exists(cpp_file):
        print("El archivo {0} no existe. Verifica la ruta.".format(cpp_file))
        exit(1)
    
    binary_file = os.path.join(filters_dir, "{0}".format(filter_name))
    output_file = os.path.join(output_dir, "{0}.root".format(filter_name))
    
    print("Compilando {0}...".format(cpp_file))
    compile_cmd = "g++ -o {0} {1} $(root-config --cflags --libs)".format(binary_file, cpp_file)
    compile_result = subprocess.call(compile_cmd, shell=True)
    if compile_result != 0:
        print("Error al compilar {0}".format(cpp_file))
        exit(1)

    print("Ejecutando {0}...".format(binary_file))
    exec_cmd = "{0} {1} {2}".format(binary_file, input_file, output_file)
    exec_result = subprocess.call(exec_cmd, shell=True)
    if exec_result != 0:
        print("Error al ejecutar {0}".format(binary_file))
        exit(1)

combine_cpp = "./filters_cpp/combine_root_files.cpp"
combine_binary = "./filters_cpp/combine_root_files"

if not os.path.exists(combine_cpp):
    print("Error: El archivo combine_root_files.cpp no existe.")
    exit(1)

print("Compilando el script de combinación...")
compile_cmd = "g++ -o {0} {1} $(root-config --cflags --libs)".format(combine_binary, combine_cpp)
compile_result = subprocess.call(compile_cmd, shell=True)
if compile_result != 0:
    print("Error al compilar combine_root_files.cpp")
    exit(1)

print("Ejecutando el binario de combinación...")
exec_cmd = "{0}".format(combine_binary)
exec_result = subprocess.call(exec_cmd, shell=True)
if exec_result != 0:
    print("Error al ejecutar combine_root_files")
    exit(1)

print("Combinación completada exitosamente.")
