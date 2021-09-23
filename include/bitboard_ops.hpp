#pragma once
#include <iostream>

//print the raw bits of a bitboard b in an 8*8 grid
void print(uint64_t b);
//prints the bitboard in a readable format
void pprint(uint64_t b);
//prints the black, white and moves in a readable way
void pprintBoard(uint64_t black, uint64_t white, uint64_t moves);

//returns a copy of the bitboard flipped vertically
uint64_t flipV(uint64_t b);
//returns a copy of the bitboard flipped horizontally
uint64_t flipH(uint64_t b);
//returns a copy of the bitboard flipped along the leading diagonal
uint64_t flipD(uint64_t b);
//returns a copy of the bitboard flipped along the anti - diagonal
uint64_t flipAD(uint64_t b);

//reverses the bit order of an 8 bit number
unsigned char reverseRow(unsigned char b);

//updates the bit at position x,y (0,0 = MSB) on bitboard b to be 1
uint64_t place(uint64_t b, unsigned int x, unsigned int y);

//returns a copy of the bitboard rotated clockwise 90 degrees
uint64_t rotC90(uint64_t b);
//returns a copy of the bitboard rotated anitclockwise by 90 degrees
uint64_t rotAC90(uint64_t b);
//performs a pseudo 45 degree clockwise rotation, mapping diagonals onto rows
uint64_t rotC45(uint64_t b);
//performs a pseudo 45 degree clockwise rotation, mapping diagonals onto rows
uint64_t rotAC45(uint64_t b);

//returns 8 bits representing the diagonal x of a bitboard
unsigned int getDiagonal(uint64_t b, unsigned int x);
//returns 8 bits representing the anti diagonal x of a bitboard
unsigned int getAntiDiagonal(uint64_t b, unsigned int x);
//returns row x of bitboard b
unsigned int getRow(uint64_t b, unsigned int x);
// returns column x of bitboard b
unsigned int getColumn(uint64_t b, unsigned int x);


//returns the bitboard b with its diagonal x ORed with the 8 bits of d
uint64_t setDiagonal(uint64_t b, uint64_t d, unsigned int x);
//returns the bitboard b with its antidiagonal x ORed with the 8 bits of d
uint64_t setAntiDiagonal(uint64_t b, uint64_t d, unsigned int x);
//returns the bitboard b with the row x ORed with the 8 bits of d
uint64_t setRow(uint64_t b, uint64_t d, unsigned int x);
//returns the bitboard b with the column x ORed with the 8 bits of d
uint64_t setColumn(uint64_t b, uint64_t d, unsigned int x);

