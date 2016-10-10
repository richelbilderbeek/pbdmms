#include<iostream>
#include<fstream>
#include<iomanip>
#include<cmath>
#include<cstdlib>
#include<cstring>
#include<list>
#include"random.h"

using namespace std;

typedef long unsigned int bigint;

const int histw=50;
int Nx=1,Np=1,Nq=1,seed=123,outputfreq=10,haploid=1,diploid=0;
bigint popsize=100, endtime=1000;
double x0=0.6,p0=0.0,q0=0.0,sc=0.1,se=0.1,sk=1.0,c=0.001,sm=1.0,sv=0.05,sq=0.1,eta=1.0,
b=0.75,histx[histw],histp[histw],histq[histw],histbinx=0.05,histbinp=0.2,histbinq=0.2;
ofstream out;


class indiv
{ 
	private: 
		double x,p,q,*X,*P,*Q,a;
	public:
		~indiv() { delete[] X; delete[] P; delete[] Q; return;}
		indiv() 
		{ 
			int i;
			X=new double[Nx]; 
			P=new double[Np]; 
			Q=new double[Nq]; 
			for(i=0;i<Nx;i++) X[i]=0.0; 
			for(i=0;i<Np;i++) P[i]=0.0; 
			for(i=0;i<Nq;i++) Q[i]=0.0; 
			x=0.0; p=0.0; q=0.0;
			a=0.0;
			return;
		}
		indiv(const indiv &y)
		{
			int i;
			X=new double[Nx]; 
			P=new double[Np]; 
			Q=new double[Nq];
			for(i=0;i<Nx;i++) X[i]=y.X[i]; 
			for(i=0;i<Np;i++) P[i]=y.P[i]; 
			for(i=0;i<Nq;i++) Q[i]=y.Q[i]; 
			x=y.x; p=y.p; q=y.q;
			a=y.a;
			return;
		}
		void init(double x0, double p0, double q0)
		{
			int i;
			for(i=0;i<Nx;i++) X[i]=x0+Normal(0.0,sv); 
			for(i=0;i<Np;i++) P[i]=p0+Normal(0.0,sv); 
			for(i=0;i<Nq;i++) Q[i]=q0+Normal(0.0,sv); 
			x=x0+Normal(0.0,sv); p=p0+Normal(0.0,sv); q=q0+Normal(0.0,sv);
			return;
		}
		void birth(indiv m, indiv f)
		{
			int i;
			x=0.0;
			p=0.0;
			q=0.0;
			if(haploid)
			{
				for(i=0;i<Nx;i++) 
				{
					if(Uniform()<0.5) X[i]=m.X[i];
					else X[i]=f.X[i];
					X[i]+=Normal(0.0,sv);
					x+=X[i];
				}	
				for(i=0;i<Np;i++) 
				{
					if(Uniform()<0.5) P[i]=m.P[i];
					else P[i]=f.P[i];
					P[i]+=Normal(0.0,sv);
					p+=P[i];
				}
				for(i=0;i<Nq;i++) 
				{
					if(Uniform()<0.5) Q[i]=m.Q[i];
					else Q[i]=f.Q[i];
					Q[i]+=Normal(0.0,sv);
					q+=Q[i];
				}
			}
			if(diploid)
			{
				for(i=0;i<=Nx-2;i+=2) 
				{
					if(Uniform()<0.5) X[i]=m.X[i];
					else X[i]=m.X[i+1];
					if(Uniform()<0.5) X[i+1]=f.X[i];
					else X[i+1]=f.X[i+1];
					X[i]+=Normal(0.0,sv);
					X[i+1]+=Normal(0.0,sv);
					x+=X[i]+X[i+1];
				}
				for(i=0;i<=Np-2;i+=2) 
				{
					if(Uniform()<0.5) P[i]=m.P[i];
					else P[i]=m.P[i+1];
					if(Uniform()<0.5) P[i+1]=f.P[i];
					else P[i+1]=f.P[i+1];
					P[i]+=Normal(0.0,sv);
					P[i+1]+=Normal(0.0,sv);
					p+=P[i]+P[i+1];
				}
				for(i=0;i<=Nq-2;i+=2) 
				{
					if(Uniform()<0.5) Q[i]=m.Q[i];
					else Q[i]=m.Q[i+1];
					if(Uniform()<0.5) Q[i+1]=f.Q[i];
					else Q[i+1]=f.Q[i+1];
					Q[i]+=Normal(0.0,sv);
					Q[i+1]+=Normal(0.0,sv);
					q+=Q[i]+Q[i+1];
				}
			}
			x/=Nx;
			p/=Np;
			q/=Nq;
			return;
		}
		void print(void)
		{
			int i;
			cout<<x<<" "<<p<<" "<<q<<endl;
			for(i=0;i<Nx;i++) cout<<X[i]<<" ";
			cout<<endl;
			for(i=0;i<Np;i++) cout<<P[i]<<" ";
			cout<<endl;
			for(i=0;i<Nq;i++) cout<<Q[i]<<" ";
			cout<<endl;
		}
		double _x(void) { return x;}
		double _p(void) { return p;}
		double _q(void) { return q;}
		double _a(void) { return a;}
		void a_(double A) { a=A; return;}
		
};

typedef list<indiv>::iterator my_iterator;


list<indiv> pop;


inline my_iterator start(void)
{ return pop.begin();}

inline my_iterator end(void)
{ return pop.end();}

inline double gauss(double xx, double sigma)
{ return exp(-(xx*xx)/(2.0*sigma*sigma));}

my_iterator randomindividual(void)
{
	my_iterator i;
	bigint j,k=0;

	j=bigint(floor(Uniform()*popsize));
	
	for(i=start(); i!=end();i++,k++)
		if(k==j) 
			return i;
	return start();
}


void initialize(void)
{
	bigint j;
	int k;
	indiv I;
	SetSeed(seed);
	I.init(x0,p0,q0);
	for(j=0;j<popsize;j++) pop.push_back(I);
	out<<"generation,popsize,rhoxp,rhoxq,rhopq,sx,sp,sq";
	for(k=0;k<histw;k++) out<<","<<(k-histw/2)*histbinx;
	for(k=0;k<histw;k++) out<<","<<(k-histw/2)*histbinp;
	for(k=0;k<histw;k++) out<<","<<(k-histw/2)*histbinq;
	out<<endl;
}


void output(bigint t)
{
	double avgp=0.0,avgq=0.0,avgx=0.0,rhoxp,rhoxq,rhopq,
		ssxx=0.0,ssxp=0.0,sspp=0.0,ssxq=0.0,ssqq=0.0,sspq=0.0,dxi,dpi,dqi,delta,
		maxx=0.0,maxp=0.0,maxq=0.0,sx,sp,sq,xi,pi,qi;
	my_iterator i;
	int j,jx,jp,jq;

	delta=1.0/popsize;
	for(j=0;j<histw;j++)
	{
		histx[j]=0.0;
		histp[j]=0.0;
		histq[j]=0.0;
	}
	
	for(i=start();i!=end();i++)
	{
		avgx+=i->_x();
		avgp+=i->_p();
		avgq+=i->_q();
	}
	avgx/=popsize;
	avgp/=popsize;
	avgq/=popsize;
	for(i=start();i!=end();i++)
	{
		xi=i->_x();
		pi=i->_p();
		qi=i->_q();
		dxi=xi-avgx;
		dpi=pi-avgp;
		dqi=qi-avgq;
		ssxx+=dxi*dxi;
		ssxp+=dxi*dpi;
		ssxq+=dxi*dqi;
		sspp+=dpi*dpi;
		sspq+=dpi*dqi;
		ssqq+=dqi*dqi;
		jx=int(histw/2.0+xi/histbinx);
		jp=int(histw/2.0+pi/histbinp);
		jq=int(histw/2.0+qi/histbinq);
		if(jx<0) jx=0;
		if(jx>=histw) jx=histw-1;
		if(jp<0) jp=0;
		if(jp>=histw) jp=histw-1;
		if(jq<0) jq=0;
		if(jq>=histw) jq=histw-1;
		histx[jx]+=delta;
		if(histx[jx]>maxx) maxx=histx[jx];
		histp[jp]+=delta;
		if(histp[jp]>maxp) maxp=histp[jp];
		histq[jq]+=delta;
		if(histq[jq]>maxq) maxq=histq[jq];

	}
	rhoxp=ssxp/sqrt(ssxx*sspp);
	rhoxq=ssxq/sqrt(ssxx*ssqq);
	rhopq=sspq/sqrt(sspp*ssqq);
	sx=sqrt(ssxx/(popsize-1.0));
	sp=sqrt(sspp/(popsize-1.0));
	sq=sqrt(ssqq/(popsize-1.0));
	out<<t<<","<<popsize<<","<<rhoxp<<","<<rhoxq<<","<<rhopq<<","<<sx<<","<<sp<<","<<sq;
	cout<<t<<" "<<popsize<<" "<<rhoxp<<" "<<rhoxq<<" "<<rhopq<<endl
		<<avgx<<" "<<avgp<<" "<<avgq<<" "<<sx<<" "<<sp<<" "<<sq<<endl;
	for(j=0;j<histw;j++) out<<","<<histx[j]/maxx;
	for(j=0;j<histw;j++) out<<","<<histp[j]/maxp;
	for(j=0;j<histw;j++) out<<","<<histq[j]/maxq;
	out<<endl;
	return;
}

void iterate(void)
{
	my_iterator i,j;
	indiv kid;
	bigint k,t;
	double nkid,comp,xi,pi,qi,xj,qj,attractiveness,draw;
	for(t=0;t<=endtime;t++)
	{
		if(popsize==0) break;
		if(t%outputfreq==0) output(t);
		for(k=0;k<popsize;k++)
		{
			if(popsize==0) break;
			i=randomindividual();
			xi=i->_x();
			pi=i->_p();
			qi=i->_q();
			comp=0.0;
			for(j=start();j!=end();j++)
			{
				if(j!=i)
				{
					xj=j->_x();
					comp+=gauss(xi-xj,sc);
				}
			}
			if(Uniform()<(1.0-comp*c/gauss(xi,sk))*(0.5+0.5*gauss(qi,sq))) 
			{
				attractiveness=eta;
				for(j=start();j!=end();j++)
				{
					if(j!=i)
					{
						qj=j->_q();
						xj=j->_x();
						attractiveness+=gauss(pi-qj,sm)*gauss(xi-xj,se);
						j->a_(attractiveness);
					}
				}
				for(nkid=0.0;;nkid+=1.0)
				{
					if(Uniform()>=b-nkid) break;
					draw=Uniform()*attractiveness;
					if(draw>eta) 
					{
						for(j=start();j!=end();j++)
						{
							if(j!=i && draw<=j->_a())
							{
								kid.birth((*i),(*j));
								pop.push_back(kid);
								popsize++;
								break;
							}
						}
					}
				}
			}
			pop.erase(i);
			popsize--;
		}
	}
	return;
}

void readparameters(char *filename)
{
	ifstream fp(filename);
	char s[50],outputfilename[50];
	int T=0;
	
	cout<<"reading parameters and initializing"<<endl;  
	if(!fp) exit(1);
	cout<<"opening parameterfile"<<endl;
	while(fp>>s)
	{ 
		if(strcmp(s,"alleles")==0) { fp>>Nx>>Np>>Nq; cout<<"parameters "<<s<<" set to "<<Nx<<" "<<Np<<" "<<Nq<<endl;}
		if(strcmp(s,"histbin")==0) { fp>>histbinx>>histbinp>>histbinq; cout<<"parameters "<<s<<" set to "<<histbinx<<" "<<histbinp<<" "<<histbinq<<endl;}
		if(strcmp(s,"seed")==0) {fp>>seed; cout<<"parameter "<<s<<" set to "<<seed<<endl;}
		if(strcmp(s,"pop0")==0) {fp>>popsize;cout<<"parameter "<<s<<" set to "<<popsize<<endl;}
		if(strcmp(s,"type0")==0) 
		{
			fp>>x0>>p0>>q0;
			cout<<"parameter x0 set to "<<x0<<endl;
			cout<<"parameter p0 set to "<<p0<<endl;
			cout<<"parameter q0 set to "<<q0<<endl;
		}
		if(strcmp(s,"end")==0) {fp>>endtime;cout<<"parameter "<<s<<" set to "<<endtime<<endl;}
		if(strcmp(s,"sc")==0) {fp>>sc;cout<<"parameter "<<s<<" set to "<<sc<<endl;}
		if(strcmp(s,"se")==0) {fp>>se;cout<<"parameter "<<s<<" set to "<<se<<endl;}
		if(strcmp(s,"sm")==0) {fp>>sm;cout<<"parameter "<<s<<" set to "<<sm<<endl;}
		if(strcmp(s,"sv")==0) {fp>>sv;cout<<"parameter "<<s<<" set to "<<sv<<endl;}
		if(strcmp(s,"sq")==0) {fp>>sq;cout<<"parameter "<<s<<" set to "<<sq<<endl;}
		if(strcmp(s,"sk")==0) {fp>>sk;cout<<"parameter "<<s<<" set to "<<sk<<endl;}
		if(strcmp(s,"c")==0) {fp>>c;cout<<"parameter "<<s<<" set to "<<c<<endl;}
		if(strcmp(s,"b")==0) {fp>>b;cout<<"parameter "<<s<<" set to "<<b<<endl;}
		if(strcmp(s,"eta")==0) {fp>>eta;cout<<"parameter "<<s<<" set to "<<eta<<endl;}
		if(strcmp(s,"output")==0) 
		{
			fp>>outputfreq>>outputfilename;
			cout<<"saving data every "<<outputfreq<<" generations in "<<outputfilename<<endl;
			out.open(outputfilename);
			if(!out) {cout<<"unable to open datafile"<<endl; exit(1);}
		}
		if(strcmp(s,"haploid")==0)
		{
			haploid=1;
			diploid=0;
			cout<<"haploid genetic system"<<endl;
		}
		if(strcmp(s,"diploid")==0)
		{
			haploid=0;
			diploid=1;
			cout<<"diploid genetic system"<<endl;
		}
	}
	fp.close();
	return;
}


void main(int argc, char *argv[])
{	
	if(argc==1) { cout<<"no parameterfile specified"<<endl; exit(1);}
	readparameters(argv[1]);
	initialize();
	iterate();
	return;
}
