// 同化棋（Ataxx）蒙特卡洛树搜索实现
// 作者：基于蒙特卡洛树搜索的AI策略

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cmath>

// 蒙特卡洛树搜索参数
const double UCB_EXPLORE_PARAM = 2.0;     // UCB探索参数
const int TERMINAL_SCORE = 50;            // 终局分数
const int MAX_SCORE_VALUE = 1000;         // 最大分数值
const double TIME_THRESHOLD = 0.88;       // 时间阈值(秒)

using namespace std;

// 方向增量数组，前8个为相邻位置，后16个为跳跃位置
static int piece_conversion_mask[4] = {0x15, 0, 0, 0x15};
static int direction_delta[24][2] = {
    {1, 1},  {0, 1},   {-1, 1},  {-1, 0},  {-1, -1}, {0, -1}, {1, -1}, {1, 0},
    {2, 0},  {2, 1},   {2, 2},   {1, 2},   {0, 2},   {-1, 2}, {-2, 2}, {-2, 1},
    {-2, 0}, {-2, -1}, {-2,-2}, {-1,-2}, {0,-2},   {1,-2},  {2,-2},  {2,-1}
};

// 时间控制
static clock_t timeout_clock = clock() + TIME_THRESHOLD * CLOCKS_PER_SEC;

// 移动操作结构体
struct MoveAction {
    int fromX, fromY, toX, toY;
};

// 重载输入输出操作符，用于读取和输出移动
istream &operator>>(istream &is, MoveAction &move) {
    return is >> move.fromX >> move.fromY >> move.toX >> move.toY;
}

ostream &operator<<(ostream &os, MoveAction &move) {
    return os << move.fromX << ' ' << move.fromY << ' ' << move.toX << ' ' << move.toY;
}

// 棋盘类 - 处理游戏状态和规则
class AtaxxBoard {
protected:
    int boardState[7];     // 使用位编码存储棋盘状态
    int playerColor;       // 当前玩家颜色 (2=黑, 3=白)

public:
    AtaxxBoard() {}
    
    // 从输入流初始化棋盘
    AtaxxBoard(istream &is) {
        memset(boardState, 0, 7 * sizeof(int));
        // 初始化棋盘，放置初始棋子
        boardState[0] = 0xC008;  // |黑|白|
        boardState[6] = 0x800C;  // |白|黑|
        playerColor = 2;         // 黑方先行
        
        int turnCount;
        is >> turnCount;
        MoveAction tempMove = {-1, -1, -1, -1};
        
        // 恢复游戏状态到当前回合
        for (int i = 1; i != turnCount; ++i) {
            is >> tempMove;
            if (tempMove.toX >= 0) makeMove(tempMove);
            is >> tempMove;
            makeMove(tempMove);
        }
        
        is >> tempMove;
        if (tempMove.toX >= 0) makeMove(tempMove);
    }

    // 拷贝构造函数
    AtaxxBoard(const AtaxxBoard &other) {
        memcpy(boardState, other.boardState, 7 * sizeof(int));
        playerColor = other.playerColor;
    }

    // 判断坐标是否在棋盘内
    inline bool isValidPosition(int x, int y) const {
        return x >= 0 && x <= 6 && y >= 0 && y <= 6;
    }

    // 获取指定位置的棋子类型
    inline int getPieceAt(int x, int y) const {
        return (boardState[x] >> ((y + 1) << 1)) & 3;
    }

    // 判断是否是当前玩家的棋子
    inline bool isCurrentPlayerPiece(int x, int y) const {
        return ((boardState[x] >> ((y + 1) << 1)) & 3) == playerColor;
    }

    // 判断位置是否为空
    inline bool isEmptyPosition(int x, int y) const {
        return !((boardState[x] >> ((y + 1) << 1)) & 2);
    }

    // 执行移动操作
    void makeMove(const MoveAction &move) {
        // 处理跳跃移动 - 如果是跳跃则移除原位置棋子
        if (abs(move.toX - move.fromX) == 2 || abs(move.toY - move.fromY) == 2)
            boardState[move.fromX] ^= (2 << ((move.fromY + 1) << 1));
        
        // 更新目标位置为当前玩家的棋子
        boardState[move.toX] &= ~(3 << ((move.toY + 1) << 1));
        boardState[move.toX] |= (playerColor << ((move.toY + 1) << 1));
        
        // 同化周围的对方棋子
        for (int i = -1; i <= 1; ++i) {
            if (move.toX + i >= 0 && move.toX + i <= 6) {
                boardState[move.toX + i] &= ~(piece_conversion_mask[0] << (move.toY << 1));
                boardState[move.toX + i] |= (piece_conversion_mask[playerColor] << (move.toY << 1));
            }
        }
        
        // 切换玩家
        playerColor = 5 - playerColor;  // 2->3, 3->2
    }

    // 评估当前局面分数 (正值对当前玩家有利)
    int evaluatePosition() const {
        int pieceCount[4] = {0};
        
        // 统计棋盘上各颜色棋子数量
        for (int i = 0; i < 7; ++i) {
            int row = boardState[i];
            for (int j = 0; j < 7; ++j)
                ++pieceCount[(row >> ((j + 1) << 1)) & 3];
        }
        
        // 返回当前玩家与对手棋子数量之差
        return pieceCount[playerColor] - pieceCount[5 - playerColor];
    }

    // 判断游戏是否结束，返回终局分数
    int checkGameOver() const {
        int pieceCount[4] = {0};
        
        // 统计棋盘上各颜色棋子数量
        for (int i = 0; i < 7; ++i) {
            int row = boardState[i];
            for (int j = 0; j < 7; ++j)
                ++pieceCount[(row >> ((j + 1) << 1)) & 3];
        }
        
        // 如果当前玩家棋子数量不超过一半，则判负
        return pieceCount[playerColor] <= 24 ? -TERMINAL_SCORE : TERMINAL_SCORE;
    }

    // 生成所有合法移动
    MoveAction* generateLegalMoves(int &moveCount) {
        moveCount = 0;
        MoveAction* legalMoves = new MoveAction[600];
        
        // 遍历棋盘寻找合法移动
        for (int x0 = 0; x0 < 7; ++x0) {
            for (int y0 = 0; y0 < 7; ++y0) {
                if (isEmptyPosition(x0, y0)) {
                    for (int dir = 0; dir < 24; ++dir) {
                        int x1 = x0 + direction_delta[dir][0];
                        int y1 = y0 + direction_delta[dir][1];
                        
                        if (!isValidPosition(x1, y1)) continue;
                        if (!isCurrentPlayerPiece(x1, y1)) continue;
                        
                        MoveAction validMove = {x1, y1, x0, y0};
                        legalMoves[moveCount++] = validMove;
                        
                        // 优化：找到短距离移动后，跳过更远的移动检查
                        if (dir < 8) dir = 8;
                    }
                }
            }
        }
        
        return legalMoves;
    }
};

// 蒙特卡洛树搜索节点类
class MCTSNode : public AtaxxBoard {
private:
    MoveAction move_from_parent;   // 从父节点到达此节点的移动
    MCTSNode *children = NULL;     // 子节点数组
    int child_count = 0;           // 子节点数量
    int terminal_score = 0;        // 终局得分缓存
    int visit_count = 0;           // 访问次数
    int total_score = 0;           // 总得分

public:
    MCTSNode() {}
    MCTSNode(istream &is) : AtaxxBoard(is) {}
    MCTSNode(const AtaxxBoard &board) : AtaxxBoard(board) {}

    // 展开节点，创建所有可能的子节点
    void expandChildren(MoveAction *legalMoves) {
        children = new MCTSNode[child_count];
        
        for (int i = 0; i < child_count; ++i) {
            children[i] = AtaxxBoard(*this);
            children[i].makeMove(legalMoves[i]);
            children[i].move_from_parent = legalMoves[i];
        }
        
        delete[] legalMoves;
    }

    // 获取终局评分（如果已计算则返回缓存值）
    int getTerminalScore() {
        if (terminal_score != 0) return terminal_score;
        return terminal_score = checkGameOver();
    }

    // 计算UCB值 (Upper Confidence Bound)
    double calculateUCB(int parent_visits, double exploration_rate) {
        // 未访问过的节点给予随机优先级，确保每个节点都会被探索
        if (visit_count == 0) 
            return -MAX_SCORE_VALUE + rand() / double(RAND_MAX);
        
        // UCB公式：平均得分 - 探索系数 * sqrt(ln(父节点访问次数) / 当前节点访问次数)
        return double(total_score) / visit_count - 
               exploration_rate * sqrt(log(parent_visits) / visit_count);
    }

    // 选择最佳子节点
    MCTSNode* selectBestChild(bool is_final_selection) {
        // 最终选择时不考虑探索因子
        double exploration_rate = is_final_selection ? 0 : UCB_EXPLORE_PARAM;
        ++visit_count;
        
        int best_index = 0;
        double best_value = MAX_SCORE_VALUE;
        
        // 遍历所有子节点，选择UCB值最小的（对手最不利的）
        for (int i = 0; i < child_count; ++i) {
            double node_value = children[i].calculateUCB(visit_count, exploration_rate);
            if (node_value < best_value) {
                best_value = node_value;
                best_index = i;
            }
        }
        
        return children + best_index;
    }

    // 更新节点价值
    int updateNodeValue(int value) {
        total_score += value;
        // 返回负值，因为是零和游戏
        return -value;
    }

    // MCTS的核心过程：选择、扩展、模拟、回溯
    int runTreeSearch() {
        // 如果是叶节点，需要展开
        if (children == NULL) {
            expandChildren(generateLegalMoves(child_count));
            // 如果没有合法移动，返回终局评分
            if (child_count == 0) return updateNodeValue(getTerminalScore());
            // 否则返回基于局面评估的分数
            return updateNodeValue(evaluatePosition());
        } 
        // 如果没有子节点，返回终局评分
        else if (child_count == 0) {
            return updateNodeValue(getTerminalScore());
        } 
        // 如果有子节点，选择一个并继续搜索
        else {
            MCTSNode *selected_child = selectBestChild(false);
            return updateNodeValue(selected_child->runTreeSearch());
        }
    }

    // 输出最佳移动
    void getBestMove() {
        cout << selectBestChild(true)->move_from_parent;
    }
};

// 主函数
int main() {
    // 优化输入输出性能
    istream::sync_with_stdio(false);
    srand(time(NULL));
    
    // 从标准输入初始化游戏状态
    MCTSNode game_root(cin);
    
    // 记录MCTS开始时间
    clock_t search_start_time = clock();
    double search_time_limit = TIME_THRESHOLD * CLOCKS_PER_SEC;
    
    // 在时间限制内进行尽可能多的MCTS迭代
    while (clock() - search_start_time < search_time_limit) {
        game_root.runTreeSearch();
    }
    
    // 选择并输出最佳移动
    game_root.getBestMove();
    return 0;
}