#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <string>
#include <random>
#include <time.h>
#include <armadillo>

using namespace std;

int msm(int seed){// homebrewed random numbergenerator using middle square method.
	int seed2=seed*seed;
	stringstream tmp_stream; // seed as string
	stringstream tmp_stream2; // seed^2 as string
	stringstream tmp_stream3; 
	tmp_stream << seed;
	//cout << tmp_stream.str() << "\n";
	tmp_stream2 << seed2;
	//cout << tmp_stream2.str() << "\n";
	int n=tmp_stream.str().size();
	
	while(tmp_stream2.str().size()<2*n){
		tmp_stream3 << 0;
		tmp_stream3 << tmp_stream2.str();
		tmp_stream2.str(tmp_stream3.str());	

	}
	stringstream rand;
	for(int i=n/2; i<2*n-n/2;i++){
		rand << tmp_stream2.str().at(i);
	}
	//int r=atoi(rand.str())
	return atoi(rand.str().c_str());
}


int lcm(int seed){// homebrewed random numbergenerator using linear congruence method
	int a=75;
	int m=pow(2,16)+1;
	int c=74; // zx81 values 
	int rand = (a*seed+c)%m;
	return rand;
}