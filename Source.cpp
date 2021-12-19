//============================================================================
// Name        : Source.cpp
// Author      : Carlie White
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE (design has been taken and reimplemnted)
// Description : This file contains information to accept user input and execute accordingly
//============================================================================

#include <algorithm>
#include <climits>
#include <iostream>
#include <string> // atoi
#include <time.h>

#include "parser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

const unsigned int DEFAULT_SIZE = 179;

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold course information
struct Course {
    string courseId; // unique identifier
    string courseName;
    string prereqs;
    Course() {
        courseId = "";
    }
};
void displayCourse(Course course);
//============================================================================
// Hash Table class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a hash table with chaining.
 */
class HashTable {

private:
    // Define structures to hold courses
    struct Node {
        Course course;
        unsigned int key;
        Node* next;

        // default constructor
        Node() {
            key = UINT_MAX;
            next = nullptr;
        }

        // initialize with a course
        Node(Course aBid) : Node() {
            course = aBid;
        }

        // initialize with a course and a key
        Node(Course aBid, unsigned int aKey) : Node(aBid) {
            key = aKey;
        }
    };

    vector<Node> nodes;

    unsigned int tableSize = DEFAULT_SIZE;

    unsigned int hash(int key);

public:
    HashTable();
    HashTable(unsigned int size);
    virtual ~HashTable();
    void Insert(Course course);
    void PrintAll();
    void Remove(string courseId);
    Course Search(string courseId);
};

/**
 * Default constructor
 */
HashTable::HashTable() {
    // FIXME (1): Initialize the structures used to hold courses
    nodes.resize(tableSize);
}

/**
 * Constructor for specifying size of the table
 * Use to improve efficiency of hashing algorithm
 * by reducing collisions without wasting memory.
 */
HashTable::HashTable(unsigned int size) {
    // invoke local tableSize to size with this->
    // resize nodes size
    this->tableSize = size;
    nodes.resize(tableSize);
}


/**
 * Destructor
 */
HashTable::~HashTable() {
    // FIXME (2): Implement logic to free storage when class is destroyed    
    nodes.erase(nodes.begin());
}

/**
 * Calculate the hash value of a given key.
 * Note that key is specifically defined as
 * unsigned int to prevent undefined results
 * of a negative list index.
 *
 * @param key The key to hash
 * @return The calculated hash
 */
unsigned int HashTable::hash(int key) {
    // FIXME (3): Implement logic to calculate a hash value
    return key % tableSize;
}

/**
 * Insert a course
 *
 * @param course The course to insert
 */
void HashTable::Insert(Course course) {
    // FIXME (5): Implement logic to insert a course
    // create the key for the given course
    unsigned key = hash(atoi(course.courseId.c_str()));
    // retrieve node using key
    Node* newNode = &(nodes.at(key));
    // if no entry found for the key
    if (newNode == nullptr) {
        // assign this node to the key position
        Node* next = new Node(course, key);
        nodes.insert(nodes.begin() + key, (*next));
    }

    // else if node is not used
    else {
        // assing old node key to UNIT_MAX, set to key, set old node to course and old node next to null pointer
        if (newNode->key == UINT_MAX) {
            newNode->key = key;
            newNode->course = course;
            newNode->next = nullptr;
        }
        // else find the next open node
        else {
            while (newNode->next != nullptr) {
                // add new newNode to end
                newNode = newNode->next;
            }
        }
    }
}

/**
 * Print all courses
 */
void HashTable::PrintAll() {
    // FIXME (6): Implement logic to print all courses
    for (unsigned int i = 0; i < nodes.size(); ++i) {
        displayCourse(nodes[i].course);
    }
}

/**
 * Search for the specified courseId
 *
 * @param courseId The course id to search for
 */
Course HashTable::Search(string courseId) {
    Course course;

    // FIXME (8): Implement logic to search for and return a course

    // create the key for the given course
    unsigned key = hash(atoi(courseId.c_str()));
    Node* node = &(nodes.at(key));
    // if no entry found for the key
    if (node == nullptr || node->key == UINT_MAX) {
        //return node course
        return course;
    }
    // if entry found for the key
    if (node != nullptr && node->key != UINT_MAX) {
        // return course
        return node->course;
    }
    // while node not equal to nullptr
    while (node != nullptr) {
        // if the current node matches, return it
        if (node->key != UINT_MAX && node->course.courseId.compare(courseId) == 0) {
            //node is equal to next node
            return node->course;
        }
        node = node->next;
    }

    return course;
}

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the course information to the console (std::out)
 *
 * @param course struct containing the course info
 */
void displayCourse(Course course) {
    cout << course.courseId << ": " << course.courseName << " | " << course.prereqs << " | "
        << endl;
    return;
}

/**
 * Load a CSV file containing courses into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the courses read
 */
void loadCourses(string csvPath, HashTable* hashTable) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    // read and display header row - optional
    vector<string> header = file.getHeader();
    for (auto const& c : header) {
        cout << c << " | ";
    }
    cout << "" << endl;

    try {
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of courses
            Course course;
            course.courseId = file[i][0];
            course.courseName = file[i][1];
            course.prereqs = file[i][2];

            cout << "Course: " << course.courseName << ", Course ID: " << course.courseId << ", Prequisites: " << course.prereqs << endl;

            // push this course to the end
            hashTable->Insert(course);
        }
    }
    catch (csv::Error& e) {
        std::cerr << e.what() << std::endl;
    }
}

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, courseKey;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        courseKey = "MATH201";
        break;
    case 3:
        csvPath = argv[1];
        courseKey = argv[2];
        break;
    default:
        csvPath = "abcuInput.csv";
        courseKey = "MATH201";
    }

    // Define a timer variable
    clock_t ticks;

    // Define a hash table to hold all the courses
    HashTable* courseTable{};

    Course course;

    int choice = 0;
    while (choice != 4) {
        cout << "Menu:" << endl;
        cout << "  1. Load Courses" << endl;
        cout << "  2. Display All Courses" << endl;
        cout << "  3. Find Course" << endl;
        cout << "  4. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            courseTable = new HashTable();

            // Initialize a timer variable before loading courses
            ticks = clock();

            // Complete the method call to load the courses
            loadCourses(csvPath, courseTable);

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            continue;

        case 2:
            courseTable->PrintAll();
            break;

        case 3:
            ticks = clock();

            course = courseTable->Search(courseKey);

            ticks = clock() - ticks; // current clock ticks minus starting clock ticks

            if (!course.courseId.empty()) {
                displayCourse(course);
            }
            else {
                cout << "Course Id " << courseKey << " not found." << endl;
            }

            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;
        }
    }

    cout << "Good bye." << endl;

    return 0;
}
