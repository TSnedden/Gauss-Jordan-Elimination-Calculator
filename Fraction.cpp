// Include necessary headers
#include "Fraction.h"
#include <iostream>  // std::cout
#include <cmath>     // std::abs

// No need to initialize anything in the default constructor
Fraction::Fraction() {}

// Constructor for a fraction with only a numerator (denominator defaults to 1)
Fraction::Fraction(long long num) {
    numerator = num;
    denominator = 1;
}

// Constructor for a fraction with both a numerator and denominator
Fraction::Fraction(long long num, long long denom) {
    numerator = num;
    denominator = denom;
    SimplestTerms(); // Simplify the constructed Fraction
}

// Getter for the numerator
long long Fraction::GetNumerator() const {
    return numerator;
}

// Getter for the denominator
long long Fraction::GetDenominator() const {
    return denominator;
}

// Returns true if the fraction is an integer (denominator == 1)
bool Fraction::IsInteger() const {
    return (denominator == 1);
}

// Setter for the numerator
void Fraction::SetNumerator(long long num) {
    numerator = num;
}

// Setter for the denominator
void Fraction::SetDenominator(long long denom) {
    denominator = denom;
}

// Addition operator overload for fractions
Fraction Fraction::operator+(const Fraction& rhs) {
    Fraction result;
    result.numerator = numerator * rhs.denominator + rhs.numerator * denominator;
    result.denominator = denominator * rhs.denominator;
    result.SimplestTerms(); // Simplify the Fraction sum
    return result;
}

// Multiplication operator overload for fractions
Fraction Fraction::operator*(const Fraction& rhs) {
    Fraction result;
    result.numerator = numerator * rhs.numerator;
    result.denominator = denominator * rhs.denominator;
    result.SimplestTerms(); // Simplify the Fraction product
    return result;
}

// Subtraction operator overload for fractions
Fraction Fraction::operator-(const Fraction& rhs) {
    Fraction result;
    result.numerator = numerator * rhs.denominator - rhs.numerator * denominator;
    result.denominator = denominator * rhs.denominator;
    result.SimplestTerms(); // Simplify the Fraction difference
    return result;
}

// Division operator overload for fractions
Fraction Fraction::operator/(const Fraction& rhs) {
    Fraction result;
    result.numerator = numerator * rhs.denominator;
    result.denominator = denominator * rhs.numerator;
    result.SimplestTerms(); // Simplify the Fraction quotient
    return result;
}

// Returns the inverse of the fraction (swap numerator and denominator)
Fraction Fraction::GetInverse() const {
    return Fraction(denominator, numerator);
}

// Returns true if the fraction is zero (numerator == 0)
bool Fraction::IsZero() const {
    return (numerator == 0);
}

// Prints the integer or fraction with optional parentheses for solution print
void Fraction::Print(bool solutionPrint, bool variablePrint) const {
    if (numerator == 0) {
        std::cout << numerator;
    }
    else {
        if (!IsInteger()) {
            // If we are printing the solution free variables, print the parentheses
            // This is in contrast to printing the matrix or constant solution
            if (solutionPrint && variablePrint) {
                std::cout << "(";
            }
            std::cout << numerator << "/" << denominator;
            if (solutionPrint && variablePrint) {
                std::cout << ")";
            }
        }
        else {
            // If we are printing the solution, don't print a '1' before the free variable
            if (!solutionPrint || !variablePrint || numerator != 1) {
                std::cout << numerator;
            }
        }
    }
}

// Function to compute GCD using the Euclidean algorithm
long long Fraction::GCD(long long a, long long b) const {
    while (b != 0) {
        long long remainder = a % b;
        a = b;
        b = remainder;
    }
    return a;
}

// This function puts the Fraction in simplest terms
// It will take care of itself in the class logic, not computational logic
void Fraction::SimplestTerms() {
    bool negNum = numerator < 0;
    bool negDenom = denominator < 0;
    bool bothNeg = false;
    // Handle negative numbers
    if (negNum && negDenom) {
        numerator *= -1;
        denominator *= -1;
        bothNeg = true;
    }
    else {
        if (negNum) {
            numerator *= -1;
        }
        else if (negDenom) {
            denominator *= -1;
        }
    }
    // Find the greatest common divisor
    long long divisor = GCD(abs(numerator), abs(denominator));
    // Divide the numerator and denominator by the GCD
    numerator = numerator / divisor;
    denominator = denominator / divisor;
    // Restore the sign if necessary
    if (!bothNeg) {
        if (negNum || negDenom) {
            numerator *= -1;
        }
    }
}