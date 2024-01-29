//Jacob Martin
//A .h file that defines the BinomialQueue class and the Binomial node struct.
//Which creates a vector of binomial nodes and a hash table to store the binomial nodes
#ifndef BINOMIAL_QUEUE_H
#define BINOMIAL_QUEUE_H

#include <iostream>
#include <vector>
#include "dsexceptions.h"
#include "quadratic_probing.h"
using namespace std;

// Binomial queue class
//
// CONSTRUCTION: with no parameters
//
// ******************PUBLIC OPERATIONS*********************
// void insert( x )       --> Insert x
// deleteMin( )           --> Return and remove smallest item
// Comparable findMin( )  --> Return smallest item
// bool isEmpty( )        --> Return true if empty; else false
// void makeEmpty( )      --> Remove all items
// void merge( rhs )      --> Absorb rhs into this heap
// ******************ERRORS********************************
// Throws UnderflowException as warranted

template <typename Comparable>
class BinomialQueue
{
public:
  BinomialQueue( ) : theTrees( DEFAULT_TREES )
  {
    for( auto & root : theTrees )
      root = nullptr;
    currentSize = 0;
  }

  BinomialQueue( const Comparable & item ) : theTrees( 1 ), currentSize{ 1 }
    { theTrees[ 0 ] = new BinomialNode{ item, nullptr, nullptr }; }

  BinomialQueue( const BinomialQueue & rhs )
    : theTrees( rhs.theTrees.size( ) ),currentSize{ rhs.currentSize }
  { 
    for( int i = 0; i < rhs.theTrees.size( ); ++i )
      theTrees[ i ] = clone( rhs.theTrees[ i ] );
  }

  BinomialQueue( BinomialQueue && rhs )
    : theTrees{ std::move( rhs.theTrees ) }, currentSize{ rhs.currentSize }
  { 
  }

  ~BinomialQueue( )
    { makeEmpty( ); }

  
  /**
   * Deep copy.
   */
  BinomialQueue & operator=( const BinomialQueue & rhs )
  {
    BinomialQueue copy = rhs;
    std::swap( *this, copy );
    return *this;
  }
      
  /**
   * Move.
   */
  BinomialQueue & operator=( BinomialQueue && rhs )
  {
    std::swap( currentSize, rhs.currentSize );
    std::swap( theTrees, rhs.theTrees );
    
    return *this;
  }
  
  /**
   * Return true if empty; false otherwise.
   */
  bool isEmpty( ) const
    { return currentSize == 0; }

  /**
   * Returns minimum item.
   * Throws UnderflowException if empty.
   */
  const Comparable & findMin( ) const
  {
    if( isEmpty( ) )
      throw UnderflowException{ };

    return theTrees[ findMinIndex( ) ]->element;
  }
  
  /**
  * Insert item x into the priority queue; allows duplicates.
  */
  bool insert(const Comparable & x )
  { 
    //BinomialQueue oneItem{ x }; merge( oneItem ); 
    if(positions_table_.Contains(x)) //checks if it is already inside the hash table
      return false;
    else
    {
      BinomialQueue oneItem{ x }; //create binomial que from parameter passed
      merge(oneItem); //merge the newly created que with the current que
      positions_table_.Insert(x, oneItem.theTrees[0]); //insert the binomial element and the binomial node as the value
      return true;
    }
  }

  /**
   * Insert item x into the priority queue; allows duplicates.
   */
  bool insert( Comparable && x )
  { 
    //BinomialQueue oneItem{ std::move( x ) }; merge( oneItem ); 
    if(positions_table_.Contains(x)) //checks if it is already inside the hash table
      return false;
    else
    {
      BinomialQueue oneItem{ x  };  //create binomial que from parameter passed
      merge(oneItem); //merge the newly created que with the current que
      positions_table_.Insert(std::move( x ), std::move(oneItem.theTrees[0])); //insert the binomial element and the binomial node as the value
      return true;
    }
  }
  
  /**
  * Remove the smallest item from the priority queue.
  * Throws UnderflowException if empty.
  */
  void deleteMin( )
  {
      Comparable x;
      deleteMin( x );
  }

  /**
  * Remove the minimum item and place it in minItem.
  * Throws UnderflowException if empty.
  */
  void deleteMin( Comparable & minItem )
  {
    if( isEmpty( ) )
      throw UnderflowException{ };

    int minIndex = findMinIndex( );
    minItem = theTrees[ minIndex ]->element;

    BinomialNode *oldRoot = theTrees[ minIndex ];
    BinomialNode *deletedTree = oldRoot->leftChild;
    delete oldRoot;

    // Construct H''
    BinomialQueue deletedQueue;
    deletedQueue.theTrees.resize( minIndex );
    deletedQueue.currentSize = ( 1 << minIndex ) - 1;
    for( int j = minIndex - 1; j >= 0; --j )
    {
      deletedQueue.theTrees[ j ] = deletedTree;
      deletedTree = deletedTree->nextSibling;
      deletedQueue.theTrees[ j ]->nextSibling = nullptr;
    }

    // Construct H'
    theTrees[ minIndex ] = nullptr;
    currentSize -= deletedQueue.currentSize + 1;

    merge( deletedQueue );
  }

  /**
   * Make the priority queue logically empty.
   */
  void makeEmpty( )
  {
    currentSize = 0;
    for( auto & root : theTrees )
      makeEmpty( root );
  }

  /**
   * Merge rhs into the priority queue.
   * rhs becomes empty. rhs must be different from this.
   * Exercise 6.35 needed to make this operation more efficient.
   */
  void merge( BinomialQueue & rhs )
  {
    if( this == &rhs )    // Avoid aliasing problems
      return;

    currentSize += rhs.currentSize;

    if( currentSize > capacity( ) )
    {
      int oldNumTrees = theTrees.size( );
      int newNumTrees = max( theTrees.size( ), rhs.theTrees.size( ) ) + 1;
      theTrees.resize( newNumTrees );
      for( int i = oldNumTrees; i < newNumTrees; ++i )
        theTrees[ i ] = nullptr;
    }

    BinomialNode *carry = nullptr;
    for( int i = 0, j = 1; j <= currentSize; ++i, j *= 2 )
    {
      BinomialNode *t1 = theTrees[ i ];
      BinomialNode *t2 = i < rhs.theTrees.size( ) ? rhs.theTrees[ i ] : nullptr;

      int whichCase = t1 == nullptr ? 0 : 1;
      whichCase += t2 == nullptr ? 0 : 2;
      whichCase += carry == nullptr ? 0 : 4;

      switch( whichCase )
      {
        case 0: /* No trees */
        case 1: /* Only this */
          break;
        case 2: /* Only rhs */
          theTrees[ i ] = t2;
          rhs.theTrees[ i ] = nullptr;
          break;
        case 4: /* Only carry */
          theTrees[ i ] = carry;
          carry = nullptr;
          break;
        case 3: /* this and rhs */
          carry = combineTrees( t1, t2 );
          theTrees[ i ] = rhs.theTrees[ i ] = nullptr;
          break;
        case 5: /* this and carry */
          carry = combineTrees( t1, carry );
          theTrees[ i ] = nullptr;
          break;
        case 6: /* rhs and carry */
          carry = combineTrees( t2, carry );
          rhs.theTrees[ i ] = nullptr;
          break;
        case 7: /* All three */
          theTrees[ i ] = carry;
          carry = combineTrees( t1, t2 );
          rhs.theTrees[ i ] = nullptr;
          break;
      }
    }

    for( auto & root : rhs.theTrees )
      root = nullptr;
    rhs.currentSize = 0;
  }    

  /**
   * Search for an item with value x.
   * Returns true if found; false otherwise.
   */
  bool Find(const Comparable & x) const
  {
    return positions_table_.findHelper(x); //calling the findHelper, which searches for the item in the position_table_ 
  }

  /*
  The function first checks whether x is in positions_table_. 
  It returns false if the item is not there.
  Otherwise, removes x from both the binomial queue and the hash table. 
  Note that the removal involves the use of the parent pointers. 
  Also note that the removal may change the values of some of the elements within the hash table.
  */
  bool Remove(const Comparable & x)
  {
    if(!positions_table_.Contains(x)) //checks if it is in the hash table
      return false;
    


    
    return true;
  }
  
  /*
  Create a queue containing the node you want to merge, 
  set the queue size = 1 
  and add to the size of the queue you want to merge into
  */
  bool insertNoMerge(const Comparable & x)
  {
    if(positions_table_.Contains(x)) //checks if it is already inside the hash table
      return false;
    
    BinomialNode *newCarry = new BinomialNode{ x, nullptr, nullptr }; //create new binomial node

    currentSize++; //update the size
    positions_table_.Insert(x, newCarry); //insert the new node inside of the hash table
    for(int i = 0; i < theTrees.size(); i++) //loop through the array of tree roots
    {
      if(theTrees[i] != nullptr) //if the tree root is storing a node
      {
        newCarry = combineTrees(newCarry, theTrees[i]); //combine the two trees and store it inside of the newly created node
        theTrees[i] = nullptr; //set the old tree root to empty
      }
      if(theTrees[i] == nullptr) //if the tree root is not storing anything
      {
        theTrees[i] = newCarry; //make the current root the newly created tree
        return true;
      }
    }
 
    theTrees.push_back(newCarry); //after looping through entire array, push the newly created tree to the back of the array of roots

    return true;
  }

private:
  struct BinomialNode
  {
    Comparable    element;
    BinomialNode *leftChild;
    BinomialNode *nextSibling;
    
    BinomialNode *parent; //parent pointer to the BinomialNode

    BinomialNode( const Comparable & e, BinomialNode *lt, BinomialNode *rt )
      : element{ e }, leftChild{ lt }, nextSibling{ rt } { }
    
    BinomialNode( Comparable && e, BinomialNode *lt, BinomialNode *rt )
      : element{ std::move( e ) }, leftChild{ lt }, nextSibling{ rt } { }
  };

  const static int DEFAULT_TREES = 1;

  vector<BinomialNode *> theTrees;  // An array of tree roots
  int currentSize;                  // Number of items in the priority queue
  
  HashTable<Comparable, BinomialNode*> positions_table_; //a hash table as a private member variable

  /**
   * Find index of tree containing the smallest item in the priority queue.
   * The priority queue must not be empty.
   * Return the index of tree containing the smallest item.
   */
  int findMinIndex( ) const
  {
    int i;
    int minIndex;

    for( i = 0; theTrees[ i ] == nullptr; ++i )
        ;

    for( minIndex = i; i < theTrees.size( ); ++i )
      if( theTrees[ i ] != nullptr &&
        theTrees[ i ]->element < theTrees[ minIndex ]->element )
        minIndex = i;

    return minIndex;
  }

  /**
   * Return the capacity.
   */
  int capacity( ) const
    { return ( 1 << theTrees.size( ) ) - 1; }

  /**
   * Return the result of merging equal-sized t1 and t2.
   */
  BinomialNode * combineTrees( BinomialNode *t1, BinomialNode *t2 )
  {
    if( t2->element < t1->element )
      return combineTrees( t2, t1 );
    t2->nextSibling = t1->leftChild;
    t1->leftChild = t2;
    return t1;
  }

  /**
   * Make a binomial tree logically empty, and free memory.
   */
  void makeEmpty( BinomialNode * & t )
  {
    if( t != nullptr )
    {
      makeEmpty( t->leftChild );
      makeEmpty( t->nextSibling );
      delete t;
      t = nullptr;
    }
  }

  /**
   * Internal method to clone subtree.
   */
  BinomialNode * clone( BinomialNode * t ) const
  {
    if( t == nullptr )
      return nullptr;
    else
      return new BinomialNode{ t->element, clone( t->leftChild ), clone( t->nextSibling ) };
  }

};

#endif
