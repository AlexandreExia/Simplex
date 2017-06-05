#include <iostream>

#include "Simplex.h"


Simplex::Simplex(int nActivityVars, int nSlackVars) : m_activityVars(nActivityVars), m_slackVars(nSlackVars)
{
	// On alloue la taille pour la 1�re dimension

	m_iterationSize = (m_activityVars + m_slackVars + 1) * (m_slackVars + 1);
	m_matrix.resize(m_iterationSize);
}


void Simplex::addValue(int iterationNumber, int rowNumber, int columnNumber, double value)
{
	m_matrix[(columnNumber * (m_slackVars + 1) + rowNumber) + m_iterationSize * iterationNumber] = value;
}

double Simplex::getValue(int iterationNumber, int rowNumber, int columnNumber) const
{
	return m_matrix[(columnNumber * (m_slackVars + 1) + rowNumber) + m_iterationSize * iterationNumber];
}

int Simplex::getColumnsNumber() const
{
	return (m_activityVars + m_slackVars + 1);
}

int Simplex::getRowsNumberPerColumn() const
{
	return (m_slackVars + 1);
}

std::vector<double> Simplex::resolve()
{
	std::vector<double> result;

	int iteration = 0;

	while (!this->isCoeffLossFuncLessThanZero(iteration))
	{
		// On r�cup�re le num�ro de colonne et de ligne du pivot

		int columnPivot = getColumnPivot(iteration);
		int rowPivot = getRowPivot(iteration, columnPivot);

		double pivotVal = getValue(iteration, rowPivot, columnPivot);

		// On remplit maintenant l'it�ration n + 1

		iteration++;

		// On ajoute une it�ration � notre matrice

		m_matrix.resize( m_iterationSize * (iteration + 1), std::numeric_limits<double>::max());

		// On remplit la colonne du pivot avec des 0 sauf pour le pivot o� l'on met 1 car on le divise par lui m�me

		for (int i = 0; i < getRowsNumberPerColumn(); i++)
		{
			if (i == rowPivot)
			{
				addValue(iteration, i, columnPivot, 1);
			}
			else
				addValue(iteration, i, columnPivot, 0);

		}

		// On remplit la ligne du pivot par le rapport entre la valeur � l'it�ration - 1 et la valeur du pivot sauf pour 0

		for (int i = 0; i < getColumnsNumber(); i++)
		{
			if (i != columnPivot)
			{
				double valueOfLastIt = getValue(iteration - 1, rowPivot, i);

				// Si la valeur vaut 0, on conserve les valeurs de la colonne de l'it�ration pr�c�dente
				if (valueOfLastIt == 0)
				{
					for (int j = 0; j < getRowsNumberPerColumn(); j++)
					{
						double valueOfLastIt = getValue(iteration - 1, j, i);
						addValue(iteration, j, i, valueOfLastIt);
					}
				}
				else
				{
					addValue(iteration, rowPivot, i, valueOfLastIt / pivotVal);
				}
			}
		}

		/*
			On remplit les colonnes restantes avec la m�thodes des rectangles, pour identifier les cases nous avons mis une valeur
			par d�faut lors de l'allocation : std::numeric_limits<double>::max()

			Cette valeur nous permettras d'identifier rapidement les cases � remplir
		*/

		for (int i = 0; i < getRowsNumberPerColumn(); i++)
		{
			for (int j = 0; j < getColumnsNumber(); j++)
			{
				double currentValue = this->getValue(iteration, i, j);

				if (currentValue == std::numeric_limits<double>::max())
				{
					double lastValue = this->getValue(iteration - 1, i, j);

					// On calcul l'�cart de colonne et de ligne qu'il y a entre le pivot et la valeur concern�e
					int columnOffset = (columnPivot - j);
					int rowOffset = (rowPivot - i);

					double firstRectangleValue = this->getValue(iteration - 1, i, j + columnOffset);

					double secondRectangleValue = this->getValue(iteration - 1, i + rowOffset, j);

					double newValue = (lastValue - (firstRectangleValue * secondRectangleValue / pivotVal));

					this->addValue(iteration, i, j, newValue);
				}
			}

		}
	} // Fin de la r�solution

	result = m_matrix;

	result.erase(result.begin(), (result.end() - m_iterationSize));

	return result;
}


/*
	M�thodes priv�es
*/

bool Simplex::isCoeffLossFuncLessThanZero(int iterationNumber) const
{
	bool result = true;

	for (int i = 0; i < this->getColumnsNumber(); i++)
	{
		/*
			m_slackVars contient le nombre de variable d'�cart et donc nous permet d'acc�der � la derni�re ligne d'une it�ration,
			c-�-d la ligne de la fonction �conomique
		*/
		if (this->getValue(iterationNumber, m_slackVars, i) > 0)
		{
			result = false;
			break;
		}
	}

	return result;
}

int Simplex::getColumnPivot(int iterationNumber) const
{
	int higherColumnValue = 0;
	int columnPivotNumber = -1;

	for (int i = 0; i < this->getColumnsNumber(); i++)
	{
		/*
			m_slackVars contient le nombre de variable d'�cart et donc nous permet d'acc�der � la derni�re ligne d'une it�ration,
			c-�-d la ligne de la fonction �conomique
		*/
		if (this->getValue(iterationNumber, m_slackVars, i) > higherColumnValue)
		{
			higherColumnValue = this->getValue(iterationNumber, 2, i);
			columnPivotNumber = i;
		}
	}

	return columnPivotNumber;

}

int Simplex::getRowPivot(int iterationNumber, int columnPivot) const
{
	double constraintCoeffRatio = std::numeric_limits<double>::max();
	
	int rowPivotNumber = -1;

	/*
		row-1 car pour le calcul des rapports entre les contraintes et la colonne du pivot, 
		nous n'utilisation pas la derni�re ligne contenant la fonction objectif
	*/
	for (int i = 0; i < this->getRowsNumberPerColumn() - 1; i++)
	{
		// On soustrait 1 car on veut acc�der � la derni�re, celle des contraintes
		double constraintValue = this->getValue(iterationNumber, i, this->getColumnsNumber() - 1);
		double coeffValue = this->getValue(iterationNumber, i, columnPivot);

		double ratioValue = (constraintValue / coeffValue);
		if (ratioValue < constraintCoeffRatio)
		{
			constraintCoeffRatio = ratioValue;
			rowPivotNumber = i;
		}
	}

	return rowPivotNumber;
}