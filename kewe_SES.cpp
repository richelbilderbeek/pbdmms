// Copyright GS van Doorn

#include<iostream>
#include<fstream>
#include<iomanip>
#include<cmath>
#include<cstdlib>
#include<cstring>
#include<list>
#include"kewe_random.h"
#include <cassert>
#include <vector>

using namespace std;

typedef long unsigned int bigint;

const int histw=50;
int Nx=2,Np=2,Nq=2,seed=123,outputfreq=10,haploid=0,diploid=1;
bigint popsize=100, endtime=1000;
double x0=0.5,p0=0.5,q0=0.5,sc=0.4,se=0.6,sk=1.2,c=0.0005,sm=0.2,sv=0.02,sq=1.0,eta=1.0,
b=4,histx[histw],histp[histw],histq[histw], hista[histw],histbinx=0.1,histbinp=0.1,histbinq=0.1, histbina=0.1;
ofstream out;


class indiv
{
    private:
        double x,p,q;
        vector<double>X;
        vector<double>P;
        vector<double>Q;
        double a; // attractiveness

    public:
        indiv()
        {
            // Make vector of loci the size of the number of loci
            int i;
            X.resize(Nx);
            P.resize(Np);
            Q.resize(Nq);

            // Initialize them all as 0.0
            for(i=0;i<Nx;i++) X[i]=0.0;
            for(i=0;i<Np;i++) P[i]=0.0;
            for(i=0;i<Nq;i++) Q[i]=0.0;
            x=0.0; p=0.0; q=0.0;
            a=0.0;
            return;
        }
        indiv(const indiv &y)
        {
            // Make vector of loci the size of the number of loci
            int i;
            X.resize(Nx);
            P.resize(Np);
            Q.resize(Nq);

            // set them all to the value of individual y.
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
            // Initialize all loci to the 0value of the loci + a random mutation
            for(i=0;i<Nx;i++) X[i]=x0+Normal(0.0,sv);
            for(i=0;i<Np;i++) P[i]=p0+Normal(0.0,sv);
            for(i=0;i<Nq;i++) Q[i]=q0+Normal(0.0,sv);
            x=x0+Normal(0.0,sv); p=p0+Normal(0.0,sv); q=q0+Normal(0.0,sv);
            return;
        }
        // Make a new baby from male m and female f
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
                    // Pick mother's locus or father's locus
                    if(Uniform()<0.5)
                        X[i]=m.X[i];
                    else
                        X[i]=f.X[i];

                    // Mutate locus
                    X[i]+=Normal(0.0,sv);
                    x+=X[i];
                }
                for(i=0;i<Np;i++)
                {
                    // Pick mother's locus or father's locus
                    if(Uniform()<0.5)
                        P[i]=m.P[i];
                    else
                        P[i]=f.P[i];

                    // Mutate locus
                    P[i]+=Normal(0.0,sv);
                    p+=P[i];
                }
                for(i=0;i<Nq;i++)
                {
                    // Pick mother's locus or father's locus
                    if(Uniform()<0.5)
                        Q[i]=m.Q[i];
                    else
                        Q[i]=f.Q[i];

                    // Mutate locus
                    Q[i]+=Normal(0.0,sv);
                    q+=Q[i];
                }
            }
            if(diploid)
            {
                if(Nx < 2)
                        throw invalid_argument("Cannot do diploid with 1 x locus");
                if(Np < 2)
                        throw invalid_argument("Cannot do diploid with 1 p locus");
                if(Nq < 2)
                        throw invalid_argument("Cannot do diploid with 1 q locus");

                for(i=0;i<=Nx-2;i+=2)
                {
                    assert(i>=0);
                    // Pick one of each 2 mother's loci
                    if(Uniform()<0.5)
                        X[i]=m.X[i];
                    else
                        X[i]=m.X[i+1];
                    // Pick one of each 2 father's loci
                    if(Uniform()<0.5)
                        X[i+1]=f.X[i];
                    else
                        X[i+1]=f.X[i+1];

                    // Mutate loci
                    X[i]+=Normal(0.0,sv);
                    X[i+1]+=Normal(0.0,sv);
                    x+=X[i]+X[i+1];
                }
                for(i=0;i<=Np-2;i+=2)
                {
                    // Pick one of each 2 mother's loci
                    if(Uniform()<0.5)
                        P[i]=m.P[i];
                    else
                        P[i]=m.P[i+1];

                    // Pick one of each 2 father's loci
                    if(Uniform()<0.5)
                        P[i+1]=f.P[i];
                    else
                        P[i+1]=f.P[i+1];

                    // Mutate loci
                    P[i]+=Normal(0.0,sv);
                    P[i+1]+=Normal(0.0,sv);
                    p+=P[i]+P[i+1];
                }

                for(i=0;i<=Nq-2;i+=2)
                {
                    // Pick one of each 2 mother's loci
                    if(Uniform()<0.5)
                        Q[i]=m.Q[i];
                    else
                        Q[i]=m.Q[i+1];

                    // Pick one of each 2 father's loci
                    if(Uniform()<0.5)
                        Q[i+1]=f.Q[i];
                    else
                        Q[i+1]=f.Q[i+1];

                    // Mutate loci
                    Q[i]+=Normal(0.0,sv);
                    Q[i+1]+=Normal(0.0,sv);
                    q+=Q[i]+Q[i+1];
                }
            }
            // Make average x, p and q
            x /= Nx;
            p /= Np;
            q /= Nq;
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

// Pick random individual
my_iterator randomindividual(void)
{
    bigint k=0;

    // Pick random number between 0 and populationsize
    const bigint j=bigint(floor(Uniform()*popsize));

    // return the random individual
    for(my_iterator i=start(); i!=end();i++,k++)
        if(k==j)
            return i;
    assert(!"Should not get here.");
}


void initialize(void)
{
    bigint j;
    int k;
    indiv I;

    SetSeed(seed);
    I.init(x0,p0,q0);

    for(j=0;j<popsize;j++)
        pop.push_back(I);

    out<<"generation,popsize,rhoxp,rhoxq,rhopq,sx,sp,sq,";

    for(k=0;k<histw;k++)
        out<<","<<(k-histw/2)*histbinx;
    for(k=0;k<histw;k++)
        out<<","<<(k-histw/2)*histbinp;
    for(k=0;k<histw;k++)
        out<<","<<(k-histw/2)*histbinq;
    out<<endl;
}


void output(bigint t, vector<vector<double>> &histX, vector<vector<double>> &histP, vector<vector<double>> &histQ)
{
    double avgp=0.0,avgq=0.0,avgx=0.0, rhoxp,rhoxq,rhopq,
        ssxx=0.0,ssxp=0.0,sspp=0.0,ssxq=0.0,ssqq=0.0,sspq=0.0,dxi,dpi,dqi,delta,
        maxx=0.0,maxp=0.0,maxq=0.0, sx,sp,sq,xi,pi,qi;
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

    vector<double> histXGen;
    vector<double> histPGen;
    vector<double> histQGen;


    assert(histXGen.empty());
    assert(histPGen.empty());
    assert(histQGen.empty());

    for(j=0;j<histw;j++)
    {
        out<<","<<histx[j]/maxx;
        histXGen.push_back(histx[j]/maxx);
    }

    for(j=0;j<histw;j++)
    {
        out<<","<<histp[j]/maxp;
        histPGen.push_back(histp[j]/maxp);
    }

    for(j=0;j<histw;j++)
    {
        out<<","<<histq[j]/maxq;
        histQGen.push_back(histq[j]/maxq);
    }
    histX.push_back(histXGen);
    histP.push_back(histPGen);
    histQ.push_back(histQGen);

    histXGen.clear();
    histPGen.clear();
    histQGen.clear();

    out<<endl;
    return;
}

void iterate(vector<vector<double>> &histX, vector<vector<double>> &histP, vector<vector<double>> &histQ)
{
    my_iterator i,j;    // iterates through a vector/list (Keeps track of the individual
    indiv kid;          // potential baby
    bigint k,t;
    double nkid,comp,xi,pi,qi,xj,qj,attractiveness,draw;

    for(t=0;t<=endtime;t++)
    {
        if(popsize==0) break;

        if(t%outputfreq==0) output(t, histX, histP, histQ); // Output once every outputfreq

        for(k=0;k<popsize;k++)
        {
            if(popsize==0) break;

            // Pick random individual and get its x, p and q loci
            i = randomindividual();
            xi=i->_x();
            pi=i->_p();
            qi=i->_q();

            comp=0.0; // competition

            for(j=start();j!=end();j++) // Go through all individuals
            {
                if(j != i)
                {
                    xj= j -> _x();          // As long as J is not I, make xj j's x.
                    comp+=gauss(xi-xj,sc);  // Add intensity competition
                }
            }

            // If individual survives calculate its attractiveness
            if(Uniform() < (1.0 - comp * c / gauss(xi,sk)) * (0.5+0.5*gauss(qi,sq))) // 1.0 - comp ... ...sq))) == survival rate
            {
                attractiveness=eta;
                for(j=start();j!=end();j++)
                {
                    if(j!=i)
                    {
                        qj=j->_q();
                        xj=j->_x();
                        attractiveness+=gauss(pi-qj,sm)*gauss(xi-xj,se); // gauss ... ... se) == A ik --> attractivenes == formula[2] under the devision line.
                        j->a_(attractiveness); //set j's a to attractiveness.
                    }
                }

                for(nkid=0.0;;nkid+=1.0)
                {
                    if(Uniform()>=b-nkid) break; // have max b kids

                    draw=Uniform()*attractiveness;
                    if(draw>eta) // can female find an attractive male?
                    {
                        for(j=start();j!=end();j++) // Go through all individuals
                        {
                            if(j!=i && draw<=j->_a()) // if male is attractive enough
                            {
                                kid.birth((*i),(*j)); // Make a baby between randomindividiual and first individual with high enough attractiveness j
                                pop.push_back(kid); // add kid to population
                                popsize++;
                                break; // stop looking for mate
                            }
                        }
                    }
                }
            }
            // Then kill the individual (females die after mating)
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

    cout<<"reading parameters and initializing"<<endl;
    if(!fp) invalid_argument("Can't find parameter file.");
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
            if(!out) invalid_argument("Unable to open datafile.");
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

// Count number of borders (from 0 to >0 or from >0 to 0) in a histogram
int countBorders(const vector<double> &histogram)
{
    if (histogram.empty()) throw std::invalid_argument("Histogram is empty");

    int size = static_cast<int>(histogram.size());
    int numOfBorders{0};
    for (int i = 0; i<size; ++i)
    {
        double l, r, o = histogram[i];
        if (i==0) l = 0.0;
        else l = histogram[i-1];
        if (i==size-1) r = 0.0;
        else r = histogram[i+1];

        if ((i==0 && r >= 0.05) || (i==size-1 && l >=0.05)) o = 0.0;
        if (l >= 0.05 && o < 0.05 && r < 0.05) ++numOfBorders;
        if (l < 0.05 && o < 0.05 && r >= 0.05) ++numOfBorders;
    }

    return numOfBorders;
}

// calculates lineages (borders / 2) and the trait with the most lineages becomes
// the number of lineages for that generation
int countLineagesForGen(const int t, const vector<vector<double>> &histX, const vector<vector<double>> &histP, const vector<vector<double>> &histQ)
{
    if (t < 0) throw std::invalid_argument("Time can't be negative");
    if (histX.empty()) throw std::invalid_argument("HistX is empty");
    if (histP.empty()) throw std::invalid_argument("HistP is empty");
    if (histQ.empty()) throw std::invalid_argument("HistQ is empty");

    int xBorders = countBorders(histX[t]);
    int pBorders = countBorders(histP[t]);
    int maxBorders = countBorders(histQ[t]);
    if (xBorders > maxBorders) maxBorders = xBorders;
    if (countBorders(histQ[t]) > maxBorders) maxBorders = pBorders;
    return maxBorders / 2;
}

//output all number of lineages for all the generations
void outputLTT(const vector<vector<double>> &histX, const vector<vector<double>> &histP, const vector<vector<double>> &histQ)
{
    if (histX.empty()) throw std::invalid_argument("HistX is empty");
    if (histP.empty()) throw std::invalid_argument("HistP is empty");
    if (histQ.empty()) throw std::invalid_argument("HistQ is empty");

    ofstream LTT("ltt.csv");

    for (int i = 0; i < static_cast<int>(histX.size()); ++i)
        LTT << i * outputfreq << "," << countLineagesForGen(i, histX, histP, histQ) << '\n';
}

int main(int argc, char *argv[])
{
    if(argc==1) invalid_argument("no parameterfile specified");
    readparameters(argv[1]);
    initialize();

    vector<vector<double>> histX;
    vector<vector<double>> histP;
    vector<vector<double>> histQ;

    iterate(histX, histP, histQ);

    outputLTT(histX, histP, histQ);


  return 0;
}
