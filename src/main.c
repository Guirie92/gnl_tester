/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guillsan <guillsan@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 14:49:07 by guillsan          #+#    #+#             */
/*   Updated: 2025/11/01 17:39:05 by guillsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/tester.h"
#include "../inc/test_utils.h"

void	create_test_files(void)
{
	/* Create all test files with specific content */
	create_test_file("files/empty", "");
	create_test_file("files/nl", "\n");
	create_test_file("files/41_no_nl", "This is exactly 41 bytes long no nl");
	create_test_file("files/41_with_nl", "This is exactly 41 bytes with nl\n");
	create_test_file("files/42_no_nl", "This is exactly 42 bytes long no nl!");
	create_test_file("files/42_with_nl", "This is exactly 42 bytes with nl!\n");
	create_test_file("files/43_no_nl", "This is exactly 43 bytes long no nl!!");
	create_test_file("files/43_with_nl", "This is exactly 43 bytes with nl!!\n");
	create_test_file("files/multiple_nlx5", "\n\n\n\n\n");
	create_test_file("files/multiple_line_no_nl", "Line 1\nLine 2\nLine 3\nLine 4\nLine 5");
	create_test_file("files/multiple_line_with_nl", "Line 1\nLine 2\nLine 3\nLine 4\nLine 5\n");
	create_test_file("files/alternate_line_nl_no_nl", "Line1\nLine2 no nlLine3\nLine4 no nl");
	create_test_file("files/alternate_line_nl_with_nl", "Line1\nLine2\nLine3\nLine4\nLine5\n");
	create_test_file("files/big_line_no_nl", "This is a very long line without newline that should test buffer behavior when reading large chunks of data without any line breaks in between, just to see how the get_next_line function handles such cases.");
	create_test_file("files/big_line_with_nl", "This is a very long line with newline that should test buffer behavior when reading large chunks of data with a line break at the end, just to see how the get_next_line function handles such cases.\n");
}

int	run_tests_for_current_buffer_size(int *error_count)
{
	t_test_case	*tests;
	int			test_count;

	srand(time(NULL));
	printf(CLR_MGT "[BUFFER_SIZE = %d]" CLR_RESET "\n", BUFFER_SIZE);

	/* Test invalid FDs */
	printf("Invalid fd: ");
	if (!test_invalid_fd())
	{
		(*error_count)++;
		printf(CLR_RED "KO" CLR_RESET);
	}
	else
	{
		printf(CLR_GREEN "OK" CLR_RESET);
	}
	printf("\n");

	/* Run mandatory tests */
	tests = get_mandatory_tests();
	test_count = get_mandatory_test_count();
	run_test_suite("Mandatory", tests, test_count, error_count);

	/* TODO: stdin tests */
	run_stdin_tests();
	printf("\n");

	return (*error_count);
}

int	main(int argc, char **argv)
{
	int total_errors = 0;

	/* Check for verbose flag */
	if (argc > 1 && (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--verbose") == 0))
	{
		set_verbose_mode(1);
		printf("Verbose mode enabled\n");
	}

	/* Create test files */
	create_test_files();

	printf("=== GNL TESTER ===\n");
	printf("Compiled with BUFFER_SIZE = %d\n\n", BUFFER_SIZE);

	/* Run tests for the CURRENT buffer size only */
	run_tests_for_current_buffer_size(&total_errors);

#if MEM_CHECK
#else
	if (total_errors == 0)
		printf(CLR_GREEN "=== ALL TESTS PASSED ===\n" CLR_RESET "\n");
	else
		printf(CLR_RED "=== TESTS COMPLETED WITH %d ERRORS ===" CLR_RESET "\n", total_errors);
#endif
	return (total_errors);
}
