#include <iostream>
#include <iomanip>
using namespace std;
// #include <functional>
// #include <algorithm>
// #include <vector>
// #include <string>
// #include <cmath>
// #include <cstdlib>
#include<utility>
// #include <exception>
// #include <bitset>
#include<random>
#include <thread>
#include <future>
#include<cmath>
using Point=pair<double,double>;




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


class Random_Point_Generator {

  mt19937 mersene1;
  mt19937 mersene2;
  uniform_real_distribution<double> generator_x;
  uniform_real_distribution<double> generator_y;
  pair<double,double> point;
public:
  Random_Point_Generator():mersene1(time(0)), generator_x(0,1),mersene2(time(0)+mersene1()), generator_y(0,1)  {}
  Point operator()(){
    return make_pair(generator_x(mersene1),generator_y(mersene2));
  }

};


int calc(int num){
  int inside_count=0;
  Random_Point_Generator point_gen;
  Point curr_point;
  for (size_t i = 0; i < num; i++) {
    curr_point=point_gen();
    double distance=sqrt((curr_point.first)*(curr_point.first)+(curr_point.second)*(curr_point.second));
    if (distance<=1){
      inside_count++;
    }
  }
  return inside_count;
}

template<int N>
class Pi_calc{
double inside_circle;
int number_of_points;



public:
  Pi_calc(int num):number_of_points(num),inside_circle(0){}
  pair<double,double> operator()(){
    Timer time;
    time.start();
    int points_per_thread= number_of_points/N;
    vector<future<int>> futures;
    for (size_t i = 0; i < N-1; i++) {
      futures.push_back(async(launch::async,calc, points_per_thread));
    }
    inside_circle=calc(points_per_thread);
    for(auto& elem: futures){
      inside_circle+=elem.get();
    }

    time.stop();
    return make_pair<double,double>(4*static_cast<double>(inside_circle)/number_of_points,time.get());
  }


};


int main(){
  const int N=10000; // кол-во точек
  system("chcp 1251"); //для корректного отображения кирилицы
 cout<<"Последовательный алгоритм (1 поток)\nОжидайте порядка 30 сек...\n";
 Pi_calc<1> p1(N);
 auto data=p1();
 cout<<"pi= "<<data.first<<endl<<"Time: "<<data.second<<" ms"<<endl<<endl;

 cout<<"Параллельный алгоритм (2 потока)\nОжидайте порядка 20 сек...\n";
 Pi_calc<2> p2(N);
 data=p2();
 cout<<"pi= "<<data.first<<endl<<"Time: "<<data.second<<" ms"<<endl<<endl;


 cout<<"Параллельный алгоритм (3 потока)\n";
 Pi_calc<3> p3(N);
 data=p3();
 cout<<"pi= "<<data.first<<endl<<"Time: "<<data.second<<" ms"<<endl<<endl;

 cout<<"Параллельный алгоритм (4 потока)\n";
 Pi_calc<4> p4(N);
 data=p4();
 cout<<"pi= "<<data.first<<endl<<"Time: "<<data.second<<" ms"<<endl<<endl;

 cout<<"Параллельный алгоритм (5 потоков)\n";
 Pi_calc<5> p5(N);
 data=p5();
 cout<<"pi= "<<data.first<<endl<<"Time: "<<data.second<<" ms"<<endl<<endl;





}
