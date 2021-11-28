/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chilton <chilton@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/24 19:22:33 by Jraye             #+#    #+#             */
/*   Updated: 2021/05/19 18:11:17 by chilton          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <fcntl.h>
# include <string.h>
# include <stdio.h>
# include <term.h>
# include <termios.h>
# include <limits.h>
# include <errno.h>
# include <signal.h>
# include "libft.h"

# define PROMPTN "\e[38;5;208mminishell> \e[0m"
# define HISTORY_FILE_NAME ".history"
# define TABLE_BLOCK_SIZE 8
# define SYNTAX_ERROR 258
# define EXIT_OUT_OF_RANGE 255
# define BACKSPACE "\177"
# define CTRL_D "\004"
# define CTRL_C "\003"

typedef struct s_table
{
	void	**table;
	size_t	size;
	size_t	available;
}t_table;

typedef struct s_env_item
{
	char	*key;
	char	*value;
}t_env_item;

typedef struct s_shell
{
	t_list		*env_list;
	t_table		history;
	char		*history_path;
	char		*cmd_last_arg;
	int			status;
}t_shell;

typedef enum e_redir_type
{
	IN,
	OUT,
	OUT_APPEND
}t_redir_type;

typedef struct s_redirect
{
	t_redir_type	type;
	char			*file_name;
}t_redirect;

typedef struct s_command
{
	char		**args;
	t_redirect	**redirs;
}t_command;

/*
**						*Termcap*
*/
char		*read_line(t_shell *shell);
void		del_cycle(char *line);
void		del_one(char *line);
void		add_symbol(char **line, char *buf);
void		quit(char *line, struct termios *old_term, t_shell *sh);
void		add_to_history(char *line, t_table *history);
/*
**						*Parsing*
*/
char		*get_token(char **line, int *status);
char		**split_line_to_tokens(char *line, int *status);
char		*parse_token(char *token, t_shell *sh);
void		get_commands(char ***tokens, t_table *commands, t_shell *sh);
/*
**						*Execute*
*/
void		exec_simple_cmd(t_command *cmd, t_shell *sh);
void		exec_pipe_cmds(t_command **cmds, size_t size, t_shell *sh);
void		exec_cmd(char *cmd_path, char **args,
				t_list *env_list, char *last_arg);
int			exec_builtin(char **args, t_shell *sh);
char		*get_cmd_path(char *cmd, t_list *env_list, int *status);
int			make_redirects(t_redirect **redirs, t_shell *sh);
int			get_exit_status(int exit);
char		*get_last_arg(char **args);
void		set_last_arg(char **last_arg, char *arg);
/*
**						*Pipes*
*/
void		init_pipes(int ***fd, size_t size);
void		free_pipes(int **fd, size_t size);
void		close_pipes(int **fd, size_t size);
void		dup_fd(size_t i, int **pipes_fd, size_t size);
/*
**						*Builtins*
*/
void		ft_exit(char **args, t_shell *sh);
void		ft_env(char **args, t_list *env_list, char *last_arg, int *status);
void		ft_echo(char **args, int *status);
void		ft_unset(char **args, t_list **env_list, int *status);
void		ft_export(char **args, t_list **env_list, int *status);
void		ft_pwd(int *status);
void		ft_cd(char **args, t_list *env_list, int *status);
/*
**						*Utilites*
*/
void		init_minishell(t_shell *sh, char **envp);
void		save_history(t_table *history, char *history_path);
char		*ft_strappendstr(char *line, char *added_line);
char		*ft_strappendchar(char *line, char c);
int			is_id_valid(char *identifer);
void		invalid_id_msg(char *id, char *cmd);
void		invalid_option_msg(char *option, char *cmd, char *msg);
void		error_msg(char *cmd, char *msg);
void		clear_command(void *cmd);
void		exit_error(char *msg);
/*
**	Environment list
*/
int			env_cmp(t_env_item *env, const char *key);
void		env_free(void *env_item);
void		env_list_add_back(t_list **lst, char *key, char *value);
char		*env_list_get_value(t_list *lst, char *key);
void		env_list_update_key(t_list **lst, char *key, char *new_value);
/*
**	Table
*/
void		add_row_to_table(t_table *table, void *new_row);
char		**env_list_to_table(t_list *list, char *cmd_last_arg);
void		free_table(t_table *table, void (*del)(void*));
void		sort_table(t_table *table, int (*cmp)());
#endif
