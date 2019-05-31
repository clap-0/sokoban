# sokoban
#include <stdio.h>
#include <termio.h>
#include <stdlib.h>

#define LEVEL 5
#define X 30
#define Y 30

char username[11];
char map[LEVEL][Y][X];
int level=-1;
int player_x[LEVEL], player_y[LEVEL];
int boxCount[LEVEL]={0}, storeCount[LEVEL]={0};
int moveCount=0, undoCount=5;
char undo;
char command;

void User(void);       // 플레이어 이름 받기
int getch(void);
void Display_help(void);   // 명령목록 출력 (명령 d)
void Command(void);         // 명령 입력 받기
void read_map(void);      // map.txt 에서 맵 읽어오기
void clear_map(void);      // 화면 지우기
void write_map(void);      // 맵 그리기
void player_move(void);    // 플레이어 움직임 (명령 h, j, k, l)
void save(void);         // 현재 상태 저장 (명령 s)
void fileload(void);      // 저장된 파일 불러오기 (명령 f)

int main()
{
   User();
   read_map();
   write_map();
   Command();

   return 0;
}

void User()   // player 이름 받기
{
   printf("Start....\n");

   // username을 공백으로 초기화
   for (int i = 0; i < 11; i++)
      username[i] = ' ';

   printf("input Name : ");
   scanf("%s", username);
}

void read_map() // map.txt에서 맵 배열에 넣기
{
   FILE *mapread;
   mapread=fopen("map.txt","r");
   
   char c;

   int x=0, y=0;
   
   while ((c = getc(mapread)) != EOF){
      if (c=='e'){
         break;
      }
      if ((49 <= c)&&(c <= 53)){   // c가 1~5 사이의 수이면 레벨 증가
         level++;
         y=-1;
         continue;
      }

      if (c=='@'){   // 플레이어 위치 저장
         player_x[level]=x;
         player_y[level]=y;
      }

      if (c=='$'){   // 상자 수 세기
         boxCount[level]++;
      }

      if (c=='O'){   // 보관 장소 수 세기
         storeCount[level]++;
      }

      if (c=='\n'){
         y++;
         x=0;
      }

      else {
         if (c!='.')
           map[level][y][x] = c;
       else
         map[level][y][x] = ' ';
         x++;
      }
   }

   // $개수와 O개수가 다르면 종료
   for (int i = 0; i < LEVEL; i++){
      if (boxCount[i] != storeCount[i]){
         printf("오류| 박스의 개수와 보관장소의 개수가 다릅니다!");
         return;
      }
   }

   level = 0;

   fclose(mapread);
}

void clear_map()   // 맵 지우기
{
   system("clear");
   system("clear");
}

void write_map()   // 맵 그리기
{
   clear_map();

   printf("   Hello %s\n\n", username);

   for (int i = 0; i < Y; i++){
      printf("%s\n", map[level][i]);
   }
}

void Command()   // 명령 입력 받기
{

   while (command != 'e')
   {
      command = getch();

      switch (command){
         case 'h' :   // 왼쪽이동
         case 'j' :   // 아래이동
         case 'k' :   // 위이동
         case 'l' :   // 오른쪽이동
            player_move();
          moveCount++;
            break;
         case 'u' :   // 되돌리기(undo)
         if (undoCount > 0){
            moveCount++;
            undoCount--;

         }
            break;
         case 'r' :   // 현재 맵 다시 시작, 움직임 횟수 유지

            break;
         case 'n' :   // 첫째 맵부터 다시 시작, 움직임 횟수 삭제
            
            break;
         case 'e' :   // 게임 종료, 종료 전 저장

            break;
         case 's' :   // 현재 상태 sokoban이라는 파일에 저장
         save();
            break;
         case 'f' :   // sokoban 파일에서 불러오기
         fileload();
         write_map();
            break;
         case 'd' :   // 명령 내용 출력
            Display_help();
            break;
      }
   }
}

int getch(void){
   int ch;

   struct termios buf;
   struct termios save;

   tcgetattr(0, &save);
   buf = save;

   buf.c_lflag&=~(ICANON|ECHO);
   buf.c_cc[VMIN]=1;
   buf.c_cc[VTIME]=0;

   tcsetattr(0, TCSAFLUSH, &buf);

   ch=getchar();
   tcsetattr(0, TCSAFLUSH, &save);
   
   return ch;
}

void player_move(){   // 플레이어 상하좌우 이동
   int x=0, y=0;

   switch (command){
      case 'h' : // 왼쪽 이동
    x--;
         undo = command;
         break;
      case 'j' : // 아래 이동
    y++;
         undo = command;
         break;
      case 'k' : // 위 이동
    y--;
         undo = command;
         break;
      case 'l' : // 오른쪽 이동
    x++;
         undo = command;
         break;
      }

      if (map[level][player_y[level]+y][player_x[level]+x] != '#'){      // 벽이 아니면 움직임
         map[level][player_y[level]][player_x[level]] = ' ';
          map[level][player_y[level]+=y][player_x[level]+=x] = '@';
      }

      write_map();
}

void save(){   // 현재 상태 저장
   FILE *savefile;
   savefile = fopen("sokoban.txt", "w");

   fprintf(savefile, "%s\n", username);   // 이름 저장
   fprintf(savefile, "%d\n", moveCount);   // 움직인 횟수 저장
   fprintf(savefile, "%d\n", undoCount);   // undo 횟수 저장
   fprintf(savefile, "%d\n", level);      // 레벨 저장

   for (int i = 0; i < Y; i++){      // 맵 저장
      fprintf(savefile, "%s\n", map[level][i]);
   }

   fclose(savefile);
}

void fileload(){
   FILE *loadfile;
   loadfile = fopen("sokoban.txt", "r");
   char c;
   int x=0, y=-1;

   fscanf(loadfile, "%s\n", username); 
   fscanf(loadfile, "%d\n%d\n%d", &moveCount, &undoCount, &level);

   while ((fscanf(loadfile, "%c", &c) != EOF)){
      if (c == '@'){
         player_x[level] = x;
         player_y[level] = y;
      }

      if (c == '\n'){
         y++;
         x=0;
      }
      else {
         map[level][y][x] = c;
         x++;
      }
   }
   fclose(loadfile);
}

void Display_help(void){   // 도움말 출력
   clear_map();
   printf("   Hello %s\n\n", username);
   printf("- h(왼쪽), j(아래), k(위), l(오른쪽)\n");
   printf("- u(undo)\n");
   printf("- r(replay)\n");
   printf("- n(new)\n");
   printf("- e(exit)\n");
   printf("- s(save)\n");
   printf("- f(file load)\n");
   printf("- d(display help)\n");
   printf("- t(top)\n");
}
