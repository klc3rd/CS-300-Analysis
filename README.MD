Kenneth Cluck

Module 8 Journal

What the program needed to do was to store course data, input from a csv file and to sort the courses for output. While a seemingly straightforward issue, the difficulty is that for each course, the other courses need to be checked in order to validate that all prerequisites were included. At first it seems you could simply scan the file for each line. This however is cumbersome and slow, as the entire would have to be parsed for each course.

My solution, instead of working with the file directly into memory using a binary tree, that\'92s used specifically to store the file. This would allow quick checking of each prerequisite, speeding up the execution time as checking the binary tree is faster and does not require parsing the entire tree for each course.

The reason data structures and algorithms are important to understand is because every software you use, requires the use of these. It is important to accomplish the processing of data, and with proper data structures and algorithms, this processing can be sped up and made more efficient.

Despite having been programming already, this is the first time I have really been required to use these structures and algorithms directly. I have already found myself considering the efficiency of certain algorithms I have seen in a new way, with a new understanding. This class has taught me how important it is to consider how the program is running underneath. Sometimes with abstract data types, it is easy to forget how it works underneath, when there may be a more efficient solution.

