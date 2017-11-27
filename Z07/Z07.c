#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <locale.h>
#include <ctype.h>
#include <string.h>

uint8_t menu() {
	uint8_t ch = 0;

	system("cls");

	printf("Меню:\n");
	printf("1 - Открыть файл.\n");
	printf("2 - Печать файла.\n");
	printf("3 - Замена слова в файле.\n");
	printf("4 - Выход.\n");
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

int32_t fileSize(FILE* fl) {
	int32_t pos = ftell(fl);
	fseek(fl, 0, SEEK_END);
	int32_t len = ftell(fl);
	fseek(fl, pos, SEEK_SET);

	return len;
}

int main(void) {
	setlocale(LC_ALL, "Russian");

	uint32_t ch = 0;
	FILE* fpin = NULL;
	char* fname = NULL;

	while ((ch = menu()) != 4) {
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

			if ((fpin = fopen(fname, "r+")) == NULL) {
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
			pass();

			char* replace = NULL;
			char* substitude = NULL;
			char* curword = NULL;

			printf("Введите заменяемое слово: ");
			replace = createLine(stdin);

			printf("Введите заменяющее слово: ");
			substitude = createLine(stdin);

			uint8_t lenrep = strlen(replace);
			uint8_t lensub = strlen(substitude);

			fseek(fpin, 0, SEEK_SET);

			if (lensub <= lenrep) {
				uint8_t delta = lenrep - lensub;

				while (curword = createWord(fpin)) {
					if (!strcmp(curword, replace)) {
						int32_t flen = fileSize(fpin);
						fseek(fpin, -lenrep, SEEK_CUR);
						fputs(substitude, fpin);

						uint16_t curpos = ftell(fpin);
						uint16_t startpos = curpos;

						while (curpos + delta < flen) {
							fseek(fpin, delta, SEEK_CUR);
							uint8_t ch = getc(fpin);
							fseek(fpin, -delta - 1, SEEK_CUR);
							putc(ch, fpin);
							curpos = ftell(fpin);
						}

						fseek(fpin, startpos, SEEK_SET);
						_chsize(_fileno(fpin), flen - delta);
						fflush(fpin);
					}

					free(curword);
				}
			}
			else {
				uint8_t delta = lensub - lenrep;

				while (curword = createWord(fpin)) {
					if (!strcmp(curword, replace)) {
						uint16_t startpos = ftell(fpin);
						_chsize(_fileno(fpin), fileSize(fpin) + delta);
						fseek(fpin, 0, SEEK_END);
						uint16_t curpos = ftell(fpin);

						while (curpos - delta > startpos) {
							fseek(fpin, -delta - 1, SEEK_CUR);
							uint8_t ch = getc(fpin);
							fseek(fpin, delta - 1, SEEK_CUR);
							putc(ch, fpin);
							fseek(fpin, -1, SEEK_CUR);
							curpos = ftell(fpin);
						}

						fseek(fpin, -lenrep - delta, SEEK_CUR);
						curpos = ftell(fpin);
						fputs(substitude, fpin);
						fflush(fpin);
					}

					free(curword);
				}
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

	free(fname);
	if (fpin) {
		fclose(fpin);
	}

	return 0;
}
