#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "chess.h"

typedef struct ChessBot_t
{
    ChessBoard_t* chessboard;
} ChessBot_t;

// Create a new chess bot instance
ChessBot_t* NewChessBot(ChessBoard_t* board);

// Delete a chess bot (free it)
void DeleteChessBot(ChessBot_t* bot);

// Search a position to a given depth, and return the evaluation
int Search(ChessBot_t* bot, int depth);