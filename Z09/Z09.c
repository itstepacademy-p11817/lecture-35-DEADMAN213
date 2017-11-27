#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <locale.h>
#include <inttypes.h>
#include <conio.h>
#include <ctype.h>
#include <string.h>

#define MENUNUM 4

enum switchPoints {
	OPENFLS = 1,
	PRINTFLS,
	COUNTLNS,
	EXIT
};

const char* menuPoints[] = {
	"Меню:\n",
	"Открыть файлы.\n",
	"Печать файлов.\n",
	"Подсчет совпадающих строк.\n",
	"Выход.\n"
};

uint8_t menu() {
	uint8_t ch = 0;

	system("cls");

	printf(menuPoints[0]);
	for (uint8_t i = 1; i <= MENUNUM; i++) {
		printf("%hhu - ", i);
		printf(menuPoints[i]);
	}

	printf("Ваш выбор: ");
	scanf("%hhu", &ch);

	return ch;
}

void pass() {
	while (getchar() != '\n');
}

void passChar(FILE* fl) {
	char ch = 0;
	while ((ch = getc(fl)) != EOF && !isalpha(ch));
	if (!feof(fl)) {
		fseek(fl, -1, SEEK_CUR);
	}
}

char* createLine(FILE* fl) {
	char* line = NULL;
	char ch = 0;
	uint16_t ln = 0;
	uint16_t ind = 0;

	while (!feof(fl)) {
		ch = getc(fl);

		if (ln == ind) {
			ln++;
			line = (char*)realloc(line, (ln *= 2) * sizeof(char));
		}

		if (ch == '\n' || ch == EOF) {
			if (ind == 0 && ch == EOF) {
				free(line);
				line = NULL;
				break;
			}
			else if (ind == 0 && ch == '\n') {
				line = (char*)realloc(line, sizeof(char));
				*line = '\n';
			}

			*(line + ind) = '\0';
			line = (char*)realloc(line, (ind + 1) * sizeof(char));
			break;
		}
		else {
			*(line + ind++) = ch;
		}
	}

	return line;
}

char* createWord(FILE* fl) {
	char* line = NULL;
	char ch = 0;
	uint16_t ln = 0;
	uint16_t ind = 0;

	passChar(fl);
	while (!feof(fl)) {
		ch = getc(fl);
		if (ln == ind) {
			ln++;
			line = (char*)realloc(line, (ln *= 2) * sizeof(char));
		}

		if (!isalpha(ch)) {
			*(line + ind) = '\0';
			line = (char*)realloc(line, (ind + 1) * sizeof(char));
			break;
		}
		else {
			*(line + ind++) = ch;
		}
	}

	if (!feof(fl)) {
		fseek(fl, -1, SEEK_CUR);
	}

	return line;
}

void printLines(FILE* fl1, FILE* fl2) {
	bool flag = false;
	char* line1 = NULL;
	char* line2 = NULL;

	fseek(fl1, 0, SEEK_SET);
	while (line1 = createLine(fl1)) {
		flag = true;
		fseek(fl2, 0, SEEK_SET); 

		while (line2 = createLine(fl2)) {
			if (strcmp(line1, line2) == 0) {
				flag = false;
				free(line2);
				break;
			}
			free(line2);
		}

		if (flag) {
			puts(line1);
		}
		free(line1);
	}
}

void printFile(FILE* fl) {
	char ch = 0;
	int32_t pos = ftell(fl);

	fseek(fl, 0, SEEK_SET);
	while ((ch = getc(fl)) != EOF) {
		putc(ch, stdout);
	}
	putc('\n', stdout);
	fseek(fl, pos, SEEK_SET);
}

int main(void) {
	setlocale(LC_ALL, "Russian");

	uint8_t ch = 0;

	char* fname1 = NULL;
	char* fname2 = NULL;

	FILE* fpin1 = NULL;
	FILE* fpin2 = NULL;

	while ((ch = menu()) != EXIT) {
		switch (ch) {
		case OPENFLS:
			pass();
			if (fpin1) {
				fclose(fpin1);
			}
			if (fname1) {
				free(fname1);
			}

			if (fpin2) {
				fclose(fpin2);
			}
			if (fname2) {
				free(fname2);
			}

			printf("Введите имя первого файла: ");
			fname1 = createLine(stdin);

			printf("Введите имя второго файла: ");
			fname2 = createLine(stdin);

			if ((fpin1 = fopen(fname1, "r+")) == NULL || (fpin2 = fopen(fname2, "r+")) == NULL) {
				printf("Ошибка при открытии файлов.\n");
			}
			else {
				printf("Файлы открыты.\n");
			}

			break;

		case PRINTFLS:
			pass();
			if (fpin1 && fpin2) {
				printf("Содержимое первого файла:\n");
				printFile(fpin1);

				printf("\nСодержимое второго файла:\n");
				printFile(fpin2);
			}
			else {
				printf("Откройте файл для печати.\n");
			}

			break;

		case COUNTLNS: {
			printf("Строки первого файла, которых нет во втором файле:\n");
			printLines(fpin1, fpin2);

			printf("Строки второго файла, которых нет в первом файле:\n");
			printLines(fpin2, fpin1);

			break;
		}

		default:
			printf("Неверный ввод.\n");
			pass();
			break;
		}

		printf("Нажмите любую клавишу.\n");
		_getch();
	}

	if (fname1) {
		free(fname1);
	}
	if (fname2) {
		free(fname2);
	}

	if (fpin1) {
		fclose(fpin1);
	}
	if (fpin2) {
		fclose(fpin2);
	}

	return 1;
}
