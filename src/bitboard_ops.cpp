#include <iostream>
#include <bitset>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

//print the raw bits of a bitboard b in an 8*8 grid
void print(uint64_t b) {
    for (int i = 7; i >=0; i--) {
        cout << i << "  "<< bitset<8> (b >> i*8) << endl;
    }
}

//prints the bitboard in a readable format
void pprint(uint64_t b) {
    string board = bitset<64>(b).to_string();
    replace(board.begin(), board.end(), '1', 'x');
    replace(board.begin(), board.end(), '0', '.');
    for (int i = 0; i <= 7; i++) {
        cout << board.substr(i*8,8) << endl;
    }
}

//prints the black, white and moves in a readable way
void pprintBoard(uint64_t black, uint64_t white, uint64_t moves) {
    assert((black & white) == 0);
    assert((black & moves) == 0);
    assert((moves & white) == 0);
    for (int i = 7; i >= 0; i--) {
        bitset<8> bk =  black >> i*8;
        bitset<8> wh = white >> i*8;
        bitset<8> mv = moves >> i*8;
        string row = "";
        for (int j = 7; j >= 0; j-- ) {
            if (bk[j] == 1) {
                row.push_back('B');
            }
            else if (wh[j] == 1) {
                row.push_back('W');
            }
            else if (mv[j] == 1) {
                row.push_back('o');
            }
            else {
                row.push_back('_');
            }
        }
        cout << row << endl;
    }
}

//returns a copy of the bitboard flipped vertically
uint64_t flipV(uint64_t b) {
    return    (b & 0xff00000000000000) >> 56
            | (b & 0x00ff000000000000) >> 40
            | (b & 0x0000ff0000000000) >> 24
            | (b & 0x000000ff00000000) >> 8
            | (b & 0x00000000ff000000) << 8
            | (b & 0x0000000000ff0000) << 24
            | (b & 0x000000000000ff00) << 40
            | (b & 0x00000000000000ff) << 56;
}

//returns a copy of the bitboard flipped horizontally
uint64_t flipH(uint64_t b) {
    return    ((b << 7) & 0x8080808080808080)
            | ((b << 5) & 0x4040404040404040)
            | ((b << 3) & 0x2020202020202020)
            | ((b << 1) & 0x1010101010101010)
            | ((b >> 1) & 0x0808080808080808)
            | ((b >> 3) & 0x0404040404040404)
            | ((b >> 5) & 0x0202020202020202)
            | ((b >> 7) & 0x0101010101010101);
}

//returns a copy of the bitboard flipped along the leading diagonal
uint64_t flipD(uint64_t b) {
    return    (b & 0x8040201008040201)
            | (b & 0x4020100804020100) >> 7
            | (b & 0x2010080402010000) >> 14 
            | (b & 0x1008040201000000) >> 21
            | (b & 0x0804020100000000) >> 28
            | (b & 0x0402010000000000) >> 35
            | (b & 0x0201000000000000) >> 42
            | (b & 0x0100000000000000) >> 49
            | (b & 0x0080402010080402) << 7
            | (b & 0x0000804020100804) << 14 
            | (b & 0x0000008040201008) << 21
            | (b & 0x0000000080402010) << 28
            | (b & 0x0000000000804020) << 35
            | (b & 0x0000000000008040) << 42
            | (b & 0x0000000000000080) << 49;
}

//reverses the bit order of an 8 bit number
unsigned char reverseRow(unsigned char b) {
   b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
   b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
   b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
   return b;
}

//returns a copy of the bitboard flipped along the anti - diagonal
uint64_t flipAD(uint64_t b) {
    return    (b & 0x0102040810204080)
            | (b & 0x0204081020408000) >> 9
            | (b & 0x0408102040800000) >> 18
            | (b & 0x0810204080000000) >> 27
            | (b & 0x1020408000000000) >> 36
            | (b & 0x2040800000000000) >> 45
            | (b & 0x4080000000000000) >> 54
            | (b & 0x8000000000000000) >> 63
            | (b & 0x0001020408102040) << 9
            | (b & 0x0000010204081020) << 18
            | (b & 0x0000000102040810) << 27
            | (b & 0x0000000001020408) << 36
            | (b & 0x0000000000010204) << 45
            | (b & 0x0000000000000102) << 54
            | (b & 0x0000000000000001) << 63; 
}

//updates the bit at position x,y (0,0 = MSB) on bitboard b to be 1
uint64_t place(uint64_t b, unsigned int x, unsigned int y) {
    return b + ((uint64_t) 0b1 << (63-x - 8*y));
}

//returns a copy of the bitboard rotated clockwise 90 degrees
uint64_t rotC90(uint64_t b) {return flipV(flipAD(b));}

//returns a copy of the bitboard rotated anitclockwise by 90 degrees
uint64_t rotAC90(uint64_t b) {return flipAD(flipV(b));}

//performs a pseudo 45 degree clockwise rotation, mapping diagonals onto rows
uint64_t rotC45(uint64_t b) {
    return (b & 0x8080808080808080) 
        |  (b & 0x4040404040404000) >> 8
        |  (b & 0x2020202020200000) >> 16
        |  (b & 0x1010101010000000) >> 24
        |  (b & 0x0808080800000000) >> 32
        |  (b & 0x0404040000000000) >> 40
        |  (b & 0x0202000000000000) >> 48
        |  (b & 0x0100000000000000) >> 56
        |  (b & 0x0000000000000040) << 56
        |  (b & 0x0000000000002020) << 48
        |  (b & 0x0000000000101010) << 40
        |  (b & 0x0000000008080808) << 32
        |  (b & 0x0000000404040404) << 24
        |  (b & 0x0000020202020202) << 16
        |  (b & 0x0001010101010101) << 8l;
}

//performs a pseudo 45 degree clockwise rotation, mapping diagonals onto rows
uint64_t rotAC45(uint64_t b) {
    return flipV(rotC45(flipV(b)));
}

//returns 8 bits representing the diagonal x of a bitboard
unsigned int getDiagonal(uint64_t b, unsigned int x){
        if (x > 10 or x < 0) {
            return 0;
        }
        unsigned int shift[11] = {32,  24,   16,   8,    0,    56,   48,   40,   32,   24,   16};
        unsigned int mask[11] = {0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff, 0xfe, 0xfc, 0xf8, 0xf0, 0xe0};
        return (rotAC45(b) >> shift[x]) & mask[x];
}

//returns 8 bits representing the anti diagonal x of a bitboard
unsigned int getAntiDiagonal(uint64_t b, unsigned int x) {
    return reverseRow(getDiagonal(flipH(b),x));
}

//returns the bitboard b with its diagonal x ORed with the 8 bits of d
uint64_t setDiagonal(uint64_t b, uint64_t d, unsigned int x){
        if (x > 10 or x < 0) {
            return 0;
        }
        unsigned int shift[11] = {32,  24,   16,   8,    0,    56,   48,   40,   32,   24,   16};
        unsigned int mask[11] = {0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff, 0xfe, 0xfc, 0xf8, 0xf0, 0xe0};

    return rotC45(rotAC45(b) | ((d & mask[x]) << shift[x]));
}

//returns the bitboard b with its antidiagonal x ORed with the 8 bits of d
uint64_t setAntiDiagonal(uint64_t b, uint64_t d, unsigned int x){ 
    return flipH(setDiagonal(flipH(b),reverseRow(d),x));
}

//returns row x of bitboard b
unsigned int getRow(uint64_t b, unsigned int x){
    return (b >> (7-x)*8) & 0xff;
}

// returns column x of bitboard b
unsigned int getColumn(uint64_t b, unsigned int x){
    return getRow(rotC90(b), x);
}

//returns the bitboard b with the row x ORed with the 8 bits of d
uint64_t setRow(uint64_t b, uint64_t d, unsigned int x) {
    return b | ((d & 0xff) << (7-x)*8);
}

//returns the bitboard b with the column x ORed with the 8 bits of d
uint64_t setColumn(uint64_t b, uint64_t d, unsigned int x) {
    return rotAC90(setRow(rotC90(b), d, x));
}

