#include <bits/stdc++.h>
using namespace std;


int main(){
    priority_queue<int> vec;
    vec.push(1);
    vec.push(2);
    vec.push(3);
    vec.push(4);
    vec.emplace( 5);
    while(!vec.empty()){
        cout << vec.top() << '\n';
        vec.pop();
    }

    return 0;
}