#include "Estado.h"


Estado::Estado(string nom, int num)
{
	nombre = nom;
	numero = num;
}

string Estado::get_nombre() {
	return nombre;
}

vector<Funcion> Estado::get_funciones() {
	return funciones;
}

int Estado::get_numero() {
	return numero;
}

void Estado::set_numero(int num) {
	numero = num;
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

vector<string> Estado::get_estados_contenidos() {
	return estados_contenidos;
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

void Estado::renombrar(vector<string> antiguos_nombres, vector<string> nuevos_nombres) {

	// Se crean nuevas funciones para el estado con el nombre(numeracion) corregido

	vector<Funcion> nuevas_funciones;
	int num_funciones = funciones.size();

	for (int i = 0; i < num_funciones; i++) { // Nivel de funciones

		
		Funcion funcion_actual = funciones.at(i);
		string id_funcion_nueva = funcion_actual.get_id();
		string input_funcion_nueva = funcion_actual.get_input();
		vector<string> outputs_funcion_nueva;

		vector<string> outputs = funciones.at(i).get_outputs();
		int num_outputs = outputs.size();

		for (int j = 0; j < num_outputs; j++) { // Nivel de outputs

			string output = outputs.at(j);

			if (existe(output,antiguos_nombres)) {

				int index = encontrar(output, antiguos_nombres);
				outputs_funcion_nueva.push_back(nuevos_nombres.at(index));
			}
			else outputs_funcion_nueva.push_back(output);
		}

		Funcion funcion_nueva(id_funcion_nueva, input_funcion_nueva, outputs_funcion_nueva);
		nuevas_funciones.push_back(funcion_nueva);
	}

	funciones = nuevas_funciones;

	// Se modifica el nombre del estado, de ser el caso

	if (existe(nombre, antiguos_nombres)) {
		int inde = encontrar(nombre,antiguos_nombres);
		nombre = nuevos_nombres.at(inde);
	}
}


bool Estado::existe(string busqueda, vector<string> lista) {

	return find(lista.begin(), lista.end(), busqueda) != lista.end();
}


int Estado::encontrar(string busqueda, vector<string> lista) {

	vector<string>::iterator itr = find(lista.begin(), lista.end(), busqueda);

	if (itr != lista.cend()) {
		return distance(lista.begin(), itr);
	}
	else {
		return NULL;
	}
}