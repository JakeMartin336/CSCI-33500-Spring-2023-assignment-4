// Main file for testing insert & delete routine for binomial queue.
// @filename: test_insert_and_delete.cc
// Homework 4 - CSCI 335.

#include <string>
#include <cstdlib>
#include <fstream>

#include "binomial_queue.h"
using namespace std;

// Use the code as is in function TestInsertAndDelete(). Do not insert or remove code.
// You can modify this function for debugging purposes. 
// @param[in] create_queue_file is filename of file to be used for the creation of binomial queue.
// @param[in] numbers_delete_name is filename of file to be used for checking delete capabilities.
void TestInsertAndDelete(const string &create_queue_file, const string &numbers_delete_name) {
  ifstream numbers_in;  
  numbers_in.open(create_queue_file);
  if (!numbers_in.is_open()) {
    cerr << "Can't open input file " << create_queue_file << endl;
    return;
  }
  ifstream numbers_delete;
  numbers_delete.open(numbers_delete_name);
  if (!numbers_delete.is_open()) {
    cerr << "Can't open delete file " << numbers_delete_name << endl;
    return;
  }

  BinomialQueue<int> numbers_queue;

  size_t inserted = 0;
  while(!numbers_in.eof()) {
    string next_line;
    getline(numbers_in, next_line);
    if (!next_line.empty()) {
      if (numbers_queue.insert(stoi(next_line)))
	inserted++;
    }
  }
  numbers_in.close();

  cout << inserted << " elements" << endl;
  cout << numbers_queue.findMin() << " is the minimum" << endl;

  vector<int> numbers_to_delete;
  while(!numbers_delete.eof()) {
    string next_line;
    getline(numbers_delete, next_line);
    if (!next_line.empty()) {
      int to_delete = stoi(next_line);
      numbers_to_delete.push_back(to_delete);
      const bool success = numbers_queue.Remove(to_delete);
      if (!success) {
        cout << to_delete << " not found" << endl;
      } else {
        cout << to_delete << " deleted" << endl;
      }
      //Check if deletion was successful.
      if (numbers_queue.Find(to_delete))
        cout << to_delete << " found - min " << numbers_queue.findMin() << endl;
      else
        cout << to_delete << " not found - min " << numbers_queue.findMin() << endl;     
    }
  }
  numbers_delete.close();
  
  // Reinsert items into binomial queue.
  for (const auto &x: numbers_to_delete) {
    bool success = true;
    if (!numbers_queue.insert(x)) {
      success = false;
    }
    if (success) {
      if (!numbers_queue.Find(x)) success = false;
    }
    if (success)
      cout << x << " good - min " << numbers_queue.findMin() << endl;
    else
      cout << x << " bad - min " << numbers_queue.findMin() << endl;    
  }    
}

// Do not change the main function.
int main(int argc, char **argv) {
  if (argc != 3) {
    cout << "Usage: " << argv[0] << " <input_file_to_create_queue_name> <input_file_to_check_delete_name>" << endl;
    return 0;
  }
  TestInsertAndDelete(string(argv[1]), string(argv[2]));
  return 0;
}
