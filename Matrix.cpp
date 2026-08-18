// Include necessary headers
#include "Matrix.h"
#include <iostream>  // std::cout
#include <cstdlib>   // std::abs
#include <cmath>     // std::abs
#include <vector>    // std::vector
#include <string>    // std::string
#include <sstream>   // std::stringstream

// No initialization in the default constructor
Matrix::Matrix() {}

// Constructor with specified row and column dimensions
Matrix::Matrix(int rowNum, int colNum) {
    rowDim = rowNum;
    colDim = colNum;
    grid = new Fraction * [rowNum];
    for (int i = 0; i < rowNum; i++) {
        grid[i] = new Fraction[colNum];
    }
}

// Copy constructor
Matrix::Matrix(const Matrix& other) {
    rowDim = other.rowDim;
    colDim = other.colDim;

    // Allocate new memory for grid
    grid = new Fraction * [rowDim];
    for (int i = 0; i < rowDim; i++) {
        grid[i] = new Fraction[colDim];
        for (int j = 0; j < colDim; j++) {
            grid[i][j] = other.grid[i][j]; // Copy each element
        }
    }
}


// Destructor
Matrix::~Matrix() {
    for (int i = 0; i < rowDim; i++) {
        delete[] grid[i];  // Free each row
    }
    delete[] grid;  // Free the row pointers
}

// Assignment operator
Matrix& Matrix::operator=(const Matrix& other) {
    if (this == &other) {
        return *this; // Check for self-assignment
    }
    // Deallocate existing memory
    for (int i = 0; i < rowDim; i++) {
        delete[] grid[i];
    }
    delete[] grid;
    // Copy dimensions
    rowDim = other.rowDim;
    colDim = other.colDim;
    // Allocate new memory and copy elements
    grid = new Fraction * [rowDim];
    for (int i = 0; i < rowDim; i++) {
        grid[i] = new Fraction[colDim];
        for (int j = 0; j < colDim; j++) {
            grid[i][j] = other.grid[i][j]; // Copy each element
        }
    }
    return *this;
}


// Getter for the grid
Fraction** Matrix::GetGrid() const {
    return grid;
}

// Getter for the number of rows
int Matrix::GetRowDim() const {
    return rowDim;
}

// Getter for the number of columns
int Matrix::GetColDim() const {
    return colDim;
}

// Setter for the number of rows
void Matrix::SetRowDim(int rowNum) {
    rowDim = rowNum;
}

// Setter for the number of columns
void Matrix::SetColDim(int colNum) {
    colDim = colNum;
}

// Sets a specific entry in the grid
void Matrix::SetGrid(int rowNum, int colNum, Fraction entry) {
    grid[rowNum][colNum] = entry;
}

// Prints the grid, with an option to print for solution (adds parentheses)
void Matrix::Print(bool solutionPrint, bool constantPrint) {
    for (int i = 0; i < rowDim; i++) {
        for (int j = 0; j < colDim; j++) {
            grid[i][j].Print(solutionPrint, constantPrint);
            std::cout << " ";
        }
        std::cout << std::endl;
    }
}
