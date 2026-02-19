// 同化棋（Ataxx）蒙特卡洛树搜索实现
// 作者：基于蒙特卡洛树搜索的AI策略

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

// 蒙特卡洛树搜索参数
#define UCB_EXPLORE_PARAM 2.0     // UCB探索参数
#define TERMINAL_SCORE 50         // 终局分数
#define MAX_SCORE_VALUE 1000      // 最大分数值
#define TIME_THRESHOLD 0.88       // 时间阈值(秒)

// 方向增量数组，前8个为相邻位置，后16个为跳跃位置
static int piece_conversion_mask[4] = {0x15, 0, 0, 0x15};
static int direction_delta[24][2] = {
    {1, 1},  {0, 1},   {-1, 1},  {-1, 0},  {-1, -1}, {0, -1}, {1, -1}, {1, 0},
    {2, 0},  {2, 1},   {2, 2},   {1, 2},   {0, 2},   {-1, 2}, {-2, 2}, {-2, 1},
    {-2, 0}, {-2, -1}, {-2,-2}, {-1,-2}, {0,-2},   {1,-2},  {2,-2},  {2,-1}
};

// 时间控制
static clock_t timeout_clock;

// 移动操作结构体
typedef struct {
    int fromX, fromY, toX, toY;
} MoveAction;

// 棋盘结构体
typedef struct {
    int boardState[7];     // 使用位编码存储棋盘状态
    int playerColor;       // 当前玩家颜色 (2=黑, 3=白)
} AtaxxBoard;

// MCTS节点结构体
typedef struct MCTSNode {
    int boardState[7];     // 使用位编码存储棋盘状态
    int playerColor;       // 当前玩家颜色 (2=黑, 3=白)
    MoveAction move_from_parent;   // 从父节点到达此节点的移动
    struct MCTSNode *children;     // 子节点数组
    int child_count;               // 子节点数量
    int terminal_score;            // 终局得分缓存
    int visit_count;               // 访问次数
    int total_score;               // 总得分
} MCTSNode;

// 函数声明
void readMoveAction(FILE *stream, MoveAction *move);
void writeMoveAction(FILE *stream, MoveAction *move);
int isValidPosition(AtaxxBoard *board, int x, int y);
int getPieceAt(AtaxxBoard *board, int x, int y);
int isCurrentPlayerPiece(AtaxxBoard *board, int x, int y);
int isEmptyPosition(AtaxxBoard *board, int x, int y);
void makeMove(AtaxxBoard *board, const MoveAction *move);
int evaluatePosition(AtaxxBoard *board);
int checkGameOver(AtaxxBoard *board);
MoveAction* generateLegalMoves(AtaxxBoard *board, int *moveCount);
void initializeBoard(AtaxxBoard *board);
void initializeBoardFromInput(AtaxxBoard *board, FILE *stream);
void copyBoard(AtaxxBoard *dest, const AtaxxBoard *src);
void expandChildren(MCTSNode *node, MoveAction *legalMoves);
int getTerminalScore(MCTSNode *node);
double calculateUCB(MCTSNode *node, int parent_visits, double exploration_rate);
MCTSNode* selectBestChild(MCTSNode *node, int is_final_selection);
int updateNodeValue(MCTSNode *node, int value);
int runTreeSearch(MCTSNode *node);
void getBestMove(MCTSNode *node);
void initializeMCTSNode(MCTSNode *node);
void initializeMCTSNodeFromInput(MCTSNode *node, FILE *stream);
void copyMCTSNode(MCTSNode *dest, const MCTSNode *src);

// 读取移动操作
void readMoveAction(FILE *stream, MoveAction *move) {
    fscanf(stream, "%d %d %d %d", &move->fromX, &move->fromY, &move->toX, &move->toY);
}

// 输出移动操作
void writeMoveAction(FILE *stream, MoveAction *move) {
    fprintf(stream, "%d %d %d %d", move->fromX, move->fromY, move->toX, move->toY);
}

// 判断坐标是否在棋盘内
int isValidPosition(AtaxxBoard *board, int x, int y) {
    return x >= 0 && x <= 6 && y >= 0 && y <= 6;
}

// 获取指定位置的棋子类型
int getPieceAt(AtaxxBoard *board, int x, int y) {
    return (board->boardState[x] >> ((y + 1) << 1)) & 3;
}

// 判断是否是当前玩家的棋子
int isCurrentPlayerPiece(AtaxxBoard *board, int x, int y) {
    return ((board->boardState[x] >> ((y + 1) << 1)) & 3) == board->playerColor;
}

// 判断位置是否为空
int isEmptyPosition(AtaxxBoard *board, int x, int y) {
    return !((board->boardState[x] >> ((y + 1) << 1)) & 2);
}

// 执行移动操作
void makeMove(AtaxxBoard *board, const MoveAction *move) {
    // 处理跳跃移动 - 如果是跳跃则移除原位置棋子
    if (abs(move->toX - move->fromX) == 2 || abs(move->toY - move->fromY) == 2)
        board->boardState[move->fromX] ^= (2 << ((move->fromY + 1) << 1));
    
    // 更新目标位置为当前玩家的棋子
    board->boardState[move->toX] &= ~(3 << ((move->toY + 1) << 1));
    board->boardState[move->toX] |= (board->playerColor << ((move->toY + 1) << 1));
    
    // 同化周围的对方棋子
    for (int i = -1; i <= 1; ++i) {
        if (move->toX + i >= 0 && move->toX + i <= 6) {
            board->boardState[move->toX + i] &= ~(piece_conversion_mask[0] << (move->toY << 1));
            board->boardState[move->toX + i] |= (piece_conversion_mask[board->playerColor] << (move->toY << 1));
        }
    }
    
    // 切换玩家
    board->playerColor = 5 - board->playerColor;  // 2->3, 3->2
}

// 评估当前局面分数 (正值对当前玩家有利)
int evaluatePosition(AtaxxBoard *board) {
    int pieceCount[4] = {0};
    
    // 统计棋盘上各颜色棋子数量
    for (int i = 0; i < 7; ++i) {
        int row = board->boardState[i];
        for (int j = 0; j < 7; ++j)
            ++pieceCount[(row >> ((j + 1) << 1)) & 3];
    }
    
    // 返回当前玩家与对手棋子数量之差
    return pieceCount[board->playerColor] - pieceCount[5 - board->playerColor];
}

// 判断游戏是否结束，返回终局分数
int checkGameOver(AtaxxBoard *board) {
    int pieceCount[4] = {0};
    
    // 统计棋盘上各颜色棋子数量
    for (int i = 0; i < 7; ++i) {
        int row = board->boardState[i];
        for (int j = 0; j < 7; ++j)
            ++pieceCount[(row >> ((j + 1) << 1)) & 3];
    }
    
    // 如果当前玩家棋子数量不超过一半，则判负
    return pieceCount[board->playerColor] <= 24 ? -TERMINAL_SCORE : TERMINAL_SCORE;
}

// 生成所有合法移动
MoveAction* generateLegalMoves(AtaxxBoard *board, int *moveCount) {
    *moveCount = 0;
    MoveAction* legalMoves = (MoveAction*)malloc(600 * sizeof(MoveAction));
    
    // 遍历棋盘寻找合法移动
    for (int x0 = 0; x0 < 7; ++x0) {
        for (int y0 = 0; y0 < 7; ++y0) {
            if (isEmptyPosition(board, x0, y0)) {
                for (int dir = 0; dir < 24; ++dir) {
                    int x1 = x0 + direction_delta[dir][0];
                    int y1 = y0 + direction_delta[dir][1];
                    
                    if (!isValidPosition(board, x1, y1)) continue;
                    if (!isCurrentPlayerPiece(board, x1, y1)) continue;
                    
                    MoveAction validMove = {x1, y1, x0, y0};
                    legalMoves[(*moveCount)++] = validMove;
                    
                    // 优化：找到短距离移动后，跳过更远的移动检查
                    if (dir < 8) dir = 8;
                }
            }
        }
    }
    
    return legalMoves;
}

// 初始化棋盘
void initializeBoard(AtaxxBoard *board) {
    memset(board->boardState, 0, 7 * sizeof(int));
    // 初始化棋盘，放置初始棋子
    board->boardState[0] = 0xC008;  // |黑|白|
    board->boardState[6] = 0x800C;  // |白|黑|
    board->playerColor = 2;         // 黑方先行
}

// 从输入流初始化棋盘
void initializeBoardFromInput(AtaxxBoard *board, FILE *stream) {
    initializeBoard(board);
    
    int turnCount;
    fscanf(stream, "%d", &turnCount);
    MoveAction tempMove = {-1, -1, -1, -1};
    
    // 恢复游戏状态到当前回合
    for (int i = 1; i != turnCount; ++i) {
        readMoveAction(stream, &tempMove);
        if (tempMove.toX >= 0) makeMove(board, &tempMove);
        readMoveAction(stream, &tempMove);
        makeMove(board, &tempMove);
    }
    
    readMoveAction(stream, &tempMove);
    if (tempMove.toX >= 0) makeMove(board, &tempMove);
}

// 复制棋盘状态
void copyBoard(AtaxxBoard *dest, const AtaxxBoard *src) {
    memcpy(dest->boardState, src->boardState, 7 * sizeof(int));
    dest->playerColor = src->playerColor;
}

// 初始化MCTS节点
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

// 从输入流初始化MCTS节点
void initializeMCTSNodeFromInput(MCTSNode *node, FILE *stream) {
    AtaxxBoard board;
    initializeBoardFromInput(&board, stream);
    
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

// 复制MCTS节点
void copyMCTSNode(MCTSNode *dest, const MCTSNode *src) {
    memcpy(dest->boardState, src->boardState, 7 * sizeof(int));
    dest->playerColor = src->playerColor;
    dest->move_from_parent = src->move_from_parent;
    dest->children = NULL;  // 不复制子节点
    dest->child_count = 0;
    dest->terminal_score = 0;
    dest->visit_count = 0;
    dest->total_score = 0;
}

// 展开节点，创建所有可能的子节点
void expandChildren(MCTSNode *node, MoveAction *legalMoves) {
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
        
        // 恢复board状态以便下一次使用
        memcpy(board.boardState, node->boardState, 7 * sizeof(int));
        board.playerColor = node->playerColor;
    }
    
    free(legalMoves);
}

// 获取终局评分（如果已计算则返回缓存值）
int getTerminalScore(MCTSNode *node) {
    if (node->terminal_score != 0) return node->terminal_score;
    
    AtaxxBoard board;
    memcpy(board.boardState, node->boardState, 7 * sizeof(int));
    board.playerColor = node->playerColor;
    
    return node->terminal_score = checkGameOver(&board);
}

// 计算UCB值 (Upper Confidence Bound)
double calculateUCB(MCTSNode *node, int parent_visits, double exploration_rate) {
    // 未访问过的节点给予随机优先级，确保每个节点都会被探索
    if (node->visit_count == 0) 
        return -MAX_SCORE_VALUE + (double)rand() / RAND_MAX;
    
    // UCB公式：平均得分 - 探索系数 * sqrt(ln(父节点访问次数) / 当前节点访问次数)
    return (double)(node->total_score) / node->visit_count - 
           exploration_rate * sqrt(log(parent_visits) / node->visit_count);
}

// 选择最佳子节点
MCTSNode* selectBestChild(MCTSNode *node, int is_final_selection) {
    // 最终选择时不考虑探索因子
    double exploration_rate = is_final_selection ? 0 : UCB_EXPLORE_PARAM;
    ++node->visit_count;
    
    int best_index = 0;
    double best_value = MAX_SCORE_VALUE;
    
    // 遍历所有子节点，选择UCB值最小的（对手最不利的）
    for (int i = 0; i < node->child_count; ++i) {
        double node_value = calculateUCB(&node->children[i], node->visit_count, exploration_rate);
        if (node_value < best_value) {
            best_value = node_value;
            best_index = i;
        }
    }
    
    return &node->children[best_index];
}

// 更新节点价值
int updateNodeValue(MCTSNode *node, int value) {
    node->total_score += value;
    // 返回负值，因为是零和游戏
    return -value;
}

// MCTS的核心过程：选择、扩展、模拟、回溯
int runTreeSearch(MCTSNode *node) {
    // 如果是叶节点，需要展开
    if (node->children == NULL) {
        AtaxxBoard board;
        memcpy(board.boardState, node->boardState, 7 * sizeof(int));
        board.playerColor = node->playerColor;
        
        MoveAction* legalMoves = generateLegalMoves(&board, &node->child_count);
        expandChildren(node, legalMoves);
        
        // 如果没有合法移动，返回终局评分
        if (node->child_count == 0) return updateNodeValue(node, getTerminalScore(node));
        // 否则返回基于局面评估的分数
        return updateNodeValue(node, evaluatePosition(&board));
    } 
    // 如果没有子节点，返回终局评分
    else if (node->child_count == 0) {
        return updateNodeValue(node, getTerminalScore(node));
    } 
    // 如果有子节点，选择一个并继续搜索
    else {
        MCTSNode *selected_child = selectBestChild(node, 0);
        return updateNodeValue(node, runTreeSearch(selected_child));
    }
}

// 输出最佳移动
void getBestMove(MCTSNode *node) {
    MCTSNode *best_child = selectBestChild(node, 1);
    writeMoveAction(stdout, &best_child->move_from_parent);
}

// 主函数
int main() {
    // 设置随机数种子
    srand(time(NULL));
    
    // 从标准输入初始化游戏状态
    MCTSNode game_root;
    initializeMCTSNodeFromInput(&game_root, stdin);
    
    // 记录MCTS开始时间
    clock_t search_start_time = clock();
    double search_time_limit = TIME_THRESHOLD * CLOCKS_PER_SEC;
    
    // 在时间限制内进行尽可能多的MCTS迭代
    while (clock() - search_start_time < search_time_limit) {
        runTreeSearch(&game_root);
    }
    
    // 选择并输出最佳移动
    getBestMove(&game_root);
    
    // 释放内存
    if (game_root.children != NULL) {
        for (int i = 0; i < game_root.child_count; ++i) {
            if (game_root.children[i].children != NULL) {
                free(game_root.children[i].children);
            }
        }
        free(game_root.children);
    }
    
    return 0;
}