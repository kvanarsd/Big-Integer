/****************************************************************************************
* Katrina VanArsdale
* kvanarsd
* Pa6
*****************************************************************************************/
#include<iostream>
#include<string>

#include"List.h"

// Node constructor
List::Node::Node(ListElement x){
   data = x;
   next = nullptr;
   prev = nullptr;
}

// Class Constructors & Destructors ----------------------------------------
    
// Creates new List in the empty state.
List::List() {
    frontDummy = new Node(-1000);
    beforeCursor = nullptr;
    beforeCursor = frontDummy;
    backDummy = new Node(-1000);
    afterCursor = nullptr;
    afterCursor = backDummy;


    backDummy->prev = frontDummy;
    frontDummy->next = backDummy;

    num_elements = 0;
    pos_cursor = 0;
}

// Copy constructor.
List::List(const List& L) {
    frontDummy = new Node(-1000);
    beforeCursor = nullptr;
    beforeCursor = frontDummy;
    backDummy = new Node(-1000);
    afterCursor = nullptr;
    afterCursor = backDummy;


    backDummy->prev = frontDummy;
    frontDummy->next = backDummy;

    num_elements = 0;
    pos_cursor = 0;

    Node* N = L.frontDummy->next;
    while(N != L.backDummy) {
        this->insertBefore(N->data);
        N = N->next;
    }
}

// Destructor
List::~List() {
    clear();
    delete frontDummy;
    delete backDummy;
}


// Access functions --------------------------------------------------------

// length()
// Returns the length of this List.
int List::length() const {
    return(num_elements);
}

// front()
// Returns the front element in this List.
// pre: length()>0
ListElement List::front() const {
    if (num_elements == 0) {
        throw std::length_error("List: front(): empty List");
    }
    return(frontDummy->next->data);
}

// back()
// Returns the back element in this List.
// pre: length()>0
ListElement List::back() const {
    if (length() <= 0) {
        throw std::length_error("List: back(): empty List");
    }
    return(backDummy->prev->data);
}

// position()
// Returns the position of cursor in this List: 0 <= position() <= length().
int List::position() const {
    return(pos_cursor);
}

// peekNext()
// Returns the element after the cursor.
// pre: position()<length()
ListElement List::peekNext() const {
    if(pos_cursor >= length()) {
        throw std::range_error("List: peekNext(): cursor at back");
    }
    return(afterCursor->data);
}

// peekPrev()
// Returns the element before the cursor.
// pre: position()>0
ListElement List::peekPrev() const {
    if(pos_cursor <= 0) {
        throw std::range_error("List: peekPrev(): cursor at front");
    }
    return(beforeCursor->data);
}


// Manipulation procedures -------------------------------------------------

// clear()
// Deletes all elements in this List, setting it to the empty state.
void List::clear() {
    moveBack();
    while(length() > 0 && pos_cursor > 0) {
        eraseBefore();
        moveBack();
    }
}

// moveFront()
// Moves cursor to position 0 in this List.
void List::moveFront() {
    beforeCursor = frontDummy;
    afterCursor = frontDummy->next;

    pos_cursor = 0;
}

// moveBack()
// Moves cursor to position length() in this List.
void List::moveBack() {
    beforeCursor = backDummy->prev;
    afterCursor = backDummy;

    pos_cursor = num_elements;
}

// moveNext()
// Advances cursor to next higher position. Returns the List element that
// was passed over. 
// pre: position()<length() 
ListElement List::moveNext() {
    if(pos_cursor >= length()) {
        throw std::range_error("List: moveNext(): cursor at back");
    }

    beforeCursor = afterCursor;
    afterCursor = afterCursor->next;

    pos_cursor += 1;

    return(beforeCursor->data);
}

// movePrev()
// Advances cursor to next lower position. Returns the List element that
// was passed over. 
// pre: position()>0
ListElement List::movePrev() {
    if(pos_cursor <= 0) {
        throw std::range_error("List: movePrev(): cursor at front");
    }
    afterCursor = beforeCursor;
    beforeCursor = beforeCursor->prev;

    pos_cursor -= 1;

    return(afterCursor->data);
}

// insertAfter()
// Inserts x after cursor.
void List::insertAfter(ListElement x) {
    Node* N = new Node(x);
    N->next = afterCursor;
    N->prev = beforeCursor;
    
    beforeCursor->next = N;
    afterCursor->prev = N;
    afterCursor = N;

    num_elements += 1;
}

// insertBefore()
// Inserts x before cursor.
void List::insertBefore(ListElement x) {
    Node* N = new Node(x);
    N->next = afterCursor;
    N->prev = beforeCursor;
    
    beforeCursor->next = N;
    afterCursor->prev = N;
    beforeCursor = N;

    pos_cursor += 1;
    num_elements += 1;
}

// setAfter()
// Overwrites the List element after the cursor with x.
// pre: position()<length()
void List::setAfter(ListElement x) {
    if(pos_cursor >= length()) {
        throw std::range_error("List: setAfter(): cursor at back");
    }

    afterCursor->data = x;
}

// setBefore()
// Overwrites the List element before the cursor with x.
// pre: position()>0
void List::setBefore(ListElement x) {
    if(pos_cursor <= 0) {
        throw std::range_error("List: setBefore(): cursor at front");
    }

    beforeCursor->data = x;
}

// eraseAfter()
// Deletes element after cursor.
// pre: position()<length()
void List::eraseAfter() {
    if(pos_cursor >= length()) {
        throw std::range_error("List: eraseAfter(): cursor at back");
    }

    Node* Temp = afterCursor->next;
    delete afterCursor;
    afterCursor = Temp;
    beforeCursor->next = afterCursor;
    afterCursor->prev = beforeCursor;

    num_elements -= 1;
}

// eraseBefore()
// Deletes element before cursor.
// pre: position()>0
void List::eraseBefore() {
    if(pos_cursor <= 0) {
        throw std::range_error("List: eraseBefore(): cursor at front");
    }

    Node* Temp = beforeCursor->prev;
    delete beforeCursor;
    beforeCursor = Temp;
    afterCursor->prev = beforeCursor;
    beforeCursor->next = afterCursor;

    pos_cursor -= 1;
    num_elements -= 1;
}


// Other Functions ---------------------------------------------------------

// findNext()
// Starting from the current cursor position, performs a linear search (in 
// the direction front-to-back) for the first occurrence of element x. If x
// is found, places the cursor immediately after the found element, then 
// returns the final cursor position. If x is not found, places the cursor 
// at position length(), and returns -1. 
int List::findNext(ListElement x) {
    while(pos_cursor < length()) {
        if(moveNext() == x) {
            return(pos_cursor);
        }
    }

    return -1;

}

// findPrev()
// Starting from the current cursor position, performs a linear search (in 
// the direction back-to-front) for the first occurrence of element x. If x
// is found, places the cursor immediately before the found element, then
// returns the final cursor position. If x is not found, places the cursor 
// at position 0, and returns -1. 
int List::findPrev(ListElement x) {
    while(pos_cursor > 0) {
        if(movePrev() == x) {
            return(pos_cursor);
        }
    }

    return -1;

}

// cleanup()
// Removes any repeated elements in this List, leaving only unique elements.
// The order of the remaining elements is obtained by retaining the frontmost 
// occurrance of each element, and removing all other occurances. The cursor 
// is not moved with respect to the retained elements, i.e. it lies between 
// the same two retained elements that it did before cleanup() was called.
void List::cleanup() {
    // keep original position;
    int pos_temp = pos_cursor;
    int rep, j;

    moveFront();
    for(int i = 0; i < num_elements; i++) {
        rep = moveNext();

        while(true) {
            j = findNext(rep);
            if(j != -1) {
                eraseBefore();
                if(j <= pos_temp) {
                    pos_temp--;
                }
            } else {
                break;
            }
        }
        findPrev(rep);
        moveNext();
    }

    while(pos_temp != pos_cursor) {
        if(pos_temp > pos_cursor) {
            moveNext();
        } else {
            movePrev();
        }
    }
}

// concat()
// Returns a new List consisting of the elements of this List, followed by
// the elements of L. The cursor in the returned List will be at postion 0.
List List::concat(const List& L) const {
    List C = List(*this);

    Node* N = L.frontDummy->next;
    while(N != L.backDummy) {
        C.insertBefore(N->data);
        N = N->next;
    }

    C.moveFront();
    return C;
}

// to_string()
// Returns a string representation of this List consisting of a comma 
// separated sequence of elements, surrounded by parentheses.
std::string List::to_string() const {
    Node* N = nullptr;
    std::string s = "(";

    for(N= frontDummy->next; N!=nullptr && N!=backDummy->prev; N=N->next){
        s += std::to_string(N->data)+", ";
    }
    s += std::to_string(N->data)+")";
    return s;
}

// equals()
// Returns true if and only if this List is the same integer sequence as R.
// The cursors in this List and in R are unchanged.
bool List::equals(const List& R) const {
    if(num_elements != R.num_elements) {
        return false;
    }

    Node* Ln = frontDummy->next;
    Node* Rn = R.frontDummy->next;
    for(int i = 0; i <= num_elements; i++) {
        if(Ln->data != Rn->data) {
            return false;
        }
        Ln = Ln->next;
        Rn = Rn->next;
    }

    return true;
}


// Overriden Operators -----------------------------------------------------

// operator<<()
// Inserts string representation of L into stream.
std::ostream& operator<<( std::ostream& stream, const List& L ) {
    return stream << L.to_string();
}

// operator==()
// Returns true if and only if A is the same integer sequence as B. The 
// cursors in both Lists are unchanged.
bool operator==( const List& A, const List& B ) {
    return A.equals(B);
}

// operator=()
// Overwrites the state of this List with state of L.
List& List::operator=( const List& L ) {
    if( this != &L ){
        List temp = L;

        std::swap(frontDummy, temp.frontDummy);
        std::swap(backDummy, temp.backDummy);
        std::swap(num_elements, temp.num_elements);
        }

    return *this;
}