#pragma once
#include <stdio.h>
#include <string>
#include <iostream>
#include "Funcion.h"
#include <vector>
using namespace std;

class Estado
{
	bool final;
	string nombre;
	vector<string> estados_contenidos; 
	vector<Funcion> funciones;
	bool existe; 



public:

	Estado(string);
	string get_nombre();
	void colocar_si_existe(bool);
	bool indicar_si_existe();
	void colocar_estado_contenido(string);
	void colocar_estado_contenido(vector<string>);
	void colocar_funcion(Funcion);
	void colocar_funcion(vector<Funcion>);
	int get_num_funciones();
	Funcion get_funcion(int);
	vector<Funcion> get_funciones();
	vector<string> outputs_por_valor(string);
	void reemplazar_outputs(vector<string>, vector<string>);
	void mostrar_detalle();
};

