
#include "s21_sprintf.h"
int s21_sprintf(char *str, const char *format, ...) {
  va_list arg;
  va_start(arg, format);
  str = s21_strcpy(str, "");
  int zero_str = 0;
  int zero_str_flag = 0;
  while (*format) {
    struct s_struct formats = {0};
    formats.accuracy = -1;
    if (*format != '%') {
      s21_add(str, format, &zero_str_flag, &formats);
    }
    if (*format == '%') {
      format++;
      while (s21_check_flags(format)) {
        set_flags(&formats, format++);
      }
      while (s21_check_digital(format) || *format == '*') {
        set_width_accuracy(&formats.Wdt, format++, &arg);
        if (formats.Wdt < 0) {
          formats.Wdt *= -1;
          formats.flag_m = 1;
        }
      }
      if (*format == '.') {
        format++;
        formats.point = 1;
        formats.accuracy = 0;
        while (s21_check_digital(format) || *format == '*')
          set_width_accuracy(&formats.accuracy, format++, &arg);
      }
      if (s21_check_lenght(format)) {
        formats.flag_l = *format;
        format++;
      }
      if (check_specf(&formats, format)) {
        s21_default_precision(&formats);
      }
      s21_check(format, str, &formats, &arg, &zero_str_flag);
    }
    format++;
    zero_str += formats.zero_simbol;
  }
  va_end(arg);
  return (int)s21_strlen(str) + zero_str;
}

void s21_check(const char *format, char *str, struct s_struct *formats, va_list *arg, int *zero_str_flag) {
  char *p_str = s21_NULL;
  p_str = calloc(1024, sizeof(char));
  if (*format == 'c') {
    s21_c(p_str, arg, formats, zero_str_flag);
  } else if (*format == 's') {
    s21_s(p_str, arg, formats);
  } else if (*format == 'd') {
    s21_d(p_str, arg, formats);
  } else if (*format == 'f') {
    s21_f(p_str, arg, formats);
  } else if (*format == 'o') {
    s21_o(p_str, arg, formats);
  } else if (*format == 'u') {
    s21_u(p_str, arg, formats);
  } else if (*format =='x' || *format == 'X') {
    s21_xX(p_str, arg, formats);
  } else if (*format == 'p') {
    s21_p(p_str, arg, formats);
  } else if (*format =='e' || *format == 'E') {
    s21_eE(p_str, arg, formats);
  } else if (*format =='g' || *format == 'G') {
    s21_gG(p_str, arg, formats);
  } else if (*format =='%') {
    s21_percent(p_str, formats);
  } else {
    ;
  }
  s21_strcat(str, p_str);
  free(p_str);
}

void s21_gG(char *tmp_str, va_list *arg, struct s_struct *formats) {
  long double l_number = 0;
  double number = 0;
  if (formats->flag_l == 'L') {
    l_number = va_arg(*arg, long double);
    s21_gG_to_str(formats, tmp_str, l_number);
  } else {
    number = va_arg(*arg, double);
    s21_gG_to_str(formats, tmp_str, number);
  } 
  add_for_fdeEfgG(formats, tmp_str);
}

void s21_gG_to_str(struct s_struct *formats, char *tmp_str,
                   long double number) {
  if (number != 0) {
    formats->sign = number < 0 ? -1 : 1;
    int local_sign = formats->sign;
    number *= formats->sign;
    int safe_pricision = formats->accuracy;
    char *str_eE = s21_NULL, *str_f = s21_NULL;
    str_eE = calloc(1024, sizeof(char));
    str_f = calloc(1024, sizeof(char));
    if (number < 1 && formats->accuracy < 0) {
      formats->accuracy = 4;
    } else {
      formats->accuracy--;
    }
    s21_eE_to_str(formats, str_eE, number);
    formats->accuracy = safe_pricision;
    int point = 0, flag_int = 0;
    int t_number = roundl(number);
    if (t_number == number) {
      flag_int = 1;
    }
    while (t_number > 1) {
      t_number /= 10;
      point++;
    }
    formats->accuracy -= point;
    if (flag_int == 0) {
      s21_float_to_str(formats, str_f, number);
      s21_delete_zero(str_f, formats);
    } else {
      s21_int_to_str(formats, str_f, number);
      s21_delete_zero(str_f, formats);
    }
    formats->accuracy = safe_pricision;
    s21_strcat(tmp_str, (formats->step >= -4 && formats->accuracy > formats->step) ? str_f : str_eE);
    formats->sign = local_sign;
    free(str_eE);
    free(str_f);
  } else {
    s21_int_to_str(formats, tmp_str, 0);
  }
}
void s21_delete_zero(char *tmp_str, struct s_struct *formats) {
  int len_str = s21_strlen(tmp_str) - 1;
  if (formats->accuracy > 0)
    while (tmp_str[len_str] == '0') {
      tmp_str[len_str] = '\0';
      len_str--;
    }
}

void s21_eE(char *tmp_str, va_list *arg, struct s_struct *formats) {
  long double ld_number = 0;
  double number = 0;

  if (formats->flag_l == 'L') {
    ld_number = va_arg(*arg, long double);
    s21_eE_to_str(formats, tmp_str, ld_number);
  } else {
    number = va_arg(*arg, double);
    s21_eE_to_str(formats, tmp_str, number);
  }
  add_for_fdeEfgG(formats, tmp_str);
}

void s21_eE_to_str(struct s_struct *formats, char *tmp_str, long double number) {
  char r_line[256] = {0}, str_notat[256] = {0};

  if (number < 0) {
    formats->sign = -1;
  } else {
    formats->sign = 1;
  }



  int notation = 0; 
  int local_sign = formats->sign;
  number *= formats->sign;
  int flag = 1, check = 1;
  if (number != 0) {
    if (number < 1) {
      flag = -1;
      while (number < 1) {
        number *= 10;
        notation--;
      }
    } else {
      while (number > 10) {
        number /= 10;
        notation++;
      }
    }
  }
  formats->step = notation;
  long double left = 0;
  long double right = 0;
  right = modfl(number, &left);
  left = roundl(left);
  for (int i = 0; i < formats->accuracy; i++) {
    right *= 10;
    check *= 10;
  }
  if (roundl(right) == check) {
    left += 1;
  }
  right = roundl(right);
  s21_int_to_str(formats, tmp_str, left);
  if ((formats->accuracy > 0) || formats->flag_h == 1) {
    s21_strcat(tmp_str, ".");
  }
  if (roundl(right) != check && right != 0) {
    s21_int_to_str(formats, r_line, right);
  }
  if (s21_strchr("eE", formats->spec)) {
    s21_string_precision(formats, r_line);
  }
  s21_strcat(tmp_str, r_line);
  if (s21_strchr("gG", formats->spec) && formats->flag_h != 1) {
    s21_delete_zero(tmp_str, formats);
  }
  s21_mantis(tmp_str, formats, notation, str_notat, flag);
  formats->sign = local_sign;
}

void s21_mantis(char *tmp_str, struct s_struct *formats, int notation, char *str_notat, int flag) {
  if (s21_strchr("eg", formats->spec)) {
    s21_strcat(tmp_str, "e");
  } else {
    s21_strcat(tmp_str, "E");
  }
  if (flag == -1) {
    s21_strcat(tmp_str, "-");
  } else {
    s21_strcat(tmp_str, "+");
  }
  if (abs(notation) > 9) {
    s21_int_to_str(formats, str_notat, notation);
    s21_strcat(tmp_str, str_notat);
  } else {
    s21_int_to_str(formats, str_notat, notation);
    s21_strcat(tmp_str, "0");
    s21_strcat(tmp_str, str_notat);
  }
}

void s21_p(char *tmp_str, va_list *arg, struct s_struct *formats) {
  char *s21p_str = s21_NULL;
  s21p_str = tmp_str;
  int *p = va_arg(*arg, int *);
  char buf[4] = "x0";
  if (p == s21_NULL) {
    s21_strcpy(buf, "0x0");
  } else
    for (int *j = p; j; j = (void *)(((s21_size_t)j) >> 4)) {
      unsigned int dig = ((s21_size_t)j) % 16;
      if (dig < 10) {
        *(s21p_str++) = ('0' + dig);
      }
      else {
        *(s21p_str++) = ('a' + (dig - 10));
      }
    }
  *s21p_str = '\0';
  s21_string_precision(formats, tmp_str);
  s21_strcat(tmp_str, buf);
  s21_reverse(tmp_str);
  s21_string_flags(formats, tmp_str);
  s21_string_width(formats, tmp_str);
}
void s21_xX(char *tmp_str, va_list *arg, struct s_struct *formats) {
  unsigned long ld_number = 0;
  unsigned short sd_number = 0;
  unsigned int number = 0;


  if (formats->flag_l == 'l') {
    ld_number = va_arg(*arg, unsigned long);
    s21_xX_to_str(formats, tmp_str, ld_number);
  } else if (formats->flag_l == 'h') {
    sd_number = (unsigned short)va_arg(*arg, unsigned int);
    s21_xX_to_str(formats, tmp_str, sd_number);
  } else {
    number = va_arg(*arg, unsigned int);
    s21_xX_to_str(formats, tmp_str, number);
  }
  s21_string_precision(formats, tmp_str);
  s21_string_flags(formats, tmp_str);
  s21_string_width(formats, tmp_str);
}
void s21_u(char *tmp_str, va_list *arg, struct s_struct *formats) {
  unsigned long ul_number = 0;
  unsigned short us_number = 0;
  unsigned int number = 0;


  if (formats->flag_l == 'l') {
    ul_number = va_arg(*arg, unsigned long);
    s21_u_to_str(formats, tmp_str, ul_number);
  } else if (formats->flag_l == 'h') {
    us_number = (unsigned short)va_arg(*arg, unsigned int);
    s21_u_to_str(formats, tmp_str, us_number);
  } else {
    number = va_arg(*arg, unsigned int);
    s21_u_to_str(formats, tmp_str, number);
  }
  s21_string_precision(formats, tmp_str);
  s21_string_flags(formats, tmp_str);
  s21_string_width(formats, tmp_str);
}

void s21_o(char *tmp_str, va_list *arg, struct s_struct *formats) {
  unsigned long int ld_number = 0;
  unsigned short int sd_number = 0;
  unsigned int number = 0;

  if (formats->flag_l == 'l') {
    ld_number = va_arg(*arg, unsigned long int);
    s21_o_to_str(formats, tmp_str, ld_number);
  } else if (formats->flag_l == 'h') {
    sd_number = (unsigned short)va_arg(*arg, unsigned int);
    s21_o_to_str(formats, tmp_str, sd_number);
  } else {
      number = va_arg(*arg, unsigned int);
      s21_o_to_str(formats, tmp_str, number);
  }
  s21_string_precision(formats, tmp_str);
  s21_string_flags(formats, tmp_str);
  s21_string_width(formats, tmp_str);
}

void s21_s(char *p_str, va_list *arg, struct s_struct *formats) {
  char *string = s21_NULL;
  string = va_arg(*arg, char *);
  if (string == s21_NULL) {
    if (formats->accuracy > 6 || formats->point == !1) {
      s21_strcat(p_str, "(null)");
    }
    else {
      s21_strcat(p_str, "");
    }
  } else {
    if (formats->accuracy > -1) {
      s21_strncat(p_str, string, formats->accuracy);
    }
    else {
      s21_strcat(p_str, string);
    }
  }
  s21_string_precision(formats, p_str);
  s21_string_flags(formats, p_str);
  s21_string_width(formats, p_str);
}

void s21_f(char *tmp_str, va_list *arg, struct s_struct *formats) {
  double d_number = 0;
  long double l_number = 0;

  if (formats->flag_l == 'L') {
    l_number = va_arg(*arg, long double);
    s21_float_to_str(formats, tmp_str, l_number);
  } else {
    d_number = va_arg(*arg, double);
    s21_float_to_str(formats, tmp_str, d_number);
  }

  add_for_fdeEfgG(formats, tmp_str);
}

void s21_d(char *tmp_str, va_list *arg, struct s_struct *formats) {
  int number = 0;
  long int li_number = 0;
  short int sh_number = 0;

  if (formats->flag_l == 'l') {
    li_number = va_arg(*arg, long int);
    s21_int_to_str(formats, tmp_str, li_number);
  } else if (formats->flag_l == 'h') {
    sh_number = (short)va_arg(*arg, int);
    s21_int_to_str(formats, tmp_str, sh_number);
  } else {
    number = va_arg(*arg, int);
    s21_int_to_str(formats, tmp_str, number);
  }
  s21_string_precision(formats, tmp_str);
  add_for_fdeEfgG(formats, tmp_str);
}

void s21_xX_to_str(struct s_struct *formats, char *tmp_str, unsigned long long number) {
  char *point = s21_NULL;
  int flag = 1;
  int x_or_X = 0;
  point = tmp_str;

  if (formats->spec == 'x') {
    x_or_X = 32;
  } else {
    x_or_X = 0;
  }


  
  if (formats->point == 1 && number == 0) {
    s21_strcat(tmp_str, " ");
  } else
    while (flag) {
      unsigned int digit;
      if (number >= 16) {
        digit = number % 16;
        number = (number - digit) / 16;
      } else {
        digit = number;
        flag = 0;
      }
      if (digit < 10) {
        *(point++) = (digit + '0');
      }
      else {
        *(point++) = (digit + 'A' + x_or_X - 10);
      }
    }
  *point = '\0';
  if (number == 0) {
    formats->flag_h = 0;
  }
  s21_reverse(tmp_str);
}

void s21_u_to_str(struct s_struct *formats, char *tmp_str, unsigned long long number) {
  char *point = s21_NULL;
  int flag = 1;
  point = tmp_str;
  if (formats->point == 1 && number == 0) {
    s21_strcat(tmp_str, " ");
  } else
    while (flag) {
      unsigned int digit;
      if (number >= 10) {
        digit = number % 10;
        number = (number - digit) / 10;
      } else {
        digit = number;
        flag = 0;
      }
      *(point++) = digit + '0';
    }

  *point = '\0';
  s21_reverse(tmp_str);
}

void s21_o_to_str(struct s_struct *formats, char *tmp_str, long long number) {
  char *point = s21_NULL;
  int flag = 1;
  point = tmp_str;
  long long check_number = number;
  if (formats->point == 1 && number == 0) {
    s21_strcat(tmp_str, " ");
  } else
    while (flag) {
      unsigned int digit;
      if (number > 7) {
        digit = number % 8;
        number = (number - digit) / 8;
      } else {
        digit = number;
        flag = 0;
      }
      *(point++) = (char)(digit + '0');
    }
  if (formats->flag_h == 1 && check_number != 0) {
     *(point++) = '0';
  }
  *point = '\0';
  s21_reverse(tmp_str);
}

void s21_default_precision(struct s_struct *formats) {
  if (formats->point && formats->accuracy <= 0) {
    formats->accuracy = 0;
  }
  if (!formats->point && s21_strchr("eEfgG", formats->spec)) {
    formats->accuracy = 6;
  }
}

void s21_float_to_str(struct s_struct *formats, char *tmp_str, long double number) {
  char result[256] = {0};
  if (number < 0) {
    formats->sign = -1;
  } else {
    formats->sign = 1;
  }

  number *= formats->sign;
  int local_sign = formats->sign;
  if (formats->accuracy <= 0) {
    s21_int_to_str(formats, tmp_str, roundl(number));
  } else {
    long double left = 0;
    long double right = 0;
    long double check = 1;
    right = modfl(number, &left);
    left = roundl(left);
    for (int i = 0; i < formats->accuracy; i++) {
      right *= 10, check *= 10;
    }
    if (roundl(right) == check) {
      left += 1;
    }
    s21_int_to_str(formats, tmp_str, left);
    s21_strcat(tmp_str, ".");
    if (roundl(right) != check) {
      s21_int_to_str(formats, result, roundl(right));
    }
    s21_string_precision(formats, result);
    s21_strcat(tmp_str, result);
  }
  formats->sign = local_sign;
}

void s21_int_to_str(struct s_struct *formats, char *tmp_str, long double number) {
  char *point = tmp_str;
  int flag = 1;

  if (number < 0) {
    formats->sign = -1;
  } else {
    formats->sign = 1;
  }
  number *= formats->sign;
  if (formats->point == 1 && number == 0 && formats->accuracy == 0 && formats->spec == 'd') {
    if (formats->Wdt == 0) {
      s21_strcat(tmp_str, "");
    } else {
      s21_strcat(tmp_str, " ");
    }
  } else
    while (flag) {
      int digit = 0;
      if (number >= 10) {
        digit = fmodl(number, 10);
        number = (number - digit) / 10;
      } else {
        digit = number;
        flag = 0;
      }
      *point = digit + '0';
      point++;
    }
  s21_reverse(tmp_str);
}

void s21_string_precision(struct s_struct *formats, char *tmp_str) {
  char *string_precision = s21_NULL;
  string_precision = calloc(1024, sizeof(char));
  int pos = 0;
  int len_tmp = s21_strlen(tmp_str);
  if (s21_strchr("feEpdouxX", formats->spec)) {
    if (formats->spec == 'p') {
      s21_strcpy(string_precision, tmp_str);
      for (int i = len_tmp; i < formats->accuracy; i++) {
        string_precision[i] = '0';
      }
      s21_strcpy(tmp_str, string_precision);
    } else {
      while (pos < formats->accuracy - len_tmp) {
        string_precision[pos++] = '0';
      }
      for (int i = 0; tmp_str[i] != '\0'; i++) {
        string_precision[pos++] = tmp_str[i];
      }
      s21_strcpy(tmp_str, string_precision);
    }
  }
  free(string_precision);
}

void s21_string_flags(struct s_struct *formats, char *tmp_str) {
  int pos = 0;
  if (!s21_strchr("cuo\%", formats->spec)) {
    if (formats->sign < 0) {
      s21_move_str(tmp_str);
      tmp_str[pos++] = '-';
      formats->sign = 0;
    }
    if (formats->flag_p && formats->sign != 0) {
      s21_move_str(tmp_str);

      if (formats->sign > 0) {
        tmp_str[pos] = '+';
      } else {
        tmp_str[pos] = '-';
      }

      formats->sign = 0;
    }
    if (formats->flag_s && formats->sign != 0) {
      s21_move_str(tmp_str);
      if (formats->sign > 0) {
        tmp_str[pos] = ' ';
      } else {
        tmp_str[pos] = '-';
      }
      formats->sign = 0;
    }
    if (formats->flag_h) {
      s21_sharp_flag(formats, tmp_str);
    }
  }
  if (formats->flag_m) {
    pos = (int)s21_strlen(tmp_str);
    while (pos < formats->Wdt) {
      tmp_str[pos++] = ' ';
    }
  }
}

void s21_sharp_flag(struct s_struct *formats, char *tmp_str) {
  int pos = 0;
  if (s21_strchr("oxX", formats->spec)) {
    if (formats->spec == 'x' || formats->spec == 'X') {
      s21_move_str(tmp_str);
    }

    if (formats->spec == 'o') {
      ;
    } else {
      s21_move_str(tmp_str);
    }
    tmp_str[pos++] = '0';

    if (formats->spec != 'o') {

      if (formats->spec == 'x') {
        tmp_str[pos] = 'x';
      } else {
        tmp_str[pos] = 'X';
      }
    }
  }

  if (s21_strchr("eEf", formats->spec)) {
    if (!s21_strchr(tmp_str, '.')) {
      tmp_str[s21_strlen(tmp_str)] = '.';
    }
  }
}

void add_for_fdeEfgG(struct s_struct *formats, char *tmp_str) {
  if (formats->flag_0 == 1 && formats->flag_m != 1 && (formats->sign == -1 || formats->flag_s || (formats->sign == 1 && formats->flag_p == 1))) {
    formats->Wdt -= 1;
    s21_string_width(formats, tmp_str);
    s21_string_flags(formats, tmp_str);
  } else {
    s21_string_flags(formats, tmp_str);
    s21_string_width(formats, tmp_str);
  }
}


void s21_string_width(struct s_struct *formats, char *tmp_str) {
  int pos_str = (int)s21_strlen(tmp_str);
  if (formats->Wdt > pos_str) {
    tmp_str[formats->Wdt + 1] = '\0';
    while (pos_str >= 0) {
      tmp_str[formats->Wdt--] = tmp_str[pos_str--];
    }
    for (pos_str = 0; pos_str <= formats->Wdt; pos_str++) {
      if (s21_strchr("dopuxX", formats->spec)) {

        if (formats->flag_0 && !formats->point) {
          tmp_str[pos_str] = '0';
        } else {
          tmp_str[pos_str] = ' ';
        }
      }
      if (s21_strchr("feEgGcs", formats->spec)) {
        if (formats->flag_0) {
          tmp_str[pos_str] = '0';
        } else {
          tmp_str[pos_str] = ' ';
        }
      }
    }
  }
}

int check_specf(struct s_struct *formats, const char *format) {
  return *format == 'c' || *format == 'd' || *format == 'e' || *format == 'E' ||
                 *format == 'f' || *format == 'g' || *format == 'G' ||
                 *format == 'o' || *format == 'p' || *format == 's' ||
                 *format == 'u' || *format == 'x' || *format == 'X' ||
                 *format == '%'
             ? 1 && (formats->spec = *format)
             : 0;
}

void set_flags(struct s_struct *formats, const char *format) {
  if (*format == ' ') formats->flag_s = 1;
  if (*format == '+') formats->flag_p = 1;
  if (*format == '-') formats->flag_m = 1;
  if (*format == '0') formats->flag_0 = 1;
  if (*format == '#') formats->flag_h = 1;
}

void set_width_accuracy(int *number, const char *format, va_list *arg) {
  if (*format == '*') {
    *number = va_arg(*arg, int);
    format++;
  } else
    *number = *number * 10 + (int)*format - 48;
}

void s21_add(char *str, const char *format, int *zero_str_flag, struct s_struct *formats) {
  if (*zero_str_flag == 0) {
    char add[2] = {0};
    add[0] = *format;
    s21_strcat(str, add);
  } else
    formats->zero_simbol++;
}

void s21_move_str(char *tmp_str) {
  int pos = s21_strlen(tmp_str);
  tmp_str[pos + 1] = '\0';
  while (pos > 0) {
    tmp_str[pos] = tmp_str[pos - 1];
    pos--;
  }
}

void s21_reverse(char *str) {
  char c = 0;
  s21_size_t length = s21_strlen(str);
  for (s21_size_t i = 0; i < length / 2; i++) {
    c = str[i];
    str[i] = str[length - 1 - i];
    str[length - 1 - i] = c;
  }
}

void s21_percent(char *tmp_str, struct s_struct *formats) {
  s21_strcat(tmp_str, "%");
  s21_string_precision(formats, tmp_str);
  s21_string_flags(formats, tmp_str);
  s21_string_width(formats, tmp_str);
}

void s21_c(char *p_str, va_list *arg, struct s_struct *formats, int *zero_str_flag) {
  if (*zero_str_flag == 0) {
    p_str[0] = va_arg(*arg, int);
    if (p_str[0] == 0) {
      formats->zero_simbol++;
      *zero_str_flag = 1;
    }
    p_str[1] = '\0';
  } else {
    formats->zero_simbol++;
    p_str[0] = '\0';
  }
  s21_string_flags(formats, p_str);
  s21_string_width(formats, p_str);
}

int s21_check_flags(const char *format) {
  return *format == ' ' || *format == '-' || *format == '+' || *format == '0' ||
                 *format == '#'
             ? 1
             : 0;
}

int s21_check_lenght(const char *format) {
  return *format == 'l' || *format == 'h' || *format == 'L' ? 1 : 0;
}

int s21_check_digital(const char *format) {
  return (int)(*format) >= 48 && (int)(*format) <= 57 ? 1 : 0;
}