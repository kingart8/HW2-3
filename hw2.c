#include <stdio.h>
#include <stdlib.h>

void write_dot(FILE* fname, int** mas, int size)
{
	fprintf(fname, "graph G{\n");

	for (int i = 0; i < size; i++)
		fprintf(fname, "%d;\n", i);

	for (int j = 0; j < size; j++) {
		for (int i = j; i < size; i++)
			for (int k = 0; k < mas[i][j]; k++)
				fprintf(fname, "%d -- %d;\n", j, i);
	}
	fprintf(fname, "}");

	fclose(fname);
}

void checking_graph(int** mas, int size) {

	for (int i, j = 0; j < size; j++) {			// �������� �� ���������� ������
		i = j;
		if (mas[i][j] != 0) {
			printf("The entered graph IS NOT simple cycle");
			return;
		}
	}

	for (int j = 0; j < size; j++)				// �������� �� ���������� ������� �����
		for (int i = 0; i < size; i++) {
			if (mas[i][j] > 1) {
				printf("The entered graph IS NOT simple cycle.\n");
				return;
			}
		}

	for (int k, j = 0; j < size; j++) {			// �������� �� �������������� ������� ������ 2-� �����
		k = 0;
		
		for (int i = 0; i < size; i++) {
			if (mas[i][j] == 1)
				k++;
		}
		
		if (size == 2) {							// �� ������ ����� ������� k!=2, � � ������� 2�2 ����� ���� ������ ���� �����, ����� ��� ���� ������� ������
			if (k == 1)
				break;
			else {
				printf("The entered graph IS NOT simple cycle.\n");
				return;
			}
		}
		if (k != 2) {
			printf("The entered graph IS NOT simple cycle.\n");
			return;
		}
	}
	printf("The entered graph IS simple cycle.\n");
}

void free_mas(char* mas) {
	free(mas);
	mas = NULL;
	mas = (char*)malloc(sizeof(char));
}

int main(void) {
	int width;											//������������ ������ ������ �������
	printf("Enter the width: ");
	scanf("%d", &width);


	printf("\nEnter the rows of your matrix. With pressing \"ENTER\" you will change column. If do not specify a number it will be '0'.\n");

	getchar();											// ����� ENTER ��� ����� �������
				
														//�������� 2������� �������
	int** mas = NULL;									// ��� �������� 2������� ������� ���������� ����� ������ ����� ����������� ������� ���� ���������� �� ������� �������
	mas = (int**)malloc(sizeof(*mas) * width);			// ������ �� ������ *mas �������� ��������� �� int
	for (int i = 0; i < width; i++) {					// ������� ������ ������� ��������� �� �������� ������� � ������ ������ ������
		mas[i] = (int*)malloc(sizeof(**mas) * width);	// ������ �� ������ **mas �������� int
	}

	char* s, c;
	s = (char*)malloc(sizeof(char));


	for (int i = 0, m = 1, j = 0; j < width; j++) {		// ��������� ������ ������� 

		while ((c = getchar()) != '\n') {

			if (c < '0' && c > '9' && c != ' ') {
				printf("ERROR: the entered values is wrong!\n");
				return -1;
			}

			if (c != ' ') {
				s[m - 1] = c;

				s = (char*)realloc(s, sizeof(char) * (++m));
			}

			else {
				s[m - 1] = '\0';

				if (i >= width) {
					printf("ERROR: numders that you entered more than entered width!\n ");
					return -1;
				}

				mas[i][j] = atoi(s);
				i++;
				m = 1;
				
				free_mas(s);	
				
			}

		}

		s[m - 1] = '\0';

		if (i >= width) {
			printf("ERROR: numders that you entered more than entered width!\n ");
			return -1;
		}

		mas[i][j] = atoi(s);

		if (i < width) {             // ������ ����� �� 0 ���� ��� �� �������
			++i;					 // ���� �������
			while (i < width)
				mas[i++][j] = 0;
		}

		i = 0;
		m = 1;
		
		free_mas(s);
	}

	free(s);
	s = NULL;

	//for (int j = 0; j < width; j++)											// ������� ����������� ������
	//	for (int i = 0; i < width; i++)
	//		printf("%d\t%s", mas[i][j], ((i == width - 1) ? "\n" : ""));

	FILE* fout;
	fout = fopen("graph.gv", "w");

	if (fout == NULL) {
		perror("fopen()");
		return EXIT_FAILURE;
	}

	write_dot(fout, mas, width);

	checking_graph(mas, width);


	for (int i = 0; i < width; i++)
		free(mas[i]);
	free(mas);
	mas = NULL;

	system("dot -Tjpg graph.gv -o file.jpg");
	return 0;
}
