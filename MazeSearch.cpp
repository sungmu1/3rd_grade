#include "Location2D.h"
#include <Windows.h>
#include <stack>
using namespace std;

extern int** alloc2DInt( int rows, int cols);
extern void free2DInt (int** mat, int rows, int cols=0);

int** map=NULL;
int rows=1;
int cols=0;

int initEnergy = 0;
int mazeMove=0;
double mana=0;

void loadMazeFile();
void printMaze(stack<Location2D> here, int r, int c, int mode);
void selectMode(char* mode);
void mazeSearch(stack<Location2D> locStack, char mode);
bool isValidLoc(int r, int c);
bool isExit(int r, int c);

void main() {
	char mode;

	loadMazeFile();
	selectMode(&mode);

	stack<Location2D> locStack;
	Location2D entry(0,1);// -> �������� 0, 1
	locStack.push(entry);

	//	printMaze(locStack,0,1,mode);
	//	getchar();
	mazeSearch(locStack, mode);
}

bool isValidLoc(int r, int c) {
	if ( r < 0 || c < 0 || r>=rows || c>=cols ) return false;
	else return map[r][c] == 0 ;
}
void selectMode(char* mode) {
	static int i = 0;

	if( i == 0 ) {
		printf("Select Mode ( A or M ) : ");
		scanf("%c", mode);
		i++;
	}
}

void printMaze(stack<Location2D> here,int r, int c, int mode) {
	system("cls");
	printf( "=============================================\n");
	printf( " ��ü �̷��� ũ�� = ");
	printf( "%d %d\n", cols, rows);
	printf( " ������ = ");
	printf( "%d\n", initEnergy);
	printf( " �̵� = ");
	printf( "%d\n", mazeMove);
	printf( " ���� ������ = ");
	printf( "%d\n", initEnergy-mazeMove);
	printf( " ���� = ");
	printf( "%.1lf\n", mana);
	printf( "=============================================\n");
	for( int i=0 ; i<rows ; i++ ) {
		for( int j=0 ; j<cols ; j++ ) {
			switch (map[i][j]) {
			case 0 : printf( "  " ); break;
			case 1 : 
				if( mode == 'e' ) {	// �ⱸ�� ã������ �� ��ü�� ������ ����
					printf( "��" );
					break;
				}
				else {
					printf("  ");	// �̷θ� Ž���߿� ���� �𸣹Ƿ� ����ó��
					break;
				}
			case 2 : printf( "��" ); break;	// ������ ��
			case 9 : printf( "��" ); break;
			case 8 : printf( "��" ); break;
			case 7 : printf( "��" ); break;
			case 6 : printf( "��" ); break;		
			}
		} printf("\n");
	}
	if( (mode == 'm' || mode == 'M') && mode != 'e') {
		printf("/////////////////////////////////////////////\n");
		printf("���� ��ġ (pop�� ����)\n");
		printf("row = %d col = %d\n", r, c) ;
		printf("���� ��ġ�� ��(map[r][c]) = %d\n",map[r][c]);
		printf("���� ����\n");
		stack<Location2D> pos;
		pos = here;
		for(int i = 0; i < here.size(); i++) {
			Location2D pos2D = pos.top();
			printf("[rows = %d , cols = %d]\t", pos2D.row, pos2D.col);
			pos.pop();
		}
	}
}

void loadMazeFile() {	// �̷θ� �д� �Լ�
	char ch;	// ���� ������ �б� ���� ���� �ӽ� ����.
	char filename[30];
	FILE *fp; //= fopen(filename, "r");	// ������ ����.

	while(1) {
		printf("���� �̸��� �Է��ϼ��� : ");
		gets(filename);
		fp = fopen(filename, "r");
		if ( fp == NULL ) {
			printf(" ���� %s�� �� �� �����ϴ�.!!!\n", filename);
			continue;
		}
		break;
	}
	while(fscanf(fp, "%c", &ch) != EOF) {	// �̷� txt������ �д´�, ������ ������ �ݺ��� ����
		if(ch == '\n') rows++;				// txt���Ͽ��� ���๮��(enter)�� ������ �������� ����.
		if(ch == '0' || ch == '1') cols++;	// txt���Ͽ��� ������ ������ ����.
	}
	cols = cols/rows;	// �� ������ ���� / ���� ���� = ���� ���� 

	initEnergy = 2 * cols * rows;

	map = alloc2DInt(rows,cols);	// map�� �����Ҵ� �޴´�.
	fclose(fp);

	/* 
	fopen�� �ٽ� �ϴ� ���� 
	���� while�ݺ����� ���� �ѹ� ���� fp�� ������ �� �о���.
	���� ���� �� ���� ������ �ٽ� ó������ �а� �ϴ¹��� ����
	�ϴ� ���Ӱ� ������ ���� �ٽ� �е��� �Ͽ���.
	*/

	FILE *map_read = fopen(filename, "r");
	for(int i=0; i<rows; i++) {
		for(int j=0; j<cols; j++) {
			fscanf(map_read, "%d", &(map[i][j]));
		}
	}
	fclose(map_read);
}

bool isExit(int r, int c) {
	return ( (r != 0 && c != 1) &&			// �Ա��� �ƴϸ鼭
		((r == 0 && map[r][c] == 0) ||		// �� ��
		(r == rows-1 && map[r][c] == 0) ||	// �Ʒ� ��
		(c == 0 && map[r][c] == 0) ||		// ���� ��
		(c == cols-1 && map[r][c] == 0)) );	// ������ ���� 0 �̶�� �ⱸ�̹Ƿ� true�� ��ȯ
}
bool isWall(int r, int c) {
	return ( r >= 0 && c >= 0 && map[r][c] == 1 );
}

void mazeSearch(stack<Location2D> locStack,char mode) {

	while( locStack.empty() == false ) {
		Location2D here = locStack.top();
		locStack.pop();	// ������ ���� ����


		mazeMove++;				// pop�� �ѹ� �Ͼ������(=�ѹ� �̵��� ������) mazeMove�� ����
		mana = mana + 0.1;		// pop�� �ѹ� �Ͼ������(=�ѹ� �̵��� ������) mana�� 0.1 ����
		
		int r = here.row;
		int c = here.col;

		if( mode == 'm' || mode == 'M' )
			getchar();

		if ( isWall(r,c+1) ) map[r][c+1] = 2;
		if ( isWall(r,c-1) ) map[r][c-1] = 2;
		if ( isWall(r-1,c) ) map[r-1][c] = 2;
		if ( isWall(r+1,c) ) map[r+1][c] = 2;


		// ���� ��ġ�� �ⱸ��� �ݺ����� �����ϰ� ���α׷��� ������.
		if( isExit(r,c) ) {
			map[r][c] = 9;
			printMaze(locStack,r,c,mode);
			printf("\n �̷� Ž�� ����\n");
			getchar();
			mode = 'e';
			printMaze(locStack,r,c,mode);
			getchar();
			free2DInt (map, rows, cols);
			return;
		}

		// ���� ��ġ�� �ⱸ�� �ƴ϶�� �ֺ��� �ѷ����� �����Ѵ�.
	
		else {
			if(map[r][c] == 0) map[r][c] = 9;
			else if(map[r][c] == 9) map[r][c] = 8;
			else if(map[r][c] == 8) map[r][c] = 7;
			else map[r][c] = 6;

			// �� ��ġ�� �����¿쿡 �ⱸ�� �ִ��� Ȯ��.
			// ������ ���ÿ� �ְ� Ž�� ����.
			if( isExit(r,c-1) ) {	// ����
				locStack.push(Location2D(r,c-1));
			}
			else if ( isExit(r, c+1) ) {	// ������
				locStack.push(Location2D(r,c+1));
			}
			else if ( isExit(r-1, c) ) {	// ��
				locStack.push(Location2D(r-1,c));
			}
			else if ( isExit(r+1, c) ) {	// �Ʒ�
				locStack.push(Location2D(r+1,c));
			}

			// �� ��ġ�� �����¿쿡 �ⱸ�� ���ٸ�
			// �� ��ġ�� �����¿찡 �� �� �ִ±�(0) ������ �Ǵ��Ͽ�
			// ���� �ִ� ���̶�� ���ÿ� �ְ� �����Ѵ�.
			// ���ÿ� �־����� ������ ���ư��� �ʾҴٸ� �ش� ��ġ�� 3���� �ٲ��ش�.
			else {
				if( isValidLoc(r, c-1) ) {	// ����
					if(map[r][c] == 9) {	// ���� ��ġ�� �ѹ� �̻� �������� ���� ���. r,c�� �㰡 �ִ°��, ������ map[r][c]�� 0�� ��� 9�� �مf��.
						locStack.push(Location2D(r,c));
						locStack.push(Location2D(r,c-1));
					}
				}
				if( isValidLoc(r-1, c) ) {	// ��
					if(map[r][c] == 9 ) {
						locStack.push(Location2D(r,c));
						locStack.push(Location2D(r-1,c));
					}
				}
				if( isValidLoc(r+1, c) ) {	// �Ʒ�
					if(map[r][c] == 9) {
						locStack.push(Location2D(r,c));
						locStack.push(Location2D(r+1,c));
					}
				}
				if( isValidLoc(r, c+1) ) {	// ������
					if(map[r][c] == 9) {
						locStack.push(Location2D(r,c));
						locStack.push(Location2D(r,c+1));
					}
				}
			}
		}
		printMaze(locStack,r,c,mode);
		if( (initEnergy - mazeMove) == 0 ) break;
	}
	printf("�̷� Ž�� ����\n");
	free2DInt (map, rows, cols);
}
