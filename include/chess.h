#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct Move_t
{
    // The squares a piece has moved from and to. Both are [0, 63].
    uint8_t from, to;

    // Bit 0: Has the white king castled king-side?
    // Bit 1: Has the white king castled queen-side?
    // Bit 2: Has the black king castled king-side?
    // Bit 3: Has the black king castled queen-side?
    // Bit 4: Has an en-passant happened on this move?
    uint8_t flags;

    // The square en-passant was done from. In the range [0, 63]
    uint8_t en_passant;
} Move_t;

typedef struct ChessBoard_t
{
    //bitboards
    uint64_t bitboards[15];

    //attack bitboards
    uint64_t attack[2];
    
    //first 3 bits are the piece (from nothing to king) and 4th bit is color
    uint8_t pieces[64];

    //0 is black and 1 is white
    bool turn;
    
    // ...
} ChessBoard_t;

// Create and initialize a blank chessboard.
/*
bitboard decimal values generated in test.c:
65280
66
36
129
16
8
16760190
71776119061217280
4755801206503243776
2594073385365405696
9295429630892703744
1152921504606846976
576460752303423488
9132735994842316800
*/
ChessBoard_t* NewChessBoard();


// Free the chess board
void DeleteChessBoard(ChessBoard_t* board);

// Make a move on the chess board.
// Takes in a constant pointer to a chess board, and a constant move.
void MakeMove(ChessBoard_t* const board, const Move_t move);

// Undo a move on the chess board.
// Takes in a constant pointer to a chess board, and a constant move.
void UndoMove(ChessBoard_t* const board, const Move_t move);

//Updates attack bitboards
//Takes a constant pointer to the chess board.
void UpdateAttackBitboards(ChessBoard_t* const board);

// Get all legal moves for a certain color in a position.
// Takes in a constant pointer to a constant chess board, and a color value.
// 0 = black
// 1 = white
// num_moves is a pointer to a uint8_t that the caller passes and returns the size of the returned array.
Move_t* GetLegalMoves(const ChessBoard_t* const board, const bool color, uint8_t* num_moves);

// See if a certain color is in check in the current position.
// Takes in a constant pointer to a constant chess board, and a color value.
// 0 = black
// 1 = white
bool IsCheck(const ChessBoard_t* const board, const bool color);

// See if it is checkmate for a certain color in the current position.
// Takes in a constant pointer to a constant chess board, and a color value.
// 0 = black
// 1 = white
bool IsCheckmate(const ChessBoard_t* const board, const bool color);

// See if it is stalemate for a certain color in the current position.
// Takes in a constant pointer to a constant chess board, and a color value.
// 0 = black
// 1 = white
bool IsStaleMate(const ChessBoard_t* const board, const bool color);

// Loads a FEN into the chessboard.
void LoadFen(ChessBoard_t* const board, const char* fen);