#include<bits/stdc++.h>
#include"Smartptr.h"

// using namespace std;

int main(){
    shared_ptr<int> p1(new int(32));
    std::cout << *p1 << std::endl;

    shared_ptr<int> p2;
    p2 = p1;

    std::cout << *p1 << " " << *p2 << std::endl;
    std::cout << p1.usecount() << " " << p2.usecount() << std::endl;


    unique_ptr<int> t1(new int (111));
    unique_ptr<int> t2(new int (122));
    
    std:: cout << *t1 << " " << *t2 << std::endl;
    t1.swap(t2);
    std:: cout << *t1 << " " << *t2 << std::endl;

    int *t3 = t1.release();
    std::cout << *t3 << std::endl; 

    int a;
    std::cin >> a;
}
