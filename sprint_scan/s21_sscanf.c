#include "../s21_sscanf.h"

int s21_sscanf(const char* inputStr, const char* format, ...) {
  int argCount = -1;  // количество успешно считанных аргументов
  char* string = (char*)inputStr;

  if (*inputStr) {
    argCount++;
    token tokens[BUFF_SIZE] = {0};
    int tokenCount = 0;

    char* formatString = (char*)format;
    parseFormatString(formatString, tokens, &tokenCount); 
    va_list ap;
    va_start(ap, format);
    processTokens(tokens, tokenCount, string, &argCount, ap);
    va_end(ap);
  }
  return argCount;
}

void processTokens(token* tokens, int tokenCount, char* string, int* argCount, va_list ap) {
  int flag = 1;  
  s21_size_t string_len = s21_strlen(string);
  for (int i = 0; i < tokenCount && flag; i++) {
    while (flag && tokens[i].str != s21_NULL && tokens[i].prevStrLen != 0 && *string != '\0') {
      if (!isSpace(*string)) {
        if (*string == *(tokens[i].str)) {
          string++;
          tokens[i].str++;
          tokens[i].prevStrLen--;
          while (isSpace(*string)) {
            string++;
          }
        } else {
          flag = 0;
        }
      } else {
        string++;
      }
    }
    if (flag) {
      while ((isSpace(*string) && (tokens[i].wasSpace == 1 || !(tokens[i].type == 'c'))) || (*string == '%' && tokens[i].skipPercent == 1)) {
        string++;
      }
      if (flag && tokens[i].type != 0) {
        processToken(tokens, i, &string, ap, argCount, &flag, string_len);
      }
    }
  }
}

void processToken(token* tokens, int tokenIndex, char** string, va_list ap, int* argCount, int* flag, s21_size_t string_len) {

  if (tokens[tokenIndex].type == 'd') {
    processDecimalToken(tokens, tokenIndex, string, ap, argCount, flag);
  } else if (tokens[tokenIndex].type == 'i') {
    processIntegerToken(tokens, tokenIndex, string, ap, argCount, flag);
  } else if (tokens[tokenIndex].type == 'u') {
    processUnsignedToken(tokens, tokenIndex, string, ap, argCount, flag);
  } else if (tokens[tokenIndex].type == 'x' || tokens[tokenIndex].type == 'X' ) {
    processHexToken(tokens, tokenIndex, string, ap, argCount, flag);
  } else if (tokens[tokenIndex].type == 'o') {
    processOctalToken(tokens, tokenIndex, string, ap, argCount, flag);
  } else if (tokens[tokenIndex].type == 'c') {
    processCharToken(tokens, tokenIndex, string, ap, argCount);
  } else if (tokens[tokenIndex].type == 's') {
    processStringToken(tokens, tokenIndex, string, ap, argCount, flag);
  } else if (tokens[tokenIndex].type == 'p') {
    processPointerToken(tokens, tokenIndex, string, ap, argCount, flag);
  } else if (tokens[tokenIndex].type == 'n') { 
    processTokenCount(string, ap, string_len);
  } else if (tokens[tokenIndex].type == 'e' || tokens[tokenIndex].type == 'E' || tokens[tokenIndex].type == 'g' || tokens[tokenIndex].type == 'G' || tokens[tokenIndex].type == 'f') {
    processFloatToken(tokens, tokenIndex, string, ap, argCount, flag);
  } else {
    ;
  }
}

void processCharToken(token* tokens, int i, char** string, va_list ap, int* argCount) {
  if (tokens[i].widthType != WIDTH_STAR) {
    (*argCount)++;
  }
  if (tokens[i].widthType == WIDTH_STAR) {
    (*string)++;
  } else if (tokens[i].widthType == WIDTH_NUMBER) {
    if (tokens[i].width == 0) {
      tokens[i].width = 1;
    }
    if (tokens[i].lengthType == LENGTH_NONE) {
      char* arg = va_arg(ap, char*);
      s21_strncpy(arg, *string, tokens[i].width);
      arg[tokens[i].width] = '\0';
      (*string) += tokens[i].width;
    } else if (tokens[i].lengthType == LENGTH_LONG_INT) {
      wchar_t* arg = va_arg(ap, wchar_t*);
      mbstowcs(arg, *string, tokens[i].width);
      arg[tokens[i].width] = '\0';
      (*string) += tokens[i].width;
    }
  } else {
    if (tokens[i].lengthType == LENGTH_NONE) {
      char* arg = va_arg(ap, char*);
      *arg = **string;
      (*string)++;
    } else if (tokens[i].lengthType == LENGTH_LONG_INT) {
      wchar_t* arg = va_arg(ap, wchar_t*);
      mbstowcs(arg, *string, 1);
      (*string)++;
    }
  }
}

void processStringToken(token* tokens, int i, char** string, va_list ap, int* argCount, int* flag) {
  if (**string == '\0') {
    *flag = 0;
  }
  if (*flag) {
    if (tokens[i].widthType != WIDTH_STAR) {
      (*argCount)++;
    }
    if (tokens[i].widthType == WIDTH_STAR) {
      while (**string != '\0' && !isSpace(**string)) {
        (*string)++;
      }
    } else {
      if (tokens[i].lengthType == LENGTH_NONE) {
        char* arg = va_arg(ap, char*);
        unsigned int j = 0;
        while ((j < tokens[i].width || tokens[i].widthType == WIDTH_NONE) && **string != '\0' && !isSpace(**string)) {
          arg[j] = **string;
          j++;
          (*string)++;
        }
        arg[j] = '\0';
      } else if (tokens[i].lengthType == LENGTH_LONG_INT) {
        wchar_t* arg = va_arg(ap, wchar_t*);
        unsigned int j = 0;
        while ((j < tokens[i].width || tokens[i].widthType == WIDTH_NONE) && !isSpace(**string) && **string != '\0') {
          mbstowcs(&arg[j], *string, 1);
          j++;
          (*string)++;
        }
        arg[j] = '\0';
      }
    }
    while (**string == ' ') {
      (*string)++;
    }
  }
}

void processPointerToken(token* tokens, int i, char** string, va_list ap, int* argCount, int* flag) {
  *flag = isValidHexFormat(*string, tokens[i].width);
  if (*flag) {
    if (tokens[i].widthType != WIDTH_STAR) {
      (*argCount)++;
    }
    if (tokens[i].widthType == WIDTH_STAR) {
      skipHexNumber(string, 0);
    } else {
      char* tmp = (char*)malloc((s21_strlen(*string) + 3) * sizeof(char));
      tmp[0] = '0';
      tmp[1] = 'x';
      tmp[2] = '\0';
      s21_strcpy(tmp + 2, *string);
      parseUnsignedFromString(tokens[i], string, ap, 16);
      free(tmp);
    }
  }
}

void processFloatToken(token* tokens, int i, char** string, va_list ap, int* argCount, int* flag) {
  *flag = isValidFloatFormat(*string, tokens[i].width);
  if (*flag) {
    if (tokens[i].widthType == WIDTH_STAR) {
      *string = skipFloatNumber(*string, tokens[i].width);
    } else {
      (*argCount)++;
      void* arg = va_arg(ap, void*);
      char* tmp = s21_NULL;
      if (tokens[i].widthType == WIDTH_NUMBER) {
        tmp = (char*)malloc((tokens[i].width + 1) * sizeof(char));
        if (tokens[i].width > s21_strlen(*string)) {
          s21_strncpy(tmp, *string, s21_strlen(*string));
          tmp[s21_strlen(*string)] = '\0';
        } else {
          s21_strncpy(tmp, *string, tokens[i].width);
          tmp[tokens[i].width] = '\0';
        }
      } else {
        tmp = (char*)malloc((s21_strlen(*string) + 1) * sizeof(char));
        s21_strcpy(tmp, *string);
      }
      if (tokens[i].lengthType == LENGTH_LONG_DOUBLE) {
        *((long double*)arg) = (long double)s21_atold(tmp);
      }
      else if (tokens[i].lengthType == LENGTH_LONG_INT) {
        *((double*)arg) = (double)s21_atold(tmp);
      }
      else {
        *((float*)arg) = (float)s21_atold(tmp);
      }
      if (tmp) {
        free(tmp);
      }
      *string = skipFloatNumber(*string, tokens[i].width);
    }
  }
}

char* skipFloatNumber(char* str, int width) {
  int flag = 0, point_flag = 0;
  if (width == 0) {
    width--;
  }

  str = skipSignAndPoint(str, &width, &point_flag);
  str = skipDigits(str, &width, &flag);
  str = skipPointAndDigits(str, &width, &flag, &point_flag);
  str = skipDigits(str, &width, &flag);
  str = skipExponential(str, &width, &flag);
  str = skipDigits(str, &width, &flag);

  return str;
}

void parseUnsignedFromString(token token, char** string, va_list ap, int base) {
  if (token.widthType == WIDTH_STAR) {
    skipNumber(string, base, BUFF_SIZE);
  } else {
    if (token.lengthType == LENGTH_NONE) {
      if (token.widthType == WIDTH_NUMBER) {
        unsigned int* arg = va_arg(ap, unsigned int*);
        char* tmp = (char*)malloc((token.width + 1) * sizeof(char));
        s21_strncpy(tmp, *string, token.width);
        tmp[token.width] = '\0';
        *arg = s21_strtol(tmp, base);
        free(tmp);
        skipNumber(string, base, token.width);
      } else {
        unsigned int* arg = va_arg(ap, unsigned int*);
        *arg = s21_strtol(*string, base);
        skipNumber(string, base, BUFF_SIZE);
      }
    } else if (token.lengthType == LENGTH_LONG_INT) {
      if (token.widthType == WIDTH_NUMBER) {
        unsigned long* arg = va_arg(ap, unsigned long*);
        char* tmp = (char*)malloc((token.width + 1) * sizeof(char));
        s21_strncpy(tmp, *string, token.width);
        tmp[token.width] = '\0';
        *arg = s21_strtol(tmp, base);
        free(tmp);
        skipNumber(string, base, token.width);
      } else {
        unsigned long* arg = va_arg(ap, unsigned long*);
        *arg = s21_strtol(*string, base);
        skipNumber(string, base, BUFF_SIZE);
      }
    } else if (token.lengthType == LENGTH_SHORT) {
      if (token.widthType == WIDTH_NUMBER) {
        unsigned short* arg = va_arg(ap, unsigned short*);
        char* tmp = (char*)malloc((token.width + 1) * sizeof(char));
        s21_strncpy(tmp, *string, token.width);
        tmp[token.width] = '\0';
        *arg = s21_strtol(tmp, base);
        free(tmp);
        skipNumber(string, base, token.width);
      } else {
        unsigned short* arg = va_arg(ap, unsigned short*);
        *arg = s21_strtol(*string, base);
        skipNumber(string, base, BUFF_SIZE);
      }
    }
  }
}

void parseNumberFromString(token token, char** string, va_list ap, int base) {
  if (token.widthType == WIDTH_STAR) {
    skipNumber(string, base, BUFF_SIZE);
  } else {
    if (token.lengthType == LENGTH_NONE) {
      if (token.widthType == WIDTH_NUMBER) {
        int* arg = va_arg(ap, int*);
        char* tmp = (char*)malloc((token.width + 1) * sizeof(char));
        s21_strncpy(tmp, *string, token.width);
        tmp[token.width] = '\0';
        *arg = s21_strtol(tmp, base);
        free(tmp);
        skipNumber(string, base, token.width);
      } else {
        int* arg = va_arg(ap, int*);
        *arg = s21_strtol(*string, base);
        skipNumber(string, base, BUFF_SIZE);
      }
    } else if (token.lengthType == LENGTH_LONG_INT) {
      if (token.widthType == WIDTH_NUMBER) {
        long* arg = va_arg(ap, long*);
        char* tmp = (char*)malloc((token.width + 1) * sizeof(char));
        s21_strncpy(tmp, *string, token.width);
        tmp[token.width] = '\0';
        *arg = s21_strtol(tmp, base);
        free(tmp);
        skipNumber(string, base, token.width);
      } else {
        long* arg = va_arg(ap, long*);
        *arg = s21_strtol(*string, base);
        skipNumber(string, base, BUFF_SIZE);
      }
    } else if (token.lengthType == LENGTH_SHORT) {
      if (token.widthType == WIDTH_NUMBER) {
        short* arg = va_arg(ap, short*);
        char* tmp = (char*)malloc((token.width + 1) * sizeof(char));
        s21_strncpy(tmp, *string, token.width);
        tmp[token.width] = '\0';
        *arg = s21_strtol(tmp, base);
        free(tmp);
        skipNumber(string, base, token.width);
      } else {
        short* arg = va_arg(ap, short*);
        *arg = s21_strtol(*string, base);
        skipNumber(string, base, BUFF_SIZE);
      }
    }
  }
}

void skipHexNumber(char** string, int width) {
  int pre_flag = 0;

  if (width == 0) {
    width--;
  }

  if ((**string == '+' || **string == '-') && width != 1) {
    if ((*(*string + 1) >= '0' && *(*string + 1) <= '9') || (*(*string + 1) >= 'a' && *(*string + 1) <= 'f') || (*(*string + 1) >= 'A' && *(*string + 1) <= 'F')) {
      (*string)++; 
      width--;
    }
  }
  if (**string == '0' && width != 0) {
    (*string)++;
    pre_flag = 1;
  }


  if ((**string == 'x' || **string == 'X') && width != 1 && pre_flag) {
    if ((*(*string + 1) >= '0' && *(*string + 1) <= '9') || (*(*string + 1) >= 'a' && *(*string + 1) <= 'f') || (*(*string + 1) >= 'A' && *(*string + 1) <= 'F')) {
      (*string) += 2;
      width -= 2;
    }
  }

  while (((**string >= '0' && **string <= '9') || (**string >= 'a' && **string <= 'f') || (**string >= 'A' && **string <= 'F')) && width != 0) {
    (*string)++;
    width--;
  }
}

void parseFormatString(char* formatString, token* tokens, int* tokens_count) {
  int i = 0; 
  int j = 0;  
  int len = s21_strlen(formatString);

  while (i < len) {
    if (formatString[i] == '%') {
      i++;
      parseToken(formatString, &i, tokens, &j);
    } else {
      skipSpacesAndUpdateToken(formatString, &i, tokens, j);
      parseLiteral(formatString, &i, tokens, j, len);
    }
  }

  *tokens_count = j;
}

void parseToken(char* formatString, int* i, token* tokens, int* j) {
  if (formatString[*i] == '*') {
    tokens[*j].widthType = WIDTH_STAR;
    tokens[*j].width = 0;
    (*i)++;
  } else if (formatString[*i] >= '0' && formatString[*i] <= '9') {
    tokens[*j].widthType = WIDTH_NUMBER;
    tokens[*j].width = 0;
    while (formatString[*i] >= '0' && formatString[*i] <= '9') {
      tokens[*j].width = tokens[*j].width * 10 + (formatString[*i] - '0');
      (*i)++;
    }
  } else {
    tokens[*j].widthType = WIDTH_NONE;
    tokens[*j].width = 0;
  }
  tokens[*j].lengthType = determineLengthType(formatString, i);
  if (formatString[*i] == '%') {
    tokens[*j].skipPercent = 1;
    (*i)++;
  } else {
    tokens[*j].type = formatString[*i];
    (*i)++;
    (*j)++;
  }
}

void parseLiteral(char* formatString, int* i, token* tokens, int j, int len) {
  skipSpacesAndUpdateToken(formatString, i, tokens, j);
  if (tokens[j].type == 0 && formatString[*i] != '%') {
    tokens[j].str = &formatString[*i];
    tokens[j].prevStrLen = 1;
    (*i)++;
    tokens[j].wasSpace = 0;
    while (formatString[*i] != '%' && *i < len) {
      if (isSpace(formatString[*i])) {
        skipSpacesAndUpdateToken(formatString, i, tokens, j);
        break;
      }
      tokens[j].prevStrLen++;
      (*i)++;
    }
  }
}

int isValidFloatFormat(char* str, int width) {
  int result = 1;
  if (width == 0) {
    width--;
  }
  if (width == 1 && (*str == '-' || *str == '+')) {
    result = 0;
  } else if ((*str == '-' || *str == '+') && (width > 1 || width < 0) && (!(*(str + 1) >= '0' && *(str + 1) <= '9') && !(*(str + 1) == '.'))) {
    result = 0;
  } else if ((*str == '-' || *str == '+') && (*(str + 1) == '.')) {
    if (width == 2) {
      result = 0;
    }
    if (*(str + 2)) {
      if (!(*(str + 2) >= '0' && *(str + 2) <= '9')) {
        result = 0;
      }
    } else {
      result = 0;
    }
  } else if (*str == '.' && width == 1) {
    result = 0;
  } else if (*str == '.' && (width > 1 || width < 0) && !(*(str + 1) >= '0' && *(str + 1) <= '9')) {
    result = 0;
  } else if (!(*str >= '0' && *str <= '9') && !(*str == '.') && !(*str == '-') && !(*str == '+')) {
    result = 0;
  } else if ((*str == '+' && *(str + 1) == '-') || (*str == '-' && *(str + 1) == '+')) {
    result = 0;
  }
  return result;
}

long s21_strtol(const char* str, int radix) {
  int sign = 1, i = 0; 
  long int res = 0; 
  if (str[i] == '-') {
    sign = -1;
    i++;
  } else if (str[i] == '+') {
    i++;
  }
  if (str[i] == '0' && radix == 16) {
    if (str[i + 1]) {
      if (str[i + 1] == 'x' || str[i + 1] == 'X') i += 2;
    }
  }
  while (str[i]) {
    if ((!(str[i] >= '0' && str[i] <= radix + 47 && radix <= 10)) &&
        (!(((str[i] >= '0' && str[i] < '9') || (str[i] >= 'A' && str[i] <= radix + 54) || (str[i] >= 'a' && str[i] <= radix + 86)) && radix > 10 && radix < 37))) {
      break;
    }
    else {
      if (str[i] >= 'A' && str[i] <= 'Z')
        res = res * radix + (str[i] - 'A' + 10);
      else if (str[i] >= 'a' && str[i] <= 'z')
        res = res * radix + (str[i] - 'a' + 10);
      else
        res = res * radix + str[i] - '0';
      i++;
    }
  }
  return res * sign;
}

long double s21_atold(char* str) {
  int sign = 1, i = 0;
  long double inc = 0.1L;
  long double res = 0.L;
  if (str[i] == '-') {
    sign = -1;
    i++;
  } else if (str[i] == '+') {
    i++;
  }
  while (str[i] != '.' && str[i] >= '0' && str[i] <= '9') {
    res = res * 10.L + str[i] - '0';
    i++;
  }
  if (str[i] == '.') i++;
  while (str[i] && str[i] >= '0' && str[i] <= '9') {
    res = res + (str[i] - '0') * inc;
    inc /= 10.L;
    i++;
  }
  inc = 10.L;
  if (str[i] == 'e' || str[i] == 'E') {
    i++;
    if (str[i] == '-') {
      inc = 0.1L;
      i++;
    } else if (str[i] == '+') {
      inc = 10.L;
      i++;
    }
    int count = 0;
    while (str[i] >= '0' && str[i] <= '9') {
      count = count * 10.L + str[i] - '0';
      i++;
    }

    while (count) {
      res *= inc;
      count--;
    }
  }
  return res * sign;
}

void processTokenCount(char** string, va_list ap, s21_size_t string_len) {
  int* arg =
      va_arg(ap, int*);  
  s21_size_t cur_len = s21_strlen(*string);
  *arg = string_len - cur_len;
}

char* skipSignAndPoint(char* str, int* width, int* point_flag) {
  if ((*str == '+' || *str == '-') && *width != 1) {
    if (*(str + 1) >= '0' && *(str + 1) <= '9') {
      *width -= 2;
      str += 2;
    } else if (*(str + 1) == '.' && *width != 2) {
      if (*(str + 2)) {
        if (*(str + 2) >= '0' && *(str + 2) <= '9') {
          *width -= 2;
          str += 2;
          *point_flag = 1;
        }
      }
    }
  }
  return str;
}

char* skipDigits(char* str, int* width, int* flag) {
  while (*str >= '0' && *str <= '9' && *width != 0) {
    str++;
    *flag = 1;
    (*width)--;
  }
  return str;
}

char* skipPointAndDigits(char* str, int* width, int* flag, int* point_flag) {
  if (*str == '.' && !(*point_flag) && *width != 0) {
    if (*flag == 1)
      str++;
    else if (*(str + 1) >= '0' && *(str + 1) <= '9' && *width != 1)
      str++;
    (*width)--;
  }
  return str;
}

char* skipExponential(char* str, int* width, int* flag) {
  if ((*str == 'e' || *str == 'E') && *flag && *width != 1 && *width != 0) {
    if (*(str + 1) >= '0' && *(str + 1) <= '9') {
      str += 2;
      *width -= 2;
    } else if ((*(str + 1) == '-' || *(str + 1) == '+') && *width != 2) {
      if (*(str + 2)) {
        if (*(str + 2) >= '0' && *(str + 2) <= '9') {
          str += 3;
          *width -= 3;
        }
      }
    }
  }
  return str;
}

void skipOctalNumber(char** string, int width) {
  if ((**string == '+' || **string == '-') && width != 1) {
    if (*(*string + 1) >= '0' && *(*string + 1) <= '7') {
      (*string) += 2;
      width -= 2;
    }
  }
  while ((**string >= '0' && **string <= '7') && width != 0) {
    (*string)++;
    width--;
  }
}

void skipDecimalNumber(char** string, int width) {
  if ((**string == '+' || **string == '-') && width != 1) {
    if (*(*string + 1) >= '0' && *(*string + 1) <= '9') {
      (*string) += 2;
      width -= 2;
    }
  }
  while ((**string >= '0' && **string <= '9') && width != 0) {
    (*string)++;
    width--;
  }
}

int determineLengthType(char* formatString, int* i) {
  int length = 0;
  if (formatString[*i] == 'h') {
    length = LENGTH_SHORT;
    (*i)++;
  } else if (formatString[*i] == 'l') {
    length = LENGTH_LONG_INT;
    (*i)++;
  } else if (formatString[*i] == 'L') {
    length = LENGTH_LONG_DOUBLE;
    (*i)++;
  } else {
    length = LENGTH_NONE;
  }
  return length;
}

int validateIntegerBase(char* str, int width, int* base) {
  int result = 1;
  if (*str == '0') {
    if (*(str + 1) == 'x' || *(str + 1) == 'X') {
      result = isValidHexFormat(str, width);
      *base = 16;
    } else {
      result = isValidOctalFormat(str, width);
      *base = 8;
    }
  } else {
    result = isValidDecimalFormat(str, width);
    *base = 10;
  }
  return result;
}

int isValidDecimalFormat(char* str, int width) {
  int result = 1;
  if (width == 0) {
    width--;
  }
  if (*str == '+' || *str == '-') {
    if (width == 1) {
      result = 0;
    }
    else if (!(*(str + 1) >= '0' && *(str + 1) <= '9')) {
      result = 0;
    }
  } else if (!(*(str) >= '0' && *(str) <= '9')) {
    result = 0;
  }
  return result;
}

void skipNumber(char** string, int base, int width) {
  if (base == 10) {
    skipDecimalNumber(string, width);
  } else if (base == 16) {
    skipHexNumber(string, width);
  } else if (base == 8) {
    skipOctalNumber(string, width);
  }
}

int isValidHexFormat(char* str, int width) {
  int result = 1;
  if (width == 0) {
    width--;
  }
  if (*str == '-' || *str == '+') {
    if (width == 1) {
      result = 0;
    }
    else if (!((*(str + 1) >= '0' && *(str + 1) <= '9') ||
               (*(str + 1) >= 'A' && *(str + 1) <= 'F') ||
               (*(str + 1) >= 'a' && *(str + 1) <= 'f')))
      result = 0;
  } else if (!((*str >= '0' && *str <= '9') || (*str >= 'A' && *str <= 'F') || (*str >= 'a' && *str <= 'f'))) {
    result = 0;
  }
  return result;
}

int isValidOctalFormat(char* str, int width) {
  int result = 1;
  if (width == 0) {
    width--;
  }
  if (*str == '-' || *str == '+') {
    if (width == 1) {
      result = 0;
    }
    else if (!(*(str + 1) >= '0' && *(str + 1) <= '7')) {
      result = 0;
    }
  } else if (!(*str >= '0' && *str <= '7')) {
    result = 0;
  }
  return result;
}

void processDecimalToken(token* tokens, int i, char** string, va_list ap, int* argCount, int* flag) {
  *flag = isValidDecimalFormat(*string, tokens[i].width);
  if (*flag) {
    if (tokens[i].widthType != WIDTH_STAR) {
      (*argCount)++;
    }
    parseNumberFromString(tokens[i], string, ap, 10);
  }
}

void processIntegerToken(token* tokens, int i, char** string, va_list ap,
                         int* argCount, int* flag) {
  int base = 0;
  *flag = validateIntegerBase(*string, tokens[i].width, &base);
  if (*flag) {
    if (tokens[i].widthType != WIDTH_STAR) (*argCount)++;
    parseNumberFromString(tokens[i], string, ap, base);
  }
}

void processUnsignedToken(token* tokens, int i, char** string, va_list ap,
                          int* argCount, int* flag) {
  *flag = isValidDecimalFormat(*string, tokens[i].width);
  if (*flag) {
    if (tokens[i].widthType != WIDTH_STAR) (*argCount)++;
    parseUnsignedFromString(tokens[i], string, ap, 10);
  }
}

void processHexToken(token* tokens, int i, char** string, va_list ap,
                     int* argCount, int* flag) {
  *flag = isValidHexFormat(*string, tokens[i].width);
  if (*flag) {
    if (tokens[i].widthType != WIDTH_STAR) (*argCount)++;
    parseUnsignedFromString(tokens[i], string, ap, 16);
  }
}

void processOctalToken(token* const tokens, int i, char** string, va_list ap,
                       int* argCount, int* flag) {
  *flag = isValidOctalFormat(*string, tokens[i].width);
  if (*flag) {
    if (tokens[i].widthType != WIDTH_STAR) (*argCount)++;
    parseUnsignedFromString(tokens[i], string, ap, 8);
  }
}

void skipSpacesAndUpdateToken(char* formatString, int* i, token* tokens,
                              int j) {
  while (isSpace(formatString[*i])) {
    (*i)++;
    tokens[j].wasSpace = 1;
  }
}

int isSpace(char c) {
  int ret = 0;
  if (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' ||
      c == '\r') {
    ret = 1;
  }
  return ret;
}
