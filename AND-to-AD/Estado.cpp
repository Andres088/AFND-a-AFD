#include "Estado.h"


Estado::Estado(string nom)
{
	nombre = nom;
	existe = true;
}

string Estado::get_nombre() {
	return nombre;
}

vector<Funcion> Estado::get_funciones() {
	return funciones;
}

void Estado::set_existe(bool existencia) {
	existe = existencia;
}

bool Estado::get_existe() {
	return existe;
}

bool Estado::get_final() {
	return final;
}

void Estado::set_final(bool fin) {
	final = fin;
}

int Estado::get_num_funciones() {

	return funciones.size();
}

Funcion Estado::get_funcion(int n) {

	return funciones.at(n);
}

void Estado::colocar_estado_contenido(string estado) {
	estados_contenidos.push_back(estado);
}

void Estado::colocar_funcion(Funcion funcion) {
	funciones.push_back(funcion);
}

void Estado::colocar_funcion(vector<Funcion> nuevas_funciones) {
	funciones = nuevas_funciones;
}

void Estado::reemplazar_outputs(vector<string> outputs_buscados, vector<string> outputs_nuevos) {

	int num_funciones = funciones.size();

	for (int i = 0; i < num_funciones; i++) { // Nivel de funciones

		vector<string> outputs = funciones.at(i).get_outputs();
		if (comparar_outputs(outputs, outputs_buscados)) funciones.at(i).reemplazar_outputs(outputs_nuevos);
	}
}

bool Estado::comparar_outputs(vector<string> outputs1, vector<string> outputs2) {

	bool match = true;
	int num_outputs1 = outputs1.size();
	int num_outputs2 = outputs2.size();

	if (num_outputs1 != num_outputs2) return false;

	for (int i = 0; i < num_outputs1; i++) {

		string output1 = outputs1.at(i);
		bool temp = false;

		for (int j = 0; j < num_outputs2; j++) {

			string output2 = outputs2.at(j);
			if (output1 == output2) temp = temp || true;
		}
		match = match && temp;
	}
	return match;
}

vector<string> Estado::outputs_unicos() {

	vector<string> outputs_unicos;

	int num_funciones = funciones.size();

	for (int i = 0; i < num_funciones; i++) { // Nivel de funciones

		vector<string> outputs = funciones.at(i).get_outputs();
		int num_outputs = outputs.size();

		for (int j = 0; j < num_outputs; j++) { // Nivel de outputs

			string output = outputs.at(j);
			if (!(std::find(outputs_unicos.begin(), outputs_unicos.end(), output) != outputs_unicos.end())) {
				if (output != "--") {
					outputs_unicos.push_back(output);
				}
			}
		}
	}
	return outputs_unicos;
}