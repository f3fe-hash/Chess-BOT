#include "bot.h"

const int MATE_SCORE = -100000;

ChessBot_t* NewChessBot(ChessBoard_t* board) {
    ChessBot_t* bot = (ChessBot_t *)malloc(sizeof(ChessBot_t));
    bot->chessboard = board;

    bot->tt = NewTranspositionTable();

    return bot;
}

void DeleteChessBot(ChessBot_t* bot) {
    free(bot->tt);
    free(bot);
}

int Search(ChessBot_t* bot, int depth, int alpha, int beta) {
    if (depth == 0)
        // Static eval
        return EvaluatePosition(bot->chessboard);

    bool SideToMove = bot->chessboard->turn;

    // TODO: Implement GetHash
    const uint64_t hash = GetHash(bot->chessboard);

    // Get legal moves.
    uint8_t num_moves = 0;
    Move_t* moves = GetLegalMoves(bot->chessboard, SideToMove, &num_moves);

    // No legal moves. Either checkmate or stalemate.
    if (num_moves == 0) {
        if (IsCheck(bot->chessboard, SideToMove))
            // Checkmate.
            return SideToMove == 1 ? MATE_SCORE : -MATE_SCORE;
        else
            // Stalemate.
            return 0;
    }

    int best_eval_cp = SideToMove == 1 ? INT32_MIN : INT32_MAX;

    // Get the tt entry, if it exists.
    bool tt_entry_found = false;
    TranspositionTableEntry_t entry = GetTTEntry(bot->tt, hash, &tt_entry_found);

    if (tt_entry_found) {
        // The lower the depth is, the closer the eval was to the root, so the more accurate it is.
        if (entry.depth <= depth) {
            // Update alpha, beta and the eval based on the TT.
            switch (entry.bound) {
                case TTBOUND_EXACT:
                    return entry.eval_cp;
                
                case TTBOUND_LOWER:
                    if (entry.eval_cp >= alpha)
                        alpha = entry.eval_cp;
                    break;
                
                case TTBOUND_UPPER:
                    if (entry.eval_cp <= beta)
                        beta = entry.eval_cp;
                    break;
                
                default:
                    break;
            }
        }
    }

    for (int i = 0; i < num_moves; i++) {
        ChessBoard_t original;
        memcpy(&original, bot->chessboard, sizeof(ChessBoard_t));

        // Search to the next depth.
        MakeMove(bot->chessboard, moves[i]);
        int eval_cp = Search(bot, depth - 1, INT32_MIN, INT32_MAX);

        //UndoMove(bot, moves[i]);
        memcpy(bot->chessboard, &original, sizeof(ChessBoard_t));

        // Update best eval
        if (SideToMove == 1) {
            // White. Keep the highest eval.
            if (eval_cp > best_eval_cp) {
                best_eval_cp = eval_cp;
            }

            // Update alpha
            if (eval_cp > alpha) {
                alpha = eval_cp;

                // Store a lower bound.
                StoreTTEntry(bot->tt, hash, depth - 1, eval_cp, TTBOUND_LOWER);
            }
        }
        else {
            // Black. Keep the lowest eval.
            if (eval_cp > best_eval_cp) {
                best_eval_cp = eval_cp;
            }

            // Update beta.
            if (eval_cp < beta) {
                beta = eval_cp;

                // Store an upper bound.
                StoreTTEntry(bot->tt, hash, depth - 1, eval_cp, TTBOUND_UPPER);
            }
        }

        // Prune. A better move was available up the search tree.
        if (beta <= alpha)
            break;
    }

    StoreTTEntry(bot->tt, hash, depth, best_eval_cp, TTBOUND_EXACT);

    return best_eval_cp;
}

Move_t SearchMove(ChessBot_t* bot, int depth) {
    bool SideToMove = bot->chessboard->turn;

    uint8_t num_moves = 0;
    Move_t* moves = GetLegalMoves(bot->chessboard, SideToMove, &num_moves);

    if (num_moves == 0) {
        // No legal moves.
        return (Move_t){};
    }

    int best_eval = SideToMove == 1 ? INT32_MIN : INT32_MAX;
    Move_t best_move;

    for (int i = 0; i < num_moves; i++) {
        ChessBoard_t original;
        memcpy(&original, bot->chessboard, sizeof(ChessBoard_t));

        // Search to the next depth.
        MakeMove(bot->chessboard, moves[i]);
        int eval = Search(bot, depth - 1, INT32_MIN, INT32_MAX);

        //UndoMove(bot, moves[i]);
        memcpy(bot->chessboard, &original, sizeof(ChessBoard_t));

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
