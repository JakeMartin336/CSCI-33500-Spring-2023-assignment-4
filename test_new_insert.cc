// Main file for testing modified insert & search routine for binomial queue.
// @filename: test_new_insert.cc
// Homework 4 - CSCI 335.

#include <string>
#include <cstdlib>
#include <fstream>

#include "binomial_queue.h"
using namespace std;

// Use the code as is in function TestNewInsertAndSearch(). Do not insert or remove code.
// You can modify this function for debugging purposes. 
// @param[in] create_queue_file is filename of file to be used for the creation of binomial queue.
// @param[in] check_search_file is filename of file to be used for checking search capabilities.
void TestNewInsertAndSearch(const string &create_queue_file, const string &check_search_file) {
  ifstream numbers_in;  
  numbers_in.open(create_queue_file);
  if (!numbers_in.is_open()) {
    cerr << "Can't open input file " << create_queue_file << endl;
    return;
  }
  ifstream numbers_search_in;
  numbers_search_in.open(check_search_file);
  if (!numbers_search_in.is_open()) {
    cerr << "Can't open search file " << check_search_file << endl;
    return;
  }

  BinomialQueue<int> numbers_queue;

  size_t inserted = 0;
  while(!numbers_in.eof()) {
    string next_line;
    getline(numbers_in, next_line);
    if (!next_line.empty()) {
      if (numbers_queue.insertNoMerge(stoi(next_line)))
	inserted++;
    }
  }
  numbers_in.close();

  cout << inserted << " elements" << endl;
  cout << numbers_queue.findMin() << " is the minimum" << endl;

  while (!numbers_search_in.eof()) {
    string next_line;
    getline(numbers_search_in, next_line);
    if (!next_line.empty()) {
      if (numbers_queue.Find(stoi(next_line)))
        cout << next_line << " in" << endl;
      else
        cout << next_line << " out" << endl;
    }
  }
  numbers_search_in.close();
}

int main(int argc, char **argv) {
  if (argc != 3) {
    cout << "Usage: " << argv[0] << " <input_file_to_create_queue> <input_file_to_check_search> " << endl;
    return 0;
  }
  const string create_queue_file(argv[1]);
  const string check_search_file(argv[2]);

  TestNewInsertAndSearch(create_queue_file, check_search_file);

  return 0;
}
