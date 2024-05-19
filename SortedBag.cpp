#include <iostream>
#include <cstring>
#include "SortedBag.h"

#include "SortedBagIterator.h"

SortedBag::SortedBag(Relation r) {
	this->r=r;
    capacityOfArray=15;
    elements=new Node[capacityOfArray];
    firstEmpty=0;
    sizeOfBag=0;
}

int SortedBag::hash(TComp e)const{
    if (e<0) {
        e=-e;
    }
    return e%capacityOfArray;
}
void SortedBag::setNextEmpty() {
    if(elements[firstEmpty].data==NULL_TCOMP) {
        return;
    }
    for(int i=0;i<capacityOfArray;i++) {
        if (elements[i].data==NULL_TCOMP) {
            firstEmpty=i;
            return;
        }
    }
}
void SortedBag::resize() {
    capacityOfArray <<= 1;
    int localFirstEmpty = 0;
    Node *newElements = new Node[capacityOfArray];
    for (int i = 0; i < capacityOfArray; i++) {
        newElements[i].data = NULL_TCOMP;
        newElements[i].next = -1;
    }
    for (int i = 0; i < capacityOfArray >> 1; i++) {
        int index = hash(elements[i].data);
        if (elements[i].data != NULL_TCOMP) {
            if (newElements[index].data == NULL_TCOMP) {
                newElements[index].data = elements[i].data;
            } else {
                int current = index;
                while (newElements[current].next != -1) {
                    current = newElements[current].next;
                }
                newElements[current].next = localFirstEmpty;
                newElements[localFirstEmpty].data = elements[i].data;
                for(int j=firstEmpty;j<capacityOfArray;j++) {
                    if (newElements[j].data==NULL_TCOMP) {
                        localFirstEmpty=j;
                        break;
                    }
                }
                for(int j=0;j<capacityOfArray;j++) {
                    if (elements[j].data == elements[i].data) {
                        elements[j].next = -1;
                        break;
                    }
                }
            }
        }
    }
        delete[] elements;
        elements = newElements;
        firstEmpty = localFirstEmpty;
    }

void SortedBag::add(TComp e) {
    int index=hash(e);
    sizeOfBag++;
    if (1.0*sizeOfBag/capacityOfArray>0.75) {
        resize();
    }
    //if the element position is empty just add it
    if (elements[index].data==NULL_TCOMP) {
        elements[index].data=e;
        setNextEmpty();
        return;
    }
    //if the element is already in the bag
    //add it to the first empty position then link it to the previous element
    int current=index;
    while (elements[current].next!=-1)
        current=elements[current].next;
    elements[firstEmpty].data=e;
    elements[firstEmpty].next=-1;
    elements[current].next=firstEmpty;
    setNextEmpty();
}


bool SortedBag::remove(TComp e) {
    int pos=hash(e);
    int prevpos=-1;
    //search for the element in the bag
    while(pos!=-1 and elements[pos].data!=e) {
        prevpos=pos;
        pos=elements[pos].next;
    }
    if(pos==-1) {
        return false;
    }
    //we know that the element is in the bag
    sizeOfBag--;
    bool over=false;
    do{
        int p=elements[pos].next;
        int pp=pos;
        while(p!=-1 and hash(elements[pos].data)!=pos ) {
            pp=p;
            p=elements[p].next;
        }
        if(p==-1) {
            over=true;
        }
        else{
            elements[pos].data=elements[p].data;
            pos=p;
        }
    } while (!over);
    if(prevpos==-1) {
        auto idx=0;
        while(idx<capacityOfArray and prevpos==-1) {
            if(elements[idx].next==e) {
                prevpos=idx;
            }
            else {
                idx++;
            }
        }
    }
    if(prevpos!=-1) {
        elements[prevpos].next=elements[pos].next;
    }
    elements[pos].data=NULL_TCOMP;
    elements[pos].next=-1;
    if(firstEmpty>pos) {
        firstEmpty=pos;
    }
    return true;
}


bool SortedBag::search(TComp elem) const {
    int index=hash(elem);
    while(index!=-1) {
        if (elements[index].data==elem) {
            return true;
        }
        index=elements[index].next;
    }
    return false;
}



int SortedBag::nrOccurrences(TComp elem) const {
    int index=hash(elem);
    int count=0;
    while(index!=-1) {
        if (elements[index].data==elem) {
            count++;
        }
        index=elements[index].next;
    }
    return count;
}



int SortedBag::size() const {
    return sizeOfBag;
}


bool SortedBag::isEmpty() const {
    return sizeOfBag==0;
}


SortedBagIterator SortedBag::iterator() const {
	return SortedBagIterator(*this);
}


SortedBag::~SortedBag() {
	delete[] elements;
}
