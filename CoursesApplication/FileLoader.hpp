/**************************************************
 * FileLoader.hpp
 * Author: Kenneth Cluck
 * Description: parses a csv formatted file, and 
 *              loads the information into a course
 *              binary tree
**************************************************/
#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <stack>
#include "CourseTable.hpp"
#include "Course.hpp"
using namespace std;
using namespace Courses;

namespace FileLoader {
    // Node for storing file input
    struct Node {
        Course course;
        Node* left;
        Node* right;

        Node() {
            left = nullptr;
            right = nullptr;
        }

        Node(Course newCourse) :
            Node() {
            course = newCourse;
        }
    };

    /**
     * This binary tree is to store file contents for future parsing
     */
    class FileTree {
    private:
        Node* root;
        void deleteNode(Node*);
        void inOrderPrint(Node*);

    public:
        FileTree();
        ~FileTree();
        void Insert(Course course);
        bool CourseExists(string);
        void PrintTree();
        vector<Course> ReturnCourses();
    };
    
    /**
     * File tree constructor
     */
    FileTree::FileTree() {
        this->root = nullptr;
    }

    /**
     * File tree deconstructor
     */
    FileTree::~FileTree() {
        // Call recursive deleteNode method
        deleteNode(this->root);
    }

    /**
     * Recursive delete node 
     */
    void FileTree::deleteNode(Node* node) {
        //If node is equal to nullptr, return
        if (node == nullptr) {
            return;
        }
        //If node is not equal to nullptr
        else {
            // Recursive call with left node
            this->deleteNode(node->left);
            // Recursive call with right node
            this->deleteNode(node->right);
            // Delete node
            delete node;
        }
    }

    /**
     * Insert course into FileTree
     */
    void FileTree::Insert(Course course) {
        Node* newNode = new Node(course);
        Node* currNode = this->root;

        if (this->root == nullptr) {
            this->root = newNode;
        }
        else {
            // If root is not null, find a place to insert it
            while (true) {
                // If the current courseId is greater than the input, insert left
                // if it is taken, go to the next loop
                if (currNode->course.courseId >= course.courseId) {
                    if (currNode->left == nullptr) {
                        currNode->left = newNode;
                        break;
                    }
                    currNode = currNode->left;
                }
                // If the current node's courseId is less than the input bid, insert
                // right, if it is taken, go to next loop
                else {
                    if (currNode->right == nullptr) {
                        currNode->right = newNode;
                        break;
                    }
                    currNode = currNode->right;
                }
            }
        }
    }
    
    /**
     * Search for course by ID
     */
    bool FileTree::CourseExists(string id) {
        bool found = false;

        Node* currNode = this->root;

        while (true) {
            // If the current bid is null, it is not found, return empty course
            if (currNode == nullptr) {
                break;
            }

            // If the current node course matches, it is found, return course
            if (currNode->course.courseId == id) {
                found = true;
                break;
            }
            else {
                // If the current courseId is greater than search, go left
                if (currNode->course.courseId > id) {
                    currNode = currNode->left;
                }
                // If the courseId is less than the search, go right
                else {
                    currNode = currNode->right;
                }
            }
        }

        return found;
    }

    /**
     * Return vector of courses
     */
    vector<Course> FileTree::ReturnCourses() {
        Node* node = this->root;
        stack<Node*> nodeStack;
        vector<Course> courses;

        // Leave loop if the current node is null and the stack is not empty
        while (node != NULL || nodeStack.empty() == false) {
            // Loop until the current node is null
            while (node != NULL) {
                // Push node to stack
                nodeStack.push(node);
                // Move left
                node = node->left;
            }

            // Take the current node and pop it from stack
            node = nodeStack.top();
            nodeStack.pop();

            // Add course to vector
            courses.push_back(node->course);

            // Move right
            node = node->right;
        }

        return courses;
    }

    /*
     * Print Tree
     */
    void FileTree::PrintTree() {
        inOrderPrint(this->root);
        cout << endl;
    }

    /*
     * Recursive method for printing tree
     */
    void FileTree::inOrderPrint(Node* node) {
        if (node == nullptr) {
            return;
        }
        else {
            this->inOrderPrint(node->left);
            cout << node->course.courseId << " - " << node->course.courseTitle << endl;
            this->inOrderPrint(node->right);
        }
    }

    //******************************
    //      Method declarations
    //******************************
    void LoadFileIntoMemory(string&, FileTree*);
    void LoadTreeIntoTable(FileTree*, CourseTable::Table*);

    /**
     * Main method for loading the specified file
     */
    void LoadFile(string& filePath, CourseTable::Table* courseTable) {
        // Create a file binary tree and insert file contents
        FileTree* fileTree = new FileTree();
        LoadFileIntoMemory(filePath, fileTree);
        
        // Load binary tree into the hash table
        LoadTreeIntoTable(fileTree, courseTable);
    }

    /**
     *  This method will load the entire file into a binary tree
     */
    void LoadFileIntoMemory(string& filePath, FileTree* fileTree) {
        cout << "Opening " << filePath << endl;
        string line, word;
        vector<string> row;

        // Open file
        fstream file(filePath, ios::in);
        if (file.is_open()) {
            int lineCount = 1;
            // Go to each line
            while (getline(file, line)) {
                // For each line, create a Course for insertion
                row.clear();
                stringstream str(line);

                // Insert row values into vectors, if the vector does
                // not have at least two values, print warning message
                while (getline(str, word, ',')) {
                    row.push_back(word);
                }
                if (row.size() < 2) {
                    cout << "! Invalid course found at " << lineCount << endl;
                }
                else {
                    string courseNumber = row.at(0);

                    // There is are invalid characters read when parsing file, creating a
                    // substring to remove these characters
                    if (courseNumber.length() > 7) {
                        courseNumber = courseNumber.substr(courseNumber.length() - 7, courseNumber.length() - 1);
                    }
                    
                    // Make course number uppercase
                    for (int i = 0; i < courseNumber.length(); i++) {
                        courseNumber[i] = toupper(courseNumber[i]);
                    }

                    Course* tempCourse = new Course();
                    tempCourse->courseId = courseNumber;
                    tempCourse->courseTitle = row.at(1);

                    // If there are remaining items in vector, these are prerequisites
                    if (row.size() >= 3) {
                        // Loop through remainder of row vector and add as prerequisites
                        for (int i = 2; i <= row.size() - 1; ++i) {
                            tempCourse->prerequisites.push_back(row.at(i));
                        }
                    }

                    // Insert course into fileTree
                    fileTree->Insert(*tempCourse);
                    lineCount++;
                }
            }
            file.close();
        }
        else {
            cout << "! Error! Could not open file " + filePath << endl;
        }
    }

    /**
     * Load tree into hash table
     */
    void LoadTreeIntoTable(FileTree* fileTree, CourseTable::Table* courseTable) {
        // Create stack of courses
        vector<Course> courses = fileTree->ReturnCourses();

        // Loop through each course
        for (int i = 0; i < courses.size(); ++i) {
            Course* currCourse = &courses.at(i);

            // If there are no prerequisites, go ahead and insert the course
            if (currCourse->prerequisites.size() == 0) {
                courseTable->Insert(*currCourse);
            }
            else {
                bool valid = true;

                // Loop through each prerequisite and check that they are in the tree
                for (int y = 0; y < currCourse->prerequisites.size(); ++y) {
                    string currPrereq = currCourse->prerequisites.at(y);

                    // Check that prerequisite is valid
                    bool foundCourse = fileTree->CourseExists(currPrereq);

                    if (foundCourse == false) {
                        valid = false;
                    }
                }

                // If course is not valid show error and continue loop
                if (!valid) {
                    cout << "! Course with invalid prerequisite found - " << currCourse->courseTitle << endl;
                }
                else {
                    courseTable->Insert(*currCourse);
                }
            }
        }

        cout << endl << "Courses loaded into data structure" << endl << endl;
    }
}