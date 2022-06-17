#ifndef nuclei_H
#define nuclei_H

class nuclei
{
public:
	int N;
	nuclei(){N=100;};
	nuclei(int iN){N=iN;};
	~nuclei();
	
};

class solver
{
public: 
	nuclei* narray; int num,N0; 
	solver(int inum,int iN0){
		num = inum;
		narrau = new nuclei[inum];
		N0=iN0;
	};
	~solver(){free narray;};
	
};

#endif