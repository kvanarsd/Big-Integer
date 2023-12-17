//-----------------------------------------------------------------------------
// Katrina VanArsdale
// kvanarsd
// Pa6
//-----------------------------------------------------------------------------

#include<iostream>
#include<string>
#include <cmath>
#include <cstring>

#include"BigInteger.h"

int power = 9;
long base = pow(10, power);

BigInteger::BigInteger() {
    signum = 0;
}

// BigInteger()
// Constructor that creates a new BigInteger from the long value x.
BigInteger::BigInteger(long x) {
    if(x < 0) {
        signum = -1;
    } else if(x > 0) {
        signum = 1;
    } else {
        signum = 0;
        return;
    }

    long digit;
    if(x < 0) x *= -1;

    while(x > 0) {
        digit = x % base;
        digits.insertBefore(digit);
        x = floor(x / base);
    }

}

// BigInteger()
// Constructor that creates a new BigInteger from the string s.
// Pre: s is a non-empty string consisting of (at least one) base 10 digit
// {0,1,2,3,4,5,6,7,8,9}, and an optional sign {+,-} prefix.
BigInteger::BigInteger(std::string s) {
    if(s.length() == 0 ) {
        throw std::invalid_argument("BigInteger: Constructor: empty string");
    }
    
    signum = 0;
    
    // if signed change signum
    if(s[0] == '+') {
        signum = 1;
    } else if(s[0] == '-') {
        signum = -1;
    } else if(s[0] == '0' && s.length() == 1) {
        return;
    }

    // if there is only a sign and no other numbers 
    if(s.length() == 1 && signum != 0) {
        throw std::invalid_argument("BigInteger: Constructor: non-numeric string");
    }

    double len = static_cast<double>(s.length());
    int i = 0;
    if(signum != 0) {
        len -= 1;
        i++;
    }
    int zeros = ceil(len / power);
    zeros = (zeros * power) - len;

    // if there are numbers with no sign default to 1
    if(signum == 0) {
        signum = 1;
    }

    int size = 1;
    size += zeros;
    std::string cur = "";

    // iterate through string
    long digit;
    for(; i < static_cast<int>(s.length()); i++, size++) {
        if(isdigit(s[i]) == 0) {
            throw std::invalid_argument("BigInteger: Constructor: non-numeric string");
        }

        if(cur == "" && size / power <= 0 && s[i] == '0') {
            continue;
        } else {
            cur += s[i];
            if(size % power == 0) {
                digit = stol(cur);
                digits.insertBefore(digit);
                digits.movePrev();
                
                cur = "";
            }
        }
    }

    if(digits.back() == 0) {
        digits.moveBack();
        digits.eraseBefore();
    }
}

// BigInteger()
// Constructor that creates a copy of N.
BigInteger::BigInteger(const BigInteger& N) {
    signum = N.signum;
    digits = N.digits;
}

// Optional Destuctor
// ~BigInteger()
// ~BigInteger();


// Access functions --------------------------------------------------------

// sign()
// Returns -1, 1 or 0 according to whether this BigInteger is positive, 
// negative or 0, respectively.
int BigInteger::sign() const {
    return(signum);
}

// compare()
// Returns -1, 1 or 0 according to whether this BigInteger is less than N,
// greater than N or equal to N, respectively.
int BigInteger::compare(const BigInteger& N) const {
    if(sign() > N.sign()) {
        return 1;
    } else if(sign() < N.sign()) {
        return -1;
    } else if(sign() == N.sign() && sign() == 0) {
        return 0;
    }

    if(digits.length() > N.digits.length()) {
        return 1;
    } else if(digits.length() < N.digits.length()) {
        return -1;
    }

    if(digits.back() > N.digits.back()) {
        return 1;
    } else if(digits.back() < N.digits.back()) {
        return -1;
    }

    List D = digits;
    List Nd = N.digits;
    for(D.moveBack(), Nd.moveBack(); 
        D.position() > 0 && Nd.position() > 0;
        D.movePrev(), Nd.movePrev()) {
            if(D.peekPrev() > Nd.peekPrev()) {
                return 1;
            } else if(D.peekPrev() < Nd.peekPrev()) {
                return -1;
            }
        }
    
    return 0;
}


// Manipulation procedures -------------------------------------------------

// makeZero()
// Re-sets this BigInteger to the zero state.
void BigInteger::makeZero() {
    digits.clear();
    signum = 0;
}

// negate()
// If this BigInteger is zero, does nothing, otherwise reverses the sign of 
// this BigInteger positive <--> negative. 
void BigInteger::negate() {
    if(signum == 0) {
        return;
    }
    
    if(signum == 1) {
        signum = -1;
    } else{
        signum = 1;
    }

    return;
}


// BigInteger Arithmetic operations ----------------------------------------

// add()
// Returns a BigInteger representing the sum of this and N.
BigInteger BigInteger::add(const BigInteger& N) const {
    // If either is zero return bigger number
    if(sign() == 0) {
        BigInteger A = N;
        return A;
    }

    if(N.sign() == 0) {
        BigInteger A = *this;
        return A;
    }

    // deal with negatives
    if(sign() > N.sign()) {
        // if A - B
        BigInteger Temp = N;
        Temp.negate(); // make B positive
        return sub(Temp); 
    } else if(sign() < N.sign()) {
        // if -A + B
        BigInteger BTemp(*this);
        BTemp.negate(); // make A positive
        BigInteger A = N.sub(BTemp); // B-A
        return A;
    }

    long carry = 0;
    List D;
    BigInteger A;

    if(digits.length() > N.digits.length()) {
        A = *this;
        D = N.digits;
    } else {
        A = N;
        D = digits;
    }

    long add;
    for(A.digits.moveFront(), D.moveFront(); 
        A.digits.position() < A.digits.length() && D.position() < D.length();
        A.digits.moveNext(), D.moveNext()) {
            add = A.digits.peekNext() + D.peekNext() + carry;
            carry = 0;

            if(add >= base) {
                carry = floor(add / base);
                add = add % base;
            } else {
                carry = 0;
            }
            A.digits.setAfter(add);
    }
    // deal with remaining carry
    long insert;
    if(carry != 0 && digits.length() != N.digits.length()) {
        add = A.digits.peekNext() + carry;
        if(add < base) {
            A.digits.setAfter(A.digits.peekNext() + carry);
            return A;
        }

        while(add >= base && A.digits.position() < A.digits.length()) {
            add = A.digits.peekNext() + carry;
            insert = add % base;
            carry = floor(add / base);
            A.digits.setAfter(insert);
            A.digits.moveNext();
        }
    } 
    
    if(carry != 0) {
        if(carry < base) {
            A.digits.insertAfter(carry);
        }

        while(carry >= base) {
            insert = carry % base;
            carry = floor(carry / base);
            A.digits.insertAfter(insert);
        }
    }
    return A;
}

// sub()
// Returns a BigInteger representing the difference of this and N.
BigInteger BigInteger::sub(const BigInteger& N) const {
    // if equal return 0
    if(compare(N) == 0) {
        BigInteger A;
        return A;
    }

    if(sign() > N.sign()) {
        // if A - - B
        BigInteger Temp = N;
        Temp.negate(); // A + B
        return add(Temp);
    } else if(sign() < N.sign()) {
        // if -A - +B
        BigInteger BTemp(*this);
        BTemp.negate(); // A - +B
        BigInteger A = BTemp.add(N); // A + B
        A.negate(); // change back to negative
        return A;
    }
    
    long carry = 0;
    List D = N.digits;
    BigInteger A = *this;

    if(compare(N) == -1) {
        A = N;
        D = digits;
    }

    long sub;
    for(A.digits.moveFront(), D.moveFront(); 
        A.digits.position() < A.digits.length() && D.position() < D.length();
        A.digits.moveNext(), D.moveNext()) {
            sub = A.digits.peekNext() - D.peekNext() + carry;
            carry = 0;

            if(sub < 0) {
                sub += base;
                carry = -1;
            } else {
                carry = 0;
            }
            A.digits.setAfter(sub);
    }

    if (carry == -1 && A.digits.length() > D.length()) {
        while(A.digits.position() < A.digits.length()) {
            if(A.digits.peekNext() == 0) {
                A.digits.setAfter(base + carry);
                A.digits.moveNext();
            } else {
                A.digits.setAfter(A.digits.peekNext() + carry);
                break;
            }
        }
    }

    if(compare(N) == -1) {
        A.negate();
    }

    return A;
}

// mult()
// Returns a BigInteger representing the product of this and N. 
BigInteger BigInteger::mult(const BigInteger& N) const {
    if(signum == 0 || N.sign() == 0) {
        BigInteger A = 0;
        return A;
    }

    long carry = 0;
    int s;
    List D;
    BigInteger A;

    if(digits.length() >= N.digits.length()) {
        A = *this;
        s = N.sign();
        D = N.digits;
    } else {
        A = N;
        s = sign();
        D = digits;
    }

    long mul;
    // temp Big integers for addition
    BigInteger F(0);
    BigInteger S;
    S.signum = 1;

    for(D.moveFront(); D.position() < D.length(); D.moveNext()) {
        // insert zeros
        for(int i = D.position(); i > 0; i--) {
            S.digits.insertBefore(0);
        } 
        
        // scalar
        for(A.digits.moveFront(); A.digits.position() < A.digits.length(); A.digits.moveNext()) {
            mul = A.digits.peekNext() * D.peekNext();
            S.digits.insertBefore(mul);
        }

        // normalize first ver
        long N;
        long insert;
        if(F.signum == 0) {
            for(S.digits.moveFront(); S.digits.position() < S.digits.length(); S.digits.moveNext()) {
                N = S.digits.peekNext();
                N += carry;
                carry = 0;
                if(N >= base) {
                    carry = floor(N / base);
                    N = N % base;
                } else {
                    carry = 0;
                }
                S.digits.setAfter(N);
            }
            if(carry != 0) {
                if(carry < base) {
                    S.digits.insertAfter(carry);
                }

                while(carry >= base) {
                    insert = carry % base;
                    carry = floor(carry / base);
                    S.digits.insertAfter(insert);
                }
            }
        } 

        // add and normalize
        F = F.add(S);

        // reset
        S.makeZero();
        S.signum = 1;
    }

    // turn negative if one value is negative
    F.signum = A.sign();
    A = F;

    if(A.sign() > s) {
        A.negate();
        return A;
    }

    if(A.sign() == s && s == -1) {
        A.negate();
        return A;
    }

    return A;
}



// Other Functions ---------------------------------------------------------

// to_string()
// Returns a string representation of this BigInteger consisting of its
// base 10 digits. If this BigInteger is negative, the returned string 
// will begin with a negative sign '-'. If this BigInteger is zero, the
// returned string will consist of the character '0' only.
std::string BigInteger::to_string() {
    std::string s = "";
    if(signum == 0) {
        s += "0";
        return s;
    } else if(signum == -1) {
        s += "-";
    }
    
    for(digits.moveBack(); digits.position() > 0; digits.movePrev()){
        int b = base / 10;
        if(digits.position() != digits.length() && s != "-" && s != "") {
            while(digits.peekPrev() < b) {
                s += "0";
                b = b / 10;
                if(b == 1) b = 0;
            }
        } else if ((s == "-" || s == "") && digits.peekPrev() == 0) {
            continue;
        }

        s += std::to_string(digits.peekPrev());
    }
    return s;
}


// Overriden Operators -----------------------------------------------------

// operator<<()
// Inserts string representation of N into stream.
std::ostream& operator<<( std::ostream& stream, BigInteger N ) {
    return stream << N.to_string();
}

// operator==()
// Returns true if and only if A equals B. 
bool operator==( const BigInteger& A, const BigInteger& B ) {
    if(A.compare(B) == 0) {
        return true;
    }
    return false;
}

// operator<()
// Returns true if and only if A is less than B. 
bool operator<( const BigInteger& A, const BigInteger& B ) {
    if(A.compare(B) == -1) {
        return true;
    }
    return false;
}

// operator<=()
// Returns true if and only if A is less than or equal to B. 
bool operator<=( const BigInteger& A, const BigInteger& B ) {
    if(A.compare(B) <= 0) {
        return true;
    }
    return false;
}

// operator>()
// Returns true if and only if A is greater than B. 
bool operator>( const BigInteger& A, const BigInteger& B ) {
    if(A.compare(B) == 1) {
        return true;
    }
    return false;
}
// operator>=()
// Returns true if and only if A is greater than or equal to B. 
bool operator>=( const BigInteger& A, const BigInteger& B )  {
    if(A.compare(B) >= 0) {
        return true;
    }
    return false;
}

// operator+()
// Returns the sum A+B. 
BigInteger operator+( const BigInteger& A, const BigInteger& B ) {
    return A.add(B);
}

// operator+=()
// Overwrites A with the sum A+B. 
BigInteger operator+=( BigInteger& A, const BigInteger& B ) {
    A = A.add(B);
    return A;
}

// operator-()
// Returns the difference A-B. 
BigInteger operator-( const BigInteger& A, const BigInteger& B ) {
    return A.sub(B);
}

// operator-=()
// Overwrites A with the difference A-B. 
BigInteger operator-=( BigInteger& A, const BigInteger& B ) {
    A = A.sub(B);
    return A;
}

// operator*()
// Returns the product A*B. 
BigInteger operator*( const BigInteger& A, const BigInteger& B ) {
    return A.mult(B);
}

// operator*=()
// Overwrites A with the product A*B. 
BigInteger operator*=( BigInteger& A, const BigInteger& B ) {
    A = A.mult(B);
    return A;
}
