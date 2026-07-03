// Muhammad Yusuf Rehman
// Lab #1 Refresher Su26

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <limits>
using namespace std;

const string INPUT_FILE_NAME = "in_numbers.txt";
const string OUTPUT_FILE_NAME = "out_numbers.txt";

const int MAX_VALUES = 1000;
const int DIVISIBLE_BY_FIVE = 5;
const int DIVISIBLE_BY_SEVEN = 7;

const int PRINT_STATS = 1;
const int PRINT_SORTED = 2;
const int QUIT = 3;

const int LABEL_WIDTH = 28;
const int VALUE_WIDTH = 12;
const int DECIMAL_PLACES = 2;

void displayHeader();
void displayMenu();
int getMenuChoice();
void clearFailedInput(const string &message);

void getData(int divisibleByFive[], int &countFive,
             int divisibleBySeven[], int &countSeven,
             int otherNumbers[], int &countOther,
             int &skippedCount);

void storeNumber(int values[], int &count, int number, int &skippedCount);

void processStatsOption(const int divisibleByFive[], int countFive,
                        const int divisibleBySeven[], int countSeven,
                        const int otherNumbers[], int countOther);

void processSortedOption(const int divisibleByFive[], int countFive,
                         const int divisibleBySeven[], int countSeven,
                         const int otherNumbers[], int countOther);

void processQuitOption(const int divisibleByFive[], int countFive,
                       const int divisibleBySeven[], int countSeven,
                       const int otherNumbers[], int countOther,
                       int skippedCount,
                       bool &done);

void printArrayReport(const string &title, const int values[], int count);
void printValues(const int values[], int count);
void printStatsTable(const int values[], int count);

int calculateTotal(const int values[], int count);
double calculateAverage(const int values[], int count);
double calculateMedian(const int values[], int count);

void copyArray(const int source[], int destination[], int count);
void sortDescending(int values[], int count);
void printSortedValues(const string &title, const int values[], int count);

void writeOutputFile(const int divisibleByFive[], int countFive,
                     const int divisibleBySeven[], int countSeven,
                     const int otherNumbers[], int countOther,
                     int skippedCount);

void writeArrayReport(ofstream &outputFile,
                      const string &title,
                      const int values[],
                      int count);

void writeValues(ofstream &outputFile, const int values[], int count);
void writeStatsTable(ofstream &outputFile, const int values[], int count);

int main()
{
    int divisibleByFive[MAX_VALUES] = {0};
    int divisibleBySeven[MAX_VALUES] = {0};
    int otherNumbers[MAX_VALUES] = {0};

    int countFive = 0;
    int countSeven = 0;
    int countOther = 0;
    int skippedCount = 0;

    int choice = 0;
    bool done = false;

    displayHeader();

    getData(divisibleByFive, countFive,
            divisibleBySeven, countSeven,
            otherNumbers, countOther,
            skippedCount);

    while (done == false)
    {
        displayMenu();
        choice = getMenuChoice();

        switch (choice)
        {
        case PRINT_STATS:
            processStatsOption(divisibleByFive, countFive,
                               divisibleBySeven, countSeven,
                               otherNumbers, countOther);
            break;

        case PRINT_SORTED:
            processSortedOption(divisibleByFive, countFive,
                                divisibleBySeven, countSeven,
                                otherNumbers, countOther);
            break;

        case QUIT:
            processQuitOption(divisibleByFive, countFive,
                              divisibleBySeven, countSeven,
                              otherNumbers, countOther,
                              skippedCount,
                              done);
            break;

        default:
            cout << "Invalid menu choice." << endl;
        }
    }

    return 0;
}

/*
Description: This function prints the title and student name.
Pre-condition: The program has started.
Post-condition: The header is shown on the screen.
*/
void displayHeader()
{
    cout << "Lab #1 Refresher Su26" << endl;
    cout << "Muhammad Yusuf Rehman" << endl;
    cout << endl;
}

/*
Description: This function shows the menu choices.
Pre-condition: The program is ready for the user to pick an option.
Post-condition: The menu is printed on the screen.
*/
void displayMenu()
{
    cout << endl;
    cout << "Menu" << endl;
    cout << "1. Print values and statistics" << endl;
    cout << "2. Print values sorted in descending order" << endl;
    cout << "3. Quit and write results to output file" << endl;
    cout << "Enter your choice: ";
}

/*
Description: This function gets the menu choice from the user.
Pre-condition: The user must type something into the keyboard.
Post-condition: A valid number input is returned.
*/
int getMenuChoice()
{
    int choice = 0;

    cin >> choice;

    while (!cin)
    {
        clearFailedInput("Invalid input. Enter a number: ");
        cin >> choice;
    }

    return choice;
}

/*
Description: This function clears bad keyboard input.
Pre-condition: The user typed input that was not valid.
Post-condition: The input stream is fixed so the user can try again.
*/
void clearFailedInput(const string &message)
{
    cout << message;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

/*
Description: This function reads numbers from the input file and puts them into arrays.
Pre-condition: The arrays and counts already exist.
Post-condition: Numbers are stored in the correct arrays.
*/
void getData(int divisibleByFive[], int &countFive,
             int divisibleBySeven[], int &countSeven,
             int otherNumbers[], int &countOther,
             int &skippedCount)
{
    ifstream inputFile;
    int number = 0;

    inputFile.open(INPUT_FILE_NAME);

    if (inputFile)
    {
        while (inputFile >> number)
        {
            if (number % DIVISIBLE_BY_FIVE == 0)
            {
                storeNumber(divisibleByFive, countFive, number, skippedCount);
            }
            else if (number % DIVISIBLE_BY_SEVEN == 0)
            {
                storeNumber(divisibleBySeven, countSeven, number, skippedCount);
            }
            else
            {
                storeNumber(otherNumbers, countOther, number, skippedCount);
            }
        }

        inputFile.close();
    }
    else
    {
        cout << "Could not open " << INPUT_FILE_NAME << "." << endl;
        cout << "Program will continue with empty arrays." << endl;
    }
}

/*
Description: This function stores one number in an array if there is space.
Pre-condition: The array and count already exist.
Post-condition: The number is stored or counted as skipped.
*/
void storeNumber(int values[], int &count, int number, int &skippedCount)
{
    if (count < MAX_VALUES)
    {
        values[count] = number;
        count++;
    }
    else
    {
        skippedCount++;
    }
}

/*
Description: This function prints all values and statistics.
Pre-condition: The arrays already have their numbers.
Post-condition: The values and statistics are printed on the screen.
*/
void processStatsOption(const int divisibleByFive[], int countFive,
                        const int divisibleBySeven[], int countSeven,
                        const int otherNumbers[], int countOther)
{
    cout << fixed << showpoint << setprecision(DECIMAL_PLACES);

    printArrayReport("Numbers Divisible by 5", divisibleByFive, countFive);
    printArrayReport("Numbers Divisible by 7", divisibleBySeven, countSeven);
    printArrayReport("Other Numbers", otherNumbers, countOther);
}

/*
Description: This function prints all values sorted from biggest to smallest.
Pre-condition: The arrays already have their numbers.
Post-condition: The sorted values are printed on the screen.
*/
void processSortedOption(const int divisibleByFive[], int countFive,
                         const int divisibleBySeven[], int countSeven,
                         const int otherNumbers[], int countOther)
{
    printSortedValues("Numbers Divisible by 5", divisibleByFive, countFive);
    printSortedValues("Numbers Divisible by 7", divisibleBySeven, countSeven);
    printSortedValues("Other Numbers", otherNumbers, countOther);
}

/*
Description: This function writes the output file and ends the menu loop.
Pre-condition: The arrays already have their numbers.
Post-condition: Results are saved to the output file and the program is ready to quit.
*/
void processQuitOption(const int divisibleByFive[], int countFive,
                       const int divisibleBySeven[], int countSeven,
                       const int otherNumbers[], int countOther,
                       int skippedCount,
                       bool &done)
{
    writeOutputFile(divisibleByFive, countFive,
                    divisibleBySeven, countSeven,
                    otherNumbers, countOther,
                    skippedCount);

    cout << "Program results were written to "
         << OUTPUT_FILE_NAME << "." << endl;
    cout << "Program will now quit." << endl;

    done = true;
}

/*
Description: This function prints one report section for one array.
Pre-condition: The array and its count already exist.
Post-condition: The values and statistics for that array are printed.
*/
void printArrayReport(const string &title, const int values[], int count)
{
    cout << endl;
    cout << title << endl;
    cout << "Values: ";
    printValues(values, count);

    if (count > 0)
    {
        printStatsTable(values, count);
    }
    else
    {
        cout << "No values stored in this array." << endl;
    }
}

/*
Description: This function prints the numbers in an array.
Pre-condition: The array and its count already exist.
Post-condition: The numbers are printed on the screen.
*/
void printValues(const int values[], int count)
{
    int index = 0;

    if (count > 0)
    {
        while (index < count)
        {
            cout << values[index] << " ";
            index++;
        }

        cout << endl;
    }
    else
    {
        cout << "None" << endl;
    }
}

/*
Description: This function prints count, sum, average, and median.
Pre-condition: The array has at least one number.
Post-condition: The statistics are printed on the screen.
*/
void printStatsTable(const int values[], int count)
{
    int total = 0;
    double average = 0.0;
    double median = 0.0;

    total = calculateTotal(values, count);
    average = calculateAverage(values, count);
    median = calculateMedian(values, count);

    cout << left << setw(LABEL_WIDTH) << "Count"
         << right << setw(VALUE_WIDTH) << count << endl;
    cout << left << setw(LABEL_WIDTH) << "Sum"
         << right << setw(VALUE_WIDTH) << total << endl;
    cout << left << setw(LABEL_WIDTH) << "Average"
         << right << setw(VALUE_WIDTH) << average << endl;
    cout << left << setw(LABEL_WIDTH) << "Median"
         << right << setw(VALUE_WIDTH) << median << endl;
}

/*
Description: This function adds all numbers in an array.
Pre-condition: The array and its count already exist.
Post-condition: The total is returned.
*/
int calculateTotal(const int values[], int count)
{
    int index = 0;
    int total = 0;

    while (index < count)
    {
        total = total + values[index];
        index++;
    }

    return total;
}

/*
Description: This function finds the average of the numbers.
Pre-condition: The array has at least one number.
Post-condition: The average is returned.
*/
double calculateAverage(const int values[], int count)
{
    int total = 0;
    double average = 0.0;

    total = calculateTotal(values, count);
    average = static_cast<double>(total) / count;

    return average;
}

/*
Description: This function finds the median of the numbers.
Pre-condition: The array has at least one number.
Post-condition: The median is returned.
*/
double calculateMedian(const int values[], int count)
{
    int sortedValues[MAX_VALUES] = {0};
    int middle = 0;
    double median = 0.0;

    copyArray(values, sortedValues, count);
    sortDescending(sortedValues, count);

    middle = count / 2;

    if (count % 2 == 0)
    {
        median = (static_cast<double>(sortedValues[middle - 1])
                  + sortedValues[middle]) / 2;
    }
    else
    {
        median = sortedValues[middle];
    }

    return median;
}

/*
Description: This function copies one array into another array.
Pre-condition: The source and destination arrays already exist.
Post-condition: The destination array has the same values as the source array.
*/
void copyArray(const int source[], int destination[], int count)
{
    int index = 0;

    while (index < count)
    {
        destination[index] = source[index];
        index++;
    }
}

/*
Description: This function sorts an array from biggest to smallest.
Pre-condition: The array and its count already exist.
Post-condition: The array is sorted in descending order.
*/
void sortDescending(int values[], int count)
{
    int pass = 0;
    int index = 0;
    int temp = 0;

    while (pass < count - 1)
    {
        index = 0;

        while (index < count - pass - 1)
        {
            if (values[index] < values[index + 1])
            {
                temp = values[index];
                values[index] = values[index + 1];
                values[index + 1] = temp;
            }

            index++;
        }

        pass++;
    }
}

/*
Description: This function prints a sorted copy of an array.
Pre-condition: The array and its count already exist.
Post-condition: The sorted values are printed without changing the original array.
*/
void printSortedValues(const string &title, const int values[], int count)
{
    int sortedValues[MAX_VALUES] = {0};

    copyArray(values, sortedValues, count);
    sortDescending(sortedValues, count);

    cout << endl;
    cout << title << " Sorted Descending" << endl;
    cout << "Values: ";
    printValues(sortedValues, count);
}

/*
Description: This function writes all results to the output file.
Pre-condition: The arrays and counts already exist.
Post-condition: The output file has the final report.
*/
void writeOutputFile(const int divisibleByFive[], int countFive,
                     const int divisibleBySeven[], int countSeven,
                     const int otherNumbers[], int countOther,
                     int skippedCount)
{
    ofstream outputFile;

    outputFile.open(OUTPUT_FILE_NAME);

    if (outputFile)
    {
        outputFile << fixed << showpoint << setprecision(DECIMAL_PLACES);

        outputFile << "Lab #1 Refresher Su26" << endl;
        outputFile << "Muhammad Yusuf Rehman" << endl;
        outputFile << endl;

        writeArrayReport(outputFile, "Numbers Divisible by 5",
                         divisibleByFive, countFive);
        writeArrayReport(outputFile, "Numbers Divisible by 7",
                         divisibleBySeven, countSeven);
        writeArrayReport(outputFile, "Other Numbers",
                         otherNumbers, countOther);

        outputFile << endl;
        outputFile << left << setw(LABEL_WIDTH)
                   << "Skipped Values"
                   << right << setw(VALUE_WIDTH)
                   << skippedCount << endl;

        outputFile.close();
    }
    else
    {
        cout << "Could not open " << OUTPUT_FILE_NAME << "." << endl;
    }
}

/*
Description: This function writes one report section to the output file.
Pre-condition: The output file is open and the array exists.
Post-condition: One section is written to the output file.
*/
void writeArrayReport(ofstream &outputFile,
                      const string &title,
                      const int values[],
                      int count)
{
    outputFile << endl;
    outputFile << title << endl;
    outputFile << "Values: ";
    writeValues(outputFile, values, count);

    if (count > 0)
    {
        writeStatsTable(outputFile, values, count);
    }
    else
    {
        outputFile << "No values stored in this array." << endl;
    }
}

/*
Description: This function writes the numbers in an array to the output file.
Pre-condition: The output file is open and the array exists.
Post-condition: The numbers are written to the output file.
*/
void writeValues(ofstream &outputFile, const int values[], int count)
{
    int index = 0;

    if (count > 0)
    {
        while (index < count)
        {
            outputFile << values[index] << " ";
            index++;
        }

        outputFile << endl;
    }
    else
    {
        outputFile << "None" << endl;
    }
}

/*
Description: This function writes count, sum, average, and median to the output file.
Pre-condition: The output file is open and the array has at least one number.
Post-condition: The statistics are written to the output file.
*/
void writeStatsTable(ofstream &outputFile, const int values[], int count)
{
    int total = 0;
    double average = 0.0;
    double median = 0.0;

    total = calculateTotal(values, count);
    average = calculateAverage(values, count);
    median = calculateMedian(values, count);

    outputFile << left << setw(LABEL_WIDTH) << "Count"
               << right << setw(VALUE_WIDTH) << count << endl;
    outputFile << left << setw(LABEL_WIDTH) << "Sum"
               << right << setw(VALUE_WIDTH) << total << endl;
    outputFile << left << setw(LABEL_WIDTH) << "Average"
               << right << setw(VALUE_WIDTH) << average << endl;
    outputFile << left << setw(LABEL_WIDTH) << "Median"
               << right << setw(VALUE_WIDTH) << median << endl;
}

/*

in_numbers.txt:
5 7 10 14 15 21 22 25 28 30 35 42 43 49 50 55 60 70 71

Sample Test Run:
Lab #1 Refresher Su26
Muhammad Yusuf Rehman


Menu
1. Print values and statistics
2. Print values sorted in descending order
3. Quit and write results to output file
Enter your choice: 1

Numbers Divisible by 5
Values: 5 10 15 25 30 35 50 55 60 70 
Count                                 10
Sum                                  355
Average                            35.50
Median                             32.50

Numbers Divisible by 7
Values: 7 14 21 28 42 49 
Count                                  6
Sum                                  161
Average                            26.83
Median                             24.50

Other Numbers
Values: 22 43 71 
Count                                  3
Sum                                  136
Average                            45.33
Median                             43.00

Menu
1. Print values and statistics
2. Print values sorted in descending order
3. Quit and write results to output file
Enter your choice: 2

Numbers Divisible by 5 Sorted Descending
Values: 70 60 55 50 35 30 25 15 10 5 

Numbers Divisible by 7 Sorted Descending
Values: 49 42 28 21 14 7 

Other Numbers Sorted Descending
Values: 71 43 22 

Menu
1. Print values and statistics
2. Print values sorted in descending order
3. Quit and write results to output file
Enter your choice: 3
Program results were written to out_numbers.txt.
Program will now quit.

out_numbers.txt:
Lab #1 Refresher Su26
Muhammad Yusuf Rehman


Numbers Divisible by 5
Values: 5 10 15 25 30 35 50 55 60 70 
Count                                 10
Sum                                  355
Average                            35.50
Median                             32.50

Numbers Divisible by 7
Values: 7 14 21 28 42 49 
Count                                  6
Sum                                  161
Average                            26.83
Median                             24.50

Other Numbers
Values: 22 43 71 
Count                                  3
Sum                                  136
Average                            45.33
Median                             43.00

Skipped Values                         0
*/