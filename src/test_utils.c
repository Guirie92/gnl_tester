/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guillsan <guillsan@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 14:46:54 by guillsan          #+#    #+#             */
/*   Updated: 2025/11/01 17:48:10 by guillsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/test_utils.h"

int				g_verbose = FALSE;
unsigned int	g_b_flush = TRUE;
float			g_flush_seconds = 0.01;

void	flush_sleep(void)
{
	if (!g_b_flush)
		return ;
	fflush(stdout);
	usleep((g_flush_seconds * 1000) * 1000); /* 40ms by default */
}

/* Function to generate a random integer in the range [min, max] */
int	get_random_number(int min, int max)
{
	if (min > max)
		return (0);
	/* Formula: rand() % (max - min + 1) + min */
	return ((rand() % (max - min + 1)) + min);
}

void set_verbose_mode(int verbose)
{
    g_verbose = verbose;
}

void	print_result(const char *test_name, int result)
{
#if MEM_CHECK

	(void)test_name;
	(void)result;
#else

	if (result)
		printf(CLR_GREEN "OK" CLR_RESET);
	else
		printf(CLR_RED "KO" CLR_RESET);
#endif

	flush_sleep();
	(void)test_name;
}

void	print_result_testnum(int result, int testnum)
{
#if MEM_CHECK

	(void)result;
	(void)testnum;
#else

	if (result)
		printf(CLR_GREEN " %d.OK" CLR_RESET, testnum);
	else
		printf(CLR_RED " %d.KO" CLR_RESET, testnum);
#endif

	flush_sleep();
}

void	print_group_result(const char *test_name, int result)
{
#if MEM_CHECK

	(void)test_name;
	(void)result;
#else

	if (result)
		printf(CLR_GREEN " [ALL_OK]" CLR_RESET);
	else
		printf(CLR_RED " [KO]" CLR_RESET);
#endif

	(void)test_name;
}

char	*read_whole_file(const char *filename)
{
	FILE	*file;
	char	*buffer;
	long	file_size;

	file = fopen(filename, "r");
	if (!file)
		return (NULL);

	fseek(file, 0, SEEK_END);
	file_size = ftell(file);
	fseek(file, 0, SEEK_SET);

	buffer = malloc(file_size + 1);
	if (!buffer)
	{
		fclose(file);
		return (NULL);
	}

	fread(buffer, 1, file_size, file);
	buffer[file_size] = '\0';
	fclose(file);
	return (buffer);
}

void	create_test_file(const char *filename, const char *content)
{
	FILE	*file;

	file = fopen(filename, "w");
	if (!file)
		return ;

	fprintf(file, "%s", content);
	fclose(file);
}

void	print_escaped(const char *str, size_t len)
{
	for (size_t i = 0; i < len && str[i]; i++)
	{
		if (str[i] == '\n') printf("\\n");
		else if (str[i] == '\t') printf("\\t");
		else if (str[i] == '\r') printf("\\r");
		else if (str[i] == '\0') printf("\\0");
		else printf("%c", str[i]);
	}
}

t_test_result	*test_single_file(const char *filename, int testidx)
{
	int		fd;
	char	*line;
	char	*whole_file;
	char	*ptr;
	int		line_num;
	int		max_itr;
#if MEM_CHECK

	int		mallocmin;
	int		mallocmax;
	int		i;
#else

	(void)testidx;
#endif

	t_test_result *result = malloc(sizeof(t_test_result));
	result->passed = 1;
	result->total_lines = 0;
	result->failures = NULL;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		printf(CLR_RED "Cannot open file: %s" CLR_RESET "\n", filename);
		result->passed = 0;
		return (result);
	}

	whole_file = read_whole_file(filename);
	if (!whole_file)
	{
		close(fd);
		result->passed = 0;
		return (result);
	}

	ptr = whole_file;
	max_itr = 15;
	line_num = 1;

#if MEM_CHECK

	i = 0;
	mallocmin = 1;
	mallocmax = 6;

	malloc_set_null(testidx % mallocmax);
#endif

	while ((line = get_next_line(fd)) != NULL && line_num < max_itr)
	{
#if MEM_CHECK

		malloc_set_null(i % mallocmax + mallocmin);
		i++;

#endif

		/* Find the next line in the whole file */
		char *next_newline = strchr(ptr, '\n');
		size_t expected_len;

		if (next_newline)
			expected_len = next_newline - ptr + 1;
		else
			expected_len = strlen(ptr);

		/* Compare with what GNL returned */
		if (strlen(line) != expected_len || strncmp(line, ptr, expected_len) != 0)
		{
			result->passed = 0;

			/* Create failure entry */
			t_test_failure *failure = malloc(sizeof(t_test_failure));
			failure->line_number = line_num;
			failure->expected = malloc(expected_len + 1);
			failure->actual = malloc(strlen(line) + 1);

			strncpy(failure->expected, ptr, expected_len);
			failure->expected[expected_len] = '\0';
			strcpy(failure->actual, line);

			/* Add to linked list */
			failure->next = result->failures;
			result->failures = failure;
		}

		ptr += expected_len;
		free(line);
		print_result_testnum(line_num, line_num);
		line_num++;
		flush_sleep();
	}

	if (ptr != whole_file + strlen(whole_file))
	{
		result->passed = 0;
		t_test_failure *failure = malloc(sizeof(t_test_failure));
		failure->line_number = INCOMPLETE_READ;
		failure->expected = strdup("EOF");
		failure->actual = strdup("More data remains in file");
		failure->next = result->failures;
		result->failures = failure;
	}

	/* Test that subsequent calls return NULL */
	char *extra_line = get_next_line(fd);
	if (extra_line != NULL)
	{
		result->passed = 0;
		t_test_failure *failure = malloc(sizeof(t_test_failure));
		failure->line_number = EXTRA_DATA;
		failure->expected = strdup("NULL");
		failure->actual = malloc(strlen(extra_line) + 1);
		strcpy(failure->actual, extra_line);
		failure->next = result->failures;
		result->failures = failure;
		free(extra_line);
	}

	free(whole_file);
	close(fd);
	return (result);
}

void print_test_summary(const char *test_name, t_test_result *result)
{
	int line_failed = 0;

	for (int i = 1; i <= result->total_lines; i++)
	{
		/* Check if this line failed */
		t_test_failure *failure = result->failures;
		while (failure)
		{
			if (failure->line_number == i)
			{
				line_failed = 1;
				break ;
			}
			failure = failure->next;
		}
#if MEM_CHECK

		(void)line_failed;
#else

		if (line_failed)
			printf(CLR_RED "%d.KO " CLR_RESET, i);
		else
			printf(CLR_GREEN "%d.OK " CLR_RESET, i);
#endif

	}

	print_group_result(test_name, result->passed);

	printf("\n");

	/* Print failure details if any and verbose mode is on */

#if MEM_CHECK

#else

	if (g_verbose && result->failures != NULL)
	{
		t_test_failure *failure = result->failures;
		while (failure)
		{
			if (failure->line_number > 0)
			{
				printf("    %s[%d]: Expected: [", test_name, failure->line_number);
				print_escaped(failure->expected, strlen(failure->expected));
				printf("], Got: [");
				print_escaped(failure->actual, strlen(failure->actual));
				printf("]\n");
			}
			else if (failure->line_number == INCOMPLETE_READ)
			{
				printf("    %s: " CLR_RED "FAIL: Didn't read entire file" CLR_RESET "\n", test_name);
			}
			else if (failure->line_number == EXTRA_DATA)
			{
				printf("    %s: " CLR_RED "FAIL: Expected NULL after EOF, got: [" CLR_RESET, test_name);
				print_escaped(failure->actual, strlen(failure->actual));
				printf(CLR_RED "]" CLR_RESET "\n");
			}
			failure = failure->next;
		}
	}
#endif

}

void	ree_test_result(t_test_result *result)
{
	t_test_failure *failure = result->failures;

	while (failure)
	{
		t_test_failure *next = failure->next;
		free(failure->expected);
		free(failure->actual);
		free(failure);
		failure = next;
	}
	free(result);
}


int	test_invalid_fd(void)
{
	char	*result;
	int		success;

	/* Test with invalid file descriptors */
	success = 1;

	/* Test negative FD */
	result = get_next_line(-1);
	if (result != NULL)
	{
		if (g_verbose)
			printf(CLR_RED "\n    FAIL: Expected NULL for fd=-1, got [%s]" CLR_RESET "\n", result);
		free(result);
		success = 0;
	}

	/* Test very large FD */
	result = get_next_line(9999);
	if (result != NULL)
	{
		if (g_verbose)
			printf(CLR_RED "\n    FAIL: Expected NULL for fd=9999, got [%s]" CLR_RESET "\n", result);
		free(result);
		success = 0;
	}

	return (success);
}
