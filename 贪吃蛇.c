#include<stdio.h>
#include<time.h>
#include<windows.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>

#define U 1
#define D 2
#define L 3 
#define R 4
#define USER_FILE "user.txt"
#define LOG_FILE  "log.txt"

// 蛇节点结构体
typedef struct SNAKE {
    int x;
    int y;
    struct SNAKE* next;
} snake;

// 全局变量
int score = 0, add = 10;
int status, sleeptime = 200;
snake* head, * food;
int endgamestatus = 0;
char currentUser[20] = {0};
int currentUserId = 0;
time_t gameStartTime;

// 函数声明
void Pos(int x, int y);
void HideCursor();
void creatMap();
void initsnake();
int biteSelf();
void createFood();
int checkWall();
void snakeMove();
void pauseGame();
void gameCircle();
void welcomeToGame();
void endGame();
void gameStart();
int isUserExists(const char* username);
int registerUser();
int loginUser();
void saveGameLog(int userId, const char* username, time_t startTime, int score);
void showGameLog();
void printUserInfo();

// 光标定位
void Pos(int x, int y) {
    COORD pos;
    HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(hOutput, pos);
}

// 隐藏控制台光标（优化界面）
void HideCursor() {
    CONSOLE_CURSOR_INFO cursor;
    cursor.dwSize = 1;
    cursor.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);
}

// 判断用户是否存在
int isUserExists(const char* username) {
    FILE* fp = fopen(USER_FILE, "r");
    if (!fp) return 0;
    int id;
    char name[20], pwd[20];
    while (fscanf(fp, "%d %s %s", &id, name, pwd) != EOF) {
        if (strcmp(name, username) == 0) {
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

// 用户注册
int registerUser() {
    char username[20], pwd[20], repwd[20];
    system("cls");
    Pos(30, 10); printf("=== 新用户注册 ===");
    Pos(30, 12); printf("用户名：");
    scanf("%s", username);
    if (isUserExists(username)) {
        Pos(30, 14); printf("用户名已存在！");
        Sleep(1500);
        return 0;
    }
    Pos(30, 13); printf("密码：");
    scanf("%s", pwd);
    Pos(30, 14); printf("确认密码：");
    scanf("%s", repwd);
    if (strcmp(pwd, repwd) != 0) {
        Pos(30, 15); printf("两次密码不一致！");
        Sleep(1500);
        return 0;
    }

    FILE* fp = fopen(USER_FILE, "r");
    int maxId = 0, id, fid;
    char fn[20], fpw[20];
    if (fp) {
        while (fscanf(fp, "%d %s %s", &fid, fn, fpw) != EOF)
            if (fid > maxId) maxId = fid;
        fclose(fp);
    }
    id = maxId + 1;

    fp = fopen(USER_FILE, "a");
    if (!fp) return 0;
    fprintf(fp, "%d %s %s\n", id, username, pwd);
    fclose(fp);

    strcpy(currentUser, username);
    currentUserId = id;
    Pos(30, 15); printf("注册成功！");
    Sleep(1500);
    return 1;
}

// 用户登录
int loginUser() {
    char username[20], pwd[20];
    system("cls");
    Pos(30, 10); printf("=== 用户登录 ===");
    Pos(30, 12); printf("用户名：");
    scanf("%s", username);
    Pos(30, 13); printf("密码：");
    scanf("%s", pwd);

    FILE* fp = fopen(USER_FILE, "r");
    if (!fp) {
        Pos(30, 15); printf("无用户，请先注册！");
        Sleep(1500);
        return 0;
    }
    int id;
    char un[20], pw[20];
    while (fscanf(fp, "%d %s %s", &id, un, pw) != EOF) {
        if (strcmp(un, username) == 0 && strcmp(pw, pwd) == 0) {
            fclose(fp);
            strcpy(currentUser, username);
            currentUserId = id;
            Pos(30, 15); printf("登录成功！");
            Sleep(1500);
            return 1;
        }
    }
    fclose(fp);
    Pos(30, 15); printf("账号或密码错误！");
    Sleep(1500);
    return 0;
}

// 保存游戏日志
void saveGameLog(int userId, const char* username, time_t startTime, int score) {
    time_t end = time(NULL);
    long dur = difftime(end, startTime);
    FILE* fp = fopen(LOG_FILE, "a");
    if (!fp) return;
    fprintf(fp, "%d %s %s%ld %d\n", userId, username, ctime(&startTime), dur, score);
    fclose(fp);
}

// 查看游戏日志
void showGameLog() {
    system("cls");
    Pos(10, 3); printf("==================== 游戏日志 ====================");
    Pos(10, 5); printf("用户ID\t用户名\t\t开始时间\t\t时长(s)\t得分");
    Pos(10, 6); printf("--------------------------------------------------");
    FILE* fp = fopen(LOG_FILE, "r");
    if (!fp) {
        Pos(10, 8); printf("暂无游戏日志");
        _getch();
        return;
    }
    int id, s, line = 8;
    char name[20], tim[35];
    long dur;
    while (fscanf(fp, "%d %s %[^\n] %ld %d", &id, name, tim, &dur, &s) != EOF) {
        Pos(10, line++);
        printf("%d\t%s\t\t%s\t%ld\t\t%d", id, name, tim, dur, s);
        if (line > 22) {
            Pos(10, 24); printf("按任意键继续...");
            _getch();
            system("cls");
            showGameLog();
            return;
        }
    }
    fclose(fp);
    Pos(10, line + 2); printf("按任意键返回游戏");
    _getch();
}

// 显示当前用户信息
void printUserInfo() {
    Pos(62, 4); printf("玩家：%s", currentUser);
    Pos(62, 6); printf("得分：%d", score);
    Pos(62, 8); printf("F5查看日志");
    Pos(62, 10); printf("空格=暂停 ESC=退出");
}

// 绘制游戏地图边界
void creatMap() {
    int i;
    // 上下边框
    for (i = 0; i <= 56; i += 2) {
        Pos(i, 0); printf("■");
        Pos(i, 26); printf("■");
    }
    // 左右边框
    for (i = 1; i < 26; i++) {
        Pos(0, i); printf("■");
        Pos(56, i); printf("■");
    }
    printUserInfo();
}

// 初始化蛇身（4节，向右）
void initsnake() {
    snake* tail = (snake*)malloc(sizeof(snake));
    tail->x = 24; tail->y = 5; tail->next = NULL;
    for (int i = 1; i <= 4; i++) {
        head = (snake*)malloc(sizeof(snake));
        head->x = 24 + 2*i;
        head->y = 5;
        head->next = tail;
        tail = head;
    }
    snake* p = head;
    while (p) {
        Pos(p->x, p->y); printf("■");
        p = p->next;
    }
}

// 检测是否撞到自己
int biteSelf() {
    snake* p = head->next;
    while (p) {
        if (p->x == head->x && p->y == head->y)
            return 1;
        p = p->next;
    }
    return 0;
}

// 生成食物（不与蛇身重叠，x偶数）
void createFood() {
    food = (snake*)malloc(sizeof(snake));
    srand((unsigned)time(NULL));
    int ok = 0;
    while (!ok) {
        food->x = rand() % 26 * 2 + 2;
        food->y = rand() % 24 + 1;
        ok = 1;
        snake* p = head;
        while (p) {
            if (p->x == food->x && p->y == food->y) {
                ok = 0; break;
            }
            p = p->next;
        }
    }
    Pos(food->x, food->y);
    printf("●");
}

// 边界碰撞检测
int checkWall() {
    if (head->x <= 0 || head->x >= 56 || head->y <= 0 || head->y >= 26)
        return 1;
    return 0;
}

// 蛇移动（重构，消除重复代码）
void snakeMove() {
    snake* newHead = (snake*)malloc(sizeof(snake));
    // 根据方向生成新蛇头
    switch (status) {
        case U: newHead->x = head->x; newHead->y = head->y - 1; break;
        case D: newHead->x = head->x; newHead->y = head->y + 1; break;
        case L: newHead->x = head->x - 2; newHead->y = head->y; break;
        case R: newHead->x = head->x + 2; newHead->y = head->y; break;
    }

    // 撞墙
    if (checkWall()) {
        endgamestatus = 1;
        endGame();
    }
    // 吃到食物：不删尾巴，加分
    if (newHead->x == food->x && newHead->y == food->y) {
        newHead->next = head;
        head = newHead;
        score += add;
        free(food);
        createFood();
    }
    // 没吃到食物：删除尾巴
    else {
        newHead->next = head;
        head = newHead;
        snake* p = head;
        while (p->next->next != NULL) p = p->next;
        Pos(p->next->x, p->next->y); printf("  ");
        free(p->next);
        p->next = NULL;
    }
    // 绘制蛇
    snake* p = head;
    while (p) {
        Pos(p->x, p->y); printf("■");
        p = p->next;
    }
    // 咬到自己
    if (biteSelf()) {
        endgamestatus = 2;
        endGame();
    }
}

// 游戏暂停
void pauseGame() {
    Pos(62, 12); printf("已暂停，按空格继续");
    while (!GetAsyncKeyState(VK_SPACE)) Sleep(50);
    Pos(62, 12); printf("                ");
}

// 游戏主循环（键盘控制、得分、速度控制）
void gameCircle() {
    status = R;
    while (1) {
        // 实时刷新得分
        Pos(62, 6); printf("得分：%d", score);
        // 键盘监听
        if (GetAsyncKeyState(VK_UP) && status != D) status = U;
        else if (GetAsyncKeyState(VK_DOWN) && status != U) status = D;
        else if (GetAsyncKeyState(VK_LEFT) && status != R) status = L;
        else if (GetAsyncKeyState(VK_RIGHT) && status != L) status = R;
        else if (GetAsyncKeyState(VK_SPACE)) pauseGame();
        else if (GetAsyncKeyState(VK_ESCAPE)) {
            endgamestatus = 3;
            break;
        }
        else if (GetAsyncKeyState(VK_F1)) {
            if (sleeptime >= 50) sleeptime -= 30, add += 2;
        }
        else if (GetAsyncKeyState(VK_F2)) {
            if (sleeptime < 350) sleeptime += 30, add -= 2;
            if (add < 1) add = 1;
        }
        else if (GetAsyncKeyState(VK_F5)) {
            showGameLog();
            // 日志返回后重建游戏界面
            system("cls");
            creatMap();
            initsnake();
            createFood();
        }

        Sleep(sleeptime);
        snakeMove();
    }
}

// 欢迎界面
void welcomeToGame() {
    system("cls");
    Pos(35, 10); printf("===== 贪吃蛇游戏 =====");
    Pos(25, 12); printf("方向键控制移动 | 空格暂停 | ESC退出");
    Pos(25, 13); printf("F1加速 | F2减速 | F5查看游戏日志");
    Pos(30, 15); system("pause");
}

// 游戏结束处理
void endGame() {
    saveGameLog(currentUserId, currentUser, gameStartTime, score);
    system("cls");
    Pos(25, 10);
    if (endgamestatus == 1) printf("游戏结束：撞墙！");
    else if (endgamestatus == 2) printf("游戏结束：咬到自己！");
    else printf("主动退出游戏");
    Pos(25, 12); printf("最终得分：%d", score);
    Pos(25, 14); printf("按任意键退出程序");
    _getch();
    exit(0);
}

// 游戏初始化入口
void gameStart() {
    HideCursor();
    system("mode con cols=100 lines=30");
    int op;
    while (1) {
        system("cls");
        Pos(30, 10); printf("===== 贪吃蛇游戏 =====");
        Pos(30, 12); printf("1.登录游戏");
        Pos(30, 13); printf("2.注册账号");
        Pos(30, 15); printf("请选择：");
        scanf("%d", &op);
        if (op == 1) { if(loginUser()) break; }
        else if (op == 2) { if(registerUser()) break; }
        else { Pos(30, 16); printf("输入错误！"); Sleep(1000); }
    }
    welcomeToGame();
    creatMap();
    initsnake();
    createFood();
    gameStartTime = time(NULL);
}

int main() {
    gameStart();
    gameCircle();
    endGame();
    return 0;
}