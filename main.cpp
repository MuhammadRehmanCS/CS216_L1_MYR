// Lab #1 Refresher Su26
// Muhammad Yusuf Rehman

#include <iostream>
#include <iomanip>
#include <fstream>
#include <limits>

using namespace std;

const char INPUT_FILE_NAME[] = "in_numbers.txt";
const char OUTPUT_FILE_NAME[] = "out_numbers.txt";
const char DIVISIBLE_BY_FIVE_LABEL[] = "Numbers Divisible by 5";
const char DIVISIBLE_BY_SEVEN_LABEL[] = "Numbers Divisible by 7";
const char OTHER_NUMBERS_LABEL[] = "Other Numbers";
const char INVALID_CHOICE_MESSAGE[] = "Invalid menu choice.";
const char MENU_TEXT[] = "\n\nMenu\n"
                         "1. Print values in each array with average, median, and sum\n"
                         "2. Print values in each array sorted in descending order\n"
                         "3. Quit and write arrays and statistics to output file\n"
                         "Enter your choice: ";
const int MAX_VALUES = 1000;
const int DIVISOR_FIVE = 5;
const int DIVISOR_SEVEN = 7;
const int DECIMAL_PLACES = 2;
const int EVEN_DIVISOR = 2;
const int ARRAY_NEIGHBOR_OFFSET = 1;
const int LABEL_WIDTH = 30;
const int VALUE_WIDTH = 12;
const int SEPARATOR_WIDTH = 72;
const int VALUES_PER_LINE = 4;

enum MenuChoice
{
    INVALID_CHOICE = 0,
    PRINT_STATS = 1,
    PRINT_SORTED,
    QUIT
};

void displayHeader();
void displayMenu();
int getMenuChoice();
void clearFailedInput(const char message[]);
void getData(int divisibleByFive[], int &countFive, int divisibleBySeven[], int &countSeven, int otherNumbers[], int &countOther, int &skippedCount);
void storeNumber(int number, int divisibleByFive[], int &countFive, int divisibleBySeven[], int &countSeven, int otherNumbers[], int &countOther, int &skippedCount);
bool addToArray(int values[], int &count, int number);
void reportFullArray(const char label[], int number);
void displayAllStats(ostream &out, const int divisibleByFive[], int countFive, const int divisibleBySeven[], int countSeven, const int otherNumbers[], int countOther, int skippedCount);
void displayOneArrayStats(ostream &out, const char title[], const int values[], int count);
void displayAllSorted(ostream &out, const int divisibleByFive[], int countFive, const int divisibleBySeven[], int countSeven, const int otherNumbers[], int countOther);
void displayOneSortedArray(ostream &out, const char title[], const int values[], int count);
void printValues(ostream &out, const int values[], int count);
void printStatsTable(ostream &out, const int values[], int count);
void printSeparator(ostream &out);
void copyArray(const int source[], int destination[], int count);
void bubbleSortDescending(int values[], int count);
int calculateTotal(const int values[], int count);
double calculateAverage(const int values[], int count);
double calculateMedian(const int values[], int count);
void writeOutputFile(const int divisibleByFive[], int countFive, const int divisibleBySeven[], int countSeven, const int otherNumbers[], int countOther, int skippedCount);

// Purpose: Runs the menu-driven lab program.
// Precondition: in_numbers.txt is in the same folder as the executable.
// Postcondition: User-selected reports are displayed and out_numbers.txt is written on quit.
int main()
{
    int divisibleByFive[MAX_VALUES] = {0};
    int divisibleBySeven[MAX_VALUES] = {0};
    int otherNumbers[MAX_VALUES] = {0};
    int countFive = 0;
    int countSeven = 0;
    int countOther = 0;
    int skippedCount = 0;
    int choice = INVALID_CHOICE;
    bool done = false;

    cout << fixed << showpoint << setprecision(DECIMAL_PLACES);

    displayHeader();
    getData(divisibleByFive, countFive, divisibleBySeven, countSeven, otherNumbers, countOther, skippedCount);

    while (!done)
    {
        displayMenu();
        choice = getMenuChoice();

        switch (choice)
        {
        case PRINT_STATS:
            displayAllStats(cout, divisibleByFive, countFive, divisibleBySeven, countSeven, otherNumbers, countOther, skippedCount);
            break;
        case PRINT_SORTED:
            displayAllSorted(cout, divisibleByFive, countFive, divisibleBySeven, countSeven, otherNumbers, countOther);
            break;
        case QUIT:
            writeOutputFile(divisibleByFive, countFive, divisibleBySeven, countSeven, otherNumbers, countOther, skippedCount);
            cout << "Program ended.\n";
            done = true;
            break;
        default:
            clearFailedInput(INVALID_CHOICE_MESSAGE);
        }
    }
}

// Purpose: Displays the lab title and student name.
// Precondition: The program has started.
// Postcondition: The header is printed to the screen.
void displayHeader()
{
    cout << "Lab #1 Refresher Su26\n"
         << "Muhammad Yusuf Rehman\n";
}

// Purpose: Displays the available menu choices.
// Precondition: None.
// Postcondition: The menu is printed to the screen.
void displayMenu()
{
    cout << MENU_TEXT;
}

// Purpose: Reads a menu choice.
// Precondition: User input is available from the keyboard.
// Postcondition: Returns the entered number or INVALID_CHOICE after failed input.
int getMenuChoice()
{
    int choice = INVALID_CHOICE;

    cin >> choice;

    if (!cin)
    {
        choice = INVALID_CHOICE;
    }

    return choice;
}

// Purpose: Displays an input error message and clears the input stream.
// Precondition: cin may be failed or may have extra input to discard.
// Postcondition: cin is ready for the next input attempt.
void clearFailedInput(const char message[])
{
    cout << message << '\n';
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Purpose: Reads the input file once and stores each number into the correct array or arrays.
// Precondition: The input file contains whole numbers only.
// Postcondition: Values are stored, skipped values are counted, and the file is closed.
void getData(int divisibleByFive[], int &countFive, int divisibleBySeven[], int &countSeven, int otherNumbers[], int &countOther, int &skippedCount)
{
    ifstream inputFile;
    int number = 0;
    int numbersRead = 0;

    inputFile.open(INPUT_FILE_NAME);

    if (!inputFile)
    {
        cout << "Error: " << INPUT_FILE_NAME << " could not be opened.\n";
    }
    else
    {
        while (inputFile >> number)
        {
            storeNumber(number, divisibleByFive, countFive, divisibleBySeven, countSeven, otherNumbers, countOther, skippedCount);
            numbersRead++;
        }

        inputFile.close();

        if (numbersRead == 0)
        {
            cout << INPUT_FILE_NAME << " has been read. The input file was empty.\n";
        }
        else
        {
            cout << INPUT_FILE_NAME << " has been read.\n";
        }
    }
}

// Purpose: Stores one number in each matching category array.
// Precondition: Counts match the values currently stored in each array.
// Postcondition: The number is stored where it belongs, or skipped if an array is full.
void storeNumber(int number, int divisibleByFive[], int &countFive, int divisibleBySeven[], int &countSeven, int otherNumbers[], int &countOther, int &skippedCount)
{
    bool belongsToDivisibleArray = false;

    if (number % DIVISOR_FIVE == 0)
    {
        if (!addToArray(divisibleByFive, countFive, number))
        {
            skippedCount++;
            reportFullArray(DIVISIBLE_BY_FIVE_LABEL, number);
        }

        belongsToDivisibleArray = true;
    }

    if (number % DIVISOR_SEVEN == 0)
    {
        if (!addToArray(divisibleBySeven, countSeven, number))
        {
            skippedCount++;
            reportFullArray(DIVISIBLE_BY_SEVEN_LABEL, number);
        }

        belongsToDivisibleArray = true;
    }

    if (!belongsToDivisibleArray)
    {
        if (!addToArray(otherNumbers, countOther, number))
        {
            skippedCount++;
            reportFullArray(OTHER_NUMBERS_LABEL, number);
        }
    }
}

// Purpose: Stores one number in one array when space is available.
// Precondition: count is the current number of stored values.
// Postcondition: Returns true if the value is stored; otherwise returns false.
bool addToArray(int values[], int &count, int number)
{
    bool wasAdded = false;

    if (count < MAX_VALUES)
    {
        values[count] = number;
        count++;
        wasAdded = true;
    }

    return wasAdded;
}

// Purpose: Reports that one value could not be stored.
// Precondition: The matching array is full.
// Postcondition: A warning message is printed to the screen.
void reportFullArray(const char label[], int number)
{
    cout << "Warning: " << label << " array is full. "
         << number << " was not stored.\n";
}

// Purpose: Writes all arrays and statistics to the selected output stream.
// Precondition: out is open and counts match stored values.
// Postcondition: A complete report is written to the selected output stream.
void displayAllStats(ostream &out, const int divisibleByFive[], int countFive, const int divisibleBySeven[], int countSeven, const int otherNumbers[], int countOther, int skippedCount)
{
    out << "\nValues and Statistics Stored in Arrays\n";
    printSeparator(out);
    displayOneArrayStats(out, DIVISIBLE_BY_FIVE_LABEL, divisibleByFive, countFive);
    displayOneArrayStats(out, DIVISIBLE_BY_SEVEN_LABEL, divisibleBySeven, countSeven);
    displayOneArrayStats(out, OTHER_NUMBERS_LABEL, otherNumbers, countOther);
    out << left << setw(LABEL_WIDTH) << "Skipped Values"
        << right << setw(VALUE_WIDTH) << skippedCount << '\n';
}

// Purpose: Writes one array and its statistics.
// Precondition: out is open and count is not negative.
// Postcondition: The selected array report is written.
void displayOneArrayStats(ostream &out, const char title[], const int values[], int count)
{
    out << '\n' << title << '\n';
    printSeparator(out);
    out << left << setw(LABEL_WIDTH) << "Values";
    printValues(out, values, count);
    printStatsTable(out, values, count);
}

// Purpose: Displays all arrays sorted in descending order.
// Precondition: Counts match stored values.
// Postcondition: Sorted copies are displayed and original arrays are unchanged.
void displayAllSorted(ostream &out, const int divisibleByFive[], int countFive, const int divisibleBySeven[], int countSeven, const int otherNumbers[], int countOther)
{
    out << "\nArrays Sorted in Descending Order\n";
    printSeparator(out);
    displayOneSortedArray(out, DIVISIBLE_BY_FIVE_LABEL, divisibleByFive, countFive);
    displayOneSortedArray(out, DIVISIBLE_BY_SEVEN_LABEL, divisibleBySeven, countSeven);
    displayOneSortedArray(out, OTHER_NUMBERS_LABEL, otherNumbers, countOther);
}

// Purpose: Displays one array sorted in descending order.
// Precondition: count is not negative.
// Postcondition: A sorted copy is displayed and the original array is unchanged.
void displayOneSortedArray(ostream &out, const char title[], const int values[], int count)
{
    int sortedValues[MAX_VALUES] = {0};

    copyArray(values, sortedValues, count);
    bubbleSortDescending(sortedValues, count);

    out << left << setw(LABEL_WIDTH) << title;
    printValues(out, sortedValues, count);
}

// Purpose: Writes the values stored in one array.
// Precondition: out is open and count is not negative.
// Postcondition: Values or a no-values message are written.
void printValues(ostream &out, const int values[], int count)
{
    int index = 0;

    if (count == 0)
    {
        out << "No values\n";
    }
    else
    {
        while (index < count)
        {
            if (index > 0 && index % VALUES_PER_LINE == 0)
            {
                out << '\n'
                    << left << setw(LABEL_WIDTH) << " ";
            }

            out << right << setw(VALUE_WIDTH) << values[index];
            index++;
        }

        out << '\n';
    }
}

// Purpose: Writes the count, sum, average, and median for one array.
// Precondition: out is open and count is not negative.
// Postcondition: A statistics table is written.
void printStatsTable(ostream &out, const int values[], int count)
{
    out << left << setw(LABEL_WIDTH) << "Statistic"
        << right << setw(VALUE_WIDTH) << "Value" << '\n';
    out << left << setw(LABEL_WIDTH) << "Count"
        << right << setw(VALUE_WIDTH) << count << '\n';
    out << left << setw(LABEL_WIDTH) << "Sum"
        << right << setw(VALUE_WIDTH) << calculateTotal(values, count) << '\n';
    out << left << setw(LABEL_WIDTH) << "Average";

    if (count == 0)
    {
        out << right << setw(VALUE_WIDTH) << "N/A" << '\n';
    }
    else
    {
        out << right << setw(VALUE_WIDTH) << calculateAverage(values, count) << '\n';
    }

    out << left << setw(LABEL_WIDTH) << "Median";

    if (count == 0)
    {
        out << right << setw(VALUE_WIDTH) << "N/A" << '\n';
    }
    else
    {
        out << right << setw(VALUE_WIDTH) << calculateMedian(values, count) << '\n';
    }
}

// Purpose: Writes a separator line.
// Precondition: out is open.
// Postcondition: A separator line is written.
void printSeparator(ostream &out)
{
    int count = 0;

    while (count < SEPARATOR_WIDTH)
    {
        out << '-';
        count++;
    }

    out << '\n';
}

// Purpose: Copies stored values from one array into another.
// Precondition: destination has room for count values.
// Postcondition: destination contains the copied values.
void copyArray(const int source[], int destination[], int count)
{
    int index = 0;

    while (index < count)
    {
        destination[index] = source[index];
        index++;
    }
}

// Purpose: Sorts one array in descending order.
// Precondition: count is not negative.
// Postcondition: The provided array is sorted in descending order.
void bubbleSortDescending(int values[], int count)
{
    int pass = 0;
    int index = 0;
    int temp = 0;

    while (pass < count - ARRAY_NEIGHBOR_OFFSET)
    {
        index = 0;

        while (index < count - ARRAY_NEIGHBOR_OFFSET - pass)
        {
            if (values[index] < values[index + ARRAY_NEIGHBOR_OFFSET])
            {
                temp = values[index];
                values[index] = values[index + ARRAY_NEIGHBOR_OFFSET];
                values[index + ARRAY_NEIGHBOR_OFFSET] = temp;
            }

            index++;
        }

        pass++;
    }
}

// Purpose: Calculates the sum of one array.
// Precondition: count is not negative.
// Postcondition: Returns the sum of the stored values.
int calculateTotal(const int values[], int count)
{
    int index = 0;
    int total = 0;

    while (index < count)
    {
        total += values[index];
        index++;
    }

    return total;
}

// Purpose: Calculates the average of one array.
// Precondition: count is greater than zero.
// Postcondition: Returns the average of the stored values.
double calculateAverage(const int values[], int count)
{
    return static_cast<double>(calculateTotal(values, count)) / count;
}

// Purpose: Calculates the median of one array.
// Precondition: count is greater than zero.
// Postcondition: Returns the median of the stored values.
double calculateMedian(const int values[], int count)
{
    int sortedValues[MAX_VALUES] = {0};
    int middle = 0;
    double median = 0.0;

    copyArray(values, sortedValues, count);
    bubbleSortDescending(sortedValues, count);

    middle = count / EVEN_DIVISOR;

    if (count % EVEN_DIVISOR == 0)
    {
        median = sortedValues[middle - ARRAY_NEIGHBOR_OFFSET];
        median += sortedValues[middle];
        median = median / EVEN_DIVISOR;
    }
    else
    {
        median = sortedValues[middle];
    }

    return median;
}

// Purpose: Writes the final report to the output file.
// Precondition: Counts match the values stored in each array.
// Postcondition: out_numbers.txt contains the final report if the file opens successfully.
void writeOutputFile(const int divisibleByFive[], int countFive, const int divisibleBySeven[], int countSeven, const int otherNumbers[], int countOther, int skippedCount)
{
    ofstream outputFile;

    outputFile.open(OUTPUT_FILE_NAME);

    if (!outputFile)
    {
        cout << "Error: " << OUTPUT_FILE_NAME << " could not be opened.\n";
    }
    else
    {
        outputFile << fixed << showpoint << setprecision(DECIMAL_PLACES);
        outputFile << "Lab #1 Refresher Su26\n"
                   << "Muhammad Yusuf Rehman\n";
        displayAllStats(outputFile, divisibleByFive, countFive, divisibleBySeven, countSeven, otherNumbers, countOther, skippedCount);
        outputFile.close();
        cout << "Arrays and statistics were written to " << OUTPUT_FILE_NAME << ".\n";
    }
}

/*
in_numbers.txt:
5 7 10 14 15 21 22 25 28 30 35 42
43 49 50 55 60 70 71 0 -5 -7 -35

Sample Test Run:
Lab #1 Refresher Su26
Muhammad Yusuf Rehman
in_numbers.txt has been read.


Menu
1. Print values in each array with average, median, and sum
2. Print values in each array sorted in descending order
3. Quit and write arrays and statistics to output file
Enter your choice: x
Invalid menu choice.


Menu
1. Print values in each array with average, median, and sum
2. Print values in each array sorted in descending order
3. Quit and write arrays and statistics to output file
Enter your choice: 9
Invalid menu choice.


Menu
1. Print values in each array with average, median, and sum
2. Print values in each array sorted in descending order
3. Quit and write arrays and statistics to output file
Enter your choice: 1

Values and Statistics Stored in Arrays
------------------------------------------------------------------------

Numbers Divisible by 5
------------------------------------------------------------------------
Values                                   5          10          15          25
                                        30          35          50          55
                                        60          70           0          -5
                                       -35
Statistic                            Value
Count                                   13
Sum                                    315
Average                              24.23
Median                               25.00

Numbers Divisible by 7
------------------------------------------------------------------------
Values                                   7          14          21          28
                                        35          42          49          70
                                         0          -7         -35
Statistic                            Value
Count                                   11
Sum                                    224
Average                              20.36
Median                               21.00

Other Numbers
------------------------------------------------------------------------
Values                                  22          43          71
Statistic                            Value
Count                                    3
Sum                                    136
Average                              45.33
Median                               43.00
Skipped Values                           0


Menu
1. Print values in each array with average, median, and sum
2. Print values in each array sorted in descending order
3. Quit and write arrays and statistics to output file
Enter your choice: 2

Arrays Sorted in Descending Order
------------------------------------------------------------------------
Numbers Divisible by 5                  70          60          55          50
                                        35          30          25          15
                                        10           5           0          -5
                                       -35
Numbers Divisible by 7                  70          49          42          35
                                        28          21          14           7
                                         0          -7         -35
Other Numbers                           71          43          22


Menu
1. Print values in each array with average, median, and sum
2. Print values in each array sorted in descending order
3. Quit and write arrays and statistics to output file
Enter your choice: 3
Arrays and statistics were written to out_numbers.txt.
Program ended.

out_numbers.txt:
Lab #1 Refresher Su26
Muhammad Yusuf Rehman

Values and Statistics Stored in Arrays
------------------------------------------------------------------------

Numbers Divisible by 5
------------------------------------------------------------------------
Values                                   5          10          15          25
                                        30          35          50          55
                                        60          70           0          -5
                                       -35
Statistic                            Value
Count                                   13
Sum                                    315
Average                              24.23
Median                               25.00

Numbers Divisible by 7
------------------------------------------------------------------------
Values                                   7          14          21          28
                                        35          42          49          70
                                         0          -7         -35
Statistic                            Value
Count                                   11
Sum                                    224
Average                              20.36
Median                               21.00

Other Numbers
------------------------------------------------------------------------
Values                                  22          43          71
Statistic                            Value
Count                                    3
Sum                                    136
Average                              45.33
Median                               43.00
Skipped Values                           0

*/