#ifndef particle_H
#define particle_H

class particle
{
public:
	double x,t,vdrift,alpha,tau,L,sigma;
	particle(){x=0.0; t=0.0; vdrift=0.0; alpha=0.0; tau=0.0; L=0.0;sigma=0.0;};
	particle(double ix,double it,double iv,double ialpha,double itau,double iL,double isigma){x=ix; t=it; vdrift=iv; alpha=ialpha; tau=itau; L=iL;sigma=isigma;};
	void step(double delta, double D,bool p){
		std::random_device rd{};
    	std::mt19937 gen{rd()};
   	 	std::normal_distribution<> d{0,sigma};
 		double xi = d(gen);
 		t=t+delta;
 		double F=force(delta,p);		
 		x=x+F+sqrt(2.0*delta*D)*xi;
 		vdrift=fabs(F+sqrt(2.0*delta*D)*xi)/delta;
 	
	}
	double periodic(double x, double P){
		if(x>0.0){
			while(x>P){
				x=x-P;
			}
		}
		if(x<0.0){
			while(x<0.0){
				x=x+P;
			}
		}


		return x;
	}

	double force(double delta,bool p){
		double U;
		double xp=periodic(x,L); 
		double tp=periodic(t,tau); 
		if(xp<alpha){
			U = delta/alpha; 
		}else{
			U = delta/(1.0-alpha); 
		}
		if(p==true){
			if(tp<0.75*tau){
				U=0.0;
			}
		}
		return U;

	}
	double potential(bool p){
		double U; 
		double xp=periodic(x,L);
		double tp=periodic(t,tau);  
		if(xp<alpha){
			U = xp/alpha; 
		}else{
			U = xp/(1.0-alpha); 
		}
		if(p==true){
			if(tp<0.75*tau){
				U=0.0;
			}
		}

		return U;

	}

};

#endif