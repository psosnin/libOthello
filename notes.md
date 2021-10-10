# Libothello

LibOthello is a high performance library for the game othello

## Bitboard operations

 normal ---> diagonal flip \
```
A8 B8 C8 D8 E8 F8 G8 H8     A8 A7 A6 A5 A4 A3 A2 A1 
A7 B7 C7 D7 E7 F7 G7 H7     B8 B7 B6 B5 B4 B3 B2 B1 
A6 B6 C6 D6 E6 F6 G6 H6     C8 C7 C6 C5 C4 C3 C2 C1
A5 B5 C5 D5 E5 F5 G5 H5     D8 D7 D6 D5 D4 D3 D2 D1
A4 B4 C4 D4 E4 F4 G4 H4 ==> E8 E7 E6 E5 E4 E3 E2 E1
A3 B3 C3 D3 E3 F3 G3 H3     F8 F7 F6 F5 F4 F3 F2 F1
A2 B2 C2 D2 E2 F2 G2 H2     G8 G7 G6 G5 G4 G3 G2 G1
A1 B1 C1 D1 E1 F1 G1 H1     H8 H7 H6 H5 H4 H3 H2 H1
```
normal ---> horizontal flip
```
A8 B8 C8 D8 E8 F8 G8 H8     H8 G8 F8 E8 D8 C8 B8 A8
A7 B7 C7 D7 E7 F7 G7 H7     H7 G7 F7 E7 D7 C7 B7 A7
A6 B6 C6 D6 E6 F6 G6 H6     H6 G6 F6 E6 D6 C6 B6 A6
A5 B5 C5 D5 E5 F5 G5 H5     H5 G5 F5 E5 D5 C5 B5 A5
A4 B4 C4 D4 E4 F4 G4 H4 ==> H4 G4 F4 E4 D4 C4 B4 A4
A3 B3 C3 D3 E3 F3 G3 H3     H3 G3 F3 E3 D3 C3 B3 A3
A2 B2 C2 D2 E2 F2 G2 H2     H2 G2 F2 E2 D2 C2 B2 A2
A1 B1 C1 D1 E1 F1 G1 H1     H1 G1 F1 E1 D1 C1 B1 A1
```
normal ---> vertical flip
```
A8 B8 C8 D8 E8 F8 G8 H8     A1 B1 C1 D1 E1 F1 G1 H1
A7 B7 C7 D7 E7 F7 G7 H7     A2 B2 C2 D2 E2 F2 G2 H2 
A6 B6 C6 D6 E6 F6 G6 H6     A3 B3 C3 D3 E3 F3 G3 H3 
A5 B5 C5 D5 E5 F5 G5 H5     A4 B4 C4 D4 E4 F4 G4 H4
A4 B4 C4 D4 E4 F4 G4 H4 ==> A5 B5 C5 D5 E5 F5 G5 H5 
A3 B3 C3 D3 E3 F3 G3 H3     A6 B6 C6 D6 E6 F6 G6 H6 
A2 B2 C2 D2 E2 F2 G2 H2     A7 B7 C7 D7 E7 F7 G7 H7 
A1 B1 C1 D1 E1 F1 G1 H1     A8 B8 C8 D8 E8 F8 G8 H8
```
Definition and rotation of diagonals:

the 11 key diagonals are defined from the top left corner down
```
_  _  0  1  2  3  4  5       _  6  6  6  6  6  6  6 
_  0  1  2  3  4  5  6       _  _  7  7  7  7  7  7 
0  1  2  3  4  5  6  7       0  0  0  8  8  8  8  8 
1  2  3  4  5  6  7  8       1  1  1  1  9  9  9  9
2  3  4  5  6  7  8  9  ==>  2  2  2  2  2  10 10 10
3  4  5  6  7  8  9  10      3  3  3  3  3  3  _  _ 
4  5  6  7  8  9  10 _       4  4  4  4  4  4  4  _
5  6  7  8  9  10 _  _       5  5  5  5  5  5  5  5
```
the 11 key anti diagonals are similarly defined
``` 
5  4  3  2  1  0  _  _       5  5  5  5  5  5  5  5
6  5  4  3  2  1  0  _       6  6  6  6  6  6  6  _
7  6  5  4  3  2  1  0       7  7  7  7  7  7  _  _
8  7  6  5  4  3  2  1       8  8  8  8  8  0  0  0 
9  8  7  6  5  4  3  2  ==>  9  9  9  9  1  1  1  1
10 9  8  7  6  5  4  3       10 10 10 2  2  2  2  2
_  10 9  8  7  6  5  4       _  _  3  3  3  3  3  3 
_  _  10 9  8  7  6  5       _  4  4  4  4  4  4  4
```
the anti diagonals can be obtained by taking the diagonal of the horizontal flip. diagonals are indexed from 0-10

## Performance Benchmarks:
Version 1.0: 

100 game benchmark in 37602 ms

Version 2.0: Changed passing of hash map by value to pointer

100,000 game benchmark in 2578 ms, 15,000x speed up...

Version 2.1: Remove some pows for logical shifts

Version 2.2: Remove use of strings from playing a move

100,000 game benchmark in 2311ms
