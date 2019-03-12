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
		//if (iteracion == 7) break; // Pruebas

		// Se crea un nombre para un estado unico nuevo

		string ultimo_estado;
		int ultimo_numero = estados.size();
		stringstream ss;
		ss << "S" << ultimo_numero;
		ultimo_estado = ss.str();
		
		// Definir los estados a unir en base a outputs multiples

		vector<Estado>estados_a_unir;
		int num_outputs = outputs_multiples.size();

		for (int i = 0; i < num_outputs; i++) {
			estados_a_unir.push_back(encontrar_estado(outputs_multiples.at(i)));
		}
		Estado nuevo_estado = unir_estados(estados_a_unir,ultimo_estado);

		colocar_estado(nuevo_estado);
		nuevos_estados.push_back(nuevo_estado);

		 //cout << "Iteracion " << iteracion << endl; // Pruebas
		 //cout << mostrar_funciones(); // Pruebas

		// Se buscan nuevamente outputs multiples

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
		Estado e(ultimo_estado);
		
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

		// Se renombran los outputs nulos con el estado final

		vector<string> output_nulo; 
		output_nulo.push_back("--");
		reemplazar_outputs_por_nuevo_estado(output_nulo, e);
	}

}

Estado Automata::unir_estados(vector<Estado> estados, string nombre) {

	Estado nuevo_estado(nombre);
	
	vector<Funcion> funciones_base = estados.at(0).get_funciones(); // funciones del primer estado
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
	
	int num_estados = estados.size();
	for (int i = 0; i < num_estados; i++) {
		if (estados.at(i).get_nombre() == nombre) return estados.at(i);
	}
	Estado vacio("vacio");
	return vacio;
}

vector<string> Automata::encontrar_outputs_multiples(vector<vector<string>> recorridos) {

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

	vector<string> nuevo_output;
	nuevo_output.push_back(nuevo_estado.get_nombre());

	int num_estados = estados.size();

	for (int n = 0; n < num_estados; n++) { // Nivel de estados

		estados.at(n).reemplazar_outputs(outputs_buscados, nuevo_output);
	}

}

void Automata::remover_estados_no_accesibles() {

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

		// cout << "Estado activo " << estado_activo.get_nombre() << endl; // Pruebas
		// cout << "Outputs: "; // Pruebas

		for (int i = 0; i < num_outputs_unicos; i++) {

			string output = outputs_estado_activo.at(i);

			// cout << output << " "; // Pruebas

			if (!existe(output, estados_accedidos)){
				if (output != "--") {
					estados_accedidos.push_back(output);
				}
			}
		}

		// cout << endl << endl; // Pruebas

		num_estados_accedidos = estados_accedidos.size();
		bool finalizar_while = false;

		for (int j = 0; j < num_estados_accedidos; j++) {

			string estado_accedido = estados_accedidos.at(j);
			if (!existe(estado_accedido, estados_revisados)) {
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

	// Pruebas

	//for (int l = 0; l < num_estados_accedidos; l++) {
	//	cout << estados_accedidos.at(l) << " ";
	//}

	//cout << endl;

	//for (int k = 0; k < num_estados; k++) {
	//	Estado e = estados.at(k);
	//	cout << e.get_nombre() << " ";
	//}

	num_estados_accedidos = estados_accedidos.size();
	sort(estados_accedidos.begin(), estados_accedidos.end());

	for (int m = 0; m < num_estados_accedidos; m++) {
		Estado est = encontrar_estado(estados_accedidos.at(m));
		estados_actualizados.push_back(est);
	}
	
	estados = estados_actualizados;
}

bool Automata::existe(string busqueda, vector<string> lista) {

	return find(lista.begin(), lista.end(), busqueda) != lista.end();
}