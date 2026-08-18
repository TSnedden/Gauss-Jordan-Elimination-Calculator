// Include necessary headers
#include <iostream>  // std::cout, std::cin
#include <cstdlib>   // std::abs
#include <fstream>   // std::ifstream, std::ofstream
#include <vector>    // std::vector
#include <string>    // std::string
#include <sstream>   // std::stringstream
#include <algorithm> // std::swap
#include "Fraction.h"
#include "Matrix.h"


std::vector<std::string> NumDimensionsChecker() {
    std::string entry;
    std::vector<std::string> dimensions;
    while (true) {
        try {
            std::cin >> entry;
            while (entry != "$") {
                dimensions.push_back(entry);
                std::cin >> entry;
            }
            if (dimensions.size() != 2) {
                throw std::runtime_error("You did not enter 2 dimensions for the augmented matrix.");
            }
            // If dimensions are correct, break out of the loop
            break;

        }
        catch (const std::exception& e) {
            std::cout << e.what() << " Please enter again, followed by a \"$\"." << std::endl;
            dimensions.clear();  // Clear invalid dimensions and retry
        }
    }
    return dimensions;
}

bool DimensionChecker(const std::string& userString) {
    for (char ch : userString) {
        if (!isdigit(ch)) {
            throw std::invalid_argument("Dimension should contain only positive non-zero integers.");
        }
    }
    int dim = std::stoi(userString);
    if (dim < 1) {
        throw std::invalid_argument("Dimension should contain only positive non-zero integers.");
    }
    return true;
}

std::vector<std::string> NumEntriesChecker(int numEntries) {
    std::string entry;
    std::vector<std::string> entries;
    while (true) {
        try {
            entries.clear();
            int counter = 0;
            std::cin >> entry;
            while (entry != "$") {
                counter++;
                entries.push_back(entry);
                std::cin >> entry;
            }
            if (counter != numEntries) {
                throw std::invalid_argument("Incorrect number of entries.");
            }
            // If valid, break out of loop
            break;
        }
        catch (const std::exception& e) {
            std::cout << e.what() << " Please enter the correct number of entries, ending with '$'." << std::endl;
            entries.clear();  // Clear invalid input for retry
        }
    }
    return entries;
}

bool MatrixEntriesChecker(const std::vector<std::string>& userEntries) {
    int divisorBarCounter = 0; // Counter for '/' characters
    for (int i = 0; i < userEntries.size(); i++) {
        const std::string& entry = userEntries.at(i);
        // Check each character in the entry
        for (int j = 0; j < entry.length(); j++) {
            char currentChar = entry.at(j);
            // If the character is not a digit, we handle different cases
            if (!isdigit(currentChar)) {
                if (currentChar == '-') {
                    // Handle negative numbers: '-' should be at the start and must not be the only character
                    if ((j != 0) || (j == 0 && entry.length() == 1) || (j == 0 && !isdigit(entry.at(1)))) {
                        throw std::invalid_argument("Invalid negative number formatting.");
                    }
                }
                else if (currentChar == '/') {
                    // Handle fractions
                    divisorBarCounter++;
                    if (j == 0 || j == entry.length() - 1) {
                        throw std::invalid_argument("Invalid fraction formatting.");
                    }
                }
                else {
                    throw std::invalid_argument("Invalid character in matrix entry.");
                }
            }
        }
        // Only one '/' character allowed
        if (divisorBarCounter > 1) {
            throw std::invalid_argument("Too many '/' characters in a fraction.");
        }
        // Reset the divisor bar counter for the next entry
        divisorBarCounter = 0;
    }
    return true;
}

// Function to convert strings to Fraction objects
std::vector<Fraction> StringSplitter(std::vector<std::string>& entries) {
    char delim = '/';
    long long numerator;
    long long denominator;
    std::vector<Fraction> Fractions;
    // Iterate through each entry
    for (int i = 0; i < entries.size(); i++) {
        bool isFraction = false;
        // Check if the entry is a fraction
        for (int j = 0; j < entries.at(i).length(); j++) {
            if (entries.at(i).at(j) == '/') {
                isFraction = true;
                break;
            }
        }
        // If entry is a fraction, split it into numerator and denominator
        if (isFraction) {
            int counter = 0;
            std::stringstream ss(entries.at(i));
            std::string item;
            while (std::getline(ss, item, delim)) {
                counter += 1;
                if (counter == 1) {
                    numerator = std::stoll(item);
                }
                else {
                    denominator = std::stoll(item);
                }
            }
            Fractions.push_back(Fraction(numerator, denominator));
        }
        // If entry is not a fraction, convert it directly to a Fraction object
        else {
            Fractions.push_back(Fraction(std::stoll(entries.at(i))));
        }
    }
    return Fractions;
}

Matrix MatrixCreator() {
    std::string userRows, userCols;
    int numRows = 0;
    int numCols = 0;
    // Validate and get matrix dimensions
    while (true) {
        try {
            std::vector<std::string> dimensions = NumDimensionsChecker();
            userRows = dimensions.at(0);
            userCols = dimensions.at(1);
            // Validate the dimensions, throws if invalid
            DimensionChecker(userRows);
            DimensionChecker(userCols);
            // Convert to integers
            numRows = std::stoi(userRows);
            numCols = std::stoi(userCols);
            break;  // Valid dimensions, exit loop
        }
        catch (const std::exception& e) {
            std::cout << e.what() << " Please enter valid dimensions followed by '$'." << std::endl;
        }
    }
    int numEntries = numRows * numCols;
    std::vector<std::string> entries;
    // Validate matrix entries
    while (true) {
        try {
            std::cout << "Enter the entries of the augmented matrix, ending the list with '$'. Fraction entries are valid." << std::endl;
            entries = NumEntriesChecker(numEntries);
            MatrixEntriesChecker(entries);  // Validate the entries
            break;  // If valid, exit loop
        }
        catch (const std::exception& e) {
            std::cout << e.what() << " Please enter valid matrix entries followed by '$'." << std::endl;
        }
    }
    std::vector<Fraction> fractions = StringSplitter(entries);
    Matrix myMatrix(numRows, numCols);
    // Set the matrix grid with the fractions
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            myMatrix.SetGrid(i, j, fractions.at(i * numCols + j));
        }
    }
    return myMatrix;
}

// Function to eliminate a column of the matrix
bool ColElim(int numRows, int numCols, int rowNum, int colNum, Fraction** myGrid) {
    Fraction* subtractRow = myGrid[rowNum];
    // If the leading coefficient of the row is negative, make it positive
    if (subtractRow[colNum].GetNumerator() < 0) {
        for (int j = colNum; j < numCols; j++) {
            subtractRow[j] = subtractRow[j] * -1;
        }
    }
    // Normalize the row to make the leading coefficient 1
    Fraction subtractInverse = subtractRow[colNum].GetInverse();
    for (int j = colNum; j < numCols; j++) {
        subtractRow[j] = subtractRow[j] * subtractInverse;
    }
    // Eliminate the leading coefficient from all other rows
    for (int i = 0; i < numRows; i++) {
        if (!myGrid[i][colNum].IsZero() && i != rowNum) {
            Fraction* currRow = myGrid[i];
            Fraction factor = currRow[colNum];
            for (int j = colNum; j < numCols; j++) {
                currRow[j] = currRow[j] - subtractRow[j] * factor;
            }
        }
    }
    return true; // return true to indicate column has been processed
}

// Function used to order the matrix in row order (leading nonzero coefficients)
void MatrixSorter(Matrix& matrix, int numRows, int numCols) {
    Fraction** myGrid = matrix.GetGrid();
    // Sort the matrix rows based on the position of their leading non-zero entries
    for (int i = 0; i < numRows - 1; i++) {
        int minLeadingCol = numCols; // Initialize with a large value
        int minRowIndex = i;
        // Find the row with the smallest leading entry column (starting from row i)
        for (int m = i; m < numRows; m++) {
            for (int j = 0; j < numCols - 1; j++) {
                if (!myGrid[m][j].IsZero() && j < minLeadingCol) {
                    minLeadingCol = j;
                    minRowIndex = m;
                    break;
                }
                
            }
        }
        // If a row with a smaller leading entry column is found, swap rows
        if (minRowIndex != i) {
            std::swap(myGrid[i], myGrid[minRowIndex]);
        }
    }
}

// Function to solve the matrix (pass by reference to edit original matrix)
void MatrixSolver(Matrix& matrix) {
    int numRows = matrix.GetRowDim();
    int numCols = matrix.GetColDim();
    Fraction** myGrid = matrix.GetGrid();
    bool* used = new bool[numRows]; // See which rows have been used to eliminate columns
    int rowNum = 0; // Start at top row, used to process rows
    int colNum = 0; // Start at leftmost column, used to indicate next column to eliminate
    bool elimCol = false;
    // Initialize the 'used' array
    for (int i = 0; i < numRows; i++) {
        used[i] = false;
    }
    // Main loop to process each row
    while (rowNum < numRows) {
        elimCol = false; // Reset elimCol at the beginning of each new row iteration

        // If the entry at myGrid[rowNum][colNum] is not 0, and the row has not been used
        // to eliminate a column (i.e., !used[rowNum]), and the colNum is not the solution
        // vector column (i.e., numCols - 1), eliminate the column at colNum
        if (!myGrid[rowNum][colNum].IsZero() && !used[rowNum] && colNum < numCols - 1) {
            // This line will return 'true' and will eliminate the 'colNum' column in 'myGrid'
            elimCol = ColElim(numRows, numCols, rowNum, colNum, myGrid);

            // Iterate colNum to indicate that column has been eliminated
            colNum++;

            // Update used[rowNum] to indicate that row has been used
            used[rowNum] = true;
        }
        // Move to the next row to continue processing the columns
        rowNum++;

        // If the column is eliminated, reset rowNum (since colNum has been incremented) to process
        // a new column
        if (elimCol) {
            rowNum = 0;
        }
        // If all rows for the current column have been processed and no elimination was performed,
        // move to the next column and reset rowNum to process rows from the top again.
        if (rowNum == numRows && !elimCol && colNum < numCols - 1) {
            colNum++;
            rowNum = 0;
        }
    }
    // Put the rows in order
    MatrixSorter(matrix, numRows, numCols);

    std::cout << "Here is the matrix in Gauss-Jordan form:" << std::endl;
    matrix.Print(false, false); // Not printing the solution, so parameters are false
    std::cout << std::endl;
    // Clean up dynamically allocated memory
    delete[] used;
}

// Function used to find the leading entries of a matrix
bool** FindLeadingEntries(Matrix matrix) {
    int numRows = matrix.GetRowDim();
    int numCols = matrix.GetColDim();
    Fraction** myGrid = matrix.GetGrid();
    bool** leadingEntries = new bool* [numRows];
    for (int i = 0; i < numRows; i++) {
        leadingEntries[i] = new bool[numCols];
    }
    // Initialize 2d array to false
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            leadingEntries[i][j] = false;
        }
    }
    // Find where leading coefficients are
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols - 1; j++) {
            if (!myGrid[i][j].IsZero()) {
                leadingEntries[i][j] = true;
                break;
            }
        }
    }
    return leadingEntries; // returns 2d array of booleans indicating where leading entries are
}
// Function to print the solution of the linear system
void SolutionPrinter(Matrix& matrix, bool** leadingEntries) {
    int numRows = matrix.GetRowDim();
    int numCols = matrix.GetColDim();
    Fraction** myGrid = matrix.GetGrid();
    bool noSolution = false;
    bool* freeVar = new bool[numCols - 1]; // Array to track free variables
    bool zeroCol; // Tracks columns of zeroes
    int numEntries = 0;
    // Initialize free variable array to false
    for (int i = 0; i < numCols - 1; i++) {
        freeVar[i] = false;
    }
    // Look for free variables in columns
    for (int j = 0; j < numCols - 1; j++) {
        zeroCol = true;
        for (int i = 0; i < numRows; i++) {
            if (!myGrid[i][j].IsZero()) {  // If any element in the column is non-zero
                zeroCol = false;
                break;  // No need to check further; this column is not a free variable column
            }
        }
        if (zeroCol) {
            freeVar[j] = true;  // Mark column as a free variable if it's entirely zero
        }
    }
    // Check for no solution
    for (int i = 0; i < numRows; i++) {
        numEntries = 0; // Counter for no solution check
        if (!myGrid[i][numCols - 1].IsZero()) {
            for (int j = 0; j < numCols - 1; j++) {
                if (!myGrid[i][j].IsZero()) {
                    numEntries++;
                }
                // No solution if it's nonhomogenous & no nonzero variables found
                if (j == numCols - 2 && numEntries == 0) {
                    std::cout << "No solution: inconsistent system." << std::endl;
                    noSolution = true;
                }
            }
        }
        if (noSolution) {
            break;
        }
    }
    if (!noSolution) {
        std::cout << "Here is the solution vector:" << std::endl;
        // Print non-free variables
        for (int i = 0; i < numRows; i++) {
            for (int j = 0; j < numCols - 1; j++) {
                // This if-statement prints each non-zero row of the matrix starting at 
                // leadingEntries[][] positions in matrix
                if (leadingEntries[i][j]) {
                    std::cout << "x_" << j + 1 << " = ";
                    if (!myGrid[i][numCols - 1].IsZero()) {
                        // Set first parameter to true, second to false (to not print fraction 
                        // parentheses for nonhomogeneous solution)
                        myGrid[i][numCols - 1].Print(true, false);
                        for (int k = j + 1; k < numCols - 1; k++) {
                            if (!myGrid[i][k].IsZero()) {
                                if (myGrid[i][k].GetNumerator() < 0) {
                                    // Print a '+' if there's a nonhomogeneous solution and the first
                                    // free variable is added
                                    std::cout << " + ";
                                }
                                break;
                            }
                        }
                    }
                    else {
                        // Check to see if there is a zero solution for a variable
                        numEntries = 0; // Counter for '0' solution check
                        for (int k = j + 1; k < numCols - 1; k++) {
                            if (!myGrid[i][k].IsZero()) {
                                numEntries++;
                                if (numEntries > 0) {
                                    break;
                                }
                            }
                            // If the solution is homogeneous & no other variables are found,
                            // the variable is equal to 0
                            if (k == numCols - 2 && numEntries == 0) {
                                std::cout << "0";

                            }
                        }
                        // If the solution is homogeneous and the leadingEntry is in the 
                        // second to last column, then the variable is equal to 0
                        if (j == numCols - 2) {
                            std::cout << "0";
                        }
                    }
                    numEntries = 0; // Counter for added free variable check
                    // Now print out the free variables in the solution vectors
                    for (int k = j + 1; k < numCols - 1; k++) {
                        // Print a '+' if it's not the first free variable added
                        if (myGrid[i][k].GetNumerator() < 0 && numEntries > 0) {
                            std::cout << " + ";
                        }
                        // Printing free variables that are added
                        if (myGrid[i][k].GetNumerator() < 0) {
                            numEntries++;
                            myGrid[i][k].SetNumerator(myGrid[i][k].GetNumerator() * -1); // Change the sign to add the free variable
                            // Set both parameters to true since we are printing 
                            // free variable solutions
                            myGrid[i][k].Print(true, true);
                            myGrid[i][k].SetNumerator(myGrid[i][k].GetNumerator() * -1); // Change the sign back
                            std::cout << "x_" << k + 1;
                            freeVar[k] = true;
                        }
                        // Printing free variables that are subtracted
                        else if (myGrid[i][k].GetNumerator() > 0) {
                            numEntries++;
                            std::cout << " - ";
                            // Set both parameters to true since we are printing 
                            // free variable solutions
                            myGrid[i][k].Print(true, true);
                            std::cout << "x_" << k + 1;
                            freeVar[k] = true;
                        }
                    }
                    std::cout << std::endl;
                    break;
                }
            }
        }
        // Print free variables
        for (int j = 0; j < numCols - 1; j++) {
            if (freeVar[j]) {
                std::cout << "x_" << j + 1 << " is a free variable." << std::endl;
            }
        }
    }
    delete[] freeVar;  // Free the allocated memory for free variables array
}

int main() {
    // Prompt user to enter dimensions of the augmented matrix
    std::cout << "Enter dimensions of the augmented matrix. For example, a 4x7 matrix would be  \"4 7\". ";
    std::cout << "Afterwards, enter a \"$\"." << std::endl;
    Matrix matrix = MatrixCreator(); // Create matrix based on user input
    MatrixSolver(matrix); // Solve the matrix (matrix is passed by reference)
    bool** leadingEntries = FindLeadingEntries(matrix); // Find leading entries of the matrix
    SolutionPrinter(matrix, leadingEntries); // Print the solution using leadingEntries
    // Clean up dynamically allocated memory
    for (int i = 0; i < matrix.GetRowDim(); i++) {
        delete[] leadingEntries[i];
    }
    delete[] leadingEntries;
    return 0;
}