#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include "Automata.h"
#include <algorithm>
using namespace std;

int main()
{
	// Se crea el automata y variables iniciales

	Automata automata1("AFND");
	int num_estados;
	int num_valores_alfabeto;
	
	cout << "Conversion de un AFND a AFD.\n\n";
	cout << "**********************************\n\n\n";
	cout << "Definir el AFND = (S, Q, f, q0, F)\n\n";
	cout << "Donde :\n\tS:  Es el alfabeto de entrada.\n";
	cout << "\tQ:  Es el conjunto finito y no vacio de los estados del Automata.\n";
	cout << "\tf:  Es la funcion de transicion que indica en que situaciones el\n";
	cout << "\t    automata pasa de un estado a otro.\n";
	cout << "\tq0: Es el estado inicial = S0.\n";
	cout << "\tF:  Es el conjunto de estados finales de aceptacion.\n\n";



	// Se escoge camino para construir automata

	string camino;
	cout << "Escriba A para ingresar su propio automata o P para utilizar uno precargado." << endl;
	cin >> camino;

	// Se ingresan valores para construir el automata

	if (camino=="A") {

		cout << "Ingrese el numero de valores del alfabeto: "; cin >> num_valores_alfabeto;

		// Se insertan los estados del automata

		for (int n = 1; n <= num_valores_alfabeto; n++) {

			string valor;
			cout << "Ingrese el valor " << n << ": "; cin >> valor;
			automata1.colocar_valor_alfabeto(valor);
		}
		cout << "Ingrese el numero de estados: "; cin >> num_estados;

		// Se insertan las funciones del automata
	
		for (int n = 0; n < num_estados; n++) { // Nivel de estados

			stringstream ss;
			ss << "S" << n;
			string estado = ss.str();
			Estado e(estado);
			e.colocar_estado_contenido(estado);

			for (int i = 0; i < num_valores_alfabeto; i++) { // Nivel de funciones

				string resultado;
				string valor_alfabeto = automata1.buscar_valor_alfabeto(i);
				vector<string> outputs;
				int num_outputs;

				cout << "Ingrese el numero de outputs de " << estado << " para " << valor_alfabeto << ": "; cin >> num_outputs;

				if (!num_outputs == 0) {

					for (int j = 0; j < num_outputs; j++) { // Nivel de outputs

						cout << "f(" << estado << "," << valor_alfabeto << "): "; cin >> resultado;
						outputs.push_back(resultado);
					}
				}
				else {
					outputs.push_back("--");
				}

				stringstream ss2;
				ss2 << "f" << i;
				string nombre_funcion = ss2.str();

				Funcion f(nombre_funcion, valor_alfabeto, outputs);
				e.colocar_funcion(f);
			}
			automata1.colocar_estado(e);
		}
	}

	// Se genera automaticamente AFND

	if (camino == "P") {

		automata1.colocar_valor_alfabeto("0");
		automata1.colocar_valor_alfabeto("1");

		vector<string> v_S0_0; v_S0_0.push_back("--");
		Funcion f_S0_0("f0", "O", v_S0_0);
		vector<string> v_S0_1; v_S0_1.push_back("S1");
		Funcion f_S0_1("f1", "1", v_S0_1);
		Estado S0("S0"); S0.colocar_funcion(f_S0_0); S0.colocar_funcion(f_S0_1);
		automata1.colocar_estado(S0);


		vector<string> v_S1_0; v_S1_0.push_back("S0"); v_S1_0.push_back("S2");
		Funcion f_S1_0("f0", "0", v_S1_0);
		vector<string> v_S1_1; v_S1_1.push_back("--");
		Funcion f_S1_1("f1", "1", v_S1_1);
		Estado S1("S1"); S1.colocar_funcion(f_S1_0); S1.colocar_funcion(f_S1_1);
		automata1.colocar_estado(S1);

		vector<string> v_S2_0; v_S2_0.push_back("S0");
		Funcion f_S2_0("f0", "0", v_S2_0);
		vector<string> v_S2_1; v_S2_1.push_back("S0");
		Funcion f_S2_1("f1", "1", v_S2_1);
		Estado S2("S2"); S2.colocar_funcion(f_S2_0); S2.colocar_funcion(f_S2_1);
		automata1.colocar_estado(S2);
		
	}
	
	if (camino == "A" || camino == "P") {

		cout << endl << automata1.mostrar_funciones() << endl;
		// Se convierte el automata de AFND a AFD
		Automata automata2 = automata1; automata2.colocar_nombre("AFD");
		automata2.convertir_a_AFD();
		cout << endl << automata2.mostrar_funciones() << endl;
	}
	


	// Fin del programa
	char fin;
	cin >> fin;
	return 0;
}


