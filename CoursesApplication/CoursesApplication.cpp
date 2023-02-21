/**************************************************
 * CoursesApplication.cpp
 * Author: Kenneth Cluck
 * CS-300-T3307 Project Two
 * Description: Loads courses from a csv formatted file
 *              for displaying and sorting
**************************************************/
#include <iostream>
#include "FileLoader.hpp"
#include "CourseTable.hpp"
using namespace std;

/**
 * Displays user menu
 */
void DisplayMenu() {
    cout << "1. Load Data Structure" << endl;
    cout << "2. Print Course List" << endl;
    cout << "3. Print Course" << endl;
    cout << "9. Exit" << endl;
    cout << "Enter an option: " << flush;
}

/**
 * Main application
 */
int main(int argc, char* argv[])
{
    string inputFile;
    int input = 0;
    CourseTable::Table *courseTable = new CourseTable::Table;

    // Loop until 9 is picked, then exit program
    while (input != 9) {
        DisplayMenu();
        cin >> input;
        cout << endl;

        switch (input) {
        case 1:
            // If no file is specified when calling program, prompt for filename
            if (argc >= 2) {
                inputFile = argv[1];
            }
            else {
                cout << "Enter a filename: " << flush;
                cin.ignore();
                getline(cin, inputFile);
            }

            FileLoader::LoadFile(inputFile, courseTable);
            break;
        case 2:
            courseTable->Print();
            break;
        case 3:
            string course;
            cout << "Enter a course number: " << flush;

            // Check if getline is valid, otherwise use cin.ignore() so
            // loop does not enter an infinite loop
            cin.ignore();
            if (getline(cin, course)) {

                // Make uppercase
                for (int i = 0; i < course.length(); i++) {
                    course[i] = toupper(course[i]);
                }

                courseTable->PrintCourse(course);
            }
            else {
                cin.ignore();
            }

            break;
        }
    }

    cout << "Goodbye." << endl;

}

