#include "Funcion.h"
#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>

Funcion::Funcion(string id_, string in, vector<string> out)
{
	id = id_;
	input = in;
	outputs = out;
}

string Funcion::get_input() {
	return input;
}

vector<string> Funcion::get_outputs() {
	return outputs;
}

string Funcion::get_id() {
	return id;
}

void Funcion::reemplazar_outputs(vector<string> outputs_nuevos) {
	outputs = outputs_nuevos;
}

bool Funcion::existe_output(string busqueda) {
	int num_outputs = outputs.size();
	for (int i = 0; i < num_outputs; i++) {
		if (outputs.at(i) == busqueda) return true;
	}
	return false;
}

void Funcion::agregar_outputs(Funcion otra_funcion) {
	
	if (existe_output("--") && (!otra_funcion.existe_output("--"))) {
		outputs = otra_funcion.get_outputs();
	}

	if ((!existe_output("--") && (!otra_funcion.existe_output("--")))) {

		vector <string> outputs_nuevos = otra_funcion.get_outputs();
		int num_outputs_nuevos = outputs_nuevos.size();

		for (int i = 0; i < num_outputs_nuevos; i++) {
			string output_nuevo = outputs_nuevos.at(i);
			if (!existe_output(output_nuevo)) {
				outputs.push_back(output_nuevo);
			}
		}
	}
}
