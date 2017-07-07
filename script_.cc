#include <stdlib.h>
#include "config.h"
#include "regx.h"

int main(int argc, char* argv[])
{
  string filename="aut.dat";
  state q(0);

  if(argc>1)
    filename=argv[1];
  if(argc>2)
    q=state (atoi(argv[2]));

  connx aut;

  CONFIG modfile(filename);
  modfile.set_map<state,symbol,state>(aut,"CONNX");

  regx__ R(aut);
  R.set_human_readable(vector <string> {"H","I","O" });

  cout<<R.version() <<endl;
  cout << R(q) << endl;

  return 0;
}
