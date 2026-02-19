#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

#define UCB_EXPLORE_PARAM 2.0
#define TERMINAL_SCORE 50
#define MAX_SCORE_VALUE 1000
#define TIME_THRESHOLD 0.965

static const int delta[24][2] = {
    {1, 1},  {0, 1},   {-1, 1},  {-1, 0},  {-1, -1}, {0, -1}, {1, -1}, {1, 0},
    {2, 0},  {2, 1},   {2, 2},   {1, 2},   {0, 2},   {-1, 2}, {-2, 2}, {-2, 1},
    {-2, 0}, {-2, -1}, {-2,-2}, {-1,-2}, {0,-2},   {1,-2},  {2,-2},  {2,-1}
};

static const int piece_conversion_mask[4] = {0x15, 0, 0, 0x15};

typedef struct {
    int fromX, fromY, toX, toY;
} MoveAction;

typedef struct {
    int boardState[7];
    int playerColor;
} AtaxxBoard;

typedef struct MCTSNode {
    int boardState[7];
    int playerColor;
    MoveAction move_from_parent;
    struct MCTSNode *children;
    int child_count;
    int terminal_score;
    int visit_count;
    int total_score;
} MCTSNode;

inline int isValidPosition(int x, int y) {
    return x >= 0 && x <= 6 && y >= 0 && y <= 6;
}

inline int getPieceAt(const AtaxxBoard *board, int x, int y) {
    return (board->boardState[x] >> ((y + 1) << 1)) & 3;
}

inline int isCurrentPlayerPiece(const AtaxxBoard *board, int x, int y) {
    return ((board->boardState[x] >> ((y + 1) << 1)) & 3) == board->playerColor;
}

inline int isEmptyPosition(const AtaxxBoard *board, int x, int y) {
    return !((board->boardState[x] >> ((y + 1) << 1)) & 2);
}

void makeMove(AtaxxBoard *board, const MoveAction *move) {
    if (abs(move->toX - move->fromX) == 2 || abs(move->toY - move->fromY) == 2)
        board->boardState[move->fromX] ^= (2 << ((move->fromY + 1) << 1));
    
    board->boardState[move->toX] &= ~(3 << ((move->toY + 1) << 1));
    board->boardState[move->toX] |= (board->playerColor << ((move->toY + 1) << 1));
    
    for (int i = -1; i <= 1; ++i) {
        if (move->toX + i >= 0 && move->toX + i <= 6) {
            board->boardState[move->toX + i] &= ~(piece_conversion_mask[0] << (move->toY << 1));
            board->boardState[move->toX + i] |= (piece_conversion_mask[board->playerColor] << (move->toY << 1));
        }
    }
    
    board->playerColor = 5 - board->playerColor;
}

int evaluatePosition(const AtaxxBoard *board) {
    int pieceCount[4] = {0};
    
    for (int i = 0; i < 7; ++i) {
        int row = board->boardState[i];
        for (int j = 0; j < 7; ++j)
            ++pieceCount[(row >> ((j + 1) << 1)) & 3];
    }
    
    return pieceCount[board->playerColor] - pieceCount[5 - board->playerColor];
}

int checkGameOver(const AtaxxBoard *board) {
    int pieceCount[4] = {0};
    
    for (int i = 0; i < 7; ++i) {
        int row = board->boardState[i];
        for (int j = 0; j < 7; ++j)
            ++pieceCount[(row >> ((j + 1) << 1)) & 3];
    }
    
    return pieceCount[board->playerColor] <= 24 ? -TERMINAL_SCORE : TERMINAL_SCORE;
}

MoveAction* generateLegalMoves(const AtaxxBoard *board, int *moveCount) {
    *moveCount = 0;
    MoveAction* legalMoves = (MoveAction*)malloc(600 * sizeof(MoveAction));
    
    for (int x0 = 0; x0 < 7; ++x0) {
        for (int y0 = 0; y0 < 7; ++y0) {
            if (isEmptyPosition(board, x0, y0)) {
                for (int dir = 0; dir < 24; ++dir) {
                    int x1 = x0 + delta[dir][0];
                    int y1 = y0 + delta[dir][1];
                    
                    if (!isValidPosition(x1, y1)) continue;
                    if (!isCurrentPlayerPiece(board, x1, y1)) continue;
                    
                    MoveAction validMove = {x1, y1, x0, y0};
                    legalMoves[(*moveCount)++] = validMove;
                }
            }
        }
    }
    return legalMoves;
}

void initializeBoard(AtaxxBoard *board) {
    memset(board->boardState, 0, 7 * sizeof(int));
    board->boardState[0] = 0xC008;
    board->boardState[6] = 0x800C;
    board->playerColor = 2;
}

void initializeBoardFromInput(AtaxxBoard *board) {
    initializeBoard(board);
    int turnID;
    scanf("%d", &turnID);
    MoveAction tempMove = {-1, -1, -1, -1};
    
    for (int i = 1; i < turnID; ++i) {
        scanf("%d %d %d %d", &tempMove.fromX, &tempMove.fromY, &tempMove.toX, &tempMove.toY);
        if (tempMove.toX >= 0) makeMove(board, &tempMove);
        scanf("%d %d %d %d", &tempMove.fromX, &tempMove.fromY, &tempMove.toX, &tempMove.toY);
        makeMove(board, &tempMove);
    }
    
    scanf("%d %d %d %d", &tempMove.fromX, &tempMove.fromY, &tempMove.toX, &tempMove.toY);
    if (tempMove.toX >= 0) makeMove(board, &tempMove);
}

void initializeMCTSNode(MCTSNode *node) {
    memset(node->boardState, 0, 7 * sizeof(int));
    node->playerColor = 2;
    node->move_from_parent.fromX = -1;
    node->move_from_parent.fromY = -1;
    node->move_from_parent.toX = -1;
    node->move_from_parent.toY = -1;
    node->children = NULL;
    node->child_count = 0;
    node->terminal_score = 0;
    node->visit_count = 0;
    node->total_score = 0;
}

void initializeMCTSNodeFromInput(MCTSNode *node) {
    AtaxxBoard board;
    initializeBoardFromInput(&board);
    
    memcpy(node->boardState, board.boardState, 7 * sizeof(int));
    node->playerColor = board.playerColor;
    node->move_from_parent.fromX = -1;
    node->move_from_parent.fromY = -1;
    node->move_from_parent.toX = -1;
    node->move_from_parent.toY = -1;
    node->children = NULL;
    node->child_count = 0;
    node->terminal_score = 0;
    node->visit_count = 0;
    node->total_score = 0;
}

void expandChildren(MCTSNode *node, MoveAction *legalMoves) {
    if (node->child_count <= 0) {
        free(legalMoves);
        return;
    }
    
    AtaxxBoard board;
    memcpy(board.boardState, node->boardState, 7 * sizeof(int));
    board.playerColor = node->playerColor;
    
    node->children = (MCTSNode*)malloc(node->child_count * sizeof(MCTSNode));
    
    for (int i = 0; i < node->child_count; ++i) {
        initializeMCTSNode(&node->children[i]);
        memcpy(node->children[i].boardState, board.boardState, 7 * sizeof(int));
        node->children[i].playerColor = board.playerColor;
        
        makeMove(&board, &legalMoves[i]);
        memcpy(node->children[i].boardState, board.boardState, 7 * sizeof(int));
        node->children[i].playerColor = board.playerColor;
        node->children[i].move_from_parent = legalMoves[i];
        
        memcpy(board.boardState, node->boardState, 7 * sizeof(int));
        board.playerColor = node->playerColor;
    }
    
    free(legalMoves);
}

inline int getTerminalScore(MCTSNode *node) {
    if (node->terminal_score != 0) return node->terminal_score;
    
    AtaxxBoard board;
    memcpy(board.boardState, node->boardState, 7 * sizeof(int));
    board.playerColor = node->playerColor;
    
    return node->terminal_score = checkGameOver(&board);
}

inline double calculateUCB(const MCTSNode *node, int parent_visits, double exploration_rate) {
    if (node->visit_count == 0) 
        return -MAX_SCORE_VALUE + (double)rand() / RAND_MAX;
    
    double avg_score = (double)(node->total_score) / node->visit_count;
    double exploration = exploration_rate * sqrt(log(parent_visits) / node->visit_count);
    return avg_score - exploration;
}

MCTSNode* selectBestChild(MCTSNode *node, int is_final_selection) {
    if (node->child_count <= 0) return NULL;
    
    double exploration_rate = is_final_selection ? 0 : UCB_EXPLORE_PARAM;
    ++node->visit_count;
    
    int best_index = 0;
    double best_value = MAX_SCORE_VALUE;
    
    for (int i = 0; i < node->child_count; ++i) {
        double node_value = calculateUCB(&node->children[i], node->visit_count, exploration_rate);
        if (node_value < best_value) {
            best_value = node_value;
            best_index = i;
        }
    }
    
    return &node->children[best_index];
}

inline int updateNodeValue(MCTSNode *node, int value) {
    node->total_score += value;
    return -value;
}

int runTreeSearch(MCTSNode *node) {
    if (node->children == NULL) {
        AtaxxBoard board;
        memcpy(board.boardState, node->boardState, 7 * sizeof(int));
        board.playerColor = node->playerColor;
        
        MoveAction* legalMoves = generateLegalMoves(&board, &node->child_count);
        expandChildren(node, legalMoves);
        
        if (node->child_count == 0) return updateNodeValue(node, getTerminalScore(node));
        return updateNodeValue(node, evaluatePosition(&board));
    } 
    else if (node->child_count == 0) {
        return updateNodeValue(node, getTerminalScore(node));
    } 
    else {
        MCTSNode *selected_child = selectBestChild(node, 0);
        if (selected_child == NULL) return updateNodeValue(node, getTerminalScore(node));
        return updateNodeValue(node, runTreeSearch(selected_child));
    }
}

int main() {
    srand(time(NULL));
    
    clock_t search_start_time = clock();
    double search_time_limit = TIME_THRESHOLD * CLOCKS_PER_SEC;
    
    MCTSNode game_root;
    initializeMCTSNode(&game_root);
    initializeMCTSNodeFromInput(&game_root);
    
    while (clock() - search_start_time < search_time_limit) {
        runTreeSearch(&game_root);
    }
    
    int startX = -1, startY = -1, resultX = -1, resultY = -1;
    
    if (game_root.child_count > 0) {
        MCTSNode *best_child = selectBestChild(&game_root, 1);
        if (best_child != NULL) {
            startX = best_child->move_from_parent.fromX;
            startY = best_child->move_from_parent.fromY;
            resultX = best_child->move_from_parent.toX;
            resultY = best_child->move_from_parent.toY;
        }
    }
    
    printf("%d %d %d %d", startX, startY, resultX, resultY);
    return 0;
}