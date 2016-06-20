/*
 * counted_ptr - simple reference counted pointer.
 *
 * The is a non-intrusive implementation that allocates an additional
 * int and pointer for every counted object.
 */

#ifndef COUNTED_PTR_H
#define COUNTED_PTR_H

/* For ANSI-challenged compilers, you may want to #define
 * NO_MEMBER_TEMPLATES or explicit */

template <class X> class CCountedPointer{

public:

	// allocate a new counter
    explicit CCountedPointer(X* p = 0) : itsCounter(0) 
	{if (p) itsCounter = new counter(p);}

    ~CCountedPointer()	{release();}

    CCountedPointer(const CCountedPointer& r) throw()	{ acquire(r.itsCounter); }
    CCountedPointer& operator=(const CCountedPointer& r)
    {
        if (this != &r) {
            release();
            acquire(r.itsCounter);
        }
        return *this;
    }

    X& operator*()  const throw()   {return *itsCounter->ptr;}
    X* operator->() const throw()   {return itsCounter->ptr;}
    X* get()        const throw()   {return itsCounter ? itsCounter->ptr : 0;}
    bool unique()   const throw()	{return (itsCounter ? itsCounter->count == 1 : true);}

private:

    struct counter{
        counter(X* p = 0, unsigned c = 1) : ptr(p), count(c) {}
        X*          ptr;
        unsigned    count;
    }* itsCounter;

    void acquire(counter* c) throw()
    { // increment the count
        itsCounter = c;
        if (c) ++c->count;
    }

    void release()
    { // decrement the count, delete if it is 0
        if (itsCounter) {
            if (--itsCounter->count == 0) {
                delete itsCounter->ptr;
                delete itsCounter;
            }
            itsCounter = 0;
        }
    }
};

#endif // COUNTED_PTR_H