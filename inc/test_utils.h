/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_utils.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guillsan <guillsan@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 14:46:31 by guillsan          #+#    #+#             */
/*   Updated: 2025/10/27 22:22:39 by guillsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_UTILS_H
# define TEST_UTILS_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <fcntl.h>
# include <unistd.h>

/* Include GNL header from parent directory */
# include "../../get_next_line.h"

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

# define TRUE  1
# define FALSE 0

/* Colors for output (matching shell script) */
# define CLR_GREEN  "\033[0;32m"
# define CLR_RED    "\033[0;31m"
# define CLR_YELLOW "\033[0;33m"
# define CLR_CYAN   "\x1b[36m"
# define CLR_MGT    "\033[0;35m"
# define CLR_RESET  "\033[0m"

/* Global verbose flag */
extern int g_verbose;

/* Test failure structure */
typedef enum e_ret_code
{
    EXTRA_DATA = -2,
    INCOMPLETE_READ = -1
}	t_ret_code;

/* Test failure structure */
typedef struct s_test_failure
{
    int line_number;
    char *expected;
    char *actual;
    struct s_test_failure *next;
}	t_test_failure;

/* Test result structure */
typedef struct s_test_result
{
    int passed;
    int total_lines;
    t_test_failure *failures;
}	t_test_result;

/* Test utilities */
void	        print_result(const char *test_name, int result);
char	        *read_whole_file(const char *filename);
void	        create_test_file(const char *filename, const char *content);
int				test_invalid_fd(void);
void	        set_verbose_mode(int verbose);
void			print_test_summary(const char *test_name, t_test_result *result);
void	        free_test_result(t_test_result *result);
void			flush_sleep();
t_test_result	*test_single_file(const char *filename);

#endif /* TEST_UTILS_H */