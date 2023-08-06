#ifndef S21_SPRINTF_H_
#define S21_SPRINTF_H_

#include "s21_string.h"

struct s_struct {
  int flag_p;     
  int flag_m;    
  int flag_s;    
  int flag_0;     
  int flag_h;    
  char flag_l;  
  int Wdt;         
  int accuracy;     
  char spec;    
  int sign;          
  int point;         
  int zero_simbol;   
  int step;
} formats;

void add_char(char *str, const char *format, int *zero_str_flag, struct s_struct *formats);
void s21_move_string(char *tmp_sr);
int check_flags(const char *format);
int check_lenght(const char *format);
int check_dig(const char *format);
void s21_reverse_string(char *str);
int check_specf(struct s_struct *formats, const char *format);
void set_flags(struct s_struct *formats, const char *format);
void set_width_accuracy(int *number, const char *format, va_list *arg);
void s21_string_flags(struct s_struct *formats, char *tmp_str);
void s21_string_width(struct s_struct *formats, char *tmp_str);
void s21_string_precision(struct s_struct *formats, char *tmp_str);
void s21_c(char *p_str, va_list *arg, struct s_struct *formats, int *zero_str_flag);
void s21_s(char *p_str, va_list *arg, struct s_struct *formats);
void s21_f(char *tmp_str, va_list *arg, struct s_struct *formats);
void s21_d(char *tmp_str, va_list *arg, struct s_struct *formats);
void s21_o(char *tmp_str, va_list *arg, struct s_struct *formats);
void s21_u(char *tmp_str, va_list *arg, struct s_struct *formats);
void s21_p(char *tmp_str, va_list *arg, struct s_struct *formats);
void s21_xX(char *tmp_str, va_list *arg, struct s_struct *formats);
void s21_eE(char *tmp_str, va_list *arg, struct s_struct *formats);
void s21_gG(char *tmp_str, va_list *arg, struct s_struct *formats);
void s21_int_to_str(struct s_struct *formats, char *tmp_str, long double number);
void s21_float_to_str(struct s_struct *formats, char *tmp_str, long double number);
void s21_o_to_str(struct s_struct *formats, char *tmp_str, long long number);
void s21_u_to_str(struct s_struct *formats, char *tmp_str, unsigned long long number);
void s21_xX_to_str(struct s_struct *struct_formatst, char *tmp_str, unsigned long long number);
void s21_eE_to_str(struct s_struct *formats, char *tmp_str, long double number);
void s21_gG_to_str(struct s_struct *formats, char *tmp_str, long double number);
void s21_delete_zero(char *tmp_str, struct s_struct *formats);
void s21_sharp_flag(struct s_struct *formats, char *tmp_str);
void s21_percent(char *tmp_str, struct s_struct *formats);
void s21_check(const char *format, char *str, struct s_struct *formats, va_list *arg, int *zero_str_flag);
int s21_sprintf(char *str, const char *format, ...);
void s21_default_precision(struct s_struct *formats);
void s21_mantis(char *tmp_str, struct s_struct *formats, int notation, char *str_notat, int flag);
void add_for_fdeEfgG(struct s_struct *formats, char *tmp_str);  // повторяющийся модуль

#endif  // S21_SPRINTF_H_
