#ifndef LPTLIST_H
#define LPTLIST_H

#include <QVector>

namespace LPT {

template<class T> class LPTList {
public:
    LPTList() {}
    // copy constructor
    LPTList(LPTList &other) : list(other.list) {}
    // Returns the item at index position i in the list.
    inline T at(int i) const {return list.at(i);}
    // Returns true if the list contains no items; otherwise returns false.
    inline bool isEmpty() const {return list.isEmpty();}
    // Returns the number of occurrences of value in the list.
    inline int count() const {return list.count();}
    // clears list
    inline void clear() {list.clear();}
    // Returns a reference to the first item in the list.
    inline T & first() {return list.first();}
    // Returns a reference to the last item in the list.
    inline T & last() {return list.last();}
    // Inserts value at the beginning of the list.
    inline void prepend(T value) {list.prepend(value);}
    // Inserts value at the end of the list.
    inline void append(T value) {list.append(value);}
    // Inserts entire list content at end of list
    inline void append(const LPTList<T> &otherList) {list.append(otherList.list);}
    // Inserts value at index position i in the list.
    inline void insert(int i, T value) {vlistinsert(i, value);}
    // Removes the item at index position i.
    inline void removeAt(int index) {list.removeAt(index);}
    // Removes the first occurrence of value in the list and returns true on success; otherwise returns false.
    inline bool removeOne(T value) {return list.removeOne(value);}
    // Returns the index position of the first occurence of value in the list. Returns -1 if no item matched.
    inline int indexOf(const T &value) {return list.indexOf(value);}
    // Returns true if the list contains an occurrence of value, otherwise returns false. Requires T to have an implementation of operator==
    inline bool contains(const T &value) {return list.contains(value);}
protected:
    QVector<T> list;
};


// attention - T must have sensible comparision operator ==
template<class T> class LPTPtrList : public LPTList<T *>{
public:
    LPTPtrList() {}
    LPTPtrList(LPTPtrList &other) : LPTList<T *>(other) {}
    inline bool containsPtrToEquivalentObject(T *o) {
        T *elem; bool contains = false;
        for (int i=0; i<LPTList<T *>::list.count(); ++i) {
            elem = LPTList<T *>::list.at(i);
            if (*elem == *o) {
                contains = true;
                break;
            }
        }
        return contains;
    }
};

} // end namespace LPT
#endif // LPTLIST_H
