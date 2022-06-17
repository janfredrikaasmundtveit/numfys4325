#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <string>
#include <random>
#include <time.h>
#include <armadillo>
#include "rng.h"
#include "nuclei.h"

using namespace std;

   
ofstream ofile;  
ofstream ofile2;  

void output(solver s,solver s2, double t){
	double E=0.0;
	double r=0.0;
	double E2=0.0;
	double r2=0.0;
	for(int i=0;i<s.gamma.absorbed; i++){
		E=E+s.gamma.energyloss[i];
		r=r+s.gamma.range[i]/s.gamma.absorbed;
	}
	for(int i=0;i<s2.gamma.absorbed; i++){
		E2=E2+s2.gamma.energyloss[i];
		r2=r2+s2.gamma.range[i]/s2.gamma.absorbed;
	}
	ofile << setw(15) << setprecision(8) << t;  
	ofile << setw(15) << setprecision(8) << E*0.5110;  
	ofile << setw(15) << setprecision(8) << E2*0.5110; 
	ofile << setw(15) << setprecision(8) << r;
	ofile << setw(15) << setprecision(8) << r2 << "\n";   
}	 

 
void output2(solver s, double t){
	ofile2 << setw(15) << setprecision(8) << t; 
	if(s.gamma.ngamma>0){
		ofile2 << setw(15) << setprecision(8) << s.gamma.photons[0].x; 
		ofile2 << setw(15) << setprecision(8) << s.gamma.photons[0].y; 
	}
	ofile2 << setw(15) << setprecision(8) << "\n";  
 
}  

int main(int argc, char const *argv[]){   
	string filename=argv[1];   
	ofile.open(filename);  
	string filename2=argv[2];    
	ofile2.open(filename2);
	std::random_device rd; 
	std::mt19937_64 gen(rd());
	std::uniform_real_distribution<double> RandomNumberGenerator(0.0,1.0);
 
	
	int Z = 20;
	int N0 = 250; 
	double steps = 100000.0;  
	double tmax = 1000.0;   
	double deltat = tmax/steps; 
	double deltax = 4.860/(8.066*8.066); 
	solver s(N0,0,20);
	solver s2(N0,0,92);
	for(double t=0.0;t<tmax;t+=deltat){ 
		s.step(deltax,deltat);
		s2.step(deltax,deltat);
		output(s,s2,t);
		output2(s,t);
		 
	}

 	ofile.close();
 	ofile2.close();  

 	cout << "removed photons: " << s.gamma.absorbed << "\n";

 
	return 0;
}
 
  