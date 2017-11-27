#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <inttypes.h>
#include <locale.h>

uint8_t menu() {
	uint8_t ch = 0;

	system("cls");

	printf("Меню:\n");
	printf("1 - Добавить строки в массив.\n");
	printf("2 - Печать массива строк.\n");
	printf("3 - Очистить массив строк.\n");
	printf("4 - Создать выходной файл.\n");
	printf("5 - Печать выходного файла.\n");
	printf("6 - Записать массив строк в файл.\n");
	printf("7 - Выход.\n");
	printf("Ваш выбор: ");
	scanf("%hhu", &ch);

	return ch;
}

void pass() {
	while (getchar() != '\n');
}

char* createLine() {
	char* line = (char*)calloc(1, sizeof(char));
	char ch = 0;
	uint8_t ln = 1;
	uint8_t ind = 0;

	while ((ch = getc(stdin)) != EOF) {
		if (ln == ind) {
			line = (char*)realloc(line, (ln *= 2) * sizeof(char));
		}

		if (ch == '\n') {
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
	
	uint8_t ch = 0;

	char** lines = NULL; 
	uint8_t nln = 0;

	FILE* fpout = NULL;
	char* fname = NULL;

	while ((ch = menu()) != 7) {
		switch (ch) {
		//добавление строк в массив
		case 1: {
			pass();
			char* line = NULL;

			printf("Введите строку или нажмите Ввод для выхода: ");
			while ((line = createLine()) != NULL && line[0] != '\0') {
				lines = (char**)realloc(lines, (nln + 1) * sizeof(char*));
				*(lines + nln++) = line;
				printf("Введите строку или нажмите Ввод для выхода: ");
			}
			free(line);
			break;
		}

		//печать массива строк
		case 2:
			for (uint8_t i = 0; i < nln; i++) {
				fputs(*(lines + i), stdout);
				putc('\n', stdout);
			}
			break;

		//очистка массива строк
		case 3:
			for (uint8_t i = 0; i < nln; i++) {
				free(*(lines + i));
			}
			free(lines);
			lines = NULL;
			nln = 0;
			break;

		//создание файла для вывода
		case 4:
			pass();
			if (fpout) {
				fclose(fpout);
			}
			printf("Введите имя файла: ");
			fname = createLine();

			if ((fpout = fopen(fname, "w+")) == NULL) {
				printf("Ошибка при создании файла.\n");
			}
			else {
				printf("Файл создан.\n");
			}
			break;
			
		//печать файла
		case 5:
			if (fpout) {
				printf("Содержимое файла:\n");
				printFile(fpout);
			}
			break;

		//запись строк в файл
		case 6:
			if (fpout) {
				_chsize(_fileno(fpout), 0);
				fseek(fpout, 0, SEEK_END);
				for (uint8_t i = 0; i < nln; i++) {
					fputs(*(lines + i), fpout);
					putc('\n', fpout);
				}
			}
			break;

		default:
			pass();
			printf("Неправильный ввод.\n");
			break;
		}
		printf("Нажмите любую клавишу.\n");
		_getch();
	}

	for (uint8_t i = 0; i < nln; i++) {
		free(*(lines + i));
	}
	free(lines);

	if (fpout) {
		fclose(fpout);
	}

	return 0;
}