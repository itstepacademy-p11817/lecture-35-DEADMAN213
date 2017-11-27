#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <locale.h>
#include <string.h>

uint8_t menu() {
	uint8_t ch = 0; 

	system("cls");

	printf("Меню:\n");
	printf("1 - Открыть файл.\n");
	printf("2 - Печать файла.\n");
	printf("3 - Подсчет количества символов в файле.\n");
	printf("4 - Подсчитать количество строк в файле.\n");
	printf("5 - Вывести самую длинную строку.\n");
	printf("6 - Выход.\n");
	printf("Ваш выбор: ");
	scanf("%hhu", &ch);

	return ch;
}

void pass() {
	while (getchar() != '\n');
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

	uint32_t ch = 0;
	FILE* fpin = NULL;
	char* fname = NULL;

	while ((ch = menu()) != 6) {
		switch (ch) {
		case 1:
			pass();
			if (fpin) {
				fclose(fpin);
			}
			if (fname) {
				free(fname);
			}

			printf("Введите имя файла: ");
			fname = createLine(stdin);

			if ((fpin = fopen(fname, "r")) == NULL) {
				printf("Ошибка при открытии файла.\n");
			}
			else {
				printf("Файл открыт.\n");
			}

			break;

		case 2:
			if (fpin) {
				printf("Содержимое файла:\n");
				printFile(fpin);
			}
			break;

		case 3: {
			uint16_t num = 0;
			char ch = 0;

			if (fpin) {
				fseek(fpin, 0, SEEK_SET);
				while ((ch = getc(fpin)) != EOF) {
					num++;
				}

				printf("В файле %hu символов.\n", num);
			}

			break;
		}

		case 4: {
			uint8_t ln = 0;
			char* line = NULL;

			if (fpin) {
				fseek(fpin, 0, SEEK_SET);
				while (!feof(fpin)) {
					line = createLine(fpin);
					free(line);
					ln++;
				}
				printf("В файле %hhu строк.\n", ln);
			}

			break;
		}

		case 5: {
			char* curline = NULL;
			char* bigline = (char*)calloc(1, sizeof(char));
			*bigline = '\0';

			if (fpin) {
				fseek(fpin, 0, SEEK_SET);
				while (!feof(fpin)) {
					curline = createLine(fpin);
					if (strlen(curline) > strlen(bigline)) {
						free(bigline);
						bigline = curline;
					}
					else {
						free(curline);
					}
				}

				printf("Самая длинная строка файла: ");
				fputs(bigline, stdout);
				free(bigline);
			}

			break;
		}

		default:
			pass();
			printf("Неправильный ввод.\n");
			break;
		}
		printf("Нажмите любую клавишу.\n");
		_getch();
	}

	if (fpin) {
		fclose(fpin);
	}

	return 0;
}