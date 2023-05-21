#pragma once

#include <memory>
#include <algorithm>
#include <limits>

template <typename T>
class MuhVector{
    public:
        typedef size_t size_type;
        typedef T value_type;
        typedef T& reference;
        typedef const T& const_reference;
        typedef T* iterator;
        typedef const T* const_iterator;

    //All of the constructors
        //default constructor
        MuhVector() {create();}
        //fill constructor
        explicit MuhVector(size_type n, const T& t = T{}) { create (n,t); }
        //copy constructor
        MuhVector (const MuhVector& v) { create(v.begin(), v.end()); }
        //range constructor
        template <class InputIterator>
        MuhVector (InputIterator first, InputIterator last) {create(first, last); }
        //move constructor
        MuhVector (MuhVector&& v)
        {
            create();
            swap(v);
            v.uncreate();
        }
        //initializer list constructor
        MuhVector (const std::initializer_list<T> il) {create(il.begin(), il.end());}

    //The lone destructor
        ~MuhVector() {uncreate();}

    //Assgs
        //copy assignment
        MuhVector& operator= (const MuhVector& other)
        {
            if(this != &other)
            {
                uncreate();
                create(other.begin(), other.end());
            }
            return *this;
        }
        // move assignment
        MuhVector& operator= (MuhVector&& other)
        {
            if (this != &other)
            {
                create(other.begin(), other.end());
                uncreate();
            }
            return *this;
        }

    //iterators
        iterator begin() { return dat; }
        const_iterator begin() const { return dat; }
        iterator end() {return avail;}
        const_iterator end() const { return avail; }

    //All of the sizes
        size_type size() const { return avail-dat; }
        size_type max_size() const {return std::numeric_limits<size_type>::max();}
        void resize(size_type siz)
        {
            if (siz < size())
            {
                iterator it = dat + siz;
                while (it != avail)
                {
                    alloc.destroy(it++);
                }
                avail = dat + siz;
            }
            else if (siz > capacity())
            {
                grow(siz);
                std::uninitialized_fill(avail, dat + siz, value_type());
                avail = dat + siz;
            }
            else if(siz > size())
            {
                std::uninitialized_fill(avail, dat + siz, value_type());
                avail = dat + siz;
            }
        }
        void resize (size_type siz, const value_type& val)
        {
            if(siz > capacity())
            {
                grow(siz);
            }

            if (siz > size())
            {
                insert(end(), siz - size(), val);
            }
            else if (siz < size())
            {
                avail = dat + siz;
            }
        }

    //The lone reserve
        size_type capacity() const { return limit - dat; }
        bool empty() const noexcept { return size() == 0; }
        void reserve (size_type n)
        {
            if (n > capacity())
            {
                grow(n);
            }
        }
        void shrink_to_fit(){
            if(limit > avail)
            {
                limit = avail;
            }
        }

    //All of the element access
        T& operator[] (size_type n) { return dat[n]; }
        const T& operator[] (size_type n) const { return dat[n]; }
        reference at (size_type n)
        {
            if (n >= size() || n < 0)
            {
                throw std::out_of_range("Index out of range");
            }
            return dat[n];
        }
        const_reference at (size_type n) const 
        {
            if (n >= size() || n < 0)
            {
                throw std::out_of_range("Index out of range");
            }
            return dat[n];
        }
        reference front()
        {
            return dat[0];
        }
        const_reference front() const
        {
            return dat[0];
        }
        reference back()
        {
            return dat[size() - 1];
        }
        const_reference back() const
        {
            return dat[size() - 1];
        }
        value_type* data() noexcept
        {
            return dat;
        }
        const value_type* data() const noexcept 
        {
            return dat;
        }

    //All of the modifiers
        template <class InputIterator>
        void assign(InputIterator first, InputIterator last)
        {
            uncreate();
            create(first, last);
        }
        void assign (size_type n, const value_type& val)
        {
            uncreate();
            create (n, val);
        }
        void assign (std::initializer_list<value_type> il)
        {
            uncreate();
            create(il);
        }
        void push_back (const value_type& t)
        {
            if (avail == limit)
            {
                grow();
            }
            unchecked_append(t);
        }
        void push_back (value_type&& val)
        {
            if(avail == limit)
            {
                grow();
            }
            unchecked_append(val);
        }
        void pop_back()
        {
            if(avail != dat)
            {
                alloc.destroy(--avail);
            }
        }
        iterator insert (const iterator position, const value_type& val)
        {
            return insert(position, 1, val);
        }
        iterator insert (iterator position, size_type n, const value_type& val)
        {
            if (position < begin() || position > end())
            {
                throw std::out_of_range("Index out of range");
            }
            if (avail + n > limit)
            {
                size_type index = position  - begin();
                grow();
                position = begin() + index;
            }
            for (iterator it = end() + n-1; it != position; --it)
            {
                *it = std::move(*(it - n));
            }
            std::uninitialized_fill(position, position + n, val);
            avail += n;
            return position;
        }
        iterator erase(iterator position)
        {
            if (position < dat || position > avail)
            {
                throw std::out_of_range("Index out of range");
            }
            std::move(position + 1, avail, position);
            alloc.destroy(avail - 1);
            avail--;
            return position;
        }
        iterator erase(iterator first, iterator last)
        {
            iterator new_available = std::uninitialized_copy(last, avail, first);
            iterator it = avail;
            while (it != new_available)
            {
                alloc.destroy(--it);
            }
            avail = new_available;
            return last;
        }
        void swap(MuhVector& x)
        {
            std::swap(dat, x.dat);
            std::swap(avail, x.avail);
            std::swap(limit, x.limit);
        }
        void clear() noexcept
        {
            uncreate();
        }

    //All of the relation operations
        bool operator== (const MuhVector<T>& other) const
        {
            if(size() != other.size())
            {
                return false;
            }
            return std::equal(begin(), end(), other.begin());
        }
        bool operator!= (const MuhVector<T>& other) const
        {
            return !(*this == other);
        }
        bool operator< (const MuhVector<T>& other) const
        {
            return std::lexicographical_compare(begin(), end(), other.begin(), other.end());
        }
        bool operator<= (const MuhVector<T>& other) const
        {
            return !(other < *this);
        }
        bool operator> (const MuhVector<T>& other) const
        {
            return std::lexicographical_compare(other.begin(), other.end(), begin(), end());
        }
        bool operator>= (const MuhVector<T>& other) const
        {
            return !(other > *this);
        }

    //The lone swap
        void swap (MuhVector<T>& x, MuhVector<T>& y)
        {
            std::swap(x, y);
        }
    private:
        iterator dat;
        iterator limit;
        iterator avail;
        std::allocator<T> alloc;
        void create() { dat = avail = limit = nullptr; }
        void create(size_type n, const T& val)
        {
            dat = alloc.allocate(n);
            limit = avail = dat + n;
            std::uninitialized_fill(dat, limit, val);
        }
        void create(const_iterator i, const_iterator j)
        {
            dat = alloc.allocate(j - i);
            limit = avail = std::uninitialized_copy(i, j, dat);
        }
        void uncreate()
        {
            if(dat)
            {
                iterator it = avail;
                while (it != dat)
                {
                    alloc.destroy(--it);
                }
                alloc.deallocate(dat, limit - dat);
            }
            dat = limit = avail = nullptr;
        }
        void grow(size_type new_capacity = 1){
            size_type new_size = std:: max(new_capacity, 2*capacity());
            iterator new_data  = alloc.allocate(new_size);
            iterator new_avail = std::uninitialized_copy(dat, avail, new_data);
            uncreate();
            dat = new_data;
            avail = new_avail;
            limit = dat + new_size;
        }
        void unchecked_append(const T& val)
        {
            alloc.construct(avail++, val);
        }
};