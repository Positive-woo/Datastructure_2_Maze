#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define m 14
#define p 14
#define Max_dir 4 // 총 가능한 이동 방향의 수

//vert 세로		horiz 가로
typedef struct Aoff {
	short int vert;
	short int horiz;
}offsets;


//스택에 넣을 데이터
typedef struct St_element {
	short int row;
	short int col;
	short int dir;
}ty_element;

typedef struct ListNode* listPointer;

//연결 스택의 노드
typedef struct ListNode {
	ty_element data;
	listPointer link;
}listNode;

						  //동	  //서    //남   //북
offsets move[Max_dir] = { {0,1}, {0,-1}, {1,0}, {-1,0} };

int maze[m + 2][p + 2]; //maze 행렬. 전역 변수
int mark[m + 2][p + 2]; //mark 행렬

//내가 구현 해야하는 부분 push, pop, isempty
int isEmpty(listPointer top) {
	return top == NULL;
}

void push(listPointer* top, ty_element position) {
	listNode* new_position = (listNode*)malloc(sizeof(listNode));
	new_position -> data = position;
	new_position->link = *top;
	*top = new_position;
}

ty_element pop(listPointer* top) {
	listNode* temp;
	ty_element item;
	if (isEmpty(*top)) {
		printf("[ERROR] Stack is EMPTY!!!\n");
		item.row = -1;
		item.col = -1;
		item.dir = -1;
		return item;
	}
	else {
		temp = *top;
		item = temp->data;
		*top = temp->link;
		free(temp);
		return item;
	}
}

//출발지 부터 도착지까지 경로 출력하는 함수
//과제 1의 reverse 와 print 기능을 합치기
void solved_path(listPointer* top) {
	listNode *temp, *prev, *curr, *next;
	ty_element item;
	if (isEmpty(*top)) {
		printf("Stack is empty\n");
	}
	else {
		printf("--------------------------------------------------\n출발지 = >>  ");
		prev = NULL;
		curr = *top;
		while (curr != NULL) {
			next = curr->link;
			curr->link = prev;
			prev = curr;
			curr = next;
		}
		*top = prev;
		while (*top != NULL) {
			temp = *top;
			item = temp->data;
			printf("[%d, %d] ", item.row, item.col);
			*top = temp->link;
		}
		printf("<< = 도착지");
	}
}

//main함수 앞부분에 text의 모습을 구현한 코드
void printMaze() {
	for (int i = 0; i <= m+1; i++) { //y
		for (int j = 0; j <= p+1; j++) { //x
			printf("%d ", maze[i][j]);
		}
		printf("\n");
	}
}



//미로 경로 탐색 함수
//  [sy, sx] 에서 시작하여  [dy, dx] 에 도달하는 경로를 찾는다
int path(int sy, int sx, int dy, int dx){

	listPointer top = NULL; // 스택의 top 변수 초기에는 NULL
	ty_element position;
	int i, j, row, col, nextRow, nextCol, dir;
	int found = 0;
	int num_btrack = 0;
	int path_length = 1; // 출발지 포함
	
	//mark 정보 초기화. 바운더리를 제외한 내부 셀에 모두 0으로 초기화한다.
	for (i = 0; i < m; i++) {
		for (j = 0; j < p; j++) {
			mark[1 + i][1 + j] = 0;
		}
	}

	//빈 스택에 출발점을 넣고 출발점 mark 1표시
	position.row = sy;
	position.col = sx;
	position.dir = 0;
	push(&top, position);
	mark[sy][sx] = 1;


	//출발 or 도착지가 벽일 경우
	if (maze[sy][sx] == 1 || maze[dy][dx] == 1) {
		printf("[입력 오류] : 출발점이나 목표점이 막힌 셀 입니다.\n");
		return 0;
	}


	printf("\n\n-----------------------------------------------------------\n");
	//top이 비어있지 않으면서(스타트 점이 들어가고) 찾지 않아야 반복문 start!
	while (!isEmpty(top) && found != 1 ) {
		position = pop(&top); //현재 위치 pop으로 받기
		row = position.row;
		col = position.col;
		dir = position.dir;

		while (dir < Max_dir && found != 1) { //모든 방향 실행 못했거나 found를 하지 못한 경우
			nextRow = row + move[dir].vert;
			nextCol = col + move[dir].horiz;

			if (maze[nextRow][nextCol] == 2) { //장애물 발견시 종료
				position.row = row;
				position.col = col;
				position.dir = dir;
				push(&top, position);
				position.row = nextRow;
				position.col = nextCol;
				position.dir = dir;
				push(&top, position);
				printf("\n\n\n[장애물] 장애물 발견으로 중단\n\n 장애물 위치 : %d, %d\n\n\n", nextRow, nextCol);
				printf("\n경로 길이 : %d\n", path_length);
				printf("백트래킹 횟수: %d\n", num_btrack);
				solved_path(&top);
				return 0;
			}
			if (nextRow == dy && nextCol == dx) { //다음 칸이 도착이라면 found 1 반환
				found = 1;
				position.row = row;
				position.col = col;
				position.dir = dir;
				push(&top, position);
				mark[nextRow][nextCol] = 1;
				position.row = nextRow;
				position.col = nextCol;
				position.dir = dir;
				push(&top, position); // 최종 목적지도 경로에 추가
				path_length++;
			}
			else if (maze[nextRow][nextCol] == 0 && mark[nextRow][nextCol] == 0) { //갈 수 있다면 이동
				mark[nextRow][nextCol] = 1;
				position.row = row;
				position.col = col;
				position.dir = dir + 1;
				push(&top, position);
				printf("[세로:%d, 가로:%d]\n", row, col);
				row = nextRow;
				col = nextCol;
				dir = 0;
				path_length++;
			}
			else {
				dir++;
			}
		}
		if (dir == Max_dir) { //모든 방향 다 돌아서 4가 되었다면 backtracking 하는 것 출력하면서 알려주기 , num_btrack 추가하기, path 하나 빼기
			printf("[세로:%d, 가로:%d] -> 도착했으나 Backtracking 해야함\n", row, col);
			num_btrack++;
			path_length--;
		}
	}

		if (found) {
			printf("\n\n-----------------------------------------------------------\n");
			printf("[성공]\n\n출발한곳 (%d, %d)\n", sy, sx);
			printf("도착한곳 (%d, %d)\n", dy, dx);
			printf("\n경로 길이 : %d\n", path_length);
			printf("백트래킹 횟수: %d\n", num_btrack);
			solved_path(&top);
			return 1;
		}
		else {
			printf("[실패.....]\n 이동가능한 경로가 없습니다.");
			path_length--;
			printf("\n경로 길이 : %d\n", path_length);
			printf("백트래킹 횟수: %d\n", num_btrack);
			solved_path(&top);
			return 0;
		}
}





int main() {
	int sy, sx, dy, dx;
	int res1, res2;
	
	FILE* pfile = fopen("maze_data.txt", "r");
	if (pfile == NULL) {
		printf("[ERROR] 파일 로드에 실패하였습니다.\n");
		return 0;
	}
	else {
		printf("[파일로드 성공]\n");
		for (int i = 0; i <= m+1; i++) {
			for (int j = 0; j <= p+1; j++) {
				fscanf(pfile, "%d", &maze[i][j]);
			}
		}
		printMaze();
	}
	fclose(pfile);
	
	while (1) {
		printf("\n출발,도착지점을 입력해주세요\n ex)(세로:2, 가로:5) (세로:8, 가로:9) = 2 5 8 9\n\n");
		res1 = scanf("%d %d %d %d", &sy, &sx, &dy, &dx);
		if (res1 != 4) break;

		if (path(sy, sx, dy, dx) == 1) {
			printf("성공 축하드립니다.\n");

		}
		else {
			printf("[실패]\n\n\n");
		}
	}
	return 0;
}