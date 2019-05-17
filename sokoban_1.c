#include <stdio.h>
#include <termio.h>

char getch(void){
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

void display_help(void){
	printf("- h(왼쪽), j(아래), k(위), l(오른쪽) : 창고지기 조정\n");
	printf("- u(undo) : 최대 5번 할 수 있음\n");
	printf("- r(replay) : 현재 맵을 처음부터 다시 시작(움직임 횟수는 계속 유지)\n");
	printf("- n(new) : 첫 번째 맵부터 다시 시작(움직임 횟수 기록 삭제)\n");
	printf("- e(exit) : 게임 종료\n");
	printf("- s(save) : 현재 상태 파일에 저장.\n");
	printf("- f(file load) : save 시점에서부터 이어서 게임 시작\n");
	printf("- d(display help) : 명령 내용 보여줌\n");
	printf("- t(top) : 게임 순위 보여줌. t만 입력하면 전체 순위, t 다음 숫자가 오면 해당 맵의 순위\n");
}

int main()
{
	char command;

	while (command != 'e')
	{
		command = getch();

		switch (command){
			case 'h' :	// 왼쪽이동

				break;
			case 'j' :	// 아래이동

				break;
			case 'k' :	// 위이동

				break;
			case 'l' :	// 오른쪽이동

				break;
			case 'u' :	// 되돌리기(undo)

				break;
			case 'r' :	// 현재 맵 다시 시작, 움직임 횟수 유지

				break;
			case 'n' :	// 첫째 맵부터 다시 시작, 움직임 횟수 삭제
				
				break;
			case 'e' :	// 게임 종료, 종료 전 저장

				break;
			case 's' :	// 현재 상태 sokoban이라는 파일에 저장

				break;
			case 'f' :	// sokoban 파일에서 불러오기

				break;
			case 'd' :	// 명령 내용 출력
				display_help();
				break;
		}
	}
	return 0;
}
