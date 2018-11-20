#include "SharedPtr.hpp"
#include <iostream>
using namespace std;

struct C;
struct P {
    //SharedPtr<C> pc;
    WeakPtr<C> pc;
};
struct C {
    SharedPtr<P> pp;
};

int main () {
    SharedPtr<int> sa;
    cout << sa.use_count() << endl;

    SharedPtr<int> sb(new int(1)), sc(new int(2));
    cout << sb.use_count() << endl;
    cout << sc.use_count() << endl;

    sb = sc;
    cout << sb.use_count() << endl;
    cout << sc.use_count() << endl;

    sb = sa;
    cout << sa.use_count() << endl;
    cout << sb.use_count() << endl;
    cout << sc.use_count() << endl;
{
    SharedPtr<P> sp(new P);
    SharedPtr<C> sc(new C);
    sp->pc = sc;
    sc->pp = sp;
}

    return 0;
}
