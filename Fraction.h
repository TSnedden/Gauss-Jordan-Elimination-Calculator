#pragma once
class Fraction
{
public:
	// Constructor for a default fraction, sets numerator and denominator to default values
	Fraction();

	// Constructor for a fraction with only a numerator (denominator defaults to 1)
	Fraction(long long num);

	// Constructor for a fraction with both a numerator and denominator
	Fraction(long long num, long long denom);

	// Getter for the numerator
	long long GetNumerator() const;

	// Getter for the denominator
	long long GetDenominator() const;

	// Returns true if the fraction is an integer (denominator == 1)
	bool IsInteger() const;

	// Setter for the numerator
	void SetNumerator(long long num);

	// Setter for the denominator
	void SetDenominator(long long denom);

	// Overload the addition operator for fractions
	Fraction operator+(const Fraction& rhs);

	// Overload the multiplication operator for fractions
	Fraction operator*(const Fraction& rhs);

	// Overload the subtraction operator for fractions
	Fraction operator-(const Fraction& rhs);

	// Overload the division operator for fractions
	Fraction operator/(const Fraction& rhs);

	// Returns the inverse of the fraction
	Fraction GetInverse() const;

	// Prints the fraction, with parentheses if needed for the solution print
	void Print(bool solutionPrint, bool constantPrint) const;

	// Returns true if the fraction is zero (numerator == 0)
	bool IsZero() const;

	// Returns GCD of numerator and denominator
	long long GCD(long long a, long long b) const;

	// Reduces Fraction to simplest form
	void SimplestTerms();


private:
	// Use long long for computational capability
	long long numerator;
	long long denominator;
};

