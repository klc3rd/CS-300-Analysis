/**************************************************
 * CourseTable.hpp
 * Author: Kenneth Cluck
 * CS-300-T3307 Project Two
 * Description: Contains hashmap for storing courses
 *				after they have been processed from
 *				the csv formatted file
**************************************************/
#pragma once
#include <iostream>
#include "Course.hpp"
using namespace std;
using namespace Courses;

namespace CourseTable {
    const unsigned int DEFAULT_SIZE = 101;  // Default node vector size
    
    class Table {
    private:
        struct Node {
            Course course;
            unsigned int key;

            // Default node constructor
            Node() {
                key = UINT_MAX;
            }

            // Initialize with a course
            Node(Course newCourse) {
                course = newCourse;
            }

            // Initialize with a course and a key
            Node(Course newCourse, unsigned int newKey) : Node(newCourse) {
                key = newKey;
            }
        };
        vector<Node> nodes;
        unsigned int tableSize = DEFAULT_SIZE;
        unsigned int hash(string);
        int partition(vector<Course>*, int, int);
        void QuickSort(vector<Course>*, int, int);
        void findAndPrint(string, bool);
    public:
        Table();
        ~Table();
        void Insert(Course);
        void Print();
        void PrintCourse(string);
    };

    /**
     * Default table constructor
     */
    Table::Table() {
        this->nodes.resize(this->tableSize);
    }

    /**
     * Destructor
     */
    Table::~Table() {
        this->nodes.erase(this->nodes.begin());
    }

    /**
     * Hashing method
     */
    unsigned int Table::hash(string key) {
        unsigned int numValue = 0;
        for (int i = 0; i < key.length(); ++i) {
            numValue += key[i];
        }
        return numValue % tableSize;
    }
    
    /**
     * Insert course into map
     */
    void Table::Insert(Course course) {
        // Get hashedKey
        unsigned int hashedKey = hash(course.courseId);

        Node* newNode = new Node();
        newNode->key = hashedKey;
        newNode->course = course;

        // Count the number of insertions
        int count = 1;

        // If the current node is null, create the node
        while (true) {
            // If every space is checked and unavailable, double the hash table size
            if (count == this->nodes.size() - 1) {
                this->tableSize = this->tableSize * 2;
                this->nodes.resize(this->tableSize);
            }

            Node* currNode = &this->nodes.at(hashedKey);

            if (currNode == nullptr || currNode->key == UINT_MAX) {
                this->nodes.at(hashedKey) = *newNode;
                break;
            }
            else {
                // Increment key, if the end of the table is reached
                // start from the beginning of the table
                if (hashedKey == this->nodes.size() - 1) {
                    hashedKey = 0;
                }
                else {
                    hashedKey++;
                }
            }

            count++;
        }
    }
    
    /**
     * Recursive method to print a course, boolean is whether
     * it is printing a prerequisite
     */
    void Table::findAndPrint(string courseId, bool prereq) {
        Course* currCourse;
        bool found = false;

        // Search through nodes for course
        for (int i = 0; i < this->nodes.size(); ++i) {
            if (this->nodes.at(i).course.courseId == courseId && this->nodes.at(i).key != UINT_MAX) {
                currCourse = &this->nodes.at(i).course;
                found = true;

                // Only call recursive call if it is not searching for a prerequisite
                if (prereq == false) {
                    cout << currCourse->courseId << ", " << currCourse->courseTitle << endl;

                    if (currCourse->prerequisites.size() > 0) {
                        for (int i = 0; i < currCourse->prerequisites.size(); ++i) {
                            findAndPrint(currCourse->prerequisites.at(i), true);
                        }
                    }
                }
                else {
                    cout << "- Prereq Course ID: " << currCourse->courseId << " Title: " << currCourse->courseTitle << endl;
                }
            }
        }

        if (found == false) {
            cout << "Course not found." << endl;
        }
        else {

        }
    }

    /**
     * Prints a course
     */
    void Table::PrintCourse(string courseId) {
        cout << endl;
        findAndPrint(courseId, false);
        cout << endl;
    }

    /**
     * Partition method to provide support for quicksort
     */
    int Table::partition(vector<Course> *courses, int lowIndex, int highIndex) {
        // Pick pivot
        int midpoint = lowIndex + (highIndex - lowIndex) / 2;
        Course pivot = courses->at(midpoint);

        while (true) {
            // Increment lowIndex if its courseId is less than the pivot courseId
            while (courses->at(lowIndex).courseId < pivot.courseId) {
                lowIndex++;
            }

            // Decrement highIndex if the pivot courseId is less than the highIndex's courseId
            while (pivot.courseId < courses->at(highIndex).courseId) {
                highIndex--;
            }

            // If one or less elements remain, it is sorted, return highIndex
            if (lowIndex >= highIndex) {
                break;
            }
            else {
                // Swap courses
                Course* tempCourse = new Course();

                // Temp course
                tempCourse->courseId = courses->at(lowIndex).courseId;
                tempCourse->courseTitle = courses->at(lowIndex).courseTitle;
                tempCourse->prerequisites = courses->at(lowIndex).prerequisites;

                // Set lowIndex course
                courses->at(lowIndex).courseId = courses->at(highIndex).courseId;
                courses->at(lowIndex).courseTitle = courses->at(highIndex).courseTitle;
                courses->at(lowIndex).prerequisites = courses->at(highIndex).prerequisites;

                // Set highIndex course
                courses->at(highIndex).courseId = tempCourse->courseId;
                courses->at(highIndex).courseTitle = tempCourse->courseTitle;
                courses->at(highIndex).prerequisites = tempCourse->prerequisites;

                // Update indexes
                lowIndex++;
                highIndex--;
            }
        }

        return highIndex;
    }

    /**
     * Quicksort for courses
     */
    void Table::QuickSort(vector<Course>* courses, int lowIndex, int highIndex) {
        // Break case, if lowIndex reaches highIndex, it is sorted
        if (lowIndex >= highIndex) {
            return;
        }
        
        // Partition the courses
        int lowEnd = partition(courses, lowIndex, highIndex);

        // Call recursive quicksort on low and high end
        QuickSort(courses, lowIndex, lowEnd);
        QuickSort(courses, lowEnd + 1, highIndex);
    }

    /**
     * Print courses
     */
    void Table::Print() {
        // Create a list of all courses
        vector<Course> courses;
        for (int i = 0; i < nodes.size() - 1; ++i) {
            if (this->nodes.at(i).key != UINT_MAX) {
                courses.push_back(nodes.at(i).course);
            }
        }

        // If there are no courses, just show a message
        if (courses.size() == 0) {
            cout << "No courses found." << endl << endl;
        }
        else {
            // Sort courses
            QuickSort(&courses, 0, courses.size() - 1);

            // Print courses
            for (int i = 0; i < courses.size(); ++i) {
                Course currCourse = courses.at(i);
                cout << currCourse.courseId << ", " << currCourse.courseTitle << endl;

                if (currCourse.prerequisites.size() > 0) {
                    cout << "- Prerequisites: ";

                    for (int y = 0; y < currCourse.prerequisites.size(); ++y) {
                        cout << currCourse.prerequisites.at(0) << " ";
                    }

                    cout << endl;
                }

                cout << endl;
            }

        }
    }
}