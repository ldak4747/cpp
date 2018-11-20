#pragma once

struct Counter {
    int u = 0;
    int wu = 0;
    Counter(int _u, int _wu): u(_u), wu(_wu){}
};

template<class T> class SharedPtr;

/*
 *  WeakPtr Declare
 * */
template<class T> class WeakPtr {
    T *ptr = nullptr;
    Counter *cnt = nullptr;

//shared_ptr需要能够访问到weak_ptr的ptr和cnt, 因为在需要用weak_ptr创建shared_ptr实例时, 需要将自己的ptr和cnt指向weak_ptr指向的ptr和cnt, 所以需要将SharedPtr<T>置为weak_ptr的友元
    friend class SharedPtr<T>;
    
    void release();    

public:
    WeakPtr() = default;
    ~WeakPtr();
    WeakPtr(const WeakPtr &other);
    WeakPtr(const SharedPtr<T> &other);
    WeakPtr<T> &operator= (const WeakPtr &other);
    WeakPtr<T> &operator= (const SharedPtr<T> &other);

    void reset();
    SharedPtr<T> lock() const;
    bool expired() const;
    int use_count() const;
};

/*
 *  SharedPtr Declare
 * */
template<class T> class SharedPtr {
    T *ptr = nullptr;
    Counter *cnt = nullptr;

//SharedPtr同样需要将WeakPtr<T>置为自己的友元, 因为weak_ptr在构造和赋值时, 都可能需要以一个shared_ptr作为构造或赋值的参数, 此时需要将weak_ptr的ptr和cnt指向shared_ptr的ptr和cnt, 所以需要设置为友元类
    friend class WeakPtr<T>;

    void release();
public:
    SharedPtr(T *p = nullptr);
    ~SharedPtr();
    SharedPtr(const SharedPtr &other);
//这是weak_ptr的存在的要点, 可通过weak_ptr创建shared_ptr, 虽然weak_ptr不影响实际引用计数(u), weak_ptr也不能获取到实际引用对象ptr, 但是持有weak_ptr的对象, 可以通过weak_ptr创建对实际引用对象ptr的shared_ptr, 实现获取实际引用对象ptr的目的, 同时还不会增加引用计数(u)
    SharedPtr(const WeakPtr<T> &other);
    SharedPtr<T> &operator= (const SharedPtr &other);
    void reset(T *p = nullptr);
    T &operator* ();
    T *operator-> ();
    int use_count() const;
};


/*
 *  WeakPtr Impl
 * */
template<class T> WeakPtr<T>::WeakPtr (const WeakPtr &other) {
    if (&other != this) {
        cnt = other.cnt;
        ptr = other.ptr;
        ++cnt->wu;
    }
};

template<class T> WeakPtr<T>::WeakPtr (const SharedPtr<T> &other) {
    cnt = other.cnt;
    ptr = other.ptr;
    ++cnt->wu;
}

//weak_ptr的释放只会影响弱引用计数(wu), 当观察到实际引用计数(u)和弱引用计数(wu)均为0时, 有权释放掉引用计数器
template<class T> void WeakPtr<T>::release () {
    if (cnt) {
        --cnt->wu;
        if (cnt && 0 == cnt->u && 0 == cnt->wu) {
            delete cnt;
            cnt = nullptr;
        }
    }
}

template<class T> WeakPtr<T>::~WeakPtr () {
    release();
}

//weak_ptr可以由另一个weak_ptr赋值, 也可以由一个shared_ptr赋值, 目的都是使自己的ptr和cnt指向赋值对象的ptr和cnt, 注意自减原对象(如果已经绑定了一个对象)的弱引用计数, 增加新指向对象的弱引用计数
template<class T> WeakPtr<T> &WeakPtr<T>::operator= (const WeakPtr &other) {
    if (&other != this) {
        release();
        ptr = other.ptr;
        cnt = other.cnt;
        ++cnt->wu;
    }

    return *this;
}

template<class T> WeakPtr<T> &WeakPtr<T>::operator= (const SharedPtr<T> &other) {
    release();
    cnt = other.cnt;
    ptr = other.ptr;
    if (cnt) {
        ++cnt->wu;
    }

    return *this;
}

//对于weak_ptr, 不存在reset到另一个weak_ptr或shared_ptr的方法, 只能释放自己
//如果一个weak_ptr需要指向另一个对象, 先释放自己再重新指向
//如: weak_ptr.reset(); weak_ptr = another
template<class T> void WeakPtr<T>::reset () {
    release();
    cnt = nullptr;
    ptr = nullptr;
}

template<class T> SharedPtr<T> WeakPtr<T>::lock () const {
    return SharedPtr<T>(*this);
}

template<class T> bool WeakPtr<T>::expired () const {
    if (cnt && cnt->u) {
        return false;
    }
    
    return true;
}

template<class T> int WeakPtr<T>::use_count () const {
    if (cnt) {
        return cnt->wu;
    }

    return 0;
}

/*
 *  SharedPtr Impl
 * */
template<class T> SharedPtr<T>::SharedPtr (T *p) {
    if (p) {
        ptr = p;
        cnt = new Counter(1, 0);
    }
};

template<class T> SharedPtr<T>::~SharedPtr () {
    release();
}

template<class T> void SharedPtr<T>::release () {
    if (cnt) {
        --cnt->u;
        if (0 == cnt->u) {
            delete ptr;
            ptr = nullptr;
            if (0 == cnt->wu) {
                delete cnt;
                cnt = nullptr;
            }
        }
    }
}

template<class T> SharedPtr<T>::SharedPtr (const SharedPtr &other) {
    if (&other != this) {
        cnt = other.cnt;
        ptr = other.ptr;
        if (cnt) {
            ++cnt->u;
        }
    }
}

template<class T> SharedPtr<T>::SharedPtr (const WeakPtr<T> &other) {
    cnt = other.cnt;
    ptr = other.ptr;
    if (cnt) {
        ++cnt->u;
    }
}

template<class T> SharedPtr<T> &SharedPtr<T>::operator= (const SharedPtr &other) {
    if (&other != this) {
        release();
        cnt = other.cnt;
        ptr = other.ptr;
        if (cnt) {
            ++cnt->u;
        }
    }

    return *this;
}

template<class T> T &SharedPtr<T>::operator* () {
    return *ptr;
}

template<class T> T *SharedPtr<T>::operator-> () {
    return ptr;
}

template<class T> int SharedPtr<T>::use_count() const {
    if (cnt) {
        return cnt->u;
    }

    return 0;
}
