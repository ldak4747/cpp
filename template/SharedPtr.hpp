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

    friend class WeakPtr<T>;

    void release();
public:
    SharedPtr(T *p = nullptr);
    ~SharedPtr();
    SharedPtr(const SharedPtr &other);
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

template<class T> void WeakPtr<T>::release () {
    if (cnt) {
        --cnt->wu;
        if (cnt && 0 == cnt->wu) {
            delete cnt;
            cnt = nullptr;
        }
    }
}

template<class T> WeakPtr<T>::~WeakPtr () {
    release();
}

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
