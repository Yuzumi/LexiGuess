//Note: Study "graphics.h" and structs

#include <iostream.h>
#include <conio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <dos.h>
#include <graphics.h>
#include <fstream.h>
#include <string.h>

#define CENTER_X 320
#define CENTER_Y 240

#define arrow_r 77
#define arrow_l 75
#define arrow_u 72
#define arrow_d 80

#define ARROW_R "\x11"
#define ARROW_L "\x10"
#define ARROW_U "\x1E"
#define ARROW_D "\x1F"


struct Time {
    int minute;
    int second;
    int milisec;
    Time()
    {
        minute = 0;
        second = 0;
        milisec = 0;
    }
} currentTime;
struct Answer {
    char *w;
    int l;
    Answer()
    {
        w = new char[6];
        l = 0;
    }
    ~Answer()
    {
        delete[] w;
    }
}ans;
struct Score {
    int s;
    Score()
    {
        s = 0;
    }
    void addScore()
    {
        s += 1;
    }
    void resetScore()
    {
        s = 0;
    }
}score;
struct Color {
    int bg_color;
    Color()
    {
        bg_color = LIGHTBLUE;
    }
}color;
struct User {
    char name[20];
}user;
struct Difficulty {
    int level;
}d;

int timerRunning = 1;

void playTone(int i, int d);
void init_border();
void arrowKeys(int &choice, int key, int size);
void highlight(char *menuOptions[], int choice, int size);
int init_menu(int &choice);
int init_subMenu(int &choice);
int exit();
void boxes();
int getRandomNumber(int min, int max);
void getWord();
int checking(char user[][6], int index);
void getWord();
int checking(char user[][6], int index);
void initDisplay();
void printScore();
void restore(int index, char answers[][6], char *currentInput);
void displayTime();
int init_difficulty();
void decrypt(char *text);
void encrypt(char *text);
void blink(int x, int y, int color, char *ch);
void addUser(int run);
void signIn(int run);
int getUser(const char *username, char *password);
void getColor();
void writeScore();

void swap(int &a, int &b)
{
    int temp = a;
    a = b;
    b = temp;
}
void swap(char *a, char *b)
{
    char *temp;
    strcpy(temp, a);
    strcpy(a, b);
    strcpy(b, temp);
}

void main()
{
    int option, submenu, c = 1;
    int gd = DETECT, gm, error;
    initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");
    getColor();
    signIn(0);
    do
    {
        option = init_menu(c);
        switch (option)
        {
            case 1:
                {
                    int sb = 1;
                    do
                    {
                        settextjustify(CENTER_TEXT, CENTER_TEXT);
                        d.level = init_subMenu(sb);
                        settextjustify(LEFT_TEXT, TOP_TEXT);
                        if (d.level > 0 && d.level < 5)
                        {
                            int sc = init_difficulty();

                            setfillstyle(SOLID_FILL, BLACK);
                            bar(140, 150, 500, 280);

                            settextjustify(CENTER_TEXT, CENTER_TEXT);
                            settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 1);
                            if (sc > 0)
                            {
                                setcolor(LIGHTGREEN);
                                outtextxy(CENTER_X, 180, "CONGRATULATIONS!");
                                outtextxy(CENTER_X, 204, "YOU SCORED:");
                                char s[100];
                                itoa(sc, s, 10);
                                outtextxy(CENTER_X, 230, s);
                            } else
                            {
                                setcolor(LIGHTRED);
                                outtextxy(CENTER_X, 180, "NICE TRY:D");
                                outtextxy(CENTER_X, 204, "THE WORD IS:");
                                outtextxy(CENTER_X, 230, ans.w);
                            }
                            rectangle(140, 150, 500, 280);
                            rectangle(141, 149, 499, 279);

                            settextjustify(LEFT_TEXT, TOP_TEXT);
                        } else break;
                        getch();
                    } while (submenu != 5);
                }
                break;
            case 2:
                {
                    cleardevice();
                    setfillstyle(WIDE_DOT_FILL, color.bg_color);
                    bar(32, 32, 607, 447);
                    for (int i = 1; i <= 5; i++)
                    {
                        setfillstyle(SOLID_FILL, 0);
                        bar(146 - (i * 10), 100, 490 + (i * 10), 379);
                        delay(30);
                    }
                    setcolor(WHITE);
                    rectangle(96, 100, 540, 379);
                    settextstyle(DEFAULT_FONT, HORIZ_DIR, 5);
                    setcolor(LIGHTRED);
                    outtextxy(CENTER_X, 150, "RULES");
                    settextjustify(LEFT_TEXT, CENTER_TEXT);
                    setcolor(YELLOW);
                    settextstyle(SMALL_FONT, HORIZ_DIR, 5);
                    outtextxy(120, 200, "[1] Every word you enter must be in the word list.");
                    outtextxy(120, 224, "[2] A correct letter turns green.");
                    outtextxy(120, 248, "[3] A correct letter in the wrong place turns yellow.");
                    outtextxy(120, 272, "[4] An incorrect letter turns red.");
                    outtextxy(120, 296, "[5] Letters can be used more than once.");
                    outtextxy(120, 320, "[6] Answers are never plurals.");
                    getch();
                    delay(50);
                }
                break;
            case 3:
                {
                    ifstream file;
                    file.open("users.dat");
                    char name[100][20], buffer[20], scores[100][4][10];
                    int sum[100] = { 0 };
                    int count = 0;

                    while (file >> name[count] >> buffer >> scores[count][0] >> scores[count][1] >> scores[count][2] >> scores[count][3])
                    {
                        if (file.eof()) break;

                        for (int i = 0; i < 4; i++)
                            decrypt(scores[count][i]);

                        for (i = 0; i < 4; i++)
                            sum[count] += atoi(scores[count][i]);

                        count++;
                    }
                    file.close();

                    for (int i = 0; i < count - 1; i++)
                    {
                        for (int j = 0; j < count - i - 1; j++)
                        {
                            if (sum[j] < sum[j + 1])
                            {
                                swap(sum[j], sum[j + 1]);
                                swap(name[j], name[j + 1]);
                                for (int k = 0; k < 4; k++)
                                {
                                    swap(scores[j][k], scores[j + 1][k]);
                                }
                            }
                        }
                    }
                    char key = 0;
                    int currentPage = 0;

                redraw:
                    cleardevice();
                    setfillstyle(WIDE_DOT_FILL, color.bg_color);
                    bar(32, 32, 607, 447);

                    setfillstyle(SOLID_FILL, 0);
                    bar(96, 50, 540, 429);

                    setcolor(WHITE);
                    rectangle(96, 50, 540, 429);
                    settextjustify(CENTER_TEXT, CENTER_TEXT);
                    settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
                    setcolor(GREEN);
                    outtextxy(CENTER_X, 100, "LEADERBOARD");


                    settextstyle(SMALL_FONT, HORIZ_DIR, 4);
                    setcolor(WHITE);
                    settextjustify(LEFT_TEXT, CENTER_TEXT);
                    outtextxy(120, 150, "PLAYER NAME");
                    settextjustify(CENTER_TEXT, CENTER_TEXT);
                    outtextxy(270, 150, "NOVICE");
                    outtextxy(340, 150, "INTERMEDIATE");
                    outtextxy(415, 150, "DIFFICULT");
                    outtextxy(480, 150, "EXTREME");
                    char pages[10];

                    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
                    sprintf(pages, "%s %d/%d %s", ARROW_R, currentPage + 1, (count + 6) / 7, ARROW_L);
                    outtextxy(480, 405, pages);

                    for (int l = 0; l < 7 && (currentPage * 7 + l) < count; l++)
                    {
                        int index = currentPage * 7 + l;
                        decrypt(name[index]);
                        settextstyle(SMALL_FONT, HORIZ_DIR, 4);
                        setcolor(WHITE);
                        settextjustify(LEFT_TEXT, CENTER_TEXT);
                        outtextxy(120, 175 + (l * 30), name[index]);
                        setcolor((strcmp(name[index], user.name) == 0) ? RED : YELLOW);
                        settextjustify(CENTER_TEXT, CENTER_TEXT);
                        for (int i = 0; i < 4; i++)
                            outtextxy(270 + (i * 70), 175 + (l * 30), scores[index][i]);
                        encrypt(name[index]);
                    }

                    key = getch();
                    if (key == 77 && (currentPage + 1) * 7 < count)
                    {
                        currentPage++;
                        goto redraw;
                    } else if (key == 75 && currentPage > 0)
                    {
                        currentPage--;
                        goto redraw;
                    } else if (key != 27)
                    {
                        goto redraw;
                    }
                }
                break;
            case 4:
                {
                    int key = 0, op = 1;
                    char *Options[] = { "CHANGE USER" ,"CHANGE THEME", "ABOUT US", "BACK" };
                    delay(10);
                    do
                    {
                        do
                        {
                            cleardevice();
                            init_border();
                            setcolor(LIGHTCYAN);
                            settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
                            outtextxy(CENTER_X, 118, "SETTINGS");
                            settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
                            setcolor(LIGHTRED);
                            outtextxy(CENTER_X, 254, "OPTIONS");
                            settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
                            setcolor(WHITE);
                            highlight(Options, op, 4);
                            outtextxy(CENTER_X + (textwidth(Options[op - 1]) / 2) + 10, 310 + (16 * (op - 1)), ARROW_R);
                            outtextxy(CENTER_X - (textwidth(Options[op - 1]) / 2) - 10, 310 + (16 * (op - 1)), ARROW_L);
                            key = getch();
                            if (key == 13)
                            {
                                playTone(300, 50);
                                break;
                            } else arrowKeys(op, key, 4);
                            playTone(200, 30);
                        } while (1);

                        switch (op)
                        {
                            case 1:
                                signIn(1);
                                break;
                            case 2:
                                {
                                    int key = 0, op = 1, c;
                                    char *Options[] = { "BLUE" ,"MAGENTA", "BROWN", "BLACK", "BACK" };
                                    delay(10);
                                    do
                                    {
                                        do
                                        {
                                            cleardevice();

                                            init_border();
                                            setcolor(LIGHTCYAN);
                                            settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
                                            outtextxy(CENTER_X, 118, "COLOR SELECTION");
                                            settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
                                            setcolor(LIGHTRED);
                                            outtextxy(CENTER_X, 254, "OPTIONS");
                                            settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
                                            setcolor(WHITE);
                                            highlight(Options, op, 5);
                                            outtextxy(CENTER_X + (textwidth(Options[op - 1]) / 2) + 10, 310 + (16 * (op - 1)), ARROW_R);
                                            outtextxy(CENTER_X - (textwidth(Options[op - 1]) / 2) - 10, 310 + (16 * (op - 1)), ARROW_L);
                                            key = getch();

                                            if (key == 13)
                                            {
                                                playTone(300, 50);
                                                break;
                                            } else arrowKeys(op, key, 5);
                                            playTone(200, 30);
                                        } while (1);
                                        switch (op)
                                        {
                                            case 1:
                                                c = 1;
                                                break;
                                            case 2:
                                                c = 5;
                                                break;
                                            case 3:
                                                c = 6;
                                                break;
                                            case 4:
                                                c = 0;
                                                break;
                                            case 5:
                                                break;
                                        }
                                        ofstream file;
                                        file.open("color.txt");
                                        file << c;
                                        file.close();
                                        getColor();
                                    } while (op != 5);

                                    break;
                                }
                            case 3:
                                {
                                    cleardevice();
                                    setfillstyle(WIDE_DOT_FILL, color.bg_color);
                                    bar(32, 32, 607, 447);
                                    for (int i = 1; i <= 5; i++)
                                    {
                                        setfillstyle(SOLID_FILL, 0);
                                        bar(146 - (i * 10), 100, 490 + (i * 10), 379);
                                        delay(30);
                                    }
                                    setcolor(WHITE);
                                    rectangle(96, 100, 540, 379);
                                    settextstyle(DEFAULT_FONT, HORIZ_DIR, 5);
                                    setcolor(LIGHTRED);
                                    outtextxy(CENTER_X, 150, "ABOUT US");
                                    settextjustify(CENTER_TEXT, CENTER_TEXT);
                                    settextstyle(SMALL_FONT, HORIZ_DIR, 4);
                                    setcolor(WHITE);
                                    outtextxy(CENTER_X, 200, "***DEVELOPERS***");
                                    outtextxy(CENTER_X, 220, "Jermaine Razehl Agulto");
                                    outtextxy(CENTER_X, 240, "Hanz Matthew Gagtan");
                                    outtextxy(CENTER_X, 260, "John Albert Olazo");
                                    outtextxy(CENTER_X, 280, "Gian Rafael Roldan");
                                    outtextxy(CENTER_X, 300, "Joshua Serohijos");
                                    setcolor(YELLOW);
                                    outtextxy(CENTER_X, 344, "Copyright \xB8 2024 Group 6. All rights reserved.");
                                    getch();
                                    delay(50);
                                }
                                break;
                        }
                    } while (op != 4);
                }
                break;
            case 5:
                break;
            default:
                break;
        }
    } while (option != 5);
    closegraph();
}

void playTone(int i, int d)
{
    sound(i);
    delay(d);
    nosound();
}
void init_border()
{
    setfillstyle(WIDE_DOT_FILL, color.bg_color);
    bar(32, 32, 607, 447);
    setfillstyle(SOLID_FILL, 0);
    bar(232, 220, 408, 420);
    setcolor(WHITE);
    rectangle(232, 220, 408, 420);
    rectangle(32, 32, 607, 447);
    settextjustify(CENTER_TEXT, CENTER_TEXT);
}
void arrowKeys(int &choice, int key, int size)
{
    if (key == arrow_d && choice < size) choice++;
    else if (key == arrow_u && choice > 1) choice--;
    else if (key == arrow_d && choice == size) choice = 1;
    else if (key == arrow_u && choice == 1) choice = size;
}
void highlight(char *menuOptions[], int choice, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (choice == i + 1)
            setcolor(YELLOW);
        outtextxy(320, 310 + (16 * i), menuOptions[i]);
        setcolor(WHITE);
    }
}
int init_menu(int &choice)
{
    int key = 0;
    char *menuOps[] = { "PLAY", "RULES", "LEADERBOARD", "SETTINGS", "QUIT" };
    int size = sizeof(menuOps) / sizeof(menuOps[0]);
    delay(10);
    do
    {
        cleardevice();

        init_border();
        setcolor(YELLOW);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 6);
        outtextxy(210, 118, "Lexi");
        setcolor(RED);
        outtextxy(425, 118, "GUESS");
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        setcolor(LIGHTRED);
        outtextxy(CENTER_X, 254, "OPTIONS");
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
        setcolor(WHITE);
        highlight(menuOps, choice, size);
        outtextxy(CENTER_X + (textwidth(menuOps[choice - 1]) / 2) + 10, 310 + (16 * (choice - 1)), ARROW_R);
        outtextxy(CENTER_X - (textwidth(menuOps[choice - 1]) / 2) - 10, 310 + (16 * (choice - 1)), ARROW_L);
        key = getch();

        if (key == 13)
        {
            playTone(300, 50);
            return choice;
        } else arrowKeys(choice, key, size);
        playTone(200, 30);
    } while (1);
}
int init_subMenu(int &choice)
{
    int key = 0;
    char *subMenuOps[] = { "NOVICE" ,"INTERMEDIATE","DIFFICULT", "EXTREME", "BACK" };
    delay(10);
    do
    {
        cleardevice();

        init_border();
        setcolor(RED);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
        outtextxy(CENTER_X, 118, "SELECT DIFFICULTY");
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
        setcolor(LIGHTRED);
        outtextxy(CENTER_X, 254, "OPTIONS");
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
        setcolor(WHITE);
        highlight(subMenuOps, choice, 5);
        outtextxy(CENTER_X + (textwidth(subMenuOps[choice - 1]) / 2) + 10, 310 + (16 * (choice - 1)), ARROW_R);
        outtextxy(CENTER_X - (textwidth(subMenuOps[choice - 1]) / 2) - 10, 310 + (16 * (choice - 1)), ARROW_L);
        key = getch();

        if (key == 13)
        {
            playTone(300, 50);
            return choice;
        } else arrowKeys(choice, key, 5);
        playTone(200, 30);
    } while (1);
}
int exit()
{
    int  key = 0, x = -90;
    int yes = 0;

    setfillstyle(SOLID_FILL, BLACK);
    for (int i = 1; i <= 5; i++)
    {
        bar(190 - (i * 10), 150, 450 + (i * 10), 280);
        delay(20);
    }
    do
    {
        setfillstyle(SOLID_FILL, BLACK);
        bar(140, 150, 500, 280);
        setcolor(WHITE);
        rectangle(140, 150, 500, 280);
        rectangle(141, 149, 499, 279);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
        settextjustify(CENTER_TEXT, CENTER_TEXT);
        outtextxy(CENTER_X, 180, "Do you wish to quit?");
        outtextxy(CENTER_X, 195, "Your progress will be lost.");

        settextjustify(2, 1);
        outtextxy(CENTER_X + 90, 250, "   ");
        outtextxy(CENTER_X - 90, 250, "  ");

        outtextxy(CENTER_X + 90, 250, "Yes");
        outtextxy(CENTER_X - 90, 250, "No");

        settextjustify(1, 1);
        outtextxy(310 + x, 264, ARROW_U);
        outtextxy(310 + x, 236, ARROW_D);

        key = getch();

        if (key == 13)
        {
            playTone(200, 50);
            break;
        } else if (yes >= 0 && yes <= 1)
        {
            if (key == arrow_r && yes < 1)
            {
                yes = 1;
                x = 88;
            } else if (key == arrow_l && yes > 0)
            {
                yes = 0;
                x = -90;
            }
        }
    } while (key != 13);
    return yes;
}
void boxes()
{
    setfillstyle(SOLID_FILL, 15);
    for (int y = 0; y < 3; y++)
    {
        for (int x = 0; x < 5; x++)
        {
            bar(46 + (x * 60), 184 - (y * 60), 94 + (x * 60), 232 - (y * 60));
            bar(46 + (x * 60), 244 + (y * 60), 94 + (x * 60), 292 + (y * 60));
            delay(10);
        }
    }
}
int getRandomNumber(int min, int max)
{
    return min + rand() % (max - min + 1);
}
void getWord()
{
    srand(time(0));
    int min = 2 + ((d.level - 1) * 101), max = 101 + ((d.level - 1) * 101);
    int line = (getRandomNumber(min, max) == ans.l) ? getRandomNumber(min + 1, max - 1) : getRandomNumber(min, max);

    int currentline = 0;

    ifstream file;
    file.open("words.dat");

    while (file >> ans.w)
    {
        currentline++;
        if (currentline == line) break;  // Stop when the random line is reached
    }
    file.close();
    decrypt(ans.w);
    ans.l = line;
}
int checking(char user[][6], int index)
{
    int targetCount[26] = { 0 }, correct = 0;
    for (int i = 0; i < 5; i++)
    {
        targetCount[ans.w[i] - 'A']++;
    }
    for (int red = 0; red < 5; red++)
    {
        setcolor(12);
        rectangle(46 + (red * 60), 64 + (index * 60), 94 + (red * 60), 112 + (index * 60));
        rectangle(47 + (red * 60), 65 + (index * 60), 93 + (red * 60), 111 + (index * 60));
        delay(10);
    }
    for (int green = 0; green < 5; green++)
    {
        if (user[index][green] == ans.w[green])
        {
            setcolor(10);
            rectangle(46 + (green * 60), 64 + (index * 60), 94 + (green * 60), 112 + (index * 60));
            rectangle(47 + (green * 60), 65 + (index * 60), 93 + (green * 60), 111 + (index * 60));
            correct++;
            targetCount[user[index][green] - 'A']--;
            delay(10);
        }
    }
    for (int yellow = 0; yellow < 5; yellow++)
    {
        if (targetCount[user[index][yellow] - 'A'] > 0 && user[index][yellow] != ans.w[yellow])
        {
            for (int i = 0; i < 5; i++)
            {
                if (user[index][yellow] == ans.w[i] && user[index][yellow] != ans.w[yellow])
                {
                    setcolor(14);
                    rectangle(46 + (yellow * 60), 64 + (index * 60), 94 + (yellow * 60), 112 + (index * 60));
                    rectangle(47 + (yellow * 60), 65 + (index * 60), 93 + (yellow * 60), 111 + (index * 60));
                    targetCount[user[index][yellow] - 'A']--;
                    delay(10);
                    break;
                }
            }
        }
    }
    setcolor(0);
    return correct;
}
void initDisplay()
{
    setfillstyle(WIDE_DOT_FILL, color.bg_color);
    bar(16, 16, 624, 464);

    setfillstyle(EMPTY_FILL, BLACK);
    bar(381, 31, 603, 446);
    setcolor(15);
    rectangle(380, 32, 604, 447);

    const char *difficulty_text[4] = { "NOVICE", "INTERMEDIATE", "DIFFICULT", "EXTREME" };
    settextstyle(SANS_SERIF_FONT, HORIZ_DIR, 2);
    setcolor(WHITE);
    settextjustify(1, 2);
    outtextxy(496, 32, "GUESS THE WORD");
    settextstyle((d.level == 2) ? 1 : d.level, HORIZ_DIR, 2);
    setcolor(d.level + 1);
    outtextxy(496, 54, difficulty_text[d.level - 1]);
    setcolor(RED);
    settextjustify(0, 2);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 2);
    outtextxy(409, 140, "SCORE:");
}
void printScore()
{
    char scr[100] = { 0 }, prev[100] = { 0 };
    sprintf(scr, "%d", score.s);

    settextjustify(1, 2);

    setcolor(BLACK);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 10);
    outtextxy(496, 240, prev);

    delay(50);

    setcolor(RED);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 10);
    outtextxy(496, 240, scr);

    settextjustify(0, 2);
}
void restore(int index, char answers[][6], char *currentInput)
{
    cleardevice();
    settextjustify(0, 2);
    initDisplay();
    boxes();
    printScore();
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
    setcolor(BLACK);
    if (index > 0)
    {
        for (int i = 0; i < index; i++)
        {
            checking(answers, i);
        }
        for (i = 0; i < index; i++)
        {
            for (int j = 0; j < 5; j++)
            {
                char letter[2];
                sprintf(letter, "%c", answers[i][j]);
                outtextxy(59 + (j * 60), 77 + (i * 60), letter);
            }
        }
    }
    for (int j = 0; j < 5; j++)
    {
        char letter[2];
        sprintf(letter, "%c", currentInput[j]);
        outtextxy(59 + (j * 60), 77 + (index * 60), letter);
        currentInput[j] = 0;
    }
}
void displayTime()
{
    char timeStr[10];
    gotoxy(7, 3);
    cout << "        ";
    sprintf(timeStr, "%02d:%02d:%02d", currentTime.minute, currentTime.second, currentTime.milisec);
    gotoxy(7, 3);
    cout << timeStr;
    if (currentTime.second > 59)
    {
        currentTime.second -= 60;
        currentTime.minute++;
    }
    if (currentTime.milisec > 0)
    {
        currentTime.milisec--;
    } else
    {
        currentTime.milisec = 30;
        if (currentTime.second > 0)
        {
            currentTime.second--;
        } else
        {
            if (currentTime.minute > 0)
            {
                currentTime.minute--;
                currentTime.second = 59;
            } else
            {
                currentTime.milisec = 0;
                currentTime.second = 0;
                currentTime.minute = 0;
                timerRunning = 0;
            }
        }
    }
}
int init_difficulty()
{
    int scr;
    timerRunning = 1;

    currentTime.minute = 0 + ((4 - d.level) % 4);
    currentTime.second = 59;

    int attempts, j;
    while (timerRunning)
    {
        attempts = 0;
        char user[6][6] = { 0 };
        char currentInput[6] = { 0 };
        int cont = 0;
        char ch = '\0';
        j = 0;

        cleardevice();

        delay(10);
        initDisplay();
        boxes();
        getWord();

        //cout << ans.w;

        printScore();

        while (1)
        {
            displayTime();
            if (timerRunning == 0) break;
            delay(30);
            setcolor(0);
            settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
            if (kbhit())
            {
                playTone(200, 20);
                ch = getch();
                switch (ch)
                {
                    case 13:
                        if (j == 5)
                        {
                            if (checking(user, attempts) == 5)
                            {
                                score.addScore();

                                playTone(500, 50);
                                cont = 1;
                                j = 0;
                                attempts++;
                                break;
                            } else
                            {
                                j = 0;
                                attempts++;
                                playTone(100, 50);
                            }
                        }
                        break;
                    case 27:
                        playTone(500, 50);
                        if (!exit())
                        {
                            strncpy(currentInput, user[attempts], j);
                            restore(attempts, user, currentInput);
                            continue;
                        } else
                        {
                            delay(50);
                            scr = score.s;
                            score.resetScore();
                            return scr;
                        }
                    case 8:
                        if (j > 0)
                        {
                            j--;
                            char letter[2] = { user[attempts][j], '\0' };
                            user[attempts][j] = '\0';
                            currentInput[j] = '\0';
                            setcolor(15);
                            outtextxy(59 + (j * 60), 77 + (attempts * 60), letter);
                        }
                        break;
                    default:
                        if (j < 5 && isalpha(ch))
                        {
                            ch = toupper(ch);
                            user[attempts][j] = ch;
                            char letter[2] = { ch, '\0' };
                            outtextxy(59 + (j * 60), 77 + (attempts * 60), letter);
                            j++;
                        }
                        break;
                }
            }
            if (cont == 1)
            {
                cont = 0;
                attempts = 0;
                break;
            }
            if (attempts == 6) break;
        }
        if (attempts == 6)
        {
            break;
        }
    }
    scr = score.s;
    writeScore();
    score.resetScore();
    return scr;
}
void decrypt(char *text)
{
    for (int i = 0; i < strlen(text); i++)
    {
        text[i] += 30;
    }
}
void encrypt(char *text)
{
    for (int i = 0; i < strlen(text); i++)
    {
        text[i] -= 30;
    }
}
void blink(int x, int y, int color, char *ch)
{
    while (!kbhit())
    {
        setcolor(color);
        outtextxy(x, y, ch);
        delay(10);
        setcolor(BLACK);
        outtextxy(x, y, ch);
        delay(10);
    }
}
void addUser(int run)
{
    int pass = 1;
    do
    {
        cleardevice();
        setfillstyle(WIDE_DOT_FILL, color.bg_color);
        bar(32, 32, 607, 447);
        setcolor(WHITE);
        rectangle(32, 32, 607, 447);

        settextjustify(CENTER_TEXT, CENTER_TEXT);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 5);
        setcolor(YELLOW);
        outtextxy(CENTER_X, 150, "SIGN UP");

        setfillstyle(SOLID_FILL, BLACK);
        bar(200, 240, 440, 278);
        bar(200, 294, 440, 326);
        setcolor(WHITE);
        rectangle(200, 240, 440, 278);
        rectangle(200, 294, 440, 326);

        settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
        outtextxy(248, 259, "USERNAME: ");
        outtextxy(248, 311, "PASSWORD: ");
        outtextxy(CENTER_X, 350, "ALREADY HAVE AN ACCOUNT? PRESS (/)");

        char fileUsername[20];
        char filePassword[20];
        int i = 0;
        char ch;
        blink(288 + (i * 8), 259, WHITE, "_");
        while (1)
        {
            blink(288 + (i * 8), 259, WHITE, "_");
            ch = getch();
            if (ch == 13 && i > 0) break;
            if (ch == '/')
            {
                signIn(run);
                return;
            } else if (ch == 8)
            {
                if (i > 0)
                {
                    i--;
                    char back[2] = { fileUsername[i], '\0' };
                    setcolor(BLACK);
                    outtextxy(288 + (i * 8), 259, back);
                    blink(288 + (i * 8), 259, WHITE, "_");
                    fileUsername[i] = '\0';
                }
                if (i == 0) blink(288 + (i * 8), 259, WHITE, "_");
            } else if (ch == 27)
            {
                if (!run)  exit(0);
                else return;
            } else if (ch != 13)
            {
                if (i < 18)
                {
                    fileUsername[i] = ch;
                    char c[2] = { ch, '\0' };
                    setcolor(WHITE);
                    outtextxy(288 + (i * 8), 259, c);
                    i++;
                    blink(288 + (i * 8), 259, WHITE, "_");
                }
            }
            playTone(200, 30);
        }
        setcolor(BLACK);
        outtextxy(288 + ((i) * 8), 259, "_");
        playTone(500, 30);
        fileUsername[i] = '\0';
        i = 0;
        while (1)
        {
            blink(288 + (i * 8), 311, WHITE, "_");
            ch = getch();
            playTone(200, 30);
            if (ch == 13 && i > 0) break;
            if (ch == '/')
            {
                signIn(run);
                return;
            }
            if (ch == 8)
            {
                if (i > 0)
                {
                    i--;
                    setcolor(BLACK);
                    outtextxy(288 + (i * 8), 311, "*");
                    blink(288 + (i * 8), 311, WHITE, "_");
                    filePassword[i] = '\0';
                }
                if (i == 0) blink(288 + (i * 8), 311, WHITE, "_");
            } else if (ch == 27)
            {
                if (!run) exit(0);
                else return;
            } else if (ch != 13)
            {
                if (i < 18)
                {
                    filePassword[i] = ch;
                    setcolor(WHITE);
                    outtextxy(288 + (i * 8), 311, "*");
                    i++;
                    blink(288 + (i * 8), 311, WHITE, "_");
                }
            }
        }
        setcolor(BLACK);
        outtextxy(288 + ((i) * 8), 311, "_");
        playTone(500, 30);
        filePassword[i] = '\0';
        char existUsername[20];
        int existing = 0;

        ifstream file_in("users.dat");

        while (file_in >> existUsername)
        {
            decrypt(existUsername);
            if (strcmp(existUsername, fileUsername) == 0)
            {
                existing = 1;
                break;
            }
            encrypt(existUsername);
        }
        file_in.close();
        char init[2] = "0";
        encrypt(init);
        if (!existing)
        {
            ofstream file_out("users.dat", ios::app);
            encrypt(fileUsername);
            encrypt(filePassword);
            file_out << fileUsername << " " << filePassword << " " << init << " " << init << " " << init << " " << init << endl;
            file_out.close();
            setcolor(YELLOW);
            outtextxy(CENTER_X, 400, "REGISTERED SUCCESSFULLY!");
            setcolor(WHITE);
            pass = 1;
        } else
        {
            setcolor(RED);
            outtextxy(CENTER_X, 400, "USER ALREADY EXISTSS!");
            setcolor(WHITE);
        }
    } while (!pass);
    playTone(500, 100);
    getch();
    signIn(run);
    return;
}
void signIn(int run)
{
    int pass = 0;
    do
    {
        cleardevice();
        setfillstyle(WIDE_DOT_FILL, color.bg_color);
        bar(32, 32, 607, 447);
        setcolor(WHITE);
        rectangle(32, 32, 607, 447);

        settextjustify(CENTER_TEXT, CENTER_TEXT);
        settextstyle(DEFAULT_FONT, HORIZ_DIR, 5);
        setcolor(LIGHTGREEN);
        outtextxy(CENTER_X, 150, "SIGN IN");

        setfillstyle(SOLID_FILL, BLACK);
        bar(200, 240, 440, 278);
        bar(200, 294, 440, 326);
        setcolor(WHITE);
        rectangle(200, 240, 440, 278);
        rectangle(200, 294, 440, 326);

        settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
        outtextxy(248, 259, "USERNAME: ");
        outtextxy(248, 311, "PASSWORD: ");
        outtextxy(CENTER_X, 350, "DON'T HAVE AN ACCOUNT? PRESS (/)");

        char user_name[20], user_password[20];
        int i = 0;
        char ch;

        while (1)
        {
            blink(288 + (i * 8), 259, WHITE, "_");
            ch = getch();
            if (ch == 13 && i > 0) break;
            playTone(200, 30);
            if (ch == '/')
            {
                addUser(run);
                return;
            } else if (ch == 8)
            {
                if (i > 0)
                {
                    i--;
                    char back[2] = { user_name[i], '\0' };
                    setcolor(BLACK);
                    outtextxy(288 + (i * 8), 259, back);
                    blink(288 + (i * 8), 259, WHITE, "_");
                    user_name[i] = '\0';
                }
                if (i == 0) blink(288 + (i * 8), 259, WHITE, "_");
            } else if (ch == 27)
            {
                if (!run) exit(0);
                else return;
            } else if (ch != 13)
            {
                if (i < 18)
                {
                    user_name[i] = ch;
                    char c[2] = { ch, '\0' };
                    setcolor(WHITE);
                    outtextxy(288 + (i * 8), 259, c);
                    i++;
                    blink(288 + (i * 8), 259, WHITE, "_");
                }
            }
        }
        setcolor(BLACK);
        outtextxy(288 + ((i) * 8), 259, "_");
        playTone(500, 30);
        user_name[i] = '\0';
        i = 0;
        while (1)
        {
            blink(288 + (i * 8), 311, WHITE, "_");
            ch = getch();
            playTone(200, 30);
            if (ch == 13 && i > 0) break;
            if (ch == '/')
            {
                addUser(run);
                return;
            }
            if (ch == 8)
            {
                if (i > 0)
                {
                    i--;
                    outtextxy(288 + (i * 8), 311, "*");
                    blink(288 + (i * 8), 311, WHITE, "_");
                    user_password[i] = '\0';
                }
                if (i == 0) blink(288 + (i * 8), 311, WHITE, "_");
            } else if (ch == 27)
            {
                if (!run) exit(0);
                else return;
            } else if (ch != 13)
            {
                if (i < 18)
                {
                    user_password[i] = ch;
                    setcolor(WHITE);
                    outtextxy(288 + (i * 8), 311, "*");
                    i++;
                    blink(288 + (i * 8), 311, WHITE, "_");
                }
            }
        }
        setcolor(BLACK);
        outtextxy(288 + ((i) * 8), 311, "_");
        playTone(500, 30);
        user_password[i] = '\0';
        char password[20];
        if (getUser(user_name, password))
        {
            if (strcmp(password, user_password) == 0)
            {
                char welcome[30];
                sprintf(welcome, "WELCOME, %s", user_name);
                setcolor(GREEN);
                outtextxy(CENTER_X, 400, welcome);
                setcolor(WHITE);
                pass = 1;
            } else
            {
                setcolor(RED);
                outtextxy(CENTER_X, 400, "WRONG PASSWORD");
                setcolor(WHITE);
            }
        } else
        {
            setcolor(RED);
            outtextxy(CENTER_X, 400, "USER NOT FOUND");
            outtextxy(CENTER_X, 416, "CREATE AN ACCOUNT FIRST!");
            setcolor(WHITE);
        }
        strcpy(user.name, user_name);

        getch();
    } while (!pass);
}
int getUser(const char *username, char *password)
{
    char fileUsername[20];
    char filePassword[20];
    ifstream file;
    file.open("users.dat");

    while (file >> fileUsername >> filePassword)
    {
        decrypt(fileUsername);
        decrypt(filePassword);
        if (strcmp(username, fileUsername) == 0)
        {
            strcpy(password, filePassword);
            file.close();
            return 1;
        }
    }
    file.close();
    return 0;
}
void getColor()
{
    ifstream file;
    file.open("color.txt");
    file >> color.bg_color;
    file.close();
}
void writeScore()
{
    char fileUsername[20];
    char buffer[20];
    char difficulties[4][10];  // Allocate space for difficulty level strings
    int scores[4] = { 0 };
    int userFound = 0;

    ifstream file_in("users.dat");
    ofstream temp_out("temp.dat");

    while (file_in >> fileUsername >> buffer >> difficulties[0] >> difficulties[1] >> difficulties[2] >> difficulties[3])
    {
        decrypt(fileUsername);
        for (int i = 0; i < 4; i++) decrypt(difficulties[i]);
        for (i = 0; i < 4; i++) scores[i] = atoi(difficulties[i]);

        if (strcmp(user.name, fileUsername) == 0)
        {
            userFound = 1;
            scores[d.level - 1] = (scores[d.level - 1] < score.s) ? score.s : scores[d.level - 1];
        }

        for (i = 0; i < 4; i++) itoa(scores[i], difficulties[i], 10);
        encrypt(fileUsername);
        for (i = 0; i < 4; i++) encrypt(difficulties[i]);

        temp_out << fileUsername << " " << buffer << " " << difficulties[0] << " " << difficulties[1] << " " << difficulties[2] << " " << difficulties[3] << endl;
    }

    file_in.close();

    if (!userFound)
    {
        encrypt(user.name);
        scores[d.level - 1] = score.s;

        for (int i = 0; i < 4; i++) itoa(scores[i], difficulties[i], 10);
        for (i = 0; i < 4; i++) encrypt(difficulties[i]);

        temp_out << user.name << " " << buffer << " " << difficulties[0] << " " << difficulties[1] << " " << difficulties[2] << " " << difficulties[3] << endl;
    }
    temp_out.close();

    remove("users.dat");
    rename("temp.dat", "users.dat");
}
