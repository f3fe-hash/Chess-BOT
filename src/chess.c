#include "chess.h"

ChessBoard_t* NewChessBoard() 
{
    uint8_t pieces[64] = {0};
    //pawns:
    for (int i = 8; i<=15; i++) pieces[i] = 0b00000001;
    for (int i = 48; i<=55; i++) pieces[i] = 0b00001001;
    //knights:
    pieces[1] = pieces[6] = 0b00000010;
    pieces[57] = pieces[62] = 0b00001010;
    //bishops:
    pieces[2] = pieces[5] = 0b00000011;
    pieces[58] = pieces[61] = 0b00001011;
    //rooks:
    pieces[0] = pieces[7] = 0b00000100;
    pieces[56] = pieces[63] = 0b00001100;
    //queens:
    pieces[4] = 0b00000101;
    pieces[59] = 0b00001101;
    //kings:
    pieces[3] = 0b00000110;
    pieces[60] = 0b00001110;

    //piece bitboards
    uint64_t bitboards[15] = {0, 71776119061217280, 4755801206503243776, 2594073385365405696, 9295429630892703744, 1152921504606846976, 576460752303423488, 0, 0, 65280, 66, 36, 129, 16, 8};
    //attack bitboards
    uint64_t attack[2] = {9132735994842316800, 16760190};

    ChessBoard_t board = {bitboards, attack, pieces, 1};
    return &board;
}

void DeleteChessBoard(ChessBoard_t* board) {
    free(board);
}

void MakeMove(ChessBoard_t* const board, const Move_t move) {
    // if not a castle
    if (move.flags & 0b1111 == 0) 
    {
        //if not the oui oui baguette move
        if (move.flags == 0b10000) 
        {
        //gets the piece being moved, accesses the correct bitboard, and xors with a shifted 1 to flip the starting square
        uint8_t piece1 = board->pieces[move.from];
        board->bitboards[piece1] ^= 1<<(63-move.from);

        //gets piece at target square
        uint8_t piece2 = board->pieces[move.to];

        //if a piece is being captured
        if (piece2 > 0) 
        {
            // removes captured piece from bitboard
            board->bitboards[piece2] ^= 1<<(63-move.to);
        }

        //puts piece on new square
        board->pieces[move.to] = piece1;

        UpdateAttackBitboards(board);
        }
        
        else 
        {

        }
    }

    else 
    {

    }
}

void UpdateAttackBitboards(ChessBoard_t* const board) {

}