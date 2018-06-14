#include <time.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <cmath>
#include <cstdlib>
using namespace std;

namespace em = emscripten;

inline int randint(int min, int max, bool isSeed=true){
  if(isSeed)
    srand(time(NULL));
  return min + (rand() % (int)(max - min + 1));
}


template<typename T, typename Functor>
void foreach(vector<T>& v, Functor& func, bool verbose=false){
  for(unsigned int i=0; i<v.size(); i++){
    if(verbose) cout << "\rforeach: " << i+1 << " of " << v.size();
    v[i] = func(v[i]);
  }
  cout << "\r" << endl;
}

template<typename T>
vector<vector<T>> matrixFromJSMatrix(const em::val& matrix){
  vector<em::val> lists = em::vecFromJSArray<em::val>(matrix);
  vector<vector<T>> output(lists.size());

  for(unsigned int i=0; i<lists.size(); i++){
    output[i] = em::vecFromJSArray<T>(lists[i]);
  }

  return output;
}
