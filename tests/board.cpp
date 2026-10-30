#include <gtest/gtest.h>
#include "chess.h"

TEST(BoardTest, StartingPositionLegalMoveCheck)
{
    // Create a chessboard and initialize it with the starting position FEN.
    ChessBoard_t* board = NewChessBoard();
    LoadFen(board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");

    // Get legal move count
    uint8_t move_count = 0;
    free(GetLegalMoves(board, 1, &move_count));

    // Exactly 20 moves
    EXPECT_EQ(move_count, 20);
}