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


arma::mat setup(int dim,int xlat,fractal *f,double delta,coordarr *coords){ // returns -Laplacian inside f
	int k=0;
	int a=-1;
	int crossings=0;
	int boudarynodes[xlat*xlat];
	int nboundary=0;
	coord temp[xlat*xlat];
  
	for (int i = 0; i < xlat; i++){
		for(int j=0; j<xlat; j++){
			coord c(2.0*i*delta-100.0,2.0*j*delta-100.0);
			if(f->isinside(c,delta)==true){	
				coord adj1(c.x,c.y+delta); 
				coord adj2(c.x,c.y-delta);
				if(f->isinside(adj1,delta)==true || f->isinside(adj2,delta)==true){
					temp[k]=c;
					k++;
				}

			}

			 
		} 
	}	
		std::cout << " lattice points on boundary:"  << nboundary <<"\n";
			

		coords->changesize(k);
		
	for(int i=0;i<k;i++){// stores every point inside and on boundary with index
		coords->addcoord(temp[i],i);
	}



	arma::mat A(k,k,arma::fill::zeros);
	for(int i=0;i<k;i++){
		
		if(i!=0){

			A(i-1,i)=-1.0;
			A(i,i-1)=-1.0;
		}
		if(i>xlat){
			A(i-xlat,i)=-1.0;
			A(i,i-xlat)=-1.0;
		}
		
		A(i,i)=4.0;
		

	}
	for(int i=0;i<k;i++){
			if(f->isonboundary(coords->coords[i],delta,&a)==true){
				a=-1;
				for(int j=0;j<k;j++){
					A(i,j)=0.0;
					A(j,i)=0.0;
				}
			}  
 
		}
		
	return A;
}

