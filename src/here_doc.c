#include "minishell.h"
#include "libft.h"
#include <readline/readline.h>

//static void	ft_read_here(char *limiter, int	*fd);

int	ft_here_doc(char *limiter)
{
//	pid_t	pid;
	int		fd[2];

	pipe(fd);
//	pid = fork();
//	if (pid < 0)
//		return (0);
//	if (pid == 0)
//		ft_read_here(limiter, fd);

//static void	ft_read_here(char *limiter, int	*fd)
//{
	int		size;
	char	*line;

	size = ft_strlen(limiter) + 1;
	while (42)
	{
		line = readline ("minishell here_doc> ");
		if (!line)
			break ;
		if (!ft_strncmp(line, limiter, size))
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, fd[1]);
		ft_putstr_fd("\n", fd[1]);
		free(line);
	}
//}
		//	else
//	{
		close(fd[1]);
//		waitpid(pid, NULL, 0);
//	}
	return (fd[0]);
}

