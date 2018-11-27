#include "Ayuda.h"

Ayuda::Ayuda()
{

}

bool Ayuda::comparar_outputs(vector<string> outputs1, vector<string> outputs2) {

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



