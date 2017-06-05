#pragma once

#include <vector>
#include <map>
#include <string>

class Simplex
{
	public:

		Simplex(int nActivityVars, int nSlackVars);

		void addValue(int iterationNumber, int rowNumber, int columnNumber, double value);
		double getValue(int iterationNumber, int rowNumber, int columnNumber) const;
		

		int getColumnsNumber() const;
		int getRowsNumberPerColumn() const;

		std::vector<double> Simplex::resolve();

	private:

		bool isCoeffLossFuncLessThanZero(int iterationNumber) const;
		int getColumnPivot(int iterationNumber) const;
		int getRowPivot(int iterationNumber, int columnPivot) const;

		int m_activityVars;
		int m_slackVars;
		int m_iterationSize;

		/*
			Le tableau contient toutes les matrices des différentes itérations, les itérations sont contigues et sont chacunes de
			taille : (m_activityVars + m_slackVars + 2) * (m_slackVars + 1)

			Le fait d'aligner les valeurs des itérations permet d'éviter d'utiliser 2 voir 3 dimensions.
		*/
		std::vector<double> m_matrix;
};

