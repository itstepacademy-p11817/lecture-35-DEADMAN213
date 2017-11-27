#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <inttypes.h>
#include <conio.h>
#include <ctype.h>
#include <string.h>

#define MENUNUM 4

enum switchPoints {
	OPENFL = 1, 
	PRINTFL, 
	COUNTWRD, 
	EXIT
};

const char* menuPoints[] = {
	"Меню:\n",
	"Открыть файл.\n",
	"Печать файла.\n",
	"Подсчет слова в файле.\n",
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
	char* line = (char*)calloc(1, sizeof(char));
	*(line) = '\0';
	char ch = 0;
	uint16_t ln = 1;
	uint16_t ind = 0;

	while (!feof(fl)) {
		ch = getc(fl);
		if (ln == ind) {
			line = (char*)realloc(line, (ln *= 2) * sizeof(char));
		}

		if (ch == '\n' || ch == EOF) {
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
	char* fname = NULL;
	FILE* fpin = NULL;

	while ((ch = menu()) != EXIT) {
		switch (ch) {
		case OPENFL:
			pass();
			if (fpin) {
				fclose(fpin);
			}
			if (fname) {
				free(fname);
			}

			printf("Введите имя файла: ");
			fname = createLine(stdin);

			if ((fpin = fopen(fname, "r+")) == NULL) {
				printf("Ошибка при открытии файла.\n");
			}
			else {
				printf("Файл открыт.\n");
			}

			break;

		case PRINTFL:
			pass();
			if (fpin) {
				printf("Содержимое файла:\n");
				printFile(fpin);
			}
			else {
				printf("Откройте файл для печати.\n");
			}

			break;

		case COUNTWRD: {
			pass(); 
			fseek(fpin, 0, SEEK_SET);

			printf("Введите слово для поиска: ");
			char* word = createLine(stdin);
			char* curword = NULL;
			uint8_t num = 0;

			while (curword = createWord(fpin)) {
				if (!strcmp(curword, word)) {
					num++;
				}
			}
			
			printf("Слово \"%s\" встречается %hhu раз.\n", word, num);

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

	return 1;
}
