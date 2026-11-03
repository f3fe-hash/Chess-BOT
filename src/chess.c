#include "chess.h"

ChessBoard_t* NewChessBoard() 
{
    return malloc(sizeof(ChessBoard_t));
}

void DeleteChessBoard(ChessBoard_t* board) 
{
    free(board);
}

void MakeMove(ChessBoard_t* const board, const Move_t move) 
{
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
            //finds value of the capturing pawn and of the pawn being captured
            uint8_t pawn1 = board->pieces[move.from];
            uint8_t pawn2 = pawn1 ^ 0b1000;
            //removes pawn from starting square and pawn captured, puts pawn on the target square
            board->pieces[move.from] = board->pieces[move.from + 1] = 0;
            board->pieces[move.to] = pawn1;
            //updates white and black pawn bitboards (finds square of captured pawn by shifting by 1 less)
            board->bitboards[pawn1] ^= 1<<(63-move.from);
            board->bitboards[pawn1] &= 1<<(63-move.to);
            board->bitboards[pawn2] ^= 1<<(62-move.from);
        }
    }

    else 
    {

    }
}

void UpdateAttackBitboards(ChessBoard_t* const board) 
{

}

void LoadFen(ChessBoard_t* const board, const char* fen) 
{
    int fen_idx, board_idx = 0;
    
    while (true) 
    {
        //stores current fen character
        char c = fen[fen_idx];

        if ('0' < c <= '9') 
        {
            board_idx += c - '0';
        }

        else
        {
            switch (c) 
            {
                //updates pawn
                case 'P':
                    board->pieces[board_idx] = 0b1001;
                    board->bitboards[0b1001] &= 1<<(63 - board_idx);
                    break;
                case 'p':
                    board->pieces[board_idx] = 0b1;
                    board->bitboards[0b1] &= 1<<(63 - board_idx);
                    break;

                //updates knight
                case 'N':
                    board->pieces[board_idx] = 0b1010;
                    board->bitboards[0b1010] &= 1<<(63 - board_idx);
                    break;
                case 'n':
                    board->pieces[board_idx] = 0b10;
                    board->bitboards[0b10] &= 1<<(63 - board_idx);
                    break;

                //updates bishop
                case 'B':
                    board->pieces[board_idx] = 0b1011;
                    board->bitboards[0b1011] &= 1<<(63 - board_idx);
                    break;
                case 'b':
                    board->pieces[board_idx] = 0b11;
                    board->bitboards[0b11] &= 1<<(63 - board_idx);
                    break;

                //updates rook
                case 'R':
                    board->pieces[board_idx] = 0b1100;
                    board->bitboards[0b1100] &= 1<<(63 - board_idx);
                    break;
                case 'r':
                    board->pieces[board_idx] = 0b100;
                    board->bitboards[0b100] &= 1<<(63 - board_idx);
                    break;

                //updates queen
                case 'Q':
                    board->pieces[board_idx] = 0b1101;
                    board->bitboards[0b1101] &= 1<<(63 - board_idx);
                    break;
                case 'q':
                    board->pieces[board_idx] = 0b101;
                    board->bitboards[0b101] &= 1<<(63 - board_idx);
                    break;

                //updates king
                case 'K':
                    board->pieces[board_idx] = 0b1110;
                    board->bitboards[0b1110] &= 1<<(63 - board_idx);
                    break;
                case 'k':
                    board->pieces[board_idx] = 0b110;
                    board->bitboards[0b110] &= 1<<(63 - board_idx);
                    break;
            }
        }

        if (board_idx >= 63) break;

        fen_idx++;
        board_idx++;
    }
}