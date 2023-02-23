#include "minishell.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "fractol_utils.h"

static void	ft_cd_error(char *str, t_ms *ms, int err, char **to_free);
static void	ft_cd_update(t_ms *ms, char **dir);

void	ft_exit(t_ms *ms)
{
	ft_putstr_fd("exit\n", 2);
	ms->exit = 0;
}

void	ft_pwd(t_ms *ms)
{
	char	*dir;

	(void) ms;
	dir = NULL;
	dir = getcwd(dir, 1000);
	printf("%s\n", dir);
	free(dir);
	exit(0);
}

void	ft_cd(char *str, t_ms *ms)
{
	char		**tmp;
	t_envlst	*lst;

	tmp = ft_calloc(sizeof(char *), 3);
	tmp[0] = getcwd(tmp[0], 1000);
	if (!str)
	{
		lst = ft_getenv("HOME", ms);
		if (!lst)
		{
			ft_cd_error(str, ms, 0, tmp);
			return ;
		}
		chdir(lst->value);
	}
	else
	{
		tmp[1] = ft_strjoin_va("%s/%s", tmp[0], str);
		if (chdir(tmp[1]) && chdir(str))
		{
			ft_cd_error(str, ms, 1, tmp);
			return ;
		}
	}
	ms->exit_status = 0;
	ft_cd_update(ms, tmp);
}

static void	ft_cd_error(char *str, t_ms *ms, int err, char **to_free)
{
	char	*tmp;

	if (!err)
		ft_putstr_fd("minishell: cd: HOME: not set\n", 2);
	else
	{
		tmp = ft_strjoin_va(
				"minishell: cd: %s: No such file or directory\n", str);
		ft_putstr_fd(tmp, 2);
	}	
	ms->exit_status = 1;
	ft_free_array(to_free, 0);
}

static void	ft_cd_update(t_ms *ms, char **dir)
{
	t_envlst	*old;
	t_envlst	*pwd;
	char		**to_export;

	to_export = ft_calloc(sizeof(char *), 4);
	old = ft_getenv("OLDPWD", ms);
	pwd = ft_getenv("PWD", ms);
	if (!old)
		to_export[0] = ft_strdup("OLDPWD\0");
	else if (pwd)
		to_export[0] = ft_strjoin("OLDPWD=%s", pwd->value);
	else
		to_export[0] = ft_strjoin("OLDPWD=%s", dir[0]);
	if (pwd)
	{
		to_export[4] = getcwd(to_export[4], 100);
		to_export[1] = ft_strjoin("PWD=%s", to_export[4]);
		free(to_export[4]);
	}
	ft_export(to_export, ms);
	ft_free_array(to_export, 0);
	ft_free_array(dir, 0);
}
