#ifndef bengisuozdemir_ozdemir_bengisu_hw1_header_H
#define bengisuozdemir_ozdemir_bengisu_hw1_header_H
#include <iostream>
#include <string>
using namespace std;

template <class Object>
class Stack
{
	private:
	struct ListNode
    {
        Object   element;
        ListNode *next;

        ListNode( const Object & theElement, ListNode * n = NULL )
            : element( theElement ), next( n ) { }
    };

    ListNode *topOfStack;  // list itself is the stack

	public:
	Stack( ); //constructor
	Stack( const Stack & rhs ); //copy const.
	~Stack( ); //destructor

	bool isEmpty( ) const; //check if the stack is empty
	bool isFull( ) const; //check if the stack is full
	void makeEmpty( ); //makes the stack empty by popping all items

	void pop( ); //to pop into stack
	void push( const Object & x ); //to push into stack
	Object topAndPop( ); //keep top tem and pop the last item, returns top item
	const Object & top( ) const; // returns the top of the stack
    const Stack & operator=( const Stack & rhs ); //overloading the = operator, gets the stack and returns *this
  };


#include "bengisuozdemir_ozdemir_bengisu_hw1_header.cpp"
#endif