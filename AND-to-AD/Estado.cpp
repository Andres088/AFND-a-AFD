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

void Estado::colocar_si_existe(bool existencia) {
	existe = existencia;
}

bool Estado::indicar_si_existe() {
	return existe;
}

void Estado::colocar_estado_contenido(string estado) {
	estados_contenidos.push_back(estado);
}

void Estado::colocar_estado_contenido(vector<string> estados) {
	estados_contenidos = estados;
}

void Estado::colocar_funcion(Funcion funcion) {
	funciones.push_back(funcion);
}

void Estado::colocar_funcion(vector<Funcion> nuevas_funciones) {
	funciones = nuevas_funciones;
}

int Estado::get_num_funciones() {

	return funciones.size();
}

Funcion Estado::get_funcion(int n) {

	return funciones.at(n);
}

void Estado::reemplazar_outputs(vector<string> outputs_buscados, vector<string> outputs_nuevos) {

	int num_funciones = funciones.size();

	for (int i = 0; i < num_funciones; i++) { // Nivel de funciones

		vector<string> outputs = funciones.at(i).get_outputs();
		if (Ayuda::comparar_outputs(outputs, outputs_buscados)) funciones.at(i).reemplazar_outputs(outputs_nuevos);
	}
}

void Estado::mostrar_detalle() {

	cout <<"Detalle del estado: " << nombre << endl;

	cout << "Estados contenidos: ";
	int num_estados = estados_contenidos.size();
	for (int i = 0; i < num_estados; i++) {
		cout << estados_contenidos.at(i) << " ";
	}

	cout << endl << "Funciones: " << endl;
	int num_funciones = funciones.size();

	for (int i = 0; i < num_funciones; i++) { // Nivel de funciones

		Funcion f = funciones.at(i);
		vector<string> outputs = f.get_outputs();
		int num_outputs = outputs.size();
		string output = "";

		for (int j = 0; j < num_outputs; j++) { // Nivel de outputs

			output = output + " " + outputs.at(j);
		}
		cout <<  "f(" << nombre << "," << f.get_input() << "): " << output << endl;
	}
}

vector<string> Estado::outputs_por_valor(string valor) {

	int num_funciones = funciones.size();
	vector <string> outputs_nuevo;

	for (int i = 0; i < num_funciones; i++) { // Nivel de funciones

		string input = funciones.at(i).get_input();
		vector<string> outputs = funciones.at(i).get_outputs();
		int num_outputs = outputs.size();

		if (input == valor) {

			for (int j = 0; j < num_outputs; j++) { // Nivel de outputs
				string output = outputs.at(j);
				outputs_nuevo.push_back(output);
			}
		}
	}

	return outputs_nuevo;
}