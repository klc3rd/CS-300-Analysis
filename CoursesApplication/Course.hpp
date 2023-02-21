/**************************************************
 * Course.hpp
 * Author: Kenneth Cluck
 * CS-300-T3307 Project Two
 * Description: Seperate file for Course struct as this
 *				struct is used in multiple locations
**************************************************/
#pragma once
#include <iostream>
#include <vector>
using namespace std;

namespace Courses {
    struct Course {
        string courseId;
        string courseTitle;
        vector<string> prerequisites;
    };
}