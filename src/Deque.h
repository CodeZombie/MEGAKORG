#ifndef Deque_H
#define Deque_H

#define DEQUE_MAX_ELEMENTS 5120

template <typename T> class Deque {
    public:
        Deque() {
            this->frontIndex = 0;
            this->backIndex = 0;
        }

        T getElement(int index) {
          return this->element[this->getElementIndex(index)];
        }

        int getElementCount() {
            int newBackIndex = this->backIndex;
            if(newBackIndex < this->frontIndex){
                newBackIndex += DEQUE_MAX_ELEMENTS;
            }
            return newBackIndex - this->frontIndex;
        }

        int pushFront(T element) {
            if(this->getElementCount() == DEQUE_MAX_ELEMENTS){ return 0; }
            this->frontIndex = this->getElementIndex(-1);//set frontIndex back by one
            this->element[this->getElementIndex(0)] = element; //set the array position at position 0 to the value of element
            return 1;
        }

        T popFront() {
            //if(this->getElementCount() == DEQUE_MAX_ELEMENTS){ throw "Cannot pop an empty deque!"; }
            T frontElement = this->element[this->getElementIndex(0)];//get the front element.
            this->frontIndex = this->getElementIndex(1);//move the front index up one.
            return frontElement;
        }

        int pushBack(T element) {
            if(this->getElementCount() == DEQUE_MAX_ELEMENTS){ return 0; }
            this->element[this->backIndex] = element;//set the last element to the value of element
            this->backIndex = this->getElementIndex(this->getElementCount() + 1);//increase backIndex by one.
            return 1;
        }

        int frontIndex;
        int backIndex;
    private:
        
        T element[DEQUE_MAX_ELEMENTS];

        int getElementIndex(int absolutePosition) {
            int pos = this->frontIndex + absolutePosition;
            if(pos >= DEQUE_MAX_ELEMENTS) {
                return pos - DEQUE_MAX_ELEMENTS ;
            }
            if(pos < 0){
              return pos + DEQUE_MAX_ELEMENTS;
            }
            return pos;
        }
};

#endif