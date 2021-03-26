#include <iostream>
#include <iomanip>
using namespace std;
// #include <functional>
#include <algorithm>
#include <vector>
// #include <string>
// #include <cmath>
// #include <cstdlib>
// #include<utility>
// #include <exception>
// #include <bitset>
// #include<random>
#include <thread>
#include<future>
#include<numeric>

class Timer{ //класс таймаре
private:
  chrono::time_point<chrono::steady_clock> start_point;
  chrono::time_point<chrono::steady_clock> stop_point;
  chrono::steady_clock::duration result;
public:
  Timer ()=default;
  chrono::time_point<chrono::steady_clock> start () {
    start_point=chrono::steady_clock::now();
    return start_point;
  }
  chrono::time_point<chrono::steady_clock> stop () {
    stop_point=chrono::steady_clock::now();
    result=stop_point-start_point;
    return stop_point;
  }
  auto get() const{
    return chrono::duration_cast<chrono::milliseconds>(result).count();
  }
};




template<typename Iterator,typename T >
T parallel_accumulate(Iterator first, Iterator last, T init,size_t num){
  const size_t length = distance(first, last);
  const size_t elem_per_thread = length/num;
  T result;
  vector<future<T>> futures;
  Iterator start=first;
  Iterator stop=start+elem_per_thread;
  futures.push_back(async(launch::async,[=]{return accumulate(start,stop,T());}));
  for (size_t i = 1; i < num-1; i++) {
    start=stop;
    stop=start+elem_per_thread;
    // cout<<"start: "<<start-first+1<<endl<<"stop: "<<stop-first+1<<endl;
    futures.push_back(   async( launch::async,[=]{return accumulate(start,stop,T());} )   );
  }
  result=accumulate(stop,last,T());

  for(auto& elem:futures){
    // auto buff=elem.get();
    // cout<<buff<<endl;
    result+=elem.get();
  }
  return result;




}
int main(){
  system("chcp 1251"); //для корректного отображения кирилицы
  cout<<endl;
  std::vector<int> v(100000000);
  iota(v.begin(),v.end(),1);
  Timer time;
  time.start();
  cout<<parallel_accumulate(v.begin(),v.end(),0,10000)<<endl;
  time.stop();
  cout<<time.get()<<" ms";


}
