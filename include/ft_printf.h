/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lazmoud <lazmoud@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:41:44 by lazmoud           #+#    #+#             */
/*   Updated: 2025/03/24 19:43:41 by mbouhia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include <stdarg.h>
# include <stdint.h>
# include <stdlib.h>
# include <unistd.h>

# define FD_GET -2
# define FD_INIT -1
# define BASE_10 "0123456789"
# define BASE_16_L "0123456789abcdef"
# define BASE_16_U "0123456789ABCDEF"
# define SIGN_BIT 1
# define PLUS_BIT 2
# define MINUS_BIT 4
# define HASH_BIT 8
# define SPACE_BIT 16
# define DOT_BIT 32
# define ZERO_BIT 64
# define IS_PERC_SET 128
# define HEX_PREF_LEN 2
# define UNDEF -1
# define NULL_LIT "(null)"

typedef struct s_number
{
	int				number;
	size_t			length;
}					t_number;

typedef enum e_base_kind
{
	B10 = 0,
	B16U,
	B16L,
}					t_base_kind;

typedef struct s_base
{
	const char		*form;
	size_t			length;
	t_base_kind		kind;
}					t_base;

typedef char		t_spex;
typedef enum e_cconversion
{
	POS_WID = 0,
	PERC,
	NEG_WID,
}					t_cconversion;

typedef struct s_flags
{
	int				perc;
	int				neg_width;
	int				pos_width;
	uint8_t			detected;
	t_cconversion	conv;
}					t_flags;

typedef struct s_fstate
{
	t_base			base;
	t_flags			flags;
	va_list			args;
	const char		*fmt;
	int				written;
	size_t			iter;
}					t_fstate;

size_t				digit_lengthu32(unsigned int num, int base);
void				__cprintf(t_fstate *state);
void				__xprintf(t_fstate *state);
void				__pprintf(t_fstate *state);
void				__iprintf(t_fstate *state);
void				__sprintf(t_fstate *state);
int					ft_printf(const char *fmt, ...);
int					ft_fprintf(int fd, const char *fmt, ...);
void				vprintf__internal(t_fstate *state);
int					ft_vfprintf(int fd, const char *fmt, va_list args);

void				print_flags(t_fstate *state);
void				ft_putu64(unsigned long num, t_fstate *state);
void				ft_putu32(unsigned int num, t_fstate *state, int cond);
void				print_flags(t_fstate *state);
void				ppad(int len, char byte, int *written);
void				ft_parse_int(t_fstate *state);
int					ft_isdigit(int c);
size_t				ft_strlen(char *s);
size_t				digit_length(unsigned long num, int base);
int					chop(const char *fmt, size_t *index, char c);
void				ft_putstr(char *s, int *count, int length);
void				ft_putchar(char c, int *count);
void				parse_flags(t_fstate *state);
void				*ft_memset(void *b, int c, size_t len);
int					clamp(int x);
void				ft_parse_int(t_fstate *state);
int					ft_io(int fd);
void				init_state(t_fstate *state, int fd, const char *fmt);
void				clean_up_state(t_fstate *state);
#endif // !FT_PRINTF_H
