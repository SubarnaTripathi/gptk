/***************************************************************************
 *   AstonGeostats, algorithms for low-rank geostatistical models          *
 *                                                                         *
 *   Copyright (C) Ben Ingram, 2008-2009                                   *
 *                                                                         *
 *   Ben Ingram, IngramBR@Aston.ac.uk                                      *
 *   Neural Computing Research Group,                                      *
 *   Aston University,                                                     *
 *   Aston Street, Aston Triangle,                                         *
 *   Birmingham. B4 7ET.                                                   *
 *   United Kingdom                                                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/



#include <iostream>
#include <cassert>
#include <ctime>

#include "itpp/itbase.h"
#include "itpp/itstat.h"

#include "optimisation/Optimisable.h"
#include "optimisation/ModelTrainer.h"
#include "optimisation/SCGModelTrainer.h"
#include "optimisation/CGModelTrainer.h"
#include "optimisation/GDModelTrainer.h"
#include "optimisation/QuasiNewtonModelTrainer.h"

#include "gaussianProcesses/ForwardModel.h"
#include "gaussianProcesses/GaussianProcess.h"
#include "optimisation/Optimisable.h"

#include "covarianceFunctions/CovarianceFunction.h"
#include "covarianceFunctions/SumCovarianceFunction.h"
#include "covarianceFunctions/GaussianCF.h"
#include "covarianceFunctions/MaternCF.h"
#include "covarianceFunctions/ExponentialCF.h"
#include "covarianceFunctions/WhiteNoiseCF.h"
#include "covarianceFunctions/ConstantCF.h"
#include "covarianceFunctions/LinearCF.h"


#include "covarianceFunctions/NegLogSigmoidTransform.h"
#include "covarianceFunctions/SigmoidTransform.h"

#include "RosenbrockND.h"

#include <fstream>
#include <sstream>
#include <cstdlib>
#include <string>

#include <gsl/gsl_sf_bessel.h>


using namespace std;
using namespace itpp;


int main()
{
	const int obs = 200, predobs = 2500;
	mat X = zeros(obs,2);
	vec y = zeros(obs);
	mat pX = zeros(predobs, 2);
	vec meanP = zeros(predobs);
	vec varP = zeros(predobs);

	int i = 0;

   cout.setf(ios::fixed);
   cout.precision(4);

/*
	string csvLine;
	std::fstream file("outputdata.csv", ios::in);
	while(getline(file, csvLine) && (i < obs))
	{
		string csvElement;
		istringstream csvStream(csvLine);

		getline(csvStream, csvElement, ',');
		X(i,0) = strtod(csvElement.c_str(),NULL);

		getline(csvStream, csvElement, ',');
		X(i,1) = strtod(csvElement.c_str(),NULL);
		
		getline(csvStream, csvElement, ',');
		y(i) = strtod(csvElement.c_str(),NULL);

		getline(csvStream, csvElement, ',');
		getline(csvStream, csvElement, ',');
		getline(csvStream, csvElement, ',');		
	covSum.setTransform(0, new NegLogSigmoidTransform());
	covSum.setTransform(1, new NegLogSigmoidTransform());
	covSum.setTransform(2, new NegLogSigmoidTransform());
		i++;
	}
*/

	string csvLine;
	std::fstream inputfile("SIC2004_input.csv", ios::in);
	while(getline(inputfile, csvLine) && (i < obs))
	{
		string csvElement;
		istringstream csvStream(csvLine);

		getline(csvStream, csvElement, ',');
		getline(csvStream, csvElement, ',');
		X(i,0) = strtod(csvElement.c_str(),NULL);

		getline(csvStream, csvElement, ',');
		X(i,1) = strtod(csvElement.c_str(),NULL);
		
		getline(csvStream, csvElement, ',');
		y(i) = strtod(csvElement.c_str(),NULL);

		i++;
	}

	i = 0;
	std::fstream predfile("predLoc.csv", ios::in);
	while(getline(predfile, csvLine) && (i < predobs))
	{
		string csvElement;
		istringstream csvStream(csvLine);

		getline(csvStream, csvElement, ',');
		pX(i,0) = strtod(csvElement.c_str(),NULL);

		getline(csvStream, csvElement, ',');
		pX(i,1) = strtod(csvElement.c_str(),NULL);

		i++;
	}


	
	cout <<endl;

	cout << "===============================================" << endl;
	
	double range = 0.20 * (min(max(X, 1) - min(X, 1)));
	double sill = variance(y);
	double nugget = 0.4 * sill;

	//MaternCF g1(range, sill, 0.5);
	GaussianCF g1(range, sill);
	WhiteNoiseCF g2(nugget);
	ConstantCF g3(mean(y));
	//LinearCF g3(0.005);
	SumCovarianceFunction gCmp(g1);
	gCmp.addCovarianceFunction(g2);
	gCmp.addCovarianceFunction(g3);	

	gCmp.displayCovarianceParameters();

	GaussianProcess gp(2, 1, X, y, gCmp);
	SCGModelTrainer gpTrainer(gp);


	gpTrainer.setAnalyticGradients(true);
	gpTrainer.Train(1000);
	gCmp.displayCovarianceParameters();
	

	gp.makePredictions(meanP, varP, pX);

/*
	for(int i=0; i< 2500 ; i++)
	{
		cout << meanP(i) << ",";
	}
	
	cout << "VARiANCE " << endl;

	for(int i=0; i< 2500 ; i++)
	{
		cout << varP(i) << ",";
	}
*/
	
//cout << "x = " << X << endl;
//cout << "y = " << y << endl;

//cout << "px = " << Xpred << endl;
//cout << "py = " << meanP << endl;
}

