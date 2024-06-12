#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define m 14
#define p 14
#define Max_dir 4 // �� ������ �̵� ������ ��

//vert ����		horiz ����
typedef struct Aoff {
	short int vert;
	short int horiz;
}offsets;


//���ÿ� ���� ������
typedef struct St_element {
	short int row;
	short int col;
	short int dir;
}ty_element;

typedef struct ListNode* listPointer;

//���� ������ ���
typedef struct ListNode {
	ty_element data;
	listPointer link;
}listNode;

						  //��	  //��    //��   //��
offsets move[Max_dir] = { {0,1}, {0,-1}, {1,0}, {-1,0} };

int maze[m + 2][p + 2]; //maze ���. ���� ����
int mark[m + 2][p + 2]; //mark ���

//���� ���� �ؾ��ϴ� �κ� push, pop, isempty
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

//����� ���� ���������� ��� ����ϴ� �Լ�
//���� 1�� reverse �� print ����� ��ġ��
void solved_path(listPointer* top) {
	listNode *temp, *prev, *curr, *next;
	ty_element item;
	if (isEmpty(*top)) {
		printf("Stack is empty\n");
	}
	else {
		printf("--------------------------------------------------\n����� = >>  ");
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
		printf("<< = ������");
	}
}

//main�Լ� �պκп� text�� ����� ������ �ڵ�
void printMaze() {
	for (int i = 0; i <= m+1; i++) { //y
		for (int j = 0; j <= p+1; j++) { //x
			printf("%d ", maze[i][j]);
		}
		printf("\n");
	}
}



//�̷� ��� Ž�� �Լ�
//  [sy, sx] ���� �����Ͽ�  [dy, dx] �� �����ϴ� ��θ� ã�´�
int path(int sy, int sx, int dy, int dx){

	listPointer top = NULL; // ������ top ���� �ʱ⿡�� NULL
	ty_element position;
	int i, j, row, col, nextRow, nextCol, dir;
	int found = 0;
	int num_btrack = 0;
	int path_length = 1; // ����� ����
	
	//mark ���� �ʱ�ȭ. �ٿ������ ������ ���� ���� ��� 0���� �ʱ�ȭ�Ѵ�.
	for (i = 0; i < m; i++) {
		for (j = 0; j < p; j++) {
			mark[1 + i][1 + j] = 0;
		}
	}

	//�� ���ÿ� ������� �ְ� ����� mark 1ǥ��
	position.row = sy;
	position.col = sx;
	position.dir = 0;
	push(&top, position);
	mark[sy][sx] = 1;


	//��� or �������� ���� ���
	if (maze[sy][sx] == 1 || maze[dy][dx] == 1) {
		printf("[�Է� ����] : ������̳� ��ǥ���� ���� �� �Դϴ�.\n");
		return 0;
	}


	printf("\n\n-----------------------------------------------------------\n");
	//top�� ������� �����鼭(��ŸƮ ���� ����) ã�� �ʾƾ� �ݺ��� start!
	while (!isEmpty(top) && found != 1 ) {
		position = pop(&top); //���� ��ġ pop���� �ޱ�
		row = position.row;
		col = position.col;
		dir = position.dir;

		while (dir < Max_dir && found != 1) { //��� ���� ���� ���߰ų� found�� ���� ���� ���
			nextRow = row + move[dir].vert;
			nextCol = col + move[dir].horiz;

			if (maze[nextRow][nextCol] == 2) { //��ֹ� �߽߰� ����
				position.row = row;
				position.col = col;
				position.dir = dir;
				push(&top, position);
				position.row = nextRow;
				position.col = nextCol;
				position.dir = dir;
				push(&top, position);
				printf("\n\n\n[��ֹ�] ��ֹ� �߰����� �ߴ�\n\n ��ֹ� ��ġ : %d, %d\n\n\n", nextRow, nextCol);
				printf("\n��� ���� : %d\n", path_length);
				printf("��Ʈ��ŷ Ƚ��: %d\n", num_btrack);
				solved_path(&top);
				return 0;
			}
			if (nextRow == dy && nextCol == dx) { //���� ĭ�� �����̶�� found 1 ��ȯ
				found = 1;
				position.row = row;
				position.col = col;
				position.dir = dir;
				push(&top, position);
				mark[nextRow][nextCol] = 1;
				position.row = nextRow;
				position.col = nextCol;
				position.dir = dir;
				push(&top, position); // ���� �������� ��ο� �߰�
				path_length++;
			}
			else if (maze[nextRow][nextCol] == 0 && mark[nextRow][nextCol] == 0) { //�� �� �ִٸ� �̵�
				mark[nextRow][nextCol] = 1;
				position.row = row;
				position.col = col;
				position.dir = dir + 1;
				push(&top, position);
				printf("[����:%d, ����:%d]\n", row, col);
				row = nextRow;
				col = nextCol;
				dir = 0;
				path_length++;
			}
			else {
				dir++;
			}
		}
		if (dir == Max_dir) { //��� ���� �� ���Ƽ� 4�� �Ǿ��ٸ� backtracking �ϴ� �� ����ϸ鼭 �˷��ֱ� , num_btrack �߰��ϱ�, path �ϳ� ����
			printf("[����:%d, ����:%d] -> ���������� Backtracking �ؾ���\n", row, col);
			num_btrack++;
			path_length--;
		}
	}

		if (found) {
			printf("\n\n-----------------------------------------------------------\n");
			printf("[����]\n\n����Ѱ� (%d, %d)\n", sy, sx);
			printf("�����Ѱ� (%d, %d)\n", dy, dx);
			printf("\n��� ���� : %d\n", path_length);
			printf("��Ʈ��ŷ Ƚ��: %d\n", num_btrack);
			solved_path(&top);
			return 1;
		}
		else {
			printf("[����.....]\n �̵������� ��ΰ� �����ϴ�.");
			path_length--;
			printf("\n��� ���� : %d\n", path_length);
			printf("��Ʈ��ŷ Ƚ��: %d\n", num_btrack);
			solved_path(&top);
			return 0;
		}
}





int main() {
	int sy, sx, dy, dx;
	int res1, res2;
	
	FILE* pfile = fopen("maze_data.txt", "r");
	if (pfile == NULL) {
		printf("[ERROR] ���� �ε忡 �����Ͽ����ϴ�.\n");
		return 0;
	}
	else {
		printf("[���Ϸε� ����]\n");
		for (int i = 0; i <= m+1; i++) {
			for (int j = 0; j <= p+1; j++) {
				fscanf(pfile, "%d", &maze[i][j]);
			}
		}
		printMaze();
	}
	fclose(pfile);
	
	while (1) {
		printf("\n���,���������� �Է����ּ���\n ex)(����:2, ����:5) (����:8, ����:9) = 2 5 8 9\n\n");
		res1 = scanf("%d %d %d %d", &sy, &sx, &dy, &dx);
		if (res1 != 4) break;

		if (path(sy, sx, dy, dx) == 1) {
			printf("���� ���ϵ帳�ϴ�.\n");

		}
		else {
			printf("[����]\n\n\n");
		}
	}
	return 0;
}