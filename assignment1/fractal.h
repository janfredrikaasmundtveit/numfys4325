#ifndef fractal_H
#define fractal_H

class coord{
	public:
	mutable double x, y;
	coord(){x=0.0;y=0.0;}
	coord(double ix,double iy){x=ix; y=iy;}
	void updatecoord(coord c){this->x=c.x;this->y=c.y;}		
	coord& operator= (const coord& c){
   		x = c.x;
   		y = c.y;
  	 return *this;
	}	
	inline coord operator+ (const coord& c){
  		 return coord(x+c.x,y+c.y);
	}
	inline coord operator- (const coord& c){
  		 return coord(x-c.x,y-c.y);
	}
	const coord operator* (double s){//scalar multiplication, note scalars can only be multiplied from the right
  		 return coord(s*x,s*y);
	}
	
	double r(){return x*x+y*y;} //returns vector(dot)vector


};

inline coord operator*(const double s, coord c){ //multiplying scalars from the left
		return c*s;
	}


class coordarr{
	public:
	coord* coords; int numcoord;
	coordarr(){numcoord=1;
		coords=new coord[1];}
	coordarr(int num){numcoord=num;
		coords=new coord[num];
	}

	coordarr& operator= (const coordarr& c){
   		numcoord=c.numcoord;
   		coords=new coord[numcoord];
   		for(int i=0;i<numcoord;i++){
   			coords[i]=c.coords[i];
   		}
  	 return *this;
	}
	void changesize(int num){
		numcoord=num;
		coords=new coord[num];
	}
	void addcoord(coord c,int i){
		coords[i]=c;
	}	
}; 

class linesegment{
	public:
	coord start,end;
	linesegment(){coord c(0.0,0.0);
		start=c; end=c;}
	linesegment(coord c1,coord c2){start=c1; end=c2;}
	linesegment& operator= (const linesegment& l){start=l.start; end=l.end;
				return *this;
		}
	void scale(double a){
		end=start+(end-start)*a;
	}
	double length(){
		coord temp;
		temp=end-start;
		return sqrt(temp.r());
	}

	linesegment rotate1(){ //rotates counterclocwise with previous end point as new startpoint
		coord tempstart,tempend;
		tempend=end-start; //shifts the start to the origin
		coord newend(-1.0*tempend.y,tempend.x);
		newend = newend + end;
		linesegment l(end, newend);
		return l;
	}
		linesegment rotate2(){ //rotates clocwise with previous end point as new startpoint
		coord tempstart,tempend;
		tempend=end-start; //shifts the start to the origin
		coord newend(tempend.y,-1.0*tempend.x);
		newend = newend + end;
		linesegment l(end, newend);
		return l;
	}
	bool isonline(coord c, double delta){// tests if point c is on the line 
		coord c1=c-start;
		coord c2=end-c;
		double d=sqrt(c2.r())+sqrt(c1.r());
		if(abs(d-length()<0.001*delta)){// if the diatance fron the startingpoint of the line to the ending point is the same as start to c to end (cauchy schwarz inequality) 
			return true;	 //NOTE: delta is potentialy small so to high presisson risks underflow
		}else{
			return false;
		}
	

	}
};


class fractal{ // koch quadratic fractal
	public:
		int l,nlines; //level of the fractal, number of lines
		linesegment *lines;
		
		fractal(){l=0;	nlines=4;	
			lines = new linesegment[nlines]; 
			coord start(-50.0,50.0); coord end(50.0,50.0); // defining the fractal to live in a 100x 100 plain
			linesegment firstline(start,end);
			lines[0]=firstline;
			lines[1]=lines[0].rotate2();
			lines[2]=lines[1].rotate2();
			lines[3]=lines[2].rotate2();      
			};
		fractal& operator= (const fractal& f){l=f.l; nlines=f.nlines; lines=f.lines;
				return *this;
		}
		~fractal(){}

		void generate(){// turns a level l fractal into level l+1 fractal
			linesegment *temp,*upline;
			upline = new linesegment[8];
			l=l+1;
			int newnlines=nlines*8;
			temp=new linesegment[newnlines];
			for(int 	i=0; i<nlines; i++){
				upline=upgrade(lines[i]);
				for(int j=0; j<8; j++){
					temp[i*8+j]=upline[j];
				}
			}
			nlines=newnlines;
			lines=temp;
		
		}
		linesegment* upgrade(linesegment l){// takes a single line in the level l and replaces it with the level 1 fractal 
		coord c=l.start;
		l.start=l.start-c;
		l.end=l.end-c;
		linesegment *lines;
		lines=new linesegment[8];
		l.scale(0.25);
		lines[0]=l;
		lines[1]=lines[0].rotate1();
		lines[2]=lines[1].rotate2();
		lines[3]=lines[2].rotate2();
		coord d=lines[3].end-lines[3].start;
		lines[4]=linesegment(lines[3].end,lines[3].end+d);
		lines[5]=lines[4].rotate1();
		lines[6]=lines[5].rotate1();
		lines[7]=lines[6].rotate2();
		for(int i=0; i<8; i++){
			lines[i].start=lines[i].start+c;
			lines[i].end=lines[i].end+c;
	
			
		}
		return lines;
		}

		bool isonboundary(coord c, double delta, int *a){
			int n=0;
			for (int i = 0; i < nlines; i++){
				if(lines[i].isonline(c,delta)==true){
					n++;
					if(i!=*a){
						*a=i;
					}
				}
			}
			if(n!=0){
				return true;
			}
			return false;
		}
		bool isinside(coord c, double delta){//checks if the point c is inside the fractal by moving to the point (100,c.y) (which is outside) an checking whether the number of crossings is odd or even.
			int crossings=0;
			double dir=0.1;
			int a=-1;
			if(c.x<0.0){
				dir=-0.1;
			}
			coord step(dir*delta,0.0);
			

			if(isonboundary(c,delta,&a)){ //beeing on boundary does count as outside
						return true;
					}
				
			while(fabs(c.x)<100.0){
				c=c+step;
				if(isonboundary(c,delta,&a)){
					crossings++;	
						
					}

				}
					
			
			
		if(crossings%2==0 || crossings==0){	
			return false;
		}else{
			return true;
		}
	}
};	

	


#endif