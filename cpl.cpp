// #define private public

#include <iostream>
#include <iomanip>
using namespace std;


class Point{
public:
  double x, y;
  Point(){ x = y = 0; }
  Point(Point* v){}
  Point(double a, double b) : x(a), y(b){}
  void show(){ 
    cout << "Point : (" << setprecision(16) << x << ", " << setprecision(16) << y << ")" << endl;
  }
  ~Point(){ 
    cout << "Point : (" << setprecision(16) << x << ", " << setprecision(16) << y << ") is erased." << endl;

  }
};

int main(){
  cout << setfill('$')<< setw(4) << 3;
}

// int main(){
//   char c;
//   double a, b;
//   Point q;  
//   while(std::cin>>a>>c>>b){
//     Point p(a, b);
//     p.show();
//   }
//   Point q1(q), q2(1);
//   q1.show();
//   q2.show();
//   q.show();
// }
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