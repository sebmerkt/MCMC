//////////
// 2D
//////////


#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <fstream>
#include <algorithm> //sort
#include <tuple>

using namespace std;
typedef std::mt19937 EngineType;


double probFunction (double x, double y) {
//	return exp(-(x - 3.1)*(x - 3.1)/0.1) + exp(-(x - 2.7)*(x - 2.7)/0.23) +  exp(-(x - 3.2)*(x - 3.2)/0.08) + exp(-(x - 2.89)*(x - 2.89)/0.15) + exp(-(x - 2.1)*(x - 2.1)/0.1) + exp(-(x - 1.7)*(x - 1.7)/0.23) +  exp(-(x - 2.2)*(x - 2.2)/0.08) + exp(-(x - 1.89)*(x - 1.89)/0.15);

//	return exp(-(x+0.5*y - 3.1)*(x+0.5*y - 3.1)/0.1) + exp(-(x+0.5*y - 2.7)*(x+0.5*y - 2.7)/0.23) +  exp(-(x+0.5*y - 3.2)*(x+0.5*y - 3.2)/0.08) + exp(-(x+0.5*y - 2.89)*(x+0.5*y - 2.89)/0.15) + exp(-(x+0.5*y - 2.1)*(x+0.5*y - 2.1)/0.1) + exp(-(x+0.5*y - 1.7)*(x+0.5*y - 1.7)/0.23) +  exp(-(x+0.5*y - 2.2)*(x+0.5*y - 2.2)/0.08) + exp(-(x+0.5*y - 1.89)*(x+0.5*y - 1.89)/0.15);


//	return exp(-(x+0.5*y - 3.1)*(x+0.5*y - 3.1)/0.1-(x+0.5*y - 2.7)*(x+0.5*y - 2.7)/0.23-(x+0.5*y - 3.2)*(x+0.5*y - 3.2)/0.08-(x+0.5*y - 2.89)*(x+0.5*y - 2.89)/0.15) + exp(-(x+0.5*y - 2.1)*(x+0.5*y - 2.1)/0.1-(x+0.5*y - 1.7)*(x+0.5*y - 1.7)/0.23-(x+0.5*y - 2.2)*(x+0.5*y - 2.2)/0.08-(x+0.5*y - 1.89)*(x+0.5*y - 1.89)/0.15);

	return exp(-((x - 1.1)*(x - 1.1)/0.2/0.2 + 2.*0.9*(x - 1.1)*(y - 1.4)/0.2/0.5 + (y - 1.4)*(y - 1.4)/0.5/0.5)/2)  +  exp(-((x - 2.1)*(x - 2.1)/1.2/1.2 + 2.*0.9*(x - 2.1)*(y - 1.4)/1.2/0.5 + (y - 1.4)*(y - 1.4)/0.5/0.5)/2);
}


double marginalize ( double x) {

	EngineType e2(10);

	int N = 10000;
	double integ = 0;

	normal_distribution<double> nDist(1.4,0.5);
	for (int i = 0; i<N; i++) {
		double y = nDist(e2);
		integ += probFunction(x, y);
	}
	integ/=N;


	return integ;
}




int main (int argc, char * * argv) {

	EngineType e1(100);
	EngineType e2(101);

	int N=100000;
	double integ=0;
	double integ2=0;

	double x1=1., y1=1.;
	double a=0;

	vector< std::tuple<double,double,double> > funcVal;


////////////calculate the volume

	for (int i = 0; i<N; i++) {

		normal_distribution<double> nDist1(x1,0.5);
		normal_distribution<double> nDist2(y1,0.5);
		double x2 = nDist1(e1);
		double y2 = nDist2(e2);

		double a = probFunction(x2,y2)/probFunction(x1,y1);
		if (a>=1.) {
			x1=x2;
			y1=y2;
		}
		else {
			uniform_real_distribution<double> u(0,1);	
			if (u(e1)<a) {
				x1=x2;
				y1=y2;
			}
		}
		
		funcVal.push_back(make_tuple(probFunction(x1,y2),x1,y1));
		integ+=probFunction(x1,y2);
	}
//	double fbar=integ/N, ffbar=integ2/N, sig=sqrt((ffbar-fbar*fbar)/N);
//	cout << fbar << " +- " << sig << endl;



	sort (funcVal.begin(), funcVal.end());
	vector< std::tuple<double,double,double> > funcValRev = funcVal;
	reverse(funcValRev.begin(),funcValRev.end());

std::cout << std::endl << "Finished calculating the volume" << std::endl;


////////////calculate the confidence interval

	vector< std::tuple<double,double,double> > confInt;
	double parV = 0;

	for (int i = 0; i<N; i++) {

		if (parV/integ>=0.68) break;

		parV += std::get<0>(funcValRev[i]);
		confInt.push_back(make_tuple(std::get<0>(funcValRev[i]), std::get<1>(funcValRev[i]), std::get<2>(funcValRev[i])));

	}
		
std::cout << std::endl << "Finished calculating the confidence interval" << std::endl;

////////////store the data




	{
	ofstream myfile ("dist.txt");
	if (myfile.is_open()) {
		for (int i =0; i<funcValRev.size(); i++ ) {
			myfile << std::get<0>(funcValRev[i]) << "\t" << std::get<1>(funcValRev[i]) << "\t" << std::get<2>(funcValRev[i]) << std::endl;
		}
		myfile.close();
	}
	else cout << "Unable to open file";
	}
	{
	ofstream myfile ("distConf.txt");
	if (myfile.is_open()) {
		for (int i =0; i<confInt.size(); i++ ) {
			myfile << std::get<0>(confInt[i]) << "\t" << std::get<1>(confInt[i]) << "\t" << std::get<2>(confInt[i]) << std::endl;
		}
		myfile.close();
	}
	else cout << "Unable to open file";
	}

std::cout << std::endl << "Finished storing the data" << std::endl;

////////////get marginalized distribution

	int N1=10000;
	double vol1D=0.;

	double z1=1.;
	double b=0;

	vector<std::pair<double,double> > funcVal1D;


	for (int i = 0; i<N1; i++) {

		normal_distribution<double> nDist1(z1,0.3);
		double z2 = nDist1(e1);
		
		

		b = marginalize(z2)/marginalize(z1);
		if (b>=1.) {
			z1=z2;
		}
		else {
			uniform_real_distribution<double> u(0,1);	
			if (u(e1)<b) {
				z1=z2;
			}
		}
		
		funcVal1D.push_back(make_pair(marginalize(z1),z1));
		vol1D+=marginalize(z1);
	}

	sort (funcVal1D.begin(), funcVal1D.end());
	vector<std::pair<double,double> > funcVal1DRev = funcVal1D;
	reverse(funcVal1DRev.begin(),funcVal1DRev.end());

	{
	ofstream myfile ("distMarg.txt");
	if (myfile.is_open()) {
		for (int i =0; i<funcVal1DRev.size(); i++ ) {
			myfile << funcVal1DRev[i].first << "\t" << funcVal1DRev[i].second << std::endl;
		}
		myfile.close();
	}
	else cout << "Unable to open file";
	}


std::cout << std::endl << "Finished calculating the marginalized distribution" << std::endl;




  return 0;
}
