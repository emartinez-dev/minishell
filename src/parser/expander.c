#include "lexer.h"
#include "minishell.h"
#include "libft.h"
#include "fractol_utils.h"
#include "ft_printf.h"

void	expand_tokens(t_lexer *lexer, t_ms *ms)
{
	t_token		*token;
	t_envlst	*env_var;

	token = lexer->token_list;
	while (token)
	{
		if (token->status != SINGLE_QUOTE && token->type != DELIMITER)
		{
			env_var = get_variable_value(token->str, ms);
			while (env_var)
			{
				token->str = replace_env_var(token->str, \
					ft_strjoin("$", env_var->var), env_var->value);
				if (env_var->var[0] == '?')
					ft_free_envlst(env_var);
				env_var = get_variable_value(token->str, ms);
			}
			if (!env_var && ft_strchr(token->str, '$'))
				token->str = replace_env_var(token->str, \
					get_var_name(token->str), "");
		}
		token = token->next;
	}
}

t_envlst	*get_variable_value(char *str, t_ms *ms)
{
	int			i;
	char		*word;
	char		*v_name;
	t_envlst	*env_variable;

	word = ft_strchr(str, '$');
	env_variable = 0;
	if (word)
	{
		i = 1;
		if (word[i] && word[i] == '?')
		{
			env_variable = ft_calloc(1, sizeof(t_envlst));
			env_variable->var = ft_strdup("?");
			env_variable->value = ft_itoa(ms->exit_status);
			return (env_variable);
		}
		while ((word[i] != '\0' && ft_isalpha(word[i])) || word[i] == '_')
			i++;
		v_name = ft_substr(word, 1, i - 1);
		env_variable = ft_getenv(v_name, ms->envlst);
		free(v_name);
	}
	return (env_variable);
}

char	*replace_env_var(char *og, char *find, char *repl)
{
	int		final_len;
	char	*str;
	int		i;
	int		j;

	final_len = ft_strlen(og) + ft_strlen(repl) - ft_strlen(find);
	str = ft_calloc(final_len + 1, sizeof(char));
	i = ft_strnstr(og, find, ft_strlen(og)) - og;
	j = -1;
	while (++j < i)
		str[j] = og[j];
	ft_strlcat(str, repl, final_len + 1);
	j = i + ft_strlen(repl);
	i += ft_strlen(find) - 1;
	while (og[i++] != '\0')
		str[j++] = og[i];
	free(og);
	free(find);
	return (str);
}

char	*get_var_name(char *str)
{
	char	*word;
	char	*v_name;
	int		i;

	v_name = NULL;
	word = ft_strchr(str, '$');
	if (word)
	{
		i = 1;
		while ((word[i] != '\0' && ft_isalpha(word[i])) || word[i] == '_')
			i++;
		v_name = ft_substr(word, 0, i);
	}
	return (v_name);
}
