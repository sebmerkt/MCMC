//////////
// 1D
//////////

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <fstream>
#include <algorithm> //sort

using namespace std;
typedef std::mt19937 EngineType;


double probFunction (double x) {
//	return exp(-(x - 3.1)*(x - 3.1)/0.1) + exp(-(x - 2.7)*(x - 2.7)/0.23) +  exp(-(x - 3.2)*(x - 3.2)/0.08) + exp(-(x - 2.89)*(x - 2.89)/0.15) + exp(-(x - 2.1)*(x - 2.1)/0.1) + exp(-(x - 1.7)*(x - 1.7)/0.23) +  exp(-(x - 2.2)*(x - 2.2)/0.08) + exp(-(x - 1.89)*(x - 1.89)/0.15);

//	return exp(-(x+0.5*y - 3.1)*(x+0.5*y - 3.1)/0.1) + exp(-(x+0.5*y - 2.7)*(x+0.5*y - 2.7)/0.23) +  exp(-(x+0.5*y - 3.2)*(x+0.5*y - 3.2)/0.08) + exp(-(x+0.5*y - 2.89)*(x+0.5*y - 2.89)/0.15) + exp(-(x+0.5*y - 2.1)*(x+0.5*y - 2.1)/0.1) + exp(-(x+0.5*y - 1.7)*(x+0.5*y - 1.7)/0.23) +  exp(-(x+0.5*y - 2.2)*(x+0.5*y - 2.2)/0.08) + exp(-(x+0.5*y - 1.89)*(x+0.5*y - 1.89)/0.15);


	return (exp(-(x - 3.1)*(x - 3.1)/0.1-(x - 2.7)*(x - 2.7)/0.23-(x - 3.2)*(x - 3.2)/0.08-(x - 2.89)*(x - 2.89)/0.15) + exp(-(x - 2.1)*(x - 2.1)/0.1-(x - 1.7)*(x - 1.7)/0.23-(x - 2.2)*(x - 2.2)/0.08-(x - 1.89)*(x - 1.89)/0.15));


}



int main (int argc, char * * argv) {

	EngineType e1(100);

	int N=100000;
	double integ=0.;
	double integ2=0.;

	double x1=1.;
	double a=0;

	vector<std::pair<double,double> > funcVal;

////////////calculate the volume
	for (int i = 0; i<N; i++) {

		normal_distribution<double> nDist1(x1,0.3);
		double x2 = nDist1(e1);
		
		

		a = probFunction(x2)/probFunction(x1);
		if (a>=1.) {
			x1=x2;
		}
		else {
			uniform_real_distribution<double> u(0,1);	
			if (u(e1)<a) {
				x1=x2;
			}
		}
		
		funcVal.push_back(make_pair(probFunction(x1),x1));
		integ+=probFunction(x1);
	}
	double mu=integ/N, ffbar=integ2/N;
//	cout << mu << /*" +- " << sig <<*/ endl;

	sort (funcVal.begin(), funcVal.end());
	vector<std::pair<double,double> > funcValRev = funcVal;
	reverse(funcValRev.begin(),funcValRev.end());

////////////calculate the confidence interval

	vector<std::pair<double,double> > confInt;
	double parV = 0;

	for (int i = 0; i<N; i++) {

		if (parV/integ>=0.68) break;

		parV += funcValRev[i].first;
		confInt.push_back(make_pair(funcValRev[i].first,funcValRev[i].second));

	}
		



////////////store the data

	{
	ofstream myfile ("dist.txt");
	if (myfile.is_open()) {
		for (int i =0; i<funcVal.size(); i++ ) {
			myfile << funcVal[i].first << "\t" << funcVal[i].second << endl;
		}
		myfile.close();
	}
	else cout << "Unable to open file";
	}
	{
	ofstream myfile ("distRev.txt");
	if (myfile.is_open()) {
		for (int i =0; i<funcValRev.size(); i++ ) {
			myfile << funcValRev[i].first << "\t" << funcValRev[i].second << endl;
		}
		myfile.close();
	}
	else cout << "Unable to open file";
	}
	{
	ofstream myfile ("distConf.txt");
	if (myfile.is_open()) {
		for (int i =0; i<confInt.size(); i++ ) {
			myfile << confInt[i].first << "\t" << confInt[i].second << endl;
		}
		myfile.close();
	}
	else cout << "Unable to open file";
	}




	
  return 0;
}
