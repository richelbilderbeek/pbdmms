#include "kewe_individual.h"

#include <cassert>
#include <stdexcept>
#include <iostream>
#include "kewe_globals.h"
#include "kewe_random.h"

indiv::indiv()
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

indiv::indiv(const indiv &y)
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

void indiv::init(double x0, double p0, double q0)
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
void indiv::birth(indiv m, indiv f)
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
                throw std::invalid_argument("Cannot do diploid with 1 x locus");
        if(Np < 2)
                throw std::invalid_argument("Cannot do diploid with 1 p locus");
        if(Nq < 2)
                throw std::invalid_argument("Cannot do diploid with 1 q locus");

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

void indiv::print(void)
{
    int i;
    std::cout<<x<<" "<<p<<" "<<q<<std::endl;
    for(i=0;i<Nx;i++) std::cout<<X[i]<<" ";
    std::cout<<std::endl;
    for(i=0;i<Np;i++) std::cout<<P[i]<<" ";
    std::cout<<std::endl;
    for(i=0;i<Nq;i++) std::cout<<Q[i]<<" ";
    std::cout << std::endl;
}

