#include <iostream>
#include <vector>
#include <map>
#include <string>

#include "Simplex.h"

using namespace std;

int main()
{

	/*
		On r�cup�re les informations utiles pour initialiser la matrice
	*/

	int nActivityVars = 0; // Le nombre de variables d'activit�es associ� � notre probl�me
	int nSlackVars = 0;

	cout << "Entrez le nombre de variables d'activitees : " << std::endl;
	cin >> nActivityVars;

	cout << "Entrez le nombre de variables d'ecart : " << std::endl;
	cin >> nSlackVars;


	Simplex simplex(nActivityVars, nSlackVars);


	cout << "Vous allez maintenant entrer les coefficients par lignes." << endl;

	// Nombre de colonnes-1 car la derni�re colonne est utilis� pour la recherche du pivot et n'est pas modifiable
	for (int i = 0; i < simplex.getRowsNumberPerColumn(); i++)
	{
		// Nombre de colonnes-1 car la derni�re colonne est utilis� pour la recherche du pivot et n'est pas modifiable
		for (int j = 0; j < simplex.getColumnsNumber(); j++)
		{
		
			cout << "Entrez le coefficient pour la colonne " << i + 1 << " et la ligne " << j + 1 << " : " << endl;

			double tmpValue = 0;

			cin >> tmpValue;

			simplex.addValue(0, i, j, tmpValue);
		}
	}

	cout << "Voici la matrice que vous avez entr�e : " << endl;

	for (int i = 0; i < simplex.getRowsNumberPerColumn(); i++)
	{
		for (int j = 0; j < simplex.getColumnsNumber(); j++)
		{
		
			cout << simplex.getValue(0, i, j) << " ";
		}
		cout << endl << endl;
	}


	vector<double> result = simplex.resolve();

	cout << "Voici le resultat de la derniere iteration : " << endl;

	for (int i = 0; i < simplex.getRowsNumberPerColumn(); i++)
	{
		for (int j = 0; j < simplex.getColumnsNumber(); j++)
		{
			cout << result[(j * (nSlackVars + 1) + i)] << " ";
		}
		cout << endl << endl;
	}

	while (1);

    return 0;
}

