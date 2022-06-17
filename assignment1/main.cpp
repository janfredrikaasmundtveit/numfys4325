#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <string>
#include <random>
#include <time.h>
#include <armadillo>
#include "setup.h"
#include "fractal.h"

 

using namespace std;
using namespace arma;

   
ofstream ofile,ffile; 
 


void outputw(mat A, vec l,coordarr coords){
	int k=0;
		while(l(k)<0.000001){
			k++;
	}

	for (int i = 0; i < A.n_rows; i++)	{
			ofile << setw(15) << setprecision(8) << l(i);
			ofile << setw(15) << setprecision(8) << coords.coords[i].x;
			ofile << setw(15) << setprecision(8) << coords.coords[i].y;		
			for (int j = k; j < k+10; j++){
					ofile << setw(15) << setprecision(8) << A(i,j);
				}
				
			ofile << setw(15) << setprecision(8) << "\n"; 
		}

	
}  
  
void outputf(fractal a){ 
	ffile << setw(15) << setprecision(8) << a.lines[0].start.x;
	ffile << setw(15) << setprecision(8) << a.lines[0].start.y;
	ffile << setw(15) << setprecision(8) << 0.0;
	ffile << setw(15) << setprecision(8) << "\n";
	for(int i=0; i<a.nlines; i++){
		ffile << setw(15) << setprecision(8) << a.lines[i].end.x;
		ffile << setw(15) << setprecision(8) << a.lines[i].end.y;
		ffile << setw(15) << setprecision(8) << 0.0;
		ffile << setw(15) << setprecision(8) << "\n"; 
	} 
}  
   
int main(){  
	   
	string filename="test.dat";     
	ofile.open(filename);     
 	ffile.open("fractal.dat");     
	fractal f;      
   
	f.generate();      
	f.generate();  
  
	//f.generate();    
	coordarr coords;   
	double delta = f.lines[0].length()/10.0;//TEST THAT DELTA GENERATES CORRECT MATRIX BEFORE SOLVING!
	int nlatx=(int)(100.0/delta); 	
	int nlat=nlatx*nlatx; 

	outputf(f);
	mat A=setup(nlat,nlatx,&f,delta,&coords);


	//cout << " lattice points inside:" << A.n_cols <<  "\n" <<" lattice points:" << nlat << "\n";

	//cout << "memory usage: " << sizeof(A) << "\n";
	vec eigval;
	mat eigvec;
	

	eig_sym(eigval, eigvec, A);

	outputw(eigvec,eigval,coords);

	
	ofile.close();
	ffile.close();  
	
	delete [] coords.coords; delete [] f.lines;

	return 0;  
}  