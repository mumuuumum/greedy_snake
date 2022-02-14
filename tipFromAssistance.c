//
// Created by 张淞钦 on 2021/12/14.
//

#include <windows.h> // 必须
#include <conio.h> // 必须
#include <stdio.h>
HANDLE consoleHandle; // 操作控制台（也就是那个黑框框）需要的一个变量
int w, h; // 高度，宽度，对应 y 和 x
// 在屏幕上 y, x 的位置打印一个 ch
void mvaddch(int y, int x, char ch) {
    COORD co = (COORD){ .X = x, .Y = y };
    SetConsoleCursorPosition(consoleHandle, co); // 设置你的光标位置
    putchar(ch); // 在这里打印一个字符
}
// 类似地，你可以以这种方式定义一些在某个位置打印字符串的函数
int main() {
    consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE); // 初始化这个操作器
    CONSOLE_SCREEN_BUFFER_INFO csbi; // 屏幕的信息
    GetConsoleScreenBufferInfo(consoleHandle, &csbi); // 获取屏幕信息
    w = csbi.dwSize.X;
    h = csbi.dwSize.Y; // 得到宽度高度
// 游戏里面，如果一直有输入的光标，就有点不好看。我们可以让它不显示
    CONSOLE_CURSOR_INFO cci; // 光标信息
    cci.dwSize = 100;
    cci.bVisible = FALSE; // 不可见
    SetConsoleCursorInfo(consoleHandle, &cci); // 将光标特性应用到控制台
// 到这里，闪烁的光标就消失了。
    while (1) {
        while (kbhit() != 0) { // 如果它检测到有键盘敲击，返回非零。没有则返回 0
// 有键盘敲击，我们获取是哪一个键
            char c = getch();
            mvaddch(h / 2, w / 2, c); // 显示在屏幕中心
            Sleep(1000); // 程序暂停 1000 毫秒
        }
    }
    // 游戏结束，我们要恢复光标显示
    cci.bVisible = TRUE; // 可见
    SetConsoleCursorInfo(consoleHandle, &cci); // 重新设置
}