#ifndef headerfile
    #include <stdint.h>
    #include <stdlib.h>
    #include "user_defined_datatypes.c"
    #include "constants.c"
    #include "logical_functions_of_decision.c"
    #include "bitwise_functions.c"
    #include "safe_arithmetic_functions.c"
    #include "new_vector_functions.c"
#endif


/*
#include <iostream>
using namespace std;
typedef unsigned int uint;

uint i;

template <typename type>
struct Data{
    type val;
    Data(type a = 0) : val(a){}
    void setValue(type a){ val = a; }
    type operator +(Data<type> a){ return type{val + a.val}; }
    bool operator >(Data<type> a){ return val > a.val; }
    bool operator <(Data<type> a){ return val < a.val; }
    template<typename t>
    friend ostream& operator<<(ostream& os, Data<t> a){ return os << a.val; }
};

template <typename type>
struct GetResult{
    template<typename t>
    static t getMax(Data<t> *a, uint n){
        t most;
        if(n) most = a[0].val;
        for(i = 1; i < n; i++){
            if(a[i].val > most){ most = a[i].val; }
        }
        return most;
    }
    template<typename t>
    static t getMin(Data<t> *a, uint n){
        t most;
        if(n) most = a[0].val;
        for(i = 1; i < n; i++){
            if(a[i].val < most){ most = a[i].val; }
        } 
        return most;
    }
    template<typename t>
    static t getSum(Data<t> *a, uint n){
        t accum = 0;
        for(i = 0; i < n; i++) accum += a[i].val;
        return accum;
    }
};

int main()
{
    Data<int> iData[1001];
    Data<double> dData[1001];
    int cases, num;
    char ch;
    int u;
    double v;
    Data<int> a(10), b(20);
    Data<double> c(3.14), d(-4.1);
    cout<<"a + b = "<<(a + b)<<endl;
    cout<<"max(a, b) = "<<(a > b ? a : b)<<endl;
    cout<<"min(a, b) = "<<(a < b ? a : b)<<endl;
    cout<<"c + d = "<<(c + d)<<endl;
    cout<<"max(c, d) = "<<(c > d ? c : d)<<endl;
    cout<<"min(c, d) = "<<(c < d ? c : d)<<endl;
    cin>>cases;
    for (int i = 0; i < cases; i++)
    {
        cin>>ch;
        cin>>num;
        for (int j = 0; j < num; j++)
        {
            if (ch == 'i')
            {
                cin>>u;
                iData[j].setValue(u);
            }
            else if (ch == 'd')
            {
                cin>>v;
                dData[j].setValue(v);
            }
        }
        if (ch == 'i')
        {
            cout<<GetResult<int>::getMax(iData, num);
            cout<<" "<<GetResult<int>::getMin(iData, num);
            cout<<" "<<GetResult<int>::getSum(iData, num)<<endl;
        }
        else if (ch == 'd')
        {
            cout<<GetResult<double>::getMax(dData, num);
            cout<<" "<<GetResult<double>::getMin(dData, num);
            cout<<" "<<GetResult<double>::getSum(dData, num)<<endl;
        }
    }
    return 0;
}

*/

/*
int main(){
    int arr[3] = {1,2,3};
    int *pp = arr;
    int* p = (int*)(&arr + 1);
    int **ppp = &pp;
    cout << *arr << '\n' << *((int*)(&arr + 1) -1)<< '\n' << *(arr + 1);
    // cout << pp << '\n' << &p << '\n' << &ppp;
    // cout << p << '\n' << *(&arr + 1) << '\n' << (arr + 3);
    // cout << *(*(&arr + 1) - 1);
    // cout << (arr == &(arr[0]));
    // cout << (p == (arr + 3));

}


*/


/*
struct Citrus{
    string al;
    double w;
    Citrus(string a = "", double b = 0) : al(a), w(b){ }
    void name_w(){ cout << al << ' ' << w; }
    void show(){ name_w(); cout << "kg, is citrus fruit.\n"; }
};


struct Mandarin : virtual Citrus{
    void show(){ name_w(); cout << "kg, is mandarin.\n"; }
};

struct Pomelo : virtual Citrus{
    void show(){ name_w(); cout << "kg, is pomelo.\n"; }
};

struct Citron : virtual Citrus{
    void show(){ name_w(); cout << "kg, is citron.\n"; }
};


struct Orange : virtual Pomelo, Mandarin{
    void show(){ name_w(); cout << "kg, is orange.\n"; }
};

struct Lime : virtual Pomelo, virtual Citron{
    void show(){ name_w(); cout << "kg, is lime.\n"; }
};


struct Tangerine : virtual Mandarin, virtual Orange{
    Tangerine(string a = "", double b = 0) : Citrus(a, b){}
    void show(){ name_w(); cout << "kg, is tangerine.\n"; }
};

struct Grapefruit : virtual Pomelo, virtual Orange{
    Grapefruit(string a = "", double b = 0) : Citrus(a, b){}
    void show(){ name_w(); cout << "kg, is grapefruit.\n"; }
};

struct Lemon : virtual Orange, virtual Lime{
    Lemon(string a = "", double b = 0) : Citrus(a, b){}
    void show(){ name_w(); cout << "kg, is lemon.\n"; }
};


int main() // WHY THERE IS STILL "VIRTUAL" QUALIFIER AFTER LEMON AND GRAPEFRUIT DEFINITION BEFORE ITS BASES???????????????????????? 
{
    Citrus     *c;
    Mandarin   *ma;
    Pomelo     *po;
    Citron     *ci;
    Orange     *og;
    Lime       *li;
    Tangerine  *ta;
    Grapefruit *gr;
    Lemon      *le;
 
    string name;    
    double weight;
    while(cin >> name >> weight)
    {
        if(name == "Tangerine")
        {
            Tangerine tangerine(name, weight);
            c = ma = og = ta = &tangerine;
            c->show();
            ma->show();
            og->show();
            ta->show();
        }
        if(name == "Grapefruit")
        {
            Grapefruit grapefruit(name, weight);
            c = po = gr = &grapefruit;
            c->show();
            po->show();
            gr->show();
        }
        if(name == "Lemon")
        {
            Lemon lemon(name, weight);
            ma = og = &lemon;
            ci = li = &lemon;
            po = le = &lemon;
            ma->show();
            po->show();
            ci->show();
            og->show();
            li->show();
            le->show();
        }
    }
}

*/
