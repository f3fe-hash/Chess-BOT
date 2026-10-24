#include "bot.h"

const int MATE_SCORE = -10000;

ChessBot_t* NewChessBot(ChessBoard_t* board)
{
    ChessBot_t* bot = (ChessBot_t *)malloc(sizeof(ChessBot_t));
    bot->chessboard = board;

    return bot;
}

void DeleteChessBot(ChessBot_t* bot)
{
    free(bot);
}

int Search(ChessBot_t* bot, int depth)
{
    if (depth == 0)
        return EvaluatePosition(bot->chessboard);

    bool SideToMove = bot->chessboard->turn;

    if (IsCheckmate(bot->chessboard, SideToMove))
    {
        return SideToMove == 1 ? MATE_SCORE : -MATE_SCORE;
    }

    if (IsStalemate(bot->chessboard, SideToMove))
    {
        return 0;
    }

    uint8_t num_moves = 0;
    Move_t* moves = GetLegalMoves(bot->chessboard, SideToMove, num_moves);

    int best_eval = SideToMove == 1 ? INT32_MIN : INT32_MAX;

    for (int i = 0; i < num_moves; i++)
    {
        MakeMove(bot->chessboard, moves[i]);
        int eval = Search(bot, depth - 1);
        UndoMove(bot, moves[i]);

        // Update best eval
        if (SideToMove == 1)
        {
            if (eval > best_eval)
            {
                best_eval = eval;
            }
        }
        else
        {
            if (eval > best_eval)
            {
                best_eval = eval;
            }
        }
    }

    return best_eval;
}
