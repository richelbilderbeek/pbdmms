#include <fstream>
#include "kewe_parameters.h"

kewe_parameters readparameters(const std::string& /* filename */)
{
    kewe_parameters parameters;
    //parameters.set_n_alleles(read_n_alleles(filename));
    /*
    ifstream fp(filename);
    char s[50],outputfilename[50];
    if(!fp) invalid_argument("Can't find parameter file.");

    std::vector<variable> variables;
    while(fp>>s)
    {
        //variables.push_back({s, })
        if(strcmp(s,"alleles")==0) { fp>>parameters.Nx>>parameters.Np>>parameters.Nq;}
        if(strcmp(s,"histbin")==0) { fp>>histbinx>>histbinp>>histbinq;}
        if(strcmp(s,"seed")==0) {fp>>parameters.seed;}
        if(strcmp(s,"pop0")==0) {fp>>parameters.popsize;}
        if(strcmp(s,"type0")==0){fp>>x0>>p0>>q0;}
        if(strcmp(s,"end")==0) {fp>>endtime;}
        if(strcmp(s,"sc")==0) {fp>>sc;}
        if(strcmp(s,"se")==0) {fp>>se;}
        if(strcmp(s,"sm")==0) {fp>>sm;}
        if(strcmp(s,"sv")==0) {fp>>sv;}
        if(strcmp(s,"sq")==0) {fp>>sq;}
        if(strcmp(s,"sk")==0) {fp>>sk;}
        if(strcmp(s,"c")==0) {fp>>c;}
        if(strcmp(s,"b")==0) {fp>>b;}
        if(strcmp(s,"eta")==0) {fp>>eta;}
        if(strcmp(s,"output")==0)
        {
            fp>>parameters.outputfreq>>outputfilename;
            out.open(outputfilename);
            if(!out) invalid_argument("Unable to open datafile.");
        }
    }
    fp.close();
  */
    return parameters;
}

void create_test_parameter_file(const std::string& filename)
{
    std::ofstream f(filename.c_str());
    f << "STUB";
}
