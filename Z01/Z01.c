#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <ctype.h>
#include <stdbool.h>
#include <inttypes.h>

FILE* printFile(FILE* fl) {
	char ch = 0;
	int32_t pos = ftell(fl);

	if (fl) {
		fseek(fl, 0, SEEK_SET);
		while ((ch = getc(fl)) != EOF) {
			putc(ch, stdout);
		}
	}

	fseek(fl, pos, SEEK_SET);

	return fl;
}

uint8_t menu() {
	uint8_t ch = 0;

	system("cls");

	printf("Меню:\n");
	printf("1 - Открытие файла.\n");
	printf("2 - Печать содержимого файла.\n");
	printf("3 - Ввод символа для поиска.\n");
	printf("4 - Подсчет количества символов.\n");
	printf("5 - Выход.\n");
	printf("Ваш выбор: ");
	scanf("%hhu", &ch);

	return ch;
}

void pass() {
	while (getchar() != '\n');
}

int main(void) {
	uint8_t ch = 0;
	char smb = 0;
	FILE* fp = NULL;

	setlocale(LC_ALL, "Russian");

	while ((ch = menu()) != 5) {
		switch (ch) {
		case 1: {
			char* fname = (char*)calloc(1, sizeof(char));
			char ch = 0;
			uint8_t ln = 1;
			uint8_t ind = 0;

			if (fp) {
				fclose(fp);
			}

			pass();
			printf("Введите имя файла: ");
			while ((ch = getc(stdin)) != EOF) {
				if (ln == ind) {
					fname = (char*)realloc(fname, (ln *= 2) * sizeof(char));
				}

				if (ch == '\n') {
					*(fname + ind) = '\0';
					fname = (char*)realloc(fname, (ind + 1) * sizeof(char));
					break;
				}
				else {
					*(fname + ind++) = ch;
				}
			}

			if ((fp = fopen(fname, "r")) == NULL) {
				printf("Ошибка при открытии файла или файл с таким именем не найден.\n");
			}
			else {
				printf("Файл открыт.\n");
			}

			break;
		}

		case 2:
			printFile(fp);
			break;

		case 3:
			printf("Введите символ для поиска: ");
			pass();
			smb = getc(stdin);
			break;

		case 4: {
			char ch = 0;
			uint8_t num = 0;
			bool flag = true;

			if (fp) {
				printf("Содержимое файла: ");
				printFile(fp);
				putchar('\n');

				fseek(fp, 0, SEEK_SET);
				while ((ch = getc(fp)) != EOF) {
					if (isalpha(ch) && flag && ch == smb) {
						num++;
						flag = false;
					}
					else if (!isalpha(ch)) {
						flag = true;
					}
					else {
						flag = false;
					}
				}

				printf("Слов начинающихся с символа \"%c\" %hhu.\n", smb, num);
			}
			else {
				printf("Файл не открыт.\n");
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

	if (fp) {
		fclose(fp);
	}

	return 0;
}