#include<fstream>
#include<iostream>
#include<sstream>
#include<string>
#include<map>
#include<vector>
#include<stdlib.h>


#define DEBUG 0

using namespace std;
//--------------------
ostream& operator << (ostream &out, map<string,double>& s)
{
  //char sep = '|';
  for (map<string,double>::iterator itr=s.begin();
       itr != s.end();
       ++itr)
    out << itr->first << " " << itr->second << endl;
  return out;
}

ostream& operator << (ostream &out, map<string,unsigned int>& s)
{
  //char sep = '|';
  for (map<string,unsigned int>::iterator itr=s.begin();
       itr != s.end();
       ++itr)
    out << itr->first << " " << itr->second << endl;
  return out;
}

ostream& operator << (ostream &out, vector<unsigned int>& s)
{
  for(vector<unsigned int>::iterator itr=s.begin();
      itr!=s.end();
      ++itr)
    out << *itr << " ";
  return out;
}
ostream& operator << (ostream &out, vector<double>& s)
{
  for(vector<double>::iterator itr=s.begin();
      itr!=s.end();
      ++itr)
    out << *itr << endl;
  return out;
}

//-------------------
//-------------------


bool check_(map<string,double>& cf,map<string,unsigned int>& cl)
  {
    for(map<string,double>::iterator itr=cf.begin();
	itr!=cf.end();
	++itr)
      if (cl.find(itr->first)==cl.end())
	return false;
    return true;
  }

//-------------------

vector <double>& construct_mult_vector(map<string,double>& cf,
				       map<string,unsigned int>& cl,
				       vector<double>& mvec)
{
  map<unsigned int,double> mmap;
  for(map<string,double>::iterator itr=cf.begin();
      itr!=cf.end();
      ++itr)
    mmap[cl[itr->first]]=itr->second;
  for(unsigned int i=0;i<cl.size();++i)
    mvec.push_back(mmap[i]);
  return mvec;
}

//-------------------


void load_data_matrix(string filename,
		      vector<vector<double> >& data)
{
  ifstream IN(filename.c_str());
  string line;
  int count=0;
  while(getline(IN,line))
    {
      count++;
      if(count % 10000 == 0)
	cout << count << " " <<flush;
      stringstream ss(line);
      double val;
      vector<double> tmp_;
      
      while(ss>>val)
	tmp_.push_back(val);
      data.push_back(tmp_);
    }
  IN.close();

  unsigned int SZ=data[0].size();
  
  for(unsigned int i=0;i<data.size();++i)
    {
      if (DEBUG)
	cout << data[i].size() << " ";
      if(data[i].size()!=SZ)
	{
	  cout << "SZ MISMATCH" << endl;
	  exit(1);
	}
    }
  cout << "SZ CHECK PASSED" << endl;
  return;
}

//-------------------

double mult_vec(vector<double>& vec1,
		vector<double>& vec2)
{
  double S=0.0;

  if(vec1.size() != vec2.size())
    {
      cout << "VEC SIZE MISMATC" << endl;
      exit(1);
    }
  
  for(unsigned int i=0;i<vec1.size();++i)
    S+=vec1[i]*vec2[i];
  
  return S;
}
//-------------------

void mult_mat_vec(vector<vector<double> >& mat,
			     vector<double>& vec,
			     vector<double>& res)
{
  res.clear();

  for(unsigned int i=0;i<mat.size();++i)
    res.push_back(mult_vec(mat[i],vec));
  
  return;
}

//-------------------
//-------------------

int main(int argc,char* argv[])
{
  string cfile="",colfile="",datfile="";
  string wkfile="", outfile="out.txt";

  if(argc<=1)
    {
      cout << "prog coefffile colfile wkfile datfile outfile" << endl;
      exit(1);
    }
  if(argc>1)
    cfile=argv[1];
  if(argc>2)
    colfile=argv[2];
  if(argc>3)
    wkfile=argv[3];
  if(argc>4)
    datfile=argv[4];
  if(argc>5)
    outfile=argv[5];
  
  map<string,double> coef;
  
  ifstream IN(cfile.c_str());
  string line;
  while(getline(IN,line))
    {
      string var;
      double val;
      stringstream ss(line);
      if(ss>>var>>val)
	coef[var]=val;
    }
  IN.close();

  map<string,unsigned int> col;
  
  ifstream IN1(colfile.c_str());
  while(getline(IN1,line))
    {
      string var;
      unsigned int val;
      stringstream ss(line);
      if(ss>>val>>var)
	col[var]=val;
    }
  IN.close();

  vector<unsigned int> wks;
  ifstream IN2(wkfile.c_str());
  while(getline(IN2,line))
    {
      unsigned int val;
      stringstream ss(line);
      while(ss>>val)
	wks.push_back(val);
    }
  IN.close();

  
  cout << coef.size() << " "
       << col.size() << " "
       << wks.size() << endl;

  cout << coef;
  cout << col;
  cout << wks;


  cout << endl << "CHECK " << check_(coef,col) << " (1 means all coeffs are found in columns) " << endl;

  vector<double> mvec_,res;
  vector<vector<double> > data;

  cout << construct_mult_vector(coef,col,mvec_) << endl;
  
  load_data_matrix(datfile,data);
  mult_mat_vec(data,mvec_,res);

  ofstream out(outfile.c_str());
  out << res;
  out.close();
  
  return 0;
}
