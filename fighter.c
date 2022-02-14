#include <windows.h>
#include <conio.h>
#include <time.h>
#include <stdio.h>
#include <setjmp.h>
//
#define VEBORDER '|'                 //竖直边界 Ve即vetical即垂直
#define LEBORDER '-'                 //水平边界 Le即level即水平
#define WIDTH 120                    //框的宽
#define HIGHT 29                     //框的高
#define INITIALSNAKELEN 3            //蛇最初的长度
#define MAXSNAKELEN 50               //蛇最大的长度
#define HEAD '@'                     //蛇头
#define BODY '*'                     //蛇身
#define FOOD '$'                     //食物
#define PROP '#'                     //穿墙一次的特殊食物 生成机制：rand()对6取余再取符号，最大为0，变量propneeded随蛇吃普通食物递加至一生成
#define EASYLIM 70                   //简单模式的极限速度
#define MEDIUMLIM 62                 //中等模式的极限速度
#define DIFFICULTLIM 50              //困难模式的极限速度
//
int difficulty;                                                           //1表示简单难度，2中等，3困难
int lefood;                                                               //食物横坐标
int vefood;                                                               //食物纵坐标
int leprop;                                                               //特殊食物横坐标
int veprop;                                                               //特殊食物纵坐标
int ConLen;                                                               //蛇当前的长度
int LeSnake [MAXSNAKELEN];                                                //蛇头与蛇身的横坐标
int VeSnake [MAXSNAKELEN];                                                //蛇头与蛇身的纵坐标
char wsad;                                                                //记录用户敲击的键
char lswsad;                                                              //记录用户上次敲击的键
char tmp;                                                                 //配合wsad、lswsad做恢复工作
char tmpt;                                                                //配合wsad、lswsad、tmp做恢复工作
int foodneeded;                                                           //1表示被需要
int propneeded;                                                           //1表示被需要
int score;                                                                //得分
int status;                                                               //蛇的状态（是否可穿墙）
int speed;                                                                //游戏速度
int laFiLe;                                                               //上次蛇尾的横坐标
int laFiVe;
int laHeLe;                                                               //上次蛇头的横坐标
int laHeVe;
jmp_buf start;                                                            //跨函数跳转
HANDLE consoleHandle;
//
void gotoXY(int x, int y);                                                    //光标移动
void Title ();
void CreatMap ();                                                             //初始制作地图边缘
void FunctionKey ();                                                         //右上角功能提示的打印
void DramaticScore ();                                                        //更新分数
void DramaticStatus ();                                                       //更新蛇的状态
void WelcomeInterface ();                                                     //欢迎界面
void WelcomeChoice ();                                                        //欢迎界面的选择
void Details ();                                                              //游戏详细规则（内含实现进入规则界面和退出）
void CreatFood ();                                                            //制作食物
void EatFood ();                                                              //蛇吃食物
int CheckFood (int le,int ve);                                                //检查食物出现的位置是否合理（食物出现在蛇身、头处）
void CreatProp ();                                                            //道具
int CheckProp (int le,int ve);
void EatProp ();
void BeginSnake ();                                                           //初始制作蛇
void MoveSnake ();                                                            //蛇的移动
void Speeder ();                                                              //调整速度的函数
int Result ();                                                                //当前一局游戏是否失败的判断 1表示失败
void FailureInterFace ();                                                    //失败界面
void FailureChoice();                                                        //失败的选择
void ReturnToMean ();                                                         //回到菜单
void Quit ();
void Pause ();
void Clean ();                                                               //清空过多的按键
//
int main () {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cci;
    GetConsoleCursorInfo(hOut, &cci);
    cci.bVisible = FALSE;
    SetConsoleCursorInfo(hOut, &cci);
    setjmp(start);
    //
    wsad = 'a';                                                               //记录用户敲击的键
    lefood = 0;                                                               //食物横坐标
    vefood = 0;
    leprop = 0;                                                               //食物横坐标
    veprop = 0;
    ConLen = INITIALSNAKELEN;                                                 //蛇当前的长度
    memset(LeSnake,0, sizeof(int) * MAXSNAKELEN);                   //坐标还原为0
    memset(VeSnake,0, sizeof(int) * MAXSNAKELEN);
    LeSnake [0] = (WIDTH - 1) / 3;                                            //蛇头与蛇身的横坐标
    VeSnake [0] = (HIGHT - 1) / 3;
    lswsad = '0';                                                             //记录用户上次敲击的键
    foodneeded = 1;                                                           //1表示被需要
    propneeded = -(rand() % 6);
    score = 0;                                                                //得分
    status = 0;
    tmp = 0;
    tmpt = 0;
    laFiLe = 0;                                                               //上一个状态蛇尾横坐标
    laFiVe = 0;
    laHeLe = 0;                                                               //上一个状态蛇头横坐标
    laHeVe = 0;
    //
    system("cls");
    Title();
    FunctionKey();
    CreatMap ();
    DramaticScore();
    DramaticStatus();
    WelcomeInterface();
    WelcomeChoice ();
    system("cls");
    Title();
    FunctionKey();
    CreatMap ();
    DramaticScore();
    DramaticStatus();
    BeginSnake ();
    while (1) {
        if (foodneeded) {
            CreatFood();
            foodneeded = 0;
            DramaticScore();
        }
        if (propneeded == 1) {
            CreatProp();
            if (difficulty == 1) {                 //不同难度产生道具的频率不一样
                propneeded = -(propneeded % 7);
            } else if (difficulty == 2) {
                propneeded = -(propneeded % 13);
            } else {
                propneeded = -(propneeded % 16);
            }
            DramaticScore();
        }
        DramaticStatus();
        MoveSnake();
        EatFood();
        EatProp();
        Clean();
        Sleep(speed);
        if (Result()) {
            system("cls");
            Title();
            FunctionKey();
            CreatMap ();
            DramaticScore();
            FailureInterFace();
            FailureChoice();
        }

    }

    return 0;
}
//
void gotoXY(int x, int y) {                                                  //移动光标
    COORD coord = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
//
void Title () {                                                               //打印标题
    gotoXY((WIDTH - 1) / 2 - 6,0);
    printf("Snake");
}
//
void CreatMap () {                                                           //制造初始地图框架
//第0行带边角
    gotoXY(0, 2);
    putchar('+');
    for (int i = 1; i < WIDTH - 1; i++) {
        gotoXY(i, 2);
        putchar(LEBORDER);
    }
    gotoXY(WIDTH - 1,2);
    putchar('+');
//最后一行带边角
    gotoXY(0, HIGHT + 1);
    putchar('+');
    for (int i = 1; i < WIDTH - 1 ; i++) {
        gotoXY(i, HIGHT + 1);
        putchar(LEBORDER);
    }
    gotoXY(WIDTH - 1, HIGHT + 1);
    putchar('+');
//第0列无边角
    for (int i = 3; i < HIGHT + 1; i++) {
        gotoXY(0, i);
        putchar(VEBORDER);
    }
//最后一列无边角
    for (int i = 3; i < HIGHT + 1; i++) {
        gotoXY(WIDTH - 1, i);
        putchar(VEBORDER);
    }
}
//
void FunctionKey () {                                                          //打印右上角功能提示
    gotoXY(WIDTH - 31,1);
    printf("return to menu:r pause:p quit:q");
}
//
void DramaticScore () {                                                      //左上角打印分数
    gotoXY(0,1);
    printf("Score:%d",score);
}
//
void DramaticStatus () {                                                      //打印状态
    gotoXY(9,1);
    if (status == 1) {
        printf("Status:Penetrable");
    } else {
        printf("Status:Impenetrable");
    }
}
//
void WelcomeInterface () {
    gotoXY((WIDTH - 1) / 3,(HIGHT - 1) / 2 - 5);
    printf("Welcome to snake game !");
    gotoXY((WIDTH - 1) / 3,(HIGHT - 1) / 2 - 4);
    printf("Make sure your keyboard is in lowercase !");
    gotoXY((WIDTH - 1) / 3,(HIGHT - 1) / 2 - 3);
    printf("Press 't' for game rules.");
    gotoXY((WIDTH - 1) / 3,(HIGHT - 1) / 2 - 2);
    printf("You have three levels.");
    gotoXY((WIDTH - 1) / 3,(HIGHT - 1) / 2 - 1);
    printf("'e' means easy.");
    gotoXY((WIDTH - 1) / 3,(HIGHT - 1) / 2);
    printf("'m' means medium.");
    gotoXY((WIDTH - 1) / 3,(HIGHT - 1) / 2 + 1);
    printf("'d' means difficult");
    gotoXY((WIDTH - 1) / 3,(HIGHT - 1) / 2 + 2);
    printf("Please start the game with pressing the lowercase initial of difficulty.");
    gotoXY((WIDTH - 1) / 3,(HIGHT - 1) / 2 + 3);
    printf("If you didn't press according to our rules,the game would't start.");
    gotoXY((WIDTH - 1) / 3,(HIGHT - 1) / 2 + 4);
    printf("Please pay attention to words under title,which are tips for you.");
}
//
void WelcomeChoice () {
    char choose = 0;
    while (choose != 'e' && choose != 'm' && choose != 'd' && choose != 't') {
        choose = getch();
    }
    switch (choose) {
        case 'e':
            difficulty = 1;
            speed = 150;
            break;
        case 'm':
            difficulty = 2;
            speed = 130;
            break;
        case 'd':
            difficulty = 3;
            speed = 100;
            break;
        case 't':
            system("cls");
            Title();
            FunctionKey();
            CreatMap ();
            DramaticScore();
            DramaticStatus();
            Details();
            system("cls");
            longjmp(start,1);
    }
}
//
void Details () {
    gotoXY((WIDTH - 1) / 3 - 2,(HIGHT - 1) / 2 - 4);
    printf("Please use lowercase !");
    gotoXY((WIDTH - 1) / 3 - 2,(HIGHT - 1) / 2 - 3);
    printf("Use 'wsad' to control direction.");
    gotoXY((WIDTH - 1) / 3 - 2,(HIGHT - 1) / 2 - 2);
    printf("'@' means snake's head.");
    gotoXY((WIDTH - 1) / 3 - 2,(HIGHT - 1) / 2 - 1);
    printf("'*' means snake's body.");
    gotoXY((WIDTH - 1) / 3 - 2,(HIGHT - 1) / 2);
    printf("'$' means snake's food,which can lengthen snake's body,and make you gain a score.");
    gotoXY((WIDTH - 1) / 3 - 2,(HIGHT - 1) / 2 + 1);
    printf("'#' means a special food,which makes snake penetrable for wall but not for body.");
    gotoXY((WIDTH - 1) / 3 - 2,(HIGHT - 1) / 2 + 2);
    printf("Besides,it is equivelent to '$'");
    gotoXY((WIDTH - 1) / 3 - 2,(HIGHT - 1) / 2 + 3);
    printf("When snake is penetrable,eating '#' doesn't give you another chance.");
    gotoXY((WIDTH - 1) / 3 - 2,(HIGHT - 1) / 2 + 4);
    printf("Press 'r' to return to menu.");
    char choose = '0';
    while (choose != 'r') {
        choose = getch();
    }
}
//
void CreatFood () {                                                            //在地图内创造普通食物
    srand(time(NULL));
    lefood = rand() % (WIDTH - 1);
    vefood = rand() % (HIGHT - 1) + 2;
    while (CheckFood(lefood,vefood)) {     //CheckFood检查食物位置是否合理
        lefood = rand() % (WIDTH - 1);
        vefood = rand() % (HIGHT - 1) + 2;
    }

    gotoXY(lefood,vefood);
    putchar(FOOD);
}
//
void EatFood () {                                                              //蛇吃普通食物
    if (LeSnake [0] == lefood && VeSnake [0] == vefood) {
        propneeded++;                                                          //逼近生成特殊食物
        if (propneeded != 1) {
            foodneeded = 1;
        }
        score++;
        Speeder();
        if (ConLen != MAXSNAKELEN) {
            ConLen++;
            LeSnake [ConLen - 1] = laFiLe;
            VeSnake [ConLen - 1] = laFiVe;
            gotoXY(LeSnake [ConLen - 1],VeSnake [ConLen - 1]);
            putchar(BODY);
        }
    }
}
//
int CheckFood (int le,int ve) {
    for (int i = 0;i < ConLen;i++) {
        if (le == LeSnake [i] && ve == VeSnake [i]) {
            return 1;
        }
    }
    if (le == WIDTH - 1 || ve == HIGHT + 2 || le == 0 || ve == 2) {
        return 1;
    }
    return 0;
}
//
void CreatProp () {
    leprop = rand() % (WIDTH - 1);
    veprop = rand() % (HIGHT - 1) + 2;
    while (CheckProp(leprop,veprop)) {
        leprop = rand() % (WIDTH - 1);
        veprop = rand() % (HIGHT - 1) + 2;
    }

    gotoXY(leprop,veprop);
    putchar(PROP);
}
//
void EatProp () {
    if (LeSnake [0] == leprop && VeSnake [0] == veprop) {
        status = 1;
        foodneeded = 1;                                                        //吃了特殊食物就必须产生普通食物了
        score++;
        Speeder();
        if (ConLen != MAXSNAKELEN) {
            ConLen++;
            LeSnake [ConLen - 1] = laFiLe;
            VeSnake [ConLen - 1] = laFiVe;
            gotoXY(LeSnake [ConLen - 1],VeSnake [ConLen - 1]);
            putchar(BODY);
        }
    }
}
//
int CheckProp (int le,int ve) {
    for (int i = 0;i < ConLen;i++) {
        if (le == LeSnake [i] && ve == VeSnake [i]) {
            return 1;
        }
    }
    if (le == WIDTH - 1 || ve == HIGHT + 2 || le == 0 || ve == 2) {
        return 1;
    }
    return 0;
}
//
void BeginSnake () {
    gotoXY(LeSnake [0],VeSnake [0]);
    putchar(HEAD);
    for (int i = 1;i < INITIALSNAKELEN;i++) {
        LeSnake [i] = LeSnake [i - 1] + 1;
        VeSnake [i] = VeSnake [i - 1];
        gotoXY(LeSnake [i],VeSnake [i]);
        putchar(BODY);
    }
}
//
void MoveSnake () {
    if (kbhit()) {
        tmpt = tmp;
        tmp = lswsad;
        lswsad = wsad;
        wsad = getch();
    }
    //备份
    laFiLe = LeSnake [ConLen - 1];
    laFiVe = VeSnake [ConLen - 1];
    //备份
    laHeLe = LeSnake [0];
    laHeVe = VeSnake [0];
    //头部的情况：
    back://蛇在我们无命令时是按照最近的合理命令执行的，一旦发生暂停等事项，我们将在暂停等函数内部恢复最近合理的指令，如果游戏继续，重新back判断移动
    switch (wsad) {
        case 'r':
            ReturnToMean();
            goto back;
        case 'q':
            Quit();
            goto back;
        case 'p':
            Pause();
            goto back;
        case 'w':
            if (lswsad != 's') {                                                        //蛇不会180°掉头
                VeSnake [0]--;
            } else {
                wsad = lswsad;
                lswsad = tmp;
                VeSnake [0]++;
            }
            break;
        case 's':
            if (lswsad != 'w') {
                VeSnake [0]++;
            } else {
                wsad = lswsad;
                lswsad = tmp;
                VeSnake [0]--;
            }
            break;
        case 'a':
            if (lswsad != 'd') {
                LeSnake [0]--;
            } else {
                wsad = lswsad;
                lswsad = tmp;
                LeSnake [0]++;
            }
            break;
        case 'd':
            if (lswsad != 'a') {
                LeSnake [0]++;
            } else {
                wsad = lswsad;
                lswsad = tmp;
                LeSnake [0]--;
            }
            break;
        default:                                                          //无效输入
            wsad = lswsad;
            lswsad = tmp;
            tmp = tmpt;
            goto back;
    }
    if (status == 1) {                                                           //可穿墙情况下，头部接触墙时的穿墙
        if (LeSnake [0] == 0) {
            LeSnake [0] = WIDTH - 2;
            status = 0;
        } else if (LeSnake [0] == WIDTH - 1){
            LeSnake [0] = 1;
            status = 0;
        } else if (VeSnake [0] == 2) {
            VeSnake [0] = HIGHT;
            status = 0;
        } else if (VeSnake [0] == HIGHT + 1){
            VeSnake [0] = 3;
            status = 0;
        }
    }
    gotoXY(LeSnake [0],VeSnake [0]);
    putchar(HEAD);
    //移动蛇身，后者的下一个坐标是前者现在的坐标，当蛇移动后原来空出的位置用‘ ’覆盖
    gotoXY(LeSnake [ConLen - 1],VeSnake [ConLen - 1]);
    putchar(' ');
    //
    for (int i = ConLen - 1;i > 1;i--) {
        LeSnake [i] = LeSnake [i - 1];
        VeSnake [i] = VeSnake [i - 1];
        gotoXY(LeSnake [i],VeSnake [i]);
        putchar(BODY);
    }
    LeSnake [1] = laHeLe;
    VeSnake [1] = laHeVe;
    gotoXY(LeSnake [1],VeSnake [1]);
    putchar(BODY);
}
//
void Speeder () {
    if (difficulty == 1) {
        if (speed - EASYLIM > 1) {
            speed = speed * 0.98;
        }
    } else if (difficulty == 2) {
        if (speed - MEDIUMLIM > 1) {
            speed = speed * 0.97;
        }
    } else {
        if (speed - DIFFICULTLIM > 1) {
            speed = speed * 0.95;
        }
    }
}
//
int Result () {
    for (int i = 2;i < ConLen;i++) {
        if (LeSnake [0] == LeSnake [i] && VeSnake [0] == VeSnake [i]) {
            return 1;
        }
    }
    if (status == 0) {
        if (VeSnake [0] == 2 || VeSnake [0] == HIGHT + 1) {
            return 1;
        }
        if (LeSnake [0] == 0 || LeSnake [0] == WIDTH - 1) {
            return 1;
        }
    }
    return 0;
}
//
void FailureInterFace () {
    gotoXY((WIDTH - 1) / 3,(HIGHT - 1) / 2 - 3);
    printf("Game over !");
    Sleep(1000);
    gotoXY((WIDTH - 1) / 3,(HIGHT - 1) / 2 - 2);
    printf("Your score is %d !",score);
    Sleep(1000);
    gotoXY((WIDTH - 1) / 3,(HIGHT - 1) / 2 - 1);
    if (score == 0) {
        printf("You must haven't be ready,try again !");
    } else {
        printf("Good job , you achieved an excellent score !");
    }
    Sleep(1000);
    gotoXY((WIDTH - 1) / 3,(HIGHT - 1) / 2);
    printf("Now choose your behavior .");
    Sleep(1000);
    gotoXY((WIDTH - 1) / 3,(HIGHT - 1) / 2 + 1);
    printf("'r' means return to menu .");
    Sleep(1000);
    gotoXY((WIDTH - 1) / 3,(HIGHT - 1) / 2 + 2);
    printf("'q' means quit the game .");
    Sleep(1000);
    gotoXY((WIDTH - 1) / 3,(HIGHT - 1) / 2 + 3);
    printf("Please press your choice .");
}
//
void FailureChoice() {
    char choose;
    while (choose != 'r' && choose != 'q') {
        choose = getch();
    }
    if (choose == 'r') {
        longjmp(start,1);
    } else {
        exit(0);
    }
}
//
void ReturnToMean () {
    wsad = lswsad;
    lswsad = tmp;
    tmp = tmpt;
    gotoXY((WIDTH - 1) / 2 - 30,1);
    printf("Press 'r' again to menu,press any other key to continue.");
    char choice = getch();
    if (choice == 'r') {
        system("cls");
        longjmp(start,1);
    }
    gotoXY((WIDTH - 1) / 2 - 30,1);
    printf("                                                        ");
}
//
void Quit () {
    wsad = lswsad;
    lswsad = tmp;
    tmp = tmpt;
    gotoXY((WIDTH - 1) / 2 - 30,1);
    printf("Press 'q' again to leave,press any other key to continue.");
    char choice = getch();
    if (choice == 'q') {
        exit(0);
    }
    gotoXY((WIDTH - 1) / 2 - 30,1);
    printf("                                                         ");
}
//
void Pause () {
    wsad = lswsad;
    lswsad = tmp;
    tmp = tmpt;
    gotoXY((WIDTH - 1) / 2 -16,1);
    printf("Press any key to continue");
    while (getch()) {
        gotoXY((WIDTH - 1) / 2 - 16,1);
        printf("                         ");
        break;
    }
}
void Clean () {
    while (1) {
        if (!kbhit()) {
            break;
        } else {
            getch();
        }
    }
}