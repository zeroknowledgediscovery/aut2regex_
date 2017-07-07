#include "regx.h"
//----------------------------------------
//----------------------------------------
const string _EMPTY_="_LAMBDA_";
//----------------------------------------
//---------------------------------------------
const string REGX_VER="regx v0.99 copyright ishanu chattopadhyay May 2016";


ostream& operator << (ostream &out, map_str &s)
{
  for (map_str::iterator itr=s.begin();
       itr != s.end();
       ++itr)
    out << itr->first << ":" << itr->second << " ";
  return out;
}

ostream& operator << (ostream &out, matrix_str &s)
{
  for (matrix_str::iterator itr=s.begin();
       itr != s.end();
       ++itr)
    out << itr->first << " --> "  << itr->second << endl;
  return out;
}

//---------------------------------------------
bool EX_UTIL__::removeSubstrs(string& s,
			      const string& p) {
  size_t n = p.length();
  bool FOUND=false;
  for (size_t i = s.find(p);
       i != string::npos;
       i = s.find(p))
    {
      FOUND = true;
      s.erase(i, n);
    }
  return FOUND;
};
//---------------------------------------------
string EX_UTIL__::removeSubstrs__(string& s) {
  s.erase( std::remove_if( s.begin(), s.end(), ::isspace ), s.end() );
  return s;
};
//---------------------------------------------
void EX_UTIL__::ReplaceStringInPlace(std::string& subject, const std::string& search,
				     const std::string& replace) {
  size_t pos = 0;
  while((pos = subject.find(search, pos)) != std::string::npos) {
    subject.replace(pos, search.length(), replace);
    pos += replace.length();
  }
}
//---------------------------------------------
string EX_UTIL__::trim(const string& str,
		       const string& whitespace)
{
  const auto strBegin = str.find_first_not_of(whitespace);
  if (strBegin == string::npos)
    return ""; // no content

  const auto strEnd = str.find_last_not_of(whitespace);
  const auto strRange = strEnd - strBegin + 1;

  return str.substr(strBegin, strRange);
};
//---------------------------------------------
string EX_UTIL__::brak(string str)
{
  return "{"+str+"}";
};

//---------------------------------------------
void EX_UTIL__::exception(string message,bool ext)
{
  cout << message <<endl;
  if(ext)
    exit(0);
};
//----------------------------------------
//----------------------------------------

regx__::regx__(connx aut)
{
  numstates=aut.size();
  alphabet=0;

  if (numstates>0)
    alphabet=aut[0].size();
  else
    EX_UTIL__::exception("NUMSTATES IS ZERO: CHECK AUT",true);

  /* alphabet intergity check*/
  for(unsigned int i=0;i<numstates;++i)
    if(aut[i].size()!=alphabet)
      EX_UTIL__::exception("AUT ROW SIZES DIFFERENT",true);

  for(unsigned int i=0;i<numstates;++i)
    for(symbol j(0);j<symbol (alphabet);++j)
      if(aut[i][j]>=0)
	{
	  if(_syseq_[i].find(aut[i][j])==_syseq_[i].end())
	    _syseq_[i][aut[i][j]]=EX_UTIL__::brak(std::to_string((int)j));
	  else
	    _syseq_[i][aut[i][j]]+="+"+EX_UTIL__::brak(std::to_string((int)j));
	}
  _syseq_0_=_syseq_;
  _ver_=REGX_VER;
};
//----------------------------------------
string& regx__::version()
{
  return _ver_;
};
//----------------------------------------

void regx__::set_human_readable(vector<string> vecstr)
{
  for(unsigned int i=0;i<vecstr.size();++i)
    _human_r_["{"+to_string(i)+"}"] = vecstr[i];
};

//----------------------------------------

string& regx__::operator()(state q)
{
  for(unsigned int i=0;i<numstates;++i)
    _const__[state (i)]="";
  _const__[q]=_EMPTY_;

  reduce(q);

  return simplify(_const__[q]);
  //return _const__[q];
};
//----------------------------------------

string& regx__::simplify(string& str_)
{
  string str__=str_;

  const string STR1="()";
  const string STR2="(+";
  const string STR2_="(";

  //  EX_UTIL__::removeSubstrs(str_,STR1);
  EX_UTIL__::ReplaceStringInPlace(str_,STR2,STR2_);
  EX_UTIL__::ReplaceStringInPlace(str_,"(*","(");

  for(unsigned int i=0;i<alphabet;++i)
    EX_UTIL__::ReplaceStringInPlace(str_,
				    "({"+to_string(i)+"})",
				    "{"+to_string(i)+"}");
  for(unsigned int i=0;i<alphabet;++i)
    EX_UTIL__::ReplaceStringInPlace(str_,
				    "({"+to_string(i)+"}*)",
				    "{"+to_string(i)+"}*");
  EX_UTIL__::ReplaceStringInPlace(str_,
				  "("+_EMPTY_+")",
				  _EMPTY_);
  EX_UTIL__::ReplaceStringInPlace(str_,
				  _EMPTY_+"*",
				  _EMPTY_);
  EX_UTIL__::ReplaceStringInPlace(str_,
				  _EMPTY_,
				  "L");
  EX_UTIL__::ReplaceStringInPlace(str_,
				  "**",
				  "*");
  //  EX_UTIL__::ReplaceStringInPlace(str_,"+*","+");
  std::size_t found = 0;
  while(true)
    {
      found=str_.find("L",found);
      if(found==string::npos)
	break;
      found++;

      string nx_="",pr_="";
      if(found<str_.length())
	nx_=str_.substr(found,1);
      if(found>1)
	pr_=str_.substr(found-2,1);
      
      for(unsigned int i=0;i<alphabet;++i)
	if((nx_==to_string(i)) 
	   || (pr_==to_string(i))
	   || (pr_=="*")   
	   )
	  str_.replace(found-1,1,"X");
    }
  EX_UTIL__::ReplaceStringInPlace(str_,"X","");
  EX_UTIL__::ReplaceStringInPlace(str_,"L",_EMPTY_);

  if(str_!=str__)
    simplify(str_);

  //return str_;
  return human_readable(str_);
};

//----------------------------------------

string& regx__::human_readable(string& str)
{
  for(unsigned int i=0;i<alphabet;++i)
    if(_human_r_.find("{"+to_string((int)i)+"}")
       !=_human_r_.end())
      EX_UTIL__::ReplaceStringInPlace(str,
				      "{"
				      +to_string((int)i)
				      +"}",
				      _human_r_["{"
						+to_string((int)i)
						+"}"]);
  return str;
};

//----------------------------------------

void regx__::reduce(state q)
{
  vector <state> index_;
  index_.push_back(q);

  for(matrix_str::iterator itr=_syseq_.begin();
      itr!=_syseq_.end();
      ++itr)
    if(itr->first!=q)
      index_.push_back(itr->first);

  if(DEBUG_)
    {
      cout << "LHS:" << endl;
      cout << _syseq_;

      cout << "RHS:" << endl;
      cout << _const__ << endl;
    }

  for(unsigned int i=0;i<_syseq_.size();++i)
    if(_syseq_[index_[i]].find(index_[i])!=_syseq_[index_[i]].end())
      {
	for(unsigned int j=0;j<_syseq_.size();++j)
	  if((i!=j)
	     &&(_syseq_[index_[i]].find(index_[j])!=_syseq_[index_[i]].end()))
	    _syseq_[index_[i]][index_[j]]="("
	      +_syseq_[index_[i]][index_[i]]+")*("
	      +_syseq_[index_[i]][index_[j]]+")";
	if(_const__[index_[i]]!="")
	  _const__[index_[i]]="("+_syseq_[index_[i]][index_[i]]+")*("
	    +_const__[index_[i]]+")";
	_syseq_[index_[i]].erase(index_[i]);
      }

  if(DEBUG_)
    {
      cout << "___ bef el" << endl;
      cout << "LHS:" << endl;
      cout << _syseq_;
      cout << "RHS:" << endl;
      cout << _const__ << endl;
      cout << "-----" << endl;
    }

  //eliminate
  if(index_.size()==1)
    return;

  map_str el_=_syseq_[index_.back()];

  for(unsigned int i=0;i<_syseq_.size();++i)
    if (state (index_[i]) != index_.back())
      for(unsigned int j=0;j<_syseq_.size();++j)
	if ((state (index_[j]) != index_.back()) && (el_.find(index_[j])!=el_.end())) 
	  _syseq_[index_[i]][index_[j]]+="+"
	    +_syseq_[index_[i]][index_.back()]
	    +"("
	    +el_[index_[j]]
	    +")";

  for(unsigned int i=0;i<_syseq_.size();++i)
    if ((state (index_[i]) != index_.back()) && (_const__[index_.back()]!=""))
      _const__[index_[i]]+="+"
	+_syseq_[index_[i]][index_.back()]
	+"("
	+_const__[index_.back()]
	+")";

  if(DEBUG_)
    {
      cout << "___ aft el" << endl;
      cout << "LHS:" << endl;
      cout << _syseq_;

      cout << "RHS:" << endl;
      cout << _const__ << endl;
      cout << "-----" << endl;
    }

  _syseq_.erase(index_.back());
  _const__.erase(index_.back());
  for(unsigned int i=0;i<_syseq_.size();++i)
    _syseq_[index_[i]].erase(index_.back());

  reduce(q);
};
//----------------------------------------

/*
  REDUCTIONS:

  (+ : (
  +) : )
  () : ""
  (.) : .



*/


