#ifndef S21_SSCANF_H
#define S21_SSCANF_H

#include "s21_string.h"

#define BUFF_SIZE 1024

// перечисление для ширины
typedef enum widthType {
  WIDTH_NONE,
  WIDTH_STAR,   // аргумент звездочка
  WIDTH_NUMBER  // минимальное количество печатаемых символов
} widthType;

// перечисление для аргументов длины
typedef enum lengthType {
  LENGTH_NONE,
  LENGTH_SHORT,       // флаг h
  LENGTH_LONG_INT,    // флаг l
  LENGTH_LONG_DOUBLE  // флаг L
} lengthType;

// структура под распаршенные токены
typedef struct token {
  char* str;     // строка перед %
  int wasSpace;  // 1 если предыдущая строка закончилась пробелом
  unsigned int width;     // ширина
  int type;               // спецификатор
  int skipPercent;        // 1 если % пропущен
  size_t prevStrLen;      // длина предыдущей строки
  widthType widthType;    // число или *
  lengthType lengthType;  // из enum
} token;

// разбор строки, парсинг токенов
void parseFormatString(char*, token*, int*);
void parseToken(char* formatString, int* i, token* tokens, int* j);
void parseLiteral(char* formatString, int* i, token* tokens, int j, int len);

// определение типа длины токена
int determineLengthType(char*, int*);

// пропуск символов в строке (пробелы, интеджеры, флоаты)
void skipSpacesAndUpdateToken(char*, int*, token*, int);
char* skipIntegerNumber(char*);
char* skipFloatNumber(char*, int);
char* skipExponential(char*, int*, int*);
char* skipPointAndDigits(char*, int*, int*, int*);
char* skipDigits(char*, int*, int*);
char* skipSignAndPoint(char*, int*, int*);

// блок валидирующих функций
int validateIntegerBase(char*, int, int*);
int isValidFloatFormat(char*, int);
int isValidOctalFormat(char*, int);
int isValidHexFormat(char*, int);
int isValidDecimalFormat(char*, int);

// парсинг числа из строки
void parseUnsignedFromString(token, char**, va_list, int);
void parseNumberFromString(token, char**, va_list, int);

// обработка токенов из строки
void processTokens(token*, int, char*, int*, va_list);
void processToken(token*, int, char**, va_list, int*, int*, s21_size_t);

// обновляет указатель чтобы указывать на следующий символ после числа
void skipNumber(char** string, int base, int width);
void skipDecimalNumber(char** string, int width);
void skipHexNumber(char** string, int width);
void skipOctalNumber(char** string, int width);

// вспомогательные функции для processTokens для каждого спецификатора
void processDecimalToken(token*, int, char**, va_list, int*, int*);
void processIntegerToken(token*, int, char**, va_list, int*, int*);
void processUnsignedToken(token*, int, char**, va_list, int*, int*);
void processHexToken(token*, int, char**, va_list, int*, int*);
void processOctalToken(token*, int, char**, va_list, int*, int*);
void processCharToken(token*, int, char**, va_list, int*);
void processStringToken(token*, int, char**, va_list, int*, int*);
void processPointerToken(token*, int, char**, va_list, int*, int*);
void processFloatToken(token*, int, char**, va_list, int*, int*);
void processTokenCount(char**, va_list, s21_size_t);

// вспомогательные функции
int isSpace(char);  // проверка символа на пробел
long s21_strtol(const char*, int);  // str в long int
long double s21_atold(char*);       // str в long double

#endif  // S21_SSCANF_H
