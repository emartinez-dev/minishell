#include "minishell.h"
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "fractol_utils.h"
#include <unistd.h>
#include "lexer.h"
//char *readline (char *prompt); 

static t_ms	ft_init(char **env);
static void	ft_free(t_ms ms);
static void	ft_prompt(t_ms *ms);

static void	ft_leaks(void)
{
	system("leaks -q minishell");
}

int	main(int ac, char **av, char **env)
{
	t_ms	ms;
	char	*prompt;
	int		stat;

	atexit(ft_leaks);
	(void) ac;
	(void) av;
	ms = ft_init(env);
	while (ms.exit)
	{
//		prompt = ft_calloc(sizeof(char), 100);
		prompt = readline (ms.prompt);
		debug_tokenize(prompt);
		ft_pruebas(prompt, &ms);
		if (!(!prompt || !*prompt))
			add_history(prompt);
		free(prompt);
		free(ms.prompt);
		ft_prompt(&ms);
	}
	stat = ms.exit_status;
	ft_free(ms);
	rl_clear_history();
	return (stat);
}

static t_ms	ft_init(char **env)
{
	t_ms	ms;

	ms.num_com = 0;
	ms.exit = 1;
	ms.exit_status = 0;
	ft_prompt(&ms);
	ms.env = ft_copy_array(env, 0);
	if (ms.env[0])
		ft_shlvl_update(&ms);
	return (ms);
}

static void	ft_prompt(t_ms *ms)
{
	char	*dir;

	dir = NULL;
	dir = getcwd(dir, 1000);
	if (ms->exit_status)
		ms->prompt = ft_strjoin_va("%s minishell %s%s %s %s%s$>%s ", CBLUE,
				CRESET, CWHITE, ft_strrchr(dir, '/') + 1, CRESET, CRED, CRESET);
	else
		ms->prompt = ft_strjoin_va("%s minishell %s%s %s %s%s$>%s ", CBLUE,
				CRESET, CWHITE, ft_strrchr(dir, '/') + 1,
				CRESET, CGREEN, CRESET);
	free(dir);
}

static void	ft_free(t_ms ms)
{
	int	i;

	i = 0;
	free(ms.prompt);
	while (ms.env[i])
	{
		free(ms.env[i]);
		i++;
	}
	free(ms.env[i]);
	free(ms.env);
}
