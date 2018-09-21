
#include "ft_ssl.h"

void		print_error(char *cause, char *error)
{
	if (cause != 0)
		ft_printf("ft_ssl: %s: %s", cause, error);
	else
		ft_printf("ft_ssl: %s", error);
}
