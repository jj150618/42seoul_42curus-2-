
#include "ft_printf.h"

void	print_with_flag(t_info *info, char *str, int *len, int s_len)
{
	int		temp;
	int		flag;
	char	c;

	flag = (info->is_zero && !info->precision && ft_strchr("diuxXs", info->conversion));
	temp = *len;
	c = (flag || (info->is_zero && info->conversion == '%')) ? '0' : ' ';
	while (!info->is_minus && info->width > (*len + s_len))
	{
		if (flag == 1)
		{
			write(1, info->suffix, s_len);
			flag = -1;
		}
		(*len) += write(1, &c, 1);
	}
	if (flag != -1)
		write(1, info->suffix, s_len);
	write(1, str, temp);
	while (info->is_minus && info->width > (*len + s_len))
		(*len) += write(1, " ", 1);
	(*len) += s_len;
}

int		print_format(t_info *info, va_list *ap, int *count)
{
	int		len;
	int		s_len; 
	char	*str;

	len = 0;
	str = handle_con(info, ap, &len, count);
	s_len = handle_flag(info, str);
	if (info->error)
		return (handle_return_fail(str, info->suffix));
	if ((ft_strchr("diuxX", info->conversion) && info->precision > len) || \
		info->conversion == 'p')
		handle_prec1(&str, info, &len, &s_len);
	else if (info->conversion == 's' && info->precision && info->precision < len)
		handle_prec2(&str, info, &len);
	if (info->error)
		return (handle_return_fail(str, info->suffix));
	else if (str)
	{
		print_with_flag(info, str, &len, s_len);
		free(str);
	}
	free(info->suffix);
	(*count) += len;
	return (0);
}
