#include "SortedBagIterator.h"
#include "SortedBag.h"
#include <exception>
#include <cstring>
#include <algorithm>

using namespace std;

SortedBagIterator::SortedBagIterator(const SortedBag& b) : bag(b) {
    elements = new TComp[b.capacityOfArray];
    int eIn=0;
    for(int i=0;i<b.capacityOfArray;i++) {
        if(b.elements[i].data!=NULL_TCOMP)
            elements[eIn++]=b.elements[i].data;
    }
    current_index=0;
    sort(elements,elements+b.sizeOfBag, bag.r);
}

TComp SortedBagIterator::getCurrent() {
    if(valid())
        return elements[current_index];
    throw std::exception();
}

bool SortedBagIterator::valid() {
    return current_index<bag.sizeOfBag;
}

void SortedBagIterator::next() {
	    if(valid())
        current_index++;
    else
        throw std::exception();
}

void SortedBagIterator::first() {
	    current_index=0;
}
