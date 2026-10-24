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
    // ...
} ChessBoard_t;

// Make a move on the chess board.
// Takes in a constant pointer to a chess board, and a constant move.
void MakeMove(ChessBoard_t* const board, const Move_t move);

// Undo a move on the chess board.
// Takes in a constant pointer to a chess board, and a constant move.
void UndoMove(ChessBoard_t* const board, const Move_t move);

// Get all legal moves for a certain color in a position.
// Takes in a constant pointer to a constant chess board, and a color value.
// 0 = black
// 1 = white
Move_t* GetLegalMoves(const ChessBoard_t* const board, const bool color);

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
