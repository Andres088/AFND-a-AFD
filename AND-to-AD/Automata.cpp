#include "Automata.h"


Automata::Automata(string nom)
{
	nombre = nom;
}

void Automata::colocar_valor_alfabeto(string valor) {
	alfabeto.push_back(valor);
}

void Automata::colocar_estado(Estado e) {
	estados.push_back(e);
}

string Automata::buscar_valor_alfabeto(int i) {

	return alfabeto.at(i);
}

string Automata::mostrar_funciones() {

	int num_estados = estados.size();
	string cadena("Tabla del automata "+ nombre + "\n\n");

	for (int n = 0; n < num_estados; n++) { // Nivel de estados

		Estado e = estados.at(n);
		int num_funciones = e.get_num_funciones();

		for (int i = 0; i < num_funciones; i++) { // Nivel de funciones

			Funcion f = e.get_funcion(i);
			vector<string> outputs = f.get_outputs();
			int num_outputs = outputs.size();
			string output = "";

			for (int j = 0; j < num_outputs; j++) { // Nivel de outputs

				output = output + " " + outputs.at(j);
			}
			cadena = cadena + "f(" + e.get_nombre() + "," + f.get_input() + "): " + output + "\n";
		}
		cadena = cadena + "\n";
	}

	cadena += "F={";
	int con = 0;
	for (int l = 0; l < num_estados; l++) {

		Estado est = estados.at(l);
		if (est.get_final()) {
			if (con > 0) cadena += ",";
			cadena += est.get_nombre();
			con += 1;
		}
	}
	cadena += "}\n";
	return cadena;
}

void Automata::colocar_nombre(string nom) {
	nombre = nom;
}

void Automata::convertir_a_AFD() {

	// Busca outputs multiples en el automata para reemplazarlos por estados unicos

	vector <vector<string>> outputs_multiples_recorridos;
	vector<string> outputs_multiples = encontrar_outputs_multiples(outputs_multiples_recorridos);
	vector<Estado> nuevos_estados;
	int iteracion = 0;
	

	while (outputs_multiples.size()!=0) { // Verifica que no hayan outputs multiples

		outputs_multiples_recorridos.push_back(outputs_multiples);

		iteracion++;

		// Se crea un nombre para un estado unico nuevo

		string ultimo_estado;
		int ultimo_numero = estados.size();
		stringstream ss;
		ss << "S" << ultimo_numero;
		ultimo_estado = ss.str();
		
		// Se crea un nuevo estado en base a la union de los estados del output multiple encontrado

		vector<Estado>estados_a_unir;
		int num_outputs = outputs_multiples.size();

		for (int i = 0; i < num_outputs; i++) {
			estados_a_unir.push_back(encontrar_estado(outputs_multiples.at(i)));
		}
		Estado nuevo_estado = unir_estados(estados_a_unir,ultimo_estado, ultimo_numero);

		colocar_estado(nuevo_estado);
		nuevos_estados.push_back(nuevo_estado);

		// Se buscan nuevos outputs multiples que no hayan sido recorridos

		outputs_multiples = encontrar_outputs_multiples(outputs_multiples_recorridos);
	}

	// Se reemplazan los outputs multiples por los nuevos estados

	int num_nuevos_estados = nuevos_estados.size();
	for (int l = 0; l < num_nuevos_estados; l++) {
		Estado nuevo_estado = nuevos_estados.at(l);
		outputs_multiples = nuevo_estado.get_estados_contenidos();
		reemplazar_outputs_por_nuevo_estado(outputs_multiples, nuevo_estado);
	}

	// Se crea el estado final con resultados nulos

	if (iteracion > 0) { 

		string ultimo_estado;
		int ultimo_numero = estados.size();
		stringstream ss;
		ss << "S" << ultimo_numero;
		ultimo_estado = ss.str();
		Estado e(ultimo_estado, ultimo_numero);
		
		int num_alfabeto = alfabeto.size();

		for (int i = 0; i < num_alfabeto; i++) {

			string nombre_funcion;
			stringstream ss;
			ss << "f" << i;
			nombre_funcion = ss.str();
			vector<string> output; output.push_back("--");
			Funcion f(nombre_funcion, alfabeto.at(i), output);
			e.colocar_funcion(f);
		}
		colocar_estado(e);

		// Se renombran los outputs nulos con el nombre del estado final

		vector<string> output_nulo; 
		output_nulo.push_back("--");
		reemplazar_outputs_por_nuevo_estado(output_nulo, e);
	}
}

Estado Automata::unir_estados(vector<Estado> estados, string nombre, int nuevo_numero) {

	// Se crea un nuevo estado en base a la unión de los distintos estados de output multiple
	// el nuevo estado herada las funciones de los estados que lo componen
	// tambien herada si alguno de los estados es de aceptacion

	Estado nuevo_estado(nombre, nuevo_numero);
	
	vector<Funcion> funciones_base = estados.at(0).get_funciones(); // Funciones del primer estado
	nuevo_estado.colocar_estado_contenido(estados.at(0).get_nombre());
	if (estados.at(0).get_final()) nuevo_estado.set_final(true);

	int num_estados = estados.size();
	int num_funciones = funciones_base.size();

	for (int i = 1; i < num_estados; i++) { // Nivel de estados

		if (estados.at(i).get_final()) nuevo_estado.set_final(true);
		nuevo_estado.colocar_estado_contenido(estados.at(i).get_nombre());
		vector<Funcion> funciones_agregar = estados.at(i).get_funciones();
		
		for (int y = 0; y < num_funciones; y++) { // Nivel de funciones
			Funcion f = funciones_agregar.at(y);
			funciones_base.at(y).agregar_outputs(f);
		}
	}
	nuevo_estado.colocar_funcion(funciones_base);
	return nuevo_estado;
}

Estado Automata::encontrar_estado(string nombre) {
	
	// Busca un estado en el Automata, si no lo encuentra retorna un estado de error

	int num_estados = estados.size();
	for (int i = 0; i < num_estados; i++) {
		if (estados.at(i).get_nombre() == nombre) return estados.at(i);
	}
	Estado error("S1989", 1989); 
	return error;
}

vector<string> Automata::encontrar_outputs_multiples(vector<vector<string>> recorridos) {

	// Busca outputs multiples en cada una de las funciones de transicion de cada estado del automata
	// Los resultados no pueden estar en la lista de outputs multiples recorridos
	// Si no encuentra alguno, devuelve una lista vacia

	int num_estados = estados.size();

	for (int n = 0; n < num_estados; n++) { // Nivel de estados

		Estado e = estados.at(n);
		int num_funciones = e.get_num_funciones();

		for (int i = 0; i < num_funciones; i++) { // Nivel de funciones

			Funcion f = e.get_funcion(i);
			vector<string> outputs = f.get_outputs();
			int num_outputs = outputs.size();
			if (num_outputs > 1) {
				if (!(find(recorridos.begin(), recorridos.end(), outputs) != recorridos.end()))
				return outputs;
			}
		}
	}
	vector<string> vacio; 
	return vacio;
}

void Automata::reemplazar_outputs_por_nuevo_estado(vector<string> outputs_buscados, Estado nuevo_estado) {

	// Reemplaza los outputs de cada estado con un nuevo nombre de estado asignado

	vector<string> nuevo_output;
	nuevo_output.push_back(nuevo_estado.get_nombre());

	int num_estados = estados.size();

	for (int n = 0; n < num_estados; n++) { 

		estados.at(n).reemplazar_outputs(outputs_buscados, nuevo_output);
	}

}

void Automata::remover_estados_no_accesibles() {

	// Busca y remueve los estados no accesibles desde el estado inicial S0

	vector<Estado> estados_antiguos = estados;
	vector<string>estados_accedidos;
	estados_accedidos.push_back("S0");
	int num_estados = estados.size();
	int num_estados_accedidos = 1;
	Estado estado_activo = estados.at(0);
	vector<string>estados_revisados;
	estados_revisados.push_back("S0");
	vector<Estado>estados_actualizados;
	
	while (num_estados_accedidos < num_estados) { 

		vector<string> outputs_estado_activo = estado_activo.outputs_unicos();
		int num_outputs_unicos = outputs_estado_activo.size();

		for (int i = 0; i < num_outputs_unicos; i++) { 

			string output = outputs_estado_activo.at(i);

			if (!Estado::existe(output, estados_accedidos)){
				if (output != "--") {
					estados_accedidos.push_back(output);
				}
			}
		}

		num_estados_accedidos = estados_accedidos.size();
		bool finalizar_while = false;

		for (int j = 0; j < num_estados_accedidos; j++) { 

			string estado_accedido = estados_accedidos.at(j);
			if (!Estado::existe(estado_accedido, estados_revisados)) {
				estados_revisados.push_back(estado_accedido);
				estado_activo = encontrar_estado(estado_accedido);
				break;
			}

			if (j == num_estados_accedidos - 1) {
				finalizar_while = true;
			}
		}
		
		if (finalizar_while) break;
	}

	num_estados_accedidos = estados_accedidos.size();
	sort(estados_accedidos.begin(), estados_accedidos.end());

	for (int m = 0; m < num_estados_accedidos; m++) {
		Estado est = encontrar_estado(estados_accedidos.at(m));
		estados_actualizados.push_back(est);
	}
	
	estados = estados_actualizados; 
}


void Automata::renombrar() {

	// Se renombran los estados con la numeracion correlativa despues de haber removido los estados no accesibles

	vector<Estado> estados_antiguos = estados;
	int num_estados = estados_antiguos.size();

	vector<string> antiguos_nombres;
	vector<string> nuevos_nombres;
	vector<Estado> estados_renombrados;


	for (int n = 0; n < num_estados; n++) {

		Estado estad = estados_antiguos.at(n);

		if (estad.get_numero() != n) {

			antiguos_nombres.push_back(estad.get_nombre());

			string nuevo_nombre;
			stringstream ss;
			ss << "S" << n;
			nuevo_nombre = ss.str();
			nuevos_nombres.push_back(nuevo_nombre);
		}
	}

	for (int l = 0; l < num_estados; l++) {

		Estado estado = estados_antiguos.at(l);
		estado.renombrar(antiguos_nombres, nuevos_nombres);
		estados_renombrados.push_back(estado);
	}

	estados = estados_renombrados;
}
