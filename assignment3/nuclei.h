#ifndef nuclei_H
#define nuclei_H



class nuclei
{
public:
	int N;
	nuclei(){N=100;};
	nuclei(int iN){N=iN;};
	~nuclei(){};

	
};

class photon
{
public:
	double x,y,E,E0,theta;
	int Z;
	photon(){x=0.0;y=0.0;E=0.0;Z=0;theta=0.0;};
	photon(double ix,double iy,double iE,int iZ){x=ix;y=iy;E=iE/0.5110;Z=iZ;theta=0.0;E0=E;};
	~photon(){};

	double comptoncc(){
		double term1,term1a,term1b,term2,term3,sigma;
		double pi= 3.14159265359;
		if(E<0.2){
			term1a=2.0*(1.0-E)/(1.0+2.0*E);
			term1b=log(1.0+2.0*E)/E;
			//term1=(term1a-term1b)*(1.0+E)/(E*E);
			term2=log(1.0+2.0*E)/E;
			term3=-(1.0+3.0*E)/((1.0+2.0*E)*(1.0+2.0*E));
			sigma=Z*2.0*pi*(term1+term2+term3);
			

		}else{
			term1=1.0+2.0*E+6.0*E*E/5.0;
			term2=-(E*E*E/2.0)+(2.0*E*E*E*E/7.0)+6.0*E*E*E*E*E/35.0;
			term3=(8.0*E*E*E*E*E*E+4.0*E*E*E*E*E*E*E)/105.0;
			sigma=Z*pi*8.0*(term1+term2+term3)/(3*(1.0+2.0*E)*(1.0+2.0*E));

		

		}
		return 0.002*fabs(sigma);

	}
	double angularcc(double angle){
		double alpha=0.65821/(0.5110*137.035);
		double pi= 3.14159265359;
		double sigma,term1;
		term1=(1.0/(1.0+E*(1.0-cos(angle))));
		sigma=alpha*alpha*term1*term1*(term1+1.0+E*(1.0-cos(angle))-sin(angle)*sin(angle))/2.0;
		
		return sigma;
	}
	
	double angledist(double delta){// returns an angle from the random distribution
		double pi= 3.14159265359;
		std::random_device rd; 
		std::mt19937_64 gen(rd());
		std::uniform_real_distribution<double> RandomNumberGenerator(0.0,1.0);
		while(true){	
			double angle=RandomNumberGenerator(gen)*2.0*pi;	
			double sigma=1000.0*angularcc(angle);
			if(RandomNumberGenerator(gen)<sigma){		
				return angle;				
			}
		}
	}
	void compton(double delta){
		std::random_device rd; 
		std::mt19937_64 gen(rd());
		std::uniform_real_distribution<double> RandomNumberGenerator(0.0,1.0);
		double sigma = comptoncc();
		double p=1.0-exp(-sigma*delta);
		//std::cout << sigma << "," << p <<"\n";
		if(RandomNumberGenerator(gen)<p){
			double angle=angledist(delta);
			theta=theta+angle;
			E=E*(1.0/(1.0+E*(1.0-cos(angle))));
		}
	};
	
};

class photongas
{
public:
	photon photons[1000];
	double range[1000],energyloss[1000];
	int ngamma,absorbed;
	photongas(){ngamma=0;absorbed=0;};
	~photongas(){};
	void createphoton(double E,int Z){
		if(ngamma<1000){
			photon temp(0.0,0.0,E,Z);
			photons[ngamma]=temp;
			ngamma++;
			
		}
	};
	void removephoton(int i){
		if(absorbed<1000){
		range[absorbed]=photons[i].x*photons[i].x+photons[i].y*photons[i].y;
		energyloss[absorbed]=photons[i].E0-photons[i].E;
		absorbed++;
		}
		for(int j=i;j<ngamma;j++){
			photons[j]=photons[j+1];
		}
		ngamma--;
	};
	
};


class solver
{
public: 
	nuclei cluster1,cluster2;
	photongas gamma;
	int N0,N02,Z;
	solver(int iN0,int iN02,int iZ){
		N0=iN0;
		N02=iN02;
		Z=iZ;
		nuclei temp(N0);
		cluster1=temp; 
		nuclei temp2(N02);
		cluster2=temp2;
		photongas gamma();
		
	};
	~solver(){};
	double decayE(){
		std::random_device rd; 
		std::mt19937_64 gen(rd());
		std::uniform_real_distribution<double> RandomNumberGenerator(0.0,1.0);
		double p=RandomNumberGenerator(gen)*17000.0;
		if(p<850.0){
			return 0.000135;			
		}
		if(p>=850.0 && p<14450){
			return 0.000525;			
		}

		return 0.000615;			
		

	};

	void decay(){
		if(cluster1.N>0){
			cluster1.N--;
			cluster2.N++;
			double E=decayE();
			gamma.createphoton(E,Z);
		
		}
	}; 
	void decay2(){
		if(cluster2.N>0){
			cluster2.N--;
			double E=decayE();
			gamma.createphoton(E,Z); 
		}
	};
	void PE(double delta,int i){
		double sigma=3.0*pow(10,-23)*Z*Z*Z*Z/(pow(gamma.photons[i].E*0.5110,3.5));
		double p=1.0-exp(-sigma*delta);
		std::random_device rd; 
		std::mt19937_64 gen(rd());
		std::uniform_real_distribution<double>RandomNumberGenerator(0.0,1.0);

		if(RandomNumberGenerator(gen)<p){
			gamma.removephoton(i);
			cluster1.N++;
			cluster2.N--;
		}
	};
		
	void move(double delta,int i){
		std::random_device rd; 
		std::mt19937_64 gen(rd());
		std::uniform_real_distribution<double> RandomNumberGenerator(0.0,1.0);
		delta=delta*RandomNumberGenerator(gen);

		gamma.photons[i].compton(delta);
		gamma.photons[i].x=gamma.photons[i].x+delta*cos(gamma.photons[i].theta);
		gamma.photons[i].y=gamma.photons[i].y+delta*sin(gamma.photons[i].theta);
		PE(delta,i);
	};

	void step(double deltat,double deltax){
		std::random_device rd; 
		std::mt19937_64 gen(rd());
		std::uniform_real_distribution<double> RandomNumberGenerator(0.0,1.0);
		double lambda1 = 0.3; 
		double lambda2 = 0.8; 
		double pd=1.0-exp(-lambda1*deltat); 
		double pd2=1.0-exp(-lambda2*deltat); 
		double N1=cluster1.N;
		double N2=cluster2.N; 
		for(int i=0;i<N1;i++){
			if(RandomNumberGenerator(gen)<pd && cluster1.N>0){
				decay();
			}
		}
		for(int i=0;i<N2;i++){
			if(RandomNumberGenerator(gen)<pd2 && cluster2.N>0){
				decay2();
			}
		}

		for(int i=0;i<gamma.ngamma;i++){
			move(deltax,i);
		}

	};
	
};





#endif