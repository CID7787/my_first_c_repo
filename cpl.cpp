// #define private public

#include <iostream>
#include <iomanip>
using namespace std;

long long unsigned int sum;

class Point{
public:
  double x, y;
  Point() : x(0), y(0) { ++sum; }
  Point(Point& v) : x(v.x), y(v.y) { ++sum; }
  Point(double a, double b) : x(a), y(b) { ++sum; }
  Point(double a) : x(a), y(a) { ++sum; }
  void show(){ cout << "Point : (" << setprecision(16) << x << ", " << setprecision(16) << y << ")\n"; }
  void showSumOfPoint(){ cout << "In total : " << sum << " point.\n"; }
};


void showPoint(Point& a, Point& b, Point& c){
  a.show();
  b.show();
  c.show();
}

int main(){
    int l(0);
    char c;
    double a, b;
    Point p, q, pt[60];
    while(std::cin>>a>>c>>b){
        if(a == b){ p.copy(pt[l].setPoint(a, b)); }
        if(a >  b){ p.copy(pt[l].setPoint(a, b).inverse()); }
        if(a <  b){ p.inverse(pt[l].setPoint(a, b)); }
        if(a <  0){ q.copy(p).inverse(); }
        if(b <  0){ q.inverse(p).copy(pt[l]); }
        pt[l++].show();
        p.show();
    }
    q.show();
    cout<<"==========gorgeous separator=========="<<endl;
    double x(0), y(0);
    for(int i = 0; i < l; i++){
        x += pt[i].x(), y -= pt[i].y();
    }
    pt[l].x(y), pt[l].y(x);
    q.copy(pt[l]).show();
    for(int i = 0; i <= l; i++){
        pt[i].show();
    }
    cout<<"==========gorgeous separator=========="<<endl;
    const Point const_point(3, 3);
    const_point.show();
    for(int i = 0; i <= l; i++){
        if(const_point.isEqual(pt[i])){
            ShowPoint(const_point);
            ShowPoint(const_point.x(), const_point.y());
            ShowPoint(Point(const_point.x(), const_point.y()));
        }
    }
    const_point.showSumOfPoint();
}
/*#1
class Account {
private:
    double balance;
public:
    Account(double initial);
    double getBalance();      // Declaration only
    void deposit(double amount);
};

// Definition outside the class using binary form
double Account::getBalance() {
    return balance;
}

void Account::deposit(double amount) {
    balance += amount;
}

// Constructor definition
Account::Account(double initial) : balance(initial) {
}
*/

/*#2
#include <bits/stdc++.h>
using namespace std;

int main() {
    vector<int> arr = {1, 2, 3, 4, 5};

    // Defining an iterator pointing to
    // the beginning of the vector
    vector<int>::iterator first =
    arr.begin();
      
    cout << *first;
    return 0;
}
*/

/*#3
class some {
public:
  int a;
  template<typename> void func(void) {
    a = b + c;
  }
protected:
  int b;
private:
  int c;
};
 
class hack_template_param{};
 
template<>
void some::func<hack_template_param>(void) {
  c = 0;
}
 
void hack(void) {
  some o;
  o.func<hack_template_param>();
};
*/
/*#4
class some {
public:
  int a;
protected:
  int b;
private:
  int c;
};
 
class hack_some {
public:
  int a;
  int b;
  int c;
};
 
void h(some& obj) {
  reinterpret_cast<hack_some*>(&obj)->c = 0;
}
*/