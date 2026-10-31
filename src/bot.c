#include "bot.h"

const int MATE_SCORE = -100000;

ChessBot_t* NewChessBot(ChessBoard_t* board) {
    ChessBot_t* bot = (ChessBot_t *)malloc(sizeof(ChessBot_t));
    bot->chessboard = board;

    return bot;
}

void DeleteChessBot(ChessBot_t* bot) {
    free(bot);
}

int Search(ChessBot_t* bot, int depth) {
    if (depth == 0)
        // Static eval
        return EvaluatePosition(bot->chessboard);

    bool SideToMove = bot->chessboard->turn;

    // Get legal moves.
    uint8_t num_moves = 0;
    Move_t* moves = GetLegalMoves(bot->chessboard, SideToMove, num_moves);

    // No legal moves. Either checkmate or stalemate.
    if (num_moves == 0) {
        if (IsCheck(bot->chessboard, SideToMove))
            // Checkmate.
            return SideToMove == 1 ? MATE_SCORE : -MATE_SCORE;
        else
            // Stalemate.
            return 0;
    }

    int best_eval = SideToMove == 1 ? INT32_MIN : INT32_MAX;

    for (int i = 0; i < num_moves; i++) {
        // Search to the next depth.
        MakeMove(bot->chessboard, moves[i]);
        int eval_cp = Search(bot, depth - 1);
        UndoMove(bot, moves[i]);

        // Update best eval
        if (SideToMove == 1) {
            // White. Keep the highest eval.
            if (eval_cp > best_eval) {
                best_eval = eval_cp;
            }
        }
        else {
            // Black. Keep the lowest eval.
            if (eval_cp > best_eval) {
                best_eval = eval_cp;
            }
        }
    }

    return best_eval;
}

Move_t SearchMove(ChessBot_t* bot, int depth) {
    bool SideToMove = bot->chessboard->turn;

    uint8_t num_moves = 0;
    Move_t* moves = GetLegalMoves(bot->chessboard, SideToMove, num_moves);

    if (num_moves == 0) {
        // No legal moves.
        return (Move_t){};
    }

    int best_eval = SideToMove == 1 ? INT32_MIN : INT32_MAX;
    Move_t best_move;

    for (int i = 0; i < num_moves; i++) {
        // Search to the next depth.
        MakeMove(bot->chessboard, moves[i]);
        int eval = Search(bot, depth - 1);
        UndoMove(bot, moves[i]);

        // Update best eval
        if (SideToMove == 1) {
            // White. Keep highest eval.
            if (eval > best_eval) {
                best_eval = eval;
                best_move = moves[i];
            }
        } else {
            // Black. Keep lowest eval.
            if (eval > best_eval) {
                best_eval = eval;
                best_move = moves[i];
            }
        }
    }

    return best_move;
}
