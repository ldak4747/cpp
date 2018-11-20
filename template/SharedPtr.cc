#include "SharedPtr.hpp"
#include <iostream>
using namespace std;

int main () {
    SharedPtr<int> sa;
    cout << sa.use_count() << endl;

    SharedPtr<int> sb(new int(1)), sc(new int(2));
    cout << sb.use_count() << endl;
    cout << sc.use_count() << endl;

    sb = sc;
    cout << sb.use_count() << endl;
    cout << sc.use_count() << endl;
    
    return 0;
}
