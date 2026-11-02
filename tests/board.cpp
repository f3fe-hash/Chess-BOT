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

TEST(BoardTest, StartingPositionBitboardsCheck)
{
    // Create a chessboard and initialize it with the starting position FEN.
    ChessBoard_t* board = NewChessBoard();
    LoadFen(board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");

    // Test black bitboards
    EXPECT_EQ(board->bitboards[0b0001], 0b00000000_11111111_00000000_00000000_00000000_00000000_00000000_00000000); // Black pawn
    EXPECT_EQ(board->bitboards[0b0010], 0b01000010_00000000_00000000_00000000_00000000_00000000_00000000_00000000); // Black knight
    EXPECT_EQ(board->bitboards[0b0011], 0b00100100_00000000_00000000_00000000_00000000_00000000_00000000_00000000); // Black bishop
    EXPECT_EQ(board->bitboards[0b0100], 0b10000001_00000000_00000000_00000000_00000000_00000000_00000000_00000000); // Black rook
    EXPECT_EQ(board->bitboards[0b0101], 0b00010000_00000000_00000000_00000000_00000000_00000000_00000000_00000000); // Black queen
    EXPECT_EQ(board->bitboards[0b0110], 0b00001000_00000000_00000000_00000000_00000000_00000000_00000000_00000000); // Black king

    // Test white bitboards
    EXPECT_EQ(board->bitboards[0b1001], 0b00000000_00000000_00000000_00000000_00000000_00000000_11111111_00000000); // White pawn
    EXPECT_EQ(board->bitboards[0b1010], 0b00000000_00000000_00000000_00000000_00000000_00000000_00000000_01000010); // White knight
    EXPECT_EQ(board->bitboards[0b1011], 0b00000000_00000000_00000000_00000000_00000000_00000000_00000000_00100100); // White bishop
    EXPECT_EQ(board->bitboards[0b1100], 0b00000000_00000000_00000000_00000000_00000000_00000000_00000000_10000001); // White rook
    EXPECT_EQ(board->bitboards[0b1101], 0b00000000_00000000_00000000_00000000_00000000_00000000_00000000_00010000); // White queen
    EXPECT_EQ(board->bitboards[0b1110], 0b00000000_00000000_00000000_00000000_00000000_00000000_00000000_00001000); // White king
}