#pragma once
#include <stdio.h>
#include <string>
#include <iostream>
#include "Funcion.h"
#include <vector>
#include <algorithm>
using namespace std;

class Estado
{
	string nombre;
	bool final; // Si es un estado de aceptación
	vector<string> estados_contenidos; // Si es un estado producto de una unión
	vector<Funcion> funciones;
	 

public:

	Estado(string);
	string get_nombre();
	bool get_final();
	void set_final(bool);
	Funcion get_funcion(int);
	vector<Funcion> get_funciones();
	int get_num_funciones();
	vector<string> get_estados_contenidos();
	void colocar_estado_contenido(string);
	void colocar_funcion(Funcion);
	void colocar_funcion(vector<Funcion>);
	void reemplazar_outputs(vector<string>, vector<string>);
	bool comparar_outputs(vector<string>, vector <string>);
	vector<string> outputs_unicos();
};

