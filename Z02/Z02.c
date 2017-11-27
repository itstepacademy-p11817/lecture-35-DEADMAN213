#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <locale.h>

uint8_t menu() {
	uint8_t ch = 0;

	system("cls");

	printf("Меню:\n");
	printf("1 - Открыть входной файл.\n");
	printf("2 - Создать выходной файл.\n");
	printf("3 - Печать файлов.\n");
	printf("4 - Ввести заменяемый символ.\n");
	printf("5 - Ввести замещающий символ.\n");
	printf("6 - Скопировать входной файл с заменой.\n");
	printf("7 - Выход.\n");
	printf("Ваш выбор: ");
	scanf("%hhu", &ch);

	return ch;
}

void pass() {
	while (getchar() != '\n');
}

char* createFileName() {
	char* fname = (char*)calloc(1, sizeof(char));
	char ch = 0;
	uint8_t ln = 1;
	uint8_t ind = 0;

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

	return fname;
}

void printFile(FILE* fl) {
	char ch = 0;

	int32_t pos = ftell(fl);
	fseek(fl, 0, SEEK_SET);
	pos = ftell(fl);

	while ((ch = getc(fl)) != EOF) {
		putc(ch, stdout);
	}

	fseek(fl, pos, SEEK_SET);
}

int main(void) {
	setlocale(LC_ALL, "Russian");
	
	char rep = 0;
	char sub = 0;
	uint8_t ch = 0;

	FILE* fpin = NULL;
	FILE* fpout = NULL;

	while ((ch = menu()) != 7) {
		switch (ch) {
		case 1:
			pass();
			if (fpin) {
				fclose(fpin);
			}
			printf("Введите имя файла: ");

			if ((fpin = fopen(createFileName(), "r")) == NULL) {
				printf("Ошибка при открытии файла или файл с таким именем не найден.\n");
			}
			else {
				printf("Файл открыт.\n");
			}

			break;

		case 2:
			pass();
			if (fpout) {
				fclose(fpout);
			}
			printf("Введите имя файла: ");

			if ((fpout = fopen(createFileName(), "w+")) == NULL) {
				printf("Ошибка при создании файла.\n");
			}
			else {
				printf("Файл создан.\n");
			}
			break;

		case 3:
			if (fpin) {
				printf("Входной файл: ");
				printFile(fpin);
				putchar('\n');
			}

			if (fpout) {
				printf("Выходной файл: ");
				printFile(fpout);
				putchar('\n');
			}

			break;

		case 4:
			pass();
			printf("Введите заменяемый символ: ");
			scanf("%c", &rep);
			break;

		case 5:
			pass();
			printf("Введите заменяющий символ: ");
			scanf("%c", &sub);
			break;

		case 6: {
			char ch = 0;

			if (fpin && fpout) {
				fseek(fpin, 0, SEEK_SET);
				fseek(fpout, 0, SEEK_SET);
				
				while ((ch = getc(fpin)) != EOF) {
					if (ch == rep) {
						ch = sub;
					}
					putc(ch, fpout);
				}
				fflush(fpout);
			}
			break;
		}

		default:
			printf("Неправильный ввод.\n");
			break;
		}
		printf("Нажмите любую клавишу.\n");
		_getch();
	}

	if (fpin) {
		fclose(fpin);
	}
	if (fpout) {
		fclose(fpout);
	}

	return 0;
}