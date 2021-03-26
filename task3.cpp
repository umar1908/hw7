#include <iostream>
#include <iomanip>
using namespace std;
#include <functional>
// #include <algorithm>
// #include <vector>
// #include <string>
// #include <cmath>
// #include <cstdlib>
// #include<utility>
// #include <exception>
// #include <bitset>
// #include<random>
#include <thread>
#include <numeric>
#include<future>
template<typename Iterator, typename T1, typename T2 >
void parallel_for_each(Iterator first, Iterator last, function<T2(T1)> f){
  const size_t length=distance(first,last);
  const size_t max_size=25;
  if (length<=max_size){
    for_each(first,last,f);
    return;
  }
  Iterator middle = first;
  advance(middle, length / 2);

  future < T2 > first_half_result =
    async(parallel_for_each < Iterator, T1,T2 >, first, middle, f);

  parallel_for_each(middle, last, f);
  first_half_result.wait();
  return;
}


void f(int& elem){
  elem=elem*elem;
}

int main(){
  system("chcp 1251"); //для корректного отображения кирилицы
  cout<<endl;

  vector<int> v(100);
  iota(v.begin(),v.end(),1);
  parallel_for_each(v.begin(),v.end(),function<void(int&)>(f));
  for(const auto& elem:v){
    cout<<elem<<endl;
  }


}
