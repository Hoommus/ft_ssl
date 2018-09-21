
#include <ft_ssl.h>

void		read_file(int fd, char **result, size_t *size)
{
	char				buffer[1024];
	char				*data;
	char				*swap;

	ft_bzero(buffer, 1024);
	*size = 0;
	data = ft_strnew(0);
	while ((*size += read(fd, buffer, 1024)) > 0)
	{
		swap = ft_strjoin(data, buffer);
		free(data);
		data = swap;
	}
	*result = data;
}
