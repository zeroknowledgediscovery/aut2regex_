#ifndef __REGXX_H
#define __REGXX_H

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <set>
#include <algorithm>
#include <boost/serialization/strong_typedef.hpp>

#define DEBUG_ 0

using namespace std;
using namespace boost;

BOOST_STRONG_TYPEDEF(unsigned int,symbol);
typedef vector <symbol> symbol_list_;

typedef int state;

/*!
  \b \f$ \widetilde{\Pi}\f$ is the morph matrix of dimension
  \f$ \vert Q \vert \times \vert \Sigma \vert\f$, such that 
  \f$ \widetilde{\Pi}\vert_{ij}\f$ is the probability of generating 
  symbol \f$j\f$ from state \f$i\f$. It is implemented as a \c map between \c state and \c vector \c <double>. Each \c vector represents a row of the corresponding \b stochastic matrix, and hence must \b sum \b to \b unity.
*/
typedef std::map < state,  vector < double > > pitilde;

/*!
  \c map between \c symbol and \c state. It represents a row of the 
  connection matrix defining the graph of the probabilistic automata.
*/
typedef std::map < symbol, state > map_sym_state;
/*!
  \b connx is the data-type for representing the underlying graph of 
  a probabilistic automata. It is implemented as a \c map between \c state and \c map_sym_state. Each map element  represents a row of the corresponding connection  matrix, such that \c connx \c var[i][j] is the new \c state after symbol \f$j\f$ is fired from \c state \f$i\f$. 
*/
typedef std::map < state,  map_sym_state > connx;

/*!
  \b uiuidbl  is the data-type for representing matrices with \c double entries. 
  It is implemented as a \c map between \c unsigned \c int  
  and a \c map between \c unsigned \c int and a \c double.
*/
typedef std::map < unsigned int, map < unsigned int, double > > matrix_dbl; 


typedef map<state,string> map_str;
typedef map<state,map_str> matrix_str;

//------------------------------------------------
/*!
  simple exception handling.
*/ 
namespace EX_UTIL__
{
  //---------------------------------------------

  /*!
    Utility function to remove substring \c const \c string
    from a given \c string returns string
  */
  string removeSubstrs__(string&);

  /*!
    Utility function to remove substring \c const \c string
    from a given \c string
  */
  bool removeSubstrs(string&, 
		     const string& );
  /*!
    Utility function to replace all occurrences of 
    substring \c const \c string \b search 
    with  \c const \c string \b replace
    from a given \c string subject 
  */
  void ReplaceStringInPlace(string& subject, 
			    const string& search,
			    const string& replace);
  /*!
    Utility function to remove leading and trailing whitespace 
    substring \c consist \c string \b whitespace   from a given \c string.
    The white space can be given to be any string, default is \b whitespace
  */
  string trim(const string& str,
	      const string& whitespace = " ");

  string brak(string str);

  /*! setting ext to \b true exits program.
    Default value (\b false) only issues console message */
  void exception(string message,bool ext=false);

}
//---------------------------------------------
//---------------------------------------------

class regx__
{
  string _ver_;
  unsigned int alphabet;
  unsigned int numstates;

  map<state,string> _reg_;
  map_str _const__;
  matrix_str _syseq_0_;
  matrix_str _syseq_;
  map <string,string> _human_r_;

 public:
  regx__(connx aut);

  void set_human_readable(vector<string>);

  string& operator()(state q);

  string& min(state q);

  void reduce(state );

  string& simplify(string&);
  string& human_readable(string& str);
  string& version();
};


//------------------------------------------------------------
#endif
