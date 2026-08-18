#pragma once
// Include necessary headers
#include "Fraction.h"
#include <iostream>  // std::cout
#include <cstdlib>   // std::abs
#include <fstream>   // std::ifstream, std::ofstream
#include <cmath>     // std::abs
#include <vector>    // std::vector
#include <string>    // std::string
#include <sstream>   // std::stringstream

// Declaration of the Matrix class
class Matrix
{
public:

    // Constructors
    Matrix();  // Default constructor
    Matrix(int rowNum, int colNum);  // Constructor with dimensions
    Matrix(const Matrix& other);  // Copy constructor

    // Destructor
    ~Matrix();

    // Assignment operator
    Matrix& operator=(const Matrix& other);


    // Getters
    Fraction** GetGrid() const;  // Returns the grid of fractions
    int GetRowDim() const;  // Returns the number of rows
    int GetColDim() const;  // Returns the number of columns

    // Setters
    void SetRowDim(int rowNum);  // Sets the number of rows
    void SetColDim(int colNum);  // Sets the number of columns
    void SetGrid(int rowNum, int colNum, Fraction entry);  // Sets a grid entry

    // Other Functions
    void Print(bool solutionPrint, bool constantPrint);  // Prints the grid or solution

private:
    Fraction** grid;  // Pointer to the grid of fractions
    int rowDim;  // Number of rows
    int colDim;  // Number of columns
};


