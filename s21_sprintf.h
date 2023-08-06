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

void s21_add(char *str, const char *format, int *zero_str_flag, struct s_struct *formats);
void s21_move_str(char *tmp_sr);
int s21_check_flags(const char *format);
int s21_check_lenght(const char *format);
int s21_check_digital(const char *format);
void s21_reverse(char *str);
int s21_check_specf(struct s_struct *formats, const char *format);
void s21_set_flags(struct s_struct *formats, const char *format);
void s21_set_acc(int *number, const char *format, va_list *arg);
void s21_flags_str(struct s_struct *formats, char *tmp_str);
void s21_width_str(struct s_struct *formats, char *tmp_str);
void s21_prec_str(struct s_struct *formats, char *tmp_str);




void s21_flag_c(char *p_str, va_list *arg, struct s_struct *formats, int *zero_str_flag);
void s21_flag_s(char *p_str, va_list *arg, struct s_struct *formats);
void s21_flag_f(char *tmp_str, va_list *arg, struct s_struct *formats);
void s21_flag_d(char *tmp_str, va_list *arg, struct s_struct *formats);
void s21_flag_o(char *tmp_str, va_list *arg, struct s_struct *formats);
void s21_flag_u(char *tmp_str, va_list *arg, struct s_struct *formats);
void s21_flag_p(char *tmp_str, va_list *arg, struct s_struct *formats);
void s21_flag_x(char *tmp_str, va_list *arg, struct s_struct *formats);
void s21_flag_e(char *tmp_str, va_list *arg, struct s_struct *formats);
void s21_flag_g(char *tmp_str, va_list *arg, struct s_struct *formats);







void s21_int_to_str(struct s_struct *formats, char *tmp_str, long double number);
void s21_float_to_str(struct s_struct *formats, char *tmp_str, long double number);
void s21_flag_o_to_str(struct s_struct *formats, char *tmp_str, long long number);
void s21_flag_u_to_str(struct s_struct *formats, char *tmp_str, unsigned long long number);
void s21_flag_x_to_str(struct s_struct *struct_formatst, char *tmp_str, unsigned long long number);
void s21_flag_e_to_str(struct s_struct *formats, char *tmp_str, long double number);
void s21_flag_g_to_str(struct s_struct *formats, char *tmp_str, long double number);
void s21_delete_0(char *tmp_str, struct s_struct *formats);
void s21_flag_h(struct s_struct *formats, char *tmp_str);
void s21_per(char *tmp_str, struct s_struct *formats);
void s21_chk(const char *format, char *str, struct s_struct *formats, va_list *arg, int *zero_str_flag);
int s21_sprintf(char *str, const char *format, ...);
void s21_init_prec(struct s_struct *formats);
void s21_mantissa(char *tmp_str, struct s_struct *formats, int notation, char *str_notat, int flag);
void s21_add_for_all_flags(struct s_struct *formats, char *tmp_str);  // повторяющийся модуль

#endif  // S21_SPRINTF_H_
