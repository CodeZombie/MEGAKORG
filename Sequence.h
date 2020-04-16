#ifndef Sequence_h
#define Sequence_h

#define SEQUENCE_MAX_ELEMENTS 32

template <typename T> class Sequence {
    public:
        Sequence() {
            this->elementCount = 0;
        }

        unsigned char addToPosition(T element, int position){
            if(this->getElementCount() == SEQUENCE_MAX_ELEMENTS){ return 0; }

            if(position < this->getElementCount()) {
                for(int i = (this->getElementCount() - 1); i >= position; i--){
                    //push the value at this index to the index to it's right.
                    // IN:  [1, 2, 3, 4, 5, _, _, _]
                    // OUT: [_, 1, 2, 3, 4, 5, _, _]
                    if(i < (SEQUENCE_MAX_ELEMENTS - 1)) { //if this is not the last element
                        this->element[i+1] = this->element[i];
                    }
                }
            }
            this->element[position] = element;
            this->elementCount++;
            return 1;
        }

        unsigned char addToEnd(T element){
            if(this->getElementCount() == SEQUENCE_MAX_ELEMENTS){
                return 0; //return error
            }

            this->element[this->getElementCount()] = element;
            this->elementCount++;
            return 1;
        }


        T getAndRemoveFromBeginning(){
            T topElement = this->element[0];
            for(int i = 0; i < this->getElementCount() - 1; i++) {
                this->element[i] = this->element[i+1];
            }

            this->elementCount--;
            return topElement;
        }


        unsigned char getElementCount(){
            return this->elementCount;
        }

    private:
        unsigned char elementCount;
        T element[SEQUENCE_MAX_ELEMENTS];
};

#endif
