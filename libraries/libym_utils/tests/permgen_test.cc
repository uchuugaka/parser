// $Id: permgen_test.cc 700 2007-05-31 00:41:30Z matsunaga $

// PermGen のテスト


#include "ym_utils/PermGen.h"


int
main(int argc,
     char** argv)
{
  using namespace nsYm;
  try {
    char buff[1024];
    while ( cin.getline(buff, 1024, '\n') ) {
      char* p = strtok(buff, " \t");
      if ( !p ) {
	cerr << "ERROR: n must be specified" << endl;
	return 2;
      }
      size_t n = atoi(p);
      p = strtok(NULL, " \t");
      if ( !p ) {
	cerr << "ERROR: k must be specified" << endl;
	return 2;
      }
      size_t k = atoi(p);
      if ( n < k ) {
	cerr << "ERROR: n(" << n << ") < k(" << k << ")" << endl;
	return 1;
      }
      PermGen pg(n, k);
      for (PermGen::iterator p = pg.begin(); !p.is_end(); ++ p) {
	for (size_t i = 0; i < k; ++ i) {
	  cout << " " << p(i);
	}
	cout << endl;
      }
      cout << "---" << endl;
    }
  }
  catch ( AssertError a ) {
    cerr << a << endl;
    return 255;
  }

  return 0;
}
