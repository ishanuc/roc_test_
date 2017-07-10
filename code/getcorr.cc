#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <stdlib.h>
#include <gsl/gsl_statistics.h>

using namespace std;

//--------------------------------
void getcorr(unsigned int row,
	     unsigned int col,
	     size_t n,
	     vector<double>& v1,
	     vector<double>& v2,
	     map<unsigned int,
	     map<unsigned int,
	     double> >&M)
{
  //vector <double> tmp(2*n,0.0);
  double val=gsl_stats_correlation(&v1[0],1, &v2[0],1,n);

  val = 1-((val+1)/(2.0));

  M[row][col]=val;
  M[col][row]=  M[row][col];
  return ;
};
//--------------------------------
ostream& operator << (ostream &out, map<unsigned int,
		      map<unsigned int,
		      double> >&M)
{
  for( map<unsigned int,
      map<unsigned int,
      double> >::iterator itr=M.begin();
      itr!=M.end();
      ++itr)
  {
    for(map<unsigned int,
	double>::iterator itr_=itr->second.begin();
	itr_!=itr->second.end();
	++itr_)
    out << itr_->second << " " ;
    out << endl;
  }
  return out;
};
//---------------------------------
int main(int argc, char* argv[])
{
  string datfile="data.dat",ofile="outres.txt";
  if(argc>1)
  datfile=argv[1];
  if(argc>2)
  ofile=(argv[2]);

  string line;
  double val1,val2;
  vector<double> vec1,vec2;
  
  ifstream IN(datfile.c_str());
  while(getline(IN,line))
    {
      stringstream ss(line);
      if(ss>>val1>>val2)
	{
	  vec1.push_back(val1);
	  vec2.push_back(val2);
	}
    }
  IN.close();

  double val=gsl_stats_correlation(&vec1[0],1, &vec2[0],1,vec1.size());

  cout  << val << endl;
  
  ofstream OUT(ofile.c_str());
  OUT << val ;
  OUT.close();

  return 0;
 }
