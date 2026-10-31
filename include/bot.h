#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "chess.h"
#include "eval.h"

typedef struct ChessBot_t {
    ChessBoard_t* chessboard;
} ChessBot_t;

// Create a new chess bot instance
ChessBot_t* NewChessBot(ChessBoard_t* board);

// Delete a chess bot (free it)
void DeleteChessBot(ChessBot_t* bot);

// Search a position to a given depth, and return the evaluation.
int Search(ChessBot_t* bot, int depth);

// Search a position to a given depth, and return the best move according to the search.
Move_t SearchMove(ChessBot_t* bot, int depth);