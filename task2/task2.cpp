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
#include<fstream>

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
T parallel_accumulate(Iterator first, Iterator last, T init,int num){
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
  const unsigned long int N=100000000; //количество элементов в контейнере
  std::vector<unsigned long long  int> v(N);
  cout<<"Идут вычисления, ожидайте...\n";
  iota(v.begin(),v.end(),1);
  Timer time;
  unsigned long long int  value;
  unsigned long long int reference=accumulate(v.begin(),v.end(),0);
  ofstream file("data.dat");
  if (!file)
    cout<<"error!";
  for (size_t i = 1; i < 1000; i++) {
    time.start();
    value=parallel_accumulate(v.begin(),v.end(),0,i);
    time.stop();
    cout<<"Результат стандартной accumulate (для сверки): "<<reference<<endl;
    cout<<"Результат parallel_accumulate ("<<i<<"потоков): " <<value<<endl;
    cout<<"Time "<<time.get()<<" ms"<<endl<<endl<<endl;
    file<<i<<" , "<<time.get()<<endl;
  }
  file.close();
//Как и ожидалось, оптимальное кол-во потоков равно колличеству вычислительных ядер на моей машине (2 ядра, 4 потока).
// При существенном увеличении кол-ва потоков, время начинает линейно увеличиваться от кол-ва потоков



}
