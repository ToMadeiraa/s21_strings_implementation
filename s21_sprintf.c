
#include "s21_sprintf.h"
int s21_sprintf(char *str, const char *format, ...) {
  va_list arg;
  va_start(arg, format);
  str = s21_strcpy(str, "");
  int zero_str = 0;
  int zero_str_flag = 0;
  while (*format) {
    struct s_struct frmt = {0};
    frmt.accuracy = -1;
    if (*format != '%') {
      s21_add(str, format, &zero_str_flag, &frmt);
    }
    if (*format == '%') {
      format++;
      while (check_flags(format)) {
        set_flags(&frmt, format++);
      }
      while (check_dig(format) || *format == '*') {
        set_width_accuracy(&frmt.width, format++, &arg);
        if (frmt.width < 0) {
          frmt.width *= -1;
          frmt.flag_m = 1;
        }
      }
      if (*format == '.') {
        format++;
        frmt.pnt = 1;
        frmt.accuracy = 0;
        while (check_dig(format) || *format == '*')
          set_width_accuracy(&frmt.accuracy, format++, &arg);
      }
      if (check_lenght(format)) {
        frmt.flag_l = *format;
        format++;
      }
      if (check_specf(&frmt, format)) {
        s21_default_precision(&frmt);
      }
      s21_check(format, str, &frmt, &arg, &zero_str_flag);
    }
    format++;
    zero_str += frmt.zero;
  }
  va_end(arg);
  return (int)s21_strlen(str) + zero_str;
}

void s21_check(const char *format, char *str, struct s_struct *frmt, va_list *arg, int *zero_str_flag) {
  char *p_str = s21_NULL;
  p_str = calloc(1024, sizeof(char));
  if (*format == 'c') {
    s21_c(p_str, arg, frmt, zero_str_flag);
  } else if (*format == 's') {
    s21_s(p_str, arg, frmt);
  } else if (*format == 'd') {
    s21_d(p_str, arg, frmt);
  } else if (*format == 'f') {
    s21_f(p_str, arg, frmt);
  } else if (*format == 'o') {
    s21_o(p_str, arg, frmt);
  } else if (*format == 'u') {
    s21_u(p_str, arg, frmt);
  } else if (*format =='x' || *format == 'X') {
    s21_xX(p_str, arg, frmt);
  } else if (*format == 'p') {
    s21_p(p_str, arg, frmt);
  } else if (*format =='e' || *format == 'E') {
    s21_eE(p_str, arg, frmt);
  } else if (*format =='g' || *format == 'G') {
    s21_gG(p_str, arg, frmt);
  } else if (*format =='%') {
    s21_percent(p_str, frmt);
  } else {
    ;
  }
  s21_strcat(str, p_str);
  free(p_str);
}

void s21_gG(char *tmp_str, va_list *arg, struct s_struct *frmt) {
  long double l_number = 0;
  double number = 0;
  if (frmt->flag_l == 'L') {
    l_number = va_arg(*arg, long double);
    s21_gG_to_str(frmt, tmp_str, l_number);
  } else {
    number = va_arg(*arg, double);
    s21_gG_to_str(frmt, tmp_str, number);
  } 
  add_for_fdeEfgG(frmt, tmp_str);
}

void s21_gG_to_str(struct s_struct *frmt, char *tmp_str,
                   long double number) {
  if (number != 0) {
    frmt->znak = number < 0 ? -1 : 1;
    int local_znak = frmt->znak;
    number *= frmt->znak;
    int safe_pricision = frmt->accuracy;
    char *str_eE = s21_NULL, *str_f = s21_NULL;
    str_eE = calloc(1024, sizeof(char));
    str_f = calloc(1024, sizeof(char));
    if (number < 1 && frmt->accuracy < 0) {
      frmt->accuracy = 4;
    } else {
      frmt->accuracy--;
    }
    s21_eE_to_str(frmt, str_eE, number);
    frmt->accuracy = safe_pricision;
    int pnt = 0, flag_int = 0;
    int t_number = roundl(number);
    if (t_number == number) {
      flag_int = 1;
    }
    while (t_number > 1) {
      t_number /= 10;
      pnt++;
    }
    frmt->accuracy -= pnt;
    if (flag_int == 0) {
      s21_float_to_str(frmt, str_f, number);
      s21_delete_zero(str_f, frmt);
    } else {
      s21_int_to_str(frmt, str_f, number);
      s21_delete_zero(str_f, frmt);
    }
    frmt->accuracy = safe_pricision;
    s21_strcat(tmp_str, (frmt->stp >= -4 && frmt->accuracy > frmt->stp) ? str_f : str_eE);
    frmt->znak = local_znak;
    free(str_eE);
    free(str_f);
  } else {
    s21_int_to_str(frmt, tmp_str, 0);
  }
}
void s21_delete_zero(char *tmp_str, struct s_struct *frmt) {
  int len_str = s21_strlen(tmp_str) - 1;
  if (frmt->accuracy > 0)
    while (tmp_str[len_str] == '0') {
      tmp_str[len_str] = '\0';
      len_str--;
    }
}

void s21_eE(char *tmp_str, va_list *arg, struct s_struct *frmt) {
  long double ld_number = 0;
  double number = 0;

  if (frmt->flag_l == 'L') {
    ld_number = va_arg(*arg, long double);
    s21_eE_to_str(frmt, tmp_str, ld_number);
  } else {
    number = va_arg(*arg, double);
    s21_eE_to_str(frmt, tmp_str, number);
  }
  add_for_fdeEfgG(frmt, tmp_str);
}

void s21_eE_to_str(struct s_struct *frmt, char *tmp_str, long double number) {
  char r_line[256] = {0}, str_notat[256] = {0};

  if (number < 0) {
    frmt->znak = -1;
  } else {
    frmt->znak = 1;
  }



  int notation = 0; 
  int local_znak = frmt->znak;
  number *= frmt->znak;
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
  frmt->stp = notation;
  long double left = 0;
  long double right = 0;
  right = modfl(number, &left);
  left = roundl(left);
  for (int i = 0; i < frmt->accuracy; i++) {
    right *= 10;
    check *= 10;
  }
  if (roundl(right) == check) {
    left += 1;
  }
  right = roundl(right);
  s21_int_to_str(frmt, tmp_str, left);
  if ((frmt->accuracy > 0) || frmt->flag_h == 1) {
    s21_strcat(tmp_str, ".");
  }
  if (roundl(right) != check && right != 0) {
    s21_int_to_str(frmt, r_line, right);
  }
  if (s21_strchr("eE", frmt->spec)) {
    s21_string_precision(frmt, r_line);
  }
  s21_strcat(tmp_str, r_line);
  if (s21_strchr("gG", frmt->spec) && frmt->flag_h != 1) {
    s21_delete_zero(tmp_str, frmt);
  }
  s21_mantis(tmp_str, frmt, notation, str_notat, flag);
  frmt->znak = local_znak;
}

void s21_mantis(char *tmp_str, struct s_struct *frmt, int notation, char *str_notat, int flag) {
  if (s21_strchr("eg", frmt->spec)) {
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
    s21_int_to_str(frmt, str_notat, notation);
    s21_strcat(tmp_str, str_notat);
  } else {
    s21_int_to_str(frmt, str_notat, notation);
    s21_strcat(tmp_str, "0");
    s21_strcat(tmp_str, str_notat);
  }
}

void s21_p(char *tmp_str, va_list *arg, struct s_struct *frmt) {
  char *s21p_str = s21_NULL;
  s21p_str = tmp_str;
  int *p = va_arg(*arg, int *);
  char buf[4] = "x0";
  if (p == s21_NULL) {
    s21_strcpy(buf, "0x0");
  } else
    for (int *j = p; j; j = (void *)(((s21_size_t)j) >> 4)) {
      unznaked int dig = ((s21_size_t)j) % 16;
      if (dig < 10) {
        *(s21p_str++) = ('0' + dig);
      }
      else {
        *(s21p_str++) = ('a' + (dig - 10));
      }
    }
  *s21p_str = '\0';
  s21_string_precision(frmt, tmp_str);
  s21_strcat(tmp_str, buf);
  s21_reverse_string(tmp_str);
  s21_string_flags(frmt, tmp_str);
  s21_string_width(frmt, tmp_str);
}
void s21_xX(char *tmp_str, va_list *arg, struct s_struct *frmt) {
  unznaked long ld_number = 0;
  unznaked short sd_number = 0;
  unznaked int number = 0;


  if (frmt->flag_l == 'l') {
    ld_number = va_arg(*arg, unznaked long);
    s21_xX_to_str(frmt, tmp_str, ld_number);
  } else if (frmt->flag_l == 'h') {
    sd_number = (unznaked short)va_arg(*arg, unznaked int);
    s21_xX_to_str(frmt, tmp_str, sd_number);
  } else {
    number = va_arg(*arg, unznaked int);
    s21_xX_to_str(frmt, tmp_str, number);
  }
  s21_string_precision(frmt, tmp_str);
  s21_string_flags(frmt, tmp_str);
  s21_string_width(frmt, tmp_str);
}
void s21_u(char *tmp_str, va_list *arg, struct s_struct *frmt) {
  unznaked long ul_number = 0;
  unznaked short us_number = 0;
  unznaked int number = 0;


  if (frmt->flag_l == 'l') {
    ul_number = va_arg(*arg, unznaked long);
    s21_u_to_str(frmt, tmp_str, ul_number);
  } else if (frmt->flag_l == 'h') {
    us_number = (unznaked short)va_arg(*arg, unznaked int);
    s21_u_to_str(frmt, tmp_str, us_number);
  } else {
    number = va_arg(*arg, unznaked int);
    s21_u_to_str(frmt, tmp_str, number);
  }
  s21_string_precision(frmt, tmp_str);
  s21_string_flags(frmt, tmp_str);
  s21_string_width(frmt, tmp_str);
}

void s21_o(char *tmp_str, va_list *arg, struct s_struct *frmt) {
  unznaked long int ld_number = 0;
  unznaked short int sd_number = 0;
  unznaked int number = 0;

  if (frmt->flag_l == 'l') {
    ld_number = va_arg(*arg, unznaked long int);
    s21_o_to_str(frmt, tmp_str, ld_number);
  } else if (frmt->flag_l == 'h') {
    sd_number = (unznaked short)va_arg(*arg, unznaked int);
    s21_o_to_str(frmt, tmp_str, sd_number);
  } else {
      number = va_arg(*arg, unznaked int);
      s21_o_to_str(frmt, tmp_str, number);
  }
  s21_string_precision(frmt, tmp_str);
  s21_string_flags(frmt, tmp_str);
  s21_string_width(frmt, tmp_str);
}

void s21_s(char *p_str, va_list *arg, struct s_struct *frmt) {
  char *string = s21_NULL;
  string = va_arg(*arg, char *);
  if (string == s21_NULL) {
    if (frmt->accuracy > 6 || frmt->pnt == !1) {
      s21_strcat(p_str, "(null)");
    }
    else {
      s21_strcat(p_str, "");
    }
  } else {
    if (frmt->accuracy > -1) {
      s21_strncat(p_str, string, frmt->accuracy);
    }
    else {
      s21_strcat(p_str, string);
    }
  }
  s21_string_precision(frmt, p_str);
  s21_string_flags(frmt, p_str);
  s21_string_width(frmt, p_str);
}

void s21_f(char *tmp_str, va_list *arg, struct s_struct *frmt) {
  double d_number = 0;
  long double l_number = 0;

  if (frmt->flag_l == 'L') {
    l_number = va_arg(*arg, long double);
    s21_float_to_str(frmt, tmp_str, l_number);
  } else {
    d_number = va_arg(*arg, double);
    s21_float_to_str(frmt, tmp_str, d_number);
  }

  add_for_fdeEfgG(frmt, tmp_str);
}

void s21_d(char *tmp_str, va_list *arg, struct s_struct *frmt) {
  int number = 0;
  long int li_number = 0;
  short int sh_number = 0;

  if (frmt->flag_l == 'l') {
    li_number = va_arg(*arg, long int);
    s21_int_to_str(frmt, tmp_str, li_number);
  } else if (frmt->flag_l == 'h') {
    sh_number = (short)va_arg(*arg, int);
    s21_int_to_str(frmt, tmp_str, sh_number);
  } else {
    number = va_arg(*arg, int);
    s21_int_to_str(frmt, tmp_str, number);
  }
  s21_string_precision(frmt, tmp_str);
  add_for_fdeEfgG(frmt, tmp_str);
}

void s21_xX_to_str(struct s_struct *frmt, char *tmp_str, unznaked long long number) {
  char *pnt = s21_NULL;
  int flag = 1;
  int x_or_X = 0;
  pnt = tmp_str;

  if (frmt->spec == 'x') {
    x_or_X = 32;
  } else {
    x_or_X = 0;
  }


  
  if (frmt->pnt == 1 && number == 0) {
    s21_strcat(tmp_str, " ");
  } else
    while (flag) {
      unznaked int digit;
      if (number >= 16) {
        digit = number % 16;
        number = (number - digit) / 16;
      } else {
        digit = number;
        flag = 0;
      }
      if (digit < 10) {
        *(pnt++) = (digit + '0');
      }
      else {
        *(pnt++) = (digit + 'A' + x_or_X - 10);
      }
    }
  *pnt = '\0';
  if (number == 0) {
    frmt->flag_h = 0;
  }
  s21_reverse_string(tmp_str);
}

void s21_u_to_str(struct s_struct *frmt, char *tmp_str, unznaked long long number) {
  char *pnt = s21_NULL;
  int flag = 1;
  pnt = tmp_str;
  if (frmt->pnt == 1 && number == 0) {
    s21_strcat(tmp_str, " ");
  } else
    while (flag) {
      unznaked int digit;
      if (number >= 10) {
        digit = number % 10;
        number = (number - digit) / 10;
      } else {
        digit = number;
        flag = 0;
      }
      *(pnt++) = digit + '0';
    }

  *pnt = '\0';
  s21_reverse_string(tmp_str);
}

void s21_o_to_str(struct s_struct *frmt, char *tmp_str, long long number) {
  char *pnt = s21_NULL;
  int flag = 1;
  pnt = tmp_str;
  long long check_number = number;
  if (frmt->pnt == 1 && number == 0) {
    s21_strcat(tmp_str, " ");
  } else
    while (flag) {
      unznaked int digit;
      if (number > 7) {
        digit = number % 8;
        number = (number - digit) / 8;
      } else {
        digit = number;
        flag = 0;
      }
      *(pnt++) = (char)(digit + '0');
    }
  if (frmt->flag_h == 1 && check_number != 0) {
     *(pnt++) = '0';
  }
  *pnt = '\0';
  s21_reverse_string(tmp_str);
}

void s21_default_precision(struct s_struct *frmt) {
  if (frmt->pnt && frmt->accuracy <= 0) {
    frmt->accuracy = 0;
  }
  if (!frmt->pnt && s21_strchr("eEfgG", frmt->spec)) {
    frmt->accuracy = 6;
  }
}

void s21_float_to_str(struct s_struct *frmt, char *tmp_str, long double number) {
  char result[256] = {0};
  if (number < 0) {
    frmt->znak = -1;
  } else {
    frmt->znak = 1;
  }

  number *= frmt->znak;
  int local_znak = frmt->znak;
  if (frmt->accuracy <= 0) {
    s21_int_to_str(frmt, tmp_str, roundl(number));
  } else {
    long double left = 0;
    long double right = 0;
    long double check = 1;
    right = modfl(number, &left);
    left = roundl(left);
    for (int i = 0; i < frmt->accuracy; i++) {
      right *= 10, check *= 10;
    }
    if (roundl(right) == check) {
      left += 1;
    }
    s21_int_to_str(frmt, tmp_str, left);
    s21_strcat(tmp_str, ".");
    if (roundl(right) != check) {
      s21_int_to_str(frmt, result, roundl(right));
    }
    s21_string_precision(frmt, result);
    s21_strcat(tmp_str, result);
  }
  frmt->znak = local_znak;
}

void s21_int_to_str(struct s_struct *frmt, char *tmp_str, long double number) {
  char *pnt = tmp_str;
  int flag = 1;

  if (number < 0) {
    frmt->znak = -1;
  } else {
    frmt->znak = 1;
  }
  number *= frmt->znak;
  if (frmt->pnt == 1 && number == 0 && frmt->accuracy == 0 && frmt->spec == 'd') {
    if (frmt->width == 0) {
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
      *pnt = digit + '0';
      pnt++;
    }
  s21_reverse_string(tmp_str);
}

void s21_string_precision(struct s_struct *frmt, char *tmp_str) {
  char *string_precision = s21_NULL;
  string_precision = calloc(1024, sizeof(char));
  int pos = 0;
  int len_tmp = s21_strlen(tmp_str);
  if (s21_strchr("feEpdouxX", frmt->spec)) {
    if (frmt->spec == 'p') {
      s21_strcpy(string_precision, tmp_str);
      for (int i = len_tmp; i < frmt->accuracy; i++) {
        string_precision[i] = '0';
      }
      s21_strcpy(tmp_str, string_precision);
    } else {
      while (pos < frmt->accuracy - len_tmp) {
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

void s21_string_flags(struct s_struct *frmt, char *tmp_str) {
  int pos = 0;
  if (!s21_strchr("cuo\%", frmt->spec)) {
    if (frmt->znak < 0) {
      s21_move_string(tmp_str);
      tmp_str[pos++] = '-';
      frmt->znak = 0;
    }
    if (frmt->flag_p && frmt->znak != 0) {
      s21_move_string(tmp_str);

      if (frmt->znak > 0) {
        tmp_str[pos] = '+';
      } else {
        tmp_str[pos] = '-';
      }

      frmt->znak = 0;
    }
    if (frmt->flag_s && frmt->znak != 0) {
      s21_move_string(tmp_str);
      if (frmt->znak > 0) {
        tmp_str[pos] = ' ';
      } else {
        tmp_str[pos] = '-';
      }
      frmt->znak = 0;
    }
    if (frmt->flag_h) {
      s21_sharp_flag(frmt, tmp_str);
    }
  }
  if (frmt->flag_m) {
    pos = (int)s21_strlen(tmp_str);
    while (pos < frmt->width) {
      tmp_str[pos++] = ' ';
    }
  }
}

void s21_sharp_flag(struct s_struct *frmt, char *tmp_str) {
  int pos = 0;
  if (s21_strchr("oxX", frmt->spec)) {
    if (frmt->spec == 'x' || frmt->spec == 'X') {
      s21_move_string(tmp_str);
    }

    if (frmt->spec == 'o') {
      ;
    } else {
      s21_move_string(tmp_str);
    }
    tmp_str[pos++] = '0';

    if (frmt->spec != 'o') {

      if (frmt->spec == 'x') {
        tmp_str[pos] = 'x';
      } else {
        tmp_str[pos] = 'X';
      }
    }
  }

  if (s21_strchr("eEf", frmt->spec)) {
    if (!s21_strchr(tmp_str, '.')) {
      tmp_str[s21_strlen(tmp_str)] = '.';
    }
  }
}

void add_for_fdeEfgG(struct s_struct *frmt, char *tmp_str) {
  if (frmt->flag_0 == 1 && frmt->flag_m != 1 && (frmt->znak == -1 || frmt->flag_s || (frmt->znak == 1 && frmt->flag_p == 1))) {
    frmt->width -= 1;
    s21_string_width(frmt, tmp_str);
    s21_string_flags(frmt, tmp_str);
  } else {
    s21_string_flags(frmt, tmp_str);
    s21_string_width(frmt, tmp_str);
  }
}


void s21_string_width(struct s_struct *frmt, char *tmp_str) {
  int pos_str = (int)s21_strlen(tmp_str);
  if (frmt->width > pos_str) {
    tmp_str[frmt->width + 1] = '\0';
    while (pos_str >= 0) {
      tmp_str[frmt->width--] = tmp_str[pos_str--];
    }
    for (pos_str = 0; pos_str <= frmt->width; pos_str++) {
      if (s21_strchr("dopuxX", frmt->spec)) {

        if (frmt->flag_0 && !frmt->pnt) {
          tmp_str[pos_str] = '0';
        } else {
          tmp_str[pos_str] = ' ';
        }
      }
      if (s21_strchr("feEgGcs", frmt->spec)) {
        if (frmt->flag_0) {
          tmp_str[pos_str] = '0';
        } else {
          tmp_str[pos_str] = ' ';
        }
      }
    }
  }
}

int check_specf(struct s_struct *frmt, const char *format) {
  return *format == 'c' || *format == 'd' || *format == 'e' || *format == 'E' ||
                 *format == 'f' || *format == 'g' || *format == 'G' ||
                 *format == 'o' || *format == 'p' || *format == 's' ||
                 *format == 'u' || *format == 'x' || *format == 'X' ||
                 *format == '%'
             ? 1 && (frmt->spec = *format)
             : 0;
}

void set_flags(struct s_struct *frmt, const char *format) {
  if (*format == ' ') frmt->flag_s = 1;
  if (*format == '+') frmt->flag_p = 1;
  if (*format == '-') frmt->flag_m = 1;
  if (*format == '0') frmt->flag_0 = 1;
  if (*format == '#') frmt->flag_h = 1;
}

void set_width_accuracy(int *number, const char *format, va_list *arg) {
  if (*format == '*') {
    *number = va_arg(*arg, int);
    format++;
  } else
    *number = *number * 10 + (int)*format - 48;
}

void s21_add(char *str, const char *format, int *zero_str_flag, struct s_struct *frmt) {
  if (*zero_str_flag == 0) {
    char add[2] = {0};
    add[0] = *format;
    s21_strcat(str, add);
  } else
    frmt->zero++;
}

void s21_move_string(char *tmp_str) {
  int pos = s21_strlen(tmp_str);
  tmp_str[pos + 1] = '\0';
  while (pos > 0) {
    tmp_str[pos] = tmp_str[pos - 1];
    pos--;
  }
}

void s21_reverse_string(char *str) {
  char c = 0;
  s21_size_t length = s21_strlen(str);
  for (s21_size_t i = 0; i < length / 2; i++) {
    c = str[i];
    str[i] = str[length - 1 - i];
    str[length - 1 - i] = c;
  }
}

void s21_percent(char *tmp_str, struct s_struct *frmt) {
  s21_strcat(tmp_str, "%");
  s21_string_precision(frmt, tmp_str);
  s21_string_flags(frmt, tmp_str);
  s21_string_width(frmt, tmp_str);
}

void s21_c(char *p_str, va_list *arg, struct s_struct *frmt, int *zero_str_flag) {
  if (*zero_str_flag == 0) {
    p_str[0] = va_arg(*arg, int);
    if (p_str[0] == 0) {
      frmt->zero++;
      *zero_str_flag = 1;
    }
    p_str[1] = '\0';
  } else {
    frmt->zero++;
    p_str[0] = '\0';
  }
  s21_string_flags(frmt, p_str);
  s21_string_width(frmt, p_str);
}

int check_flags(const char *format) {
  return *format == ' ' || *format == '-' || *format == '+' || *format == '0' ||
                 *format == '#'
             ? 1
             : 0;
}

int check_lenght(const char *format) {
  return *format == 'l' || *format == 'h' || *format == 'L' ? 1 : 0;
}

int check_dig(const char *format) {
  return (int)(*format) >= 48 && (int)(*format) <= 57 ? 1 : 0;
}