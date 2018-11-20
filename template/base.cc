#include "gtest/gtest.h"
#include <vector>

/*
 *  模板函数中, 返回值类型泛型, 如果参数和函数实现, 无法让编译器直观的推断返回值类型时, 必须需要调用方显式实例化
 *  这种情况下, 调用方需要对返回值做显式实例化
 * */
template<class Ret, class Arg1, class Arg2> Ret Max (Arg1 a, Arg2 b) {
    return (a > b)?a:b;
}
TEST (test, function) {
    //参数类型经编译器推断为一个是int, 一个是浮点数, 比大小结果必然是1.1. 同时指定返回值类型是double, 所以返回都是1.1
    double big = Max<double>(1.1, 1);
    std::cout << big << std::endl;                  //1.1
    big = Max<double>(1, 1.1);
    std::cout << big << std::endl;                  //1.1

    //指定了参数1类型为int, 参数2类型为float, 传入的分别是1.1和1, 1.1的int经编译器强制类型转换后为1, 这样参数1和参数2比大小结果是1, 所以返回1
    big = Max<double, int, float>(1.1, 1);
    std::cout << big << std::endl;                  //1
}

/*
 *  长期以来对于模板类的模板函数, 它的声明和定义必须在一起, 也就是必须是一个内联成员函数
 *  那么真的必须是这样吗?
 *
 *  答案是否定的! 看看下面的func方法是怎么实现在类外部定义的~
 *  答案是: 连续两个template
 * */
template<class T1> class C {
    T1 i;

public:
    C(T1 _i): i(_i){}
    template<class T2> void func(T2 j);
};
//how to define C::func?
template<class T1> template<class T2> void C<T1>::func (T2 j) {
    std::cout << ((i > j)?i:j) << std::endl;
}
TEST (test, how_to_define_template_member_function_not_inline) {
    C<int> c(10);
    c.func(20);
}

/*
 *  可以指定默认的模板泛型类型, 注意, 只要必须是编译期间可推断的类型
 * */
template<class T1, class VT = std::vector<T1>, class T2 = double> class C_DEF {
    T1 i;
    VT v;
    T2 d;

public:
    C_DEF(int _i, T2 _d): i(_i), d(_d) {
        v.reserve(i);
    }
    size_t getCapacity () { return v.capacity(); }
    T2 getD () { return d; }
};
TEST (test, default_template_args) {
    C_DEF<int> c(10, 1.1);
    std::cout << c.getCapacity() << std::endl;
    std::cout << c.getD() << std::endl;
}

/*
 *  模板参数不仅可以是泛型, 还可以是非浮点的右值常量, 比如底下的"template<class T, class VT = std::vector<T>, int LEN = 100>"
 *  下面的例子就是, 通过模板指定一个类的默认成员值(最好就是整型右值, 字符串、空指针、浮点数都不可以)
 *  在c++11可以声明时初始化的情况下, 这招优势不再明显
 * */
template<class T, class VT = std::vector<T>, int LEN = 100> class Vector_Array {
    VT v;
    int len = LEN;

public:
    Vector_Array(int _len = -1) {
        if (_len > 0) {
            len = _len;
        }
        v.reserve(len);
    }
    size_t getCapacity() { return v.capacity(); }
};
TEST (test, vector_or_array) {
    Vector_Array<int> v;
    std::cout << v.getCapacity() << std::endl;
}










int main (int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
