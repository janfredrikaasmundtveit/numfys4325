#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <string>
#include <random>
#include <time.h>
#include <armadillo>
#include "particle.h"


using namespace std;

   
ofstream ofile; 
ofstream pfile; 


void output(particle p){
	ofile << setw(15) << setprecision(8) << p.t;
	ofile << setw(15) << setprecision(8) << p.x;
	ofile << setw(15) << setprecision(8) << p.potential(false) << "\n";
}	
void outputv(double tau, double v[],int np){
	double vav=0;
	for(int i=0; i<np; i++){
		vav += v[i]/((double)(np));
	}

	pfile << setw(15) << setprecision(8) << tau;
	pfile << setw(15) << setprecision(8) << vav << "\n";
	
}	


int main(int argc, char const *argv[]){
	string filename=argv[1];  
	double r1 = atof(argv[2]); // radius in \mu m
	double L = atof(argv[3]); // lenghth scale in \mu m
 	double alpha = atof(argv[4]); // parametre
 	double T = atof(argv[5]); // temprature in eV
 	double U = atof(argv[6]); // potential strength
	double gamma=r1*18.8495559215;
	double D = T/U;
	int np = 10;
	bool flashing=true;	   
	ofile.open(filename);
	string prob="prob";
	prob.append(filename);
	pfile.open(prob);   

	double delta = 0.1; 

	particle particles[np]; 
	
	double tau=0.6;

	for(int i=0; i<np; i++){
		particle p(0.0,0.0,0.0,alpha,tau,L,sqrt(2*T*gamma));
		particles[i]=p; 
		
	}

	double Vdrift[np] = {0.0};
	while(particles[0].t<100.0){
	
		for(int i=0; i<np; i++){
			particles[i].step(delta,D,flashing);
			Vdrift[i] += particles[i].vdrift*delta/1000.0; 
		
			if(i==0){
				output(particles[i]);
			}
		/*	int U=floor((particles[i].potential(flashing))*100);
			if(U>=0 && U<10000){
				pU[U]++;
			}*/
			
		 }
	}

		//outputv(tau,Vdrift,np);
	

 	ofile.close();
 	pfile.close();

	return 0;
}
 
 