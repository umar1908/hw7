#include <iostream>
#include <iomanip>
using namespace std;
// #include <functional>
#include <algorithm>
// #include <vector>
// #include <string>
// #include <cmath>
// #include <cstdlib>
// #include<utility>
// #include <exception>
// #include <bitset>
// #include<random>
#include <random>
#include <chrono>
// #include <execution>


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




// Мой компиялтор не поддерживает библиотеку <execution> для параллельного выполнения
// стандартных алгоритмов
//пишу код вслепую.
int main(){
  system("chcp 1251"); //для корректного отображения кирилицы
  cout<<endl;
  mt19937 mersene (time(0));
  uniform_int_distribution<int> gen(0,100000);


  const int N=10000000;
  vector<int> v_ref(N);
  vector<int> v_test(N);
  for(auto& elem:v_ref){
    elem=gen(mersene);
  }
  v_test=v_ref;

  Timer time_ref;
  Timer time_test;


  cout<<"Тестирование алгоритма sort\n";
  time_ref.start();
  sort(v_ref.begin(),v_ref.end());
  time_ref.stop();
  time_test.start();
  sort(execution::par,v_test.begin(),v_test.end());
  time_test.stop();
  cout<<"Последовательный вариант: "<<time_ref.get()<<" ms"<<endl;
  cout<<"Параллельное время: "<<time_test.get()<<" ms"<<endl;
  cout<<endl<<endl<<endl;

  cout<<"Тестирование алгоритма accumulate\n";
  time_ref.start();
  accumulate(v_ref.begin(),v_ref.end(),0);
  time_ref.stop();
  time_test.start();
  accumulate(execution::par,v_test.begin(),v_test.end(),0);
  time_test.stop();
  cout<<"Последовательный вариант: "<<time_ref.get()<<" ms"<<endl;
  cout<<"Параллельное время: "<<time_test.get()<<" ms"<<endl;
  cout<<endl<<endl<<endl;

  cout<<"Тестирование алгоритма transform\n";
  vector<int> out(N+100);
  time_ref.start();
  transform(v_ref.begin(), v_ref.end(), out.begin(),[](int v) { return v * v; });
  time_ref.stop();
  time_test.start();
  transform(execution::par, v_test.begin(), v_test.end(), out.begin(),[](int v) { return v * v; });
  time_test.stop();
  cout<<"Последовательный вариант: "<<time_ref.get()<<" ms"<<endl;
  cout<<"Параллельное время: "<<time_test.get()<<" ms"<<endl;
  cout<<endl<<endl<<endl;




}
