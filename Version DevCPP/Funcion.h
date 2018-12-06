#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include "Ayuda.h"
using namespace std;

class Funcion
{
	string id;
	string input;
	vector<string> outputs;

public:

	Funcion(string, string, vector<string>);
	string get_input();
	vector<string> get_outputs();
	string get_id();
	bool existe_output(string);
	void agregar_outputs(Funcion);
	void reemplazar_outputs(vector<string>);


};

