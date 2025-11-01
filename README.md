# GNL Tester

A lightweight, self-contained tester for **42’s GNL**.  
It compiles your files and runs them.



## Project Structure

```
GNL/ # ---------------------------------- target repo
├─ get_next_line.h
├─ *.c
└─ tester/ # ---------------------------- this tester
	├─ run_tests.sh # ------------------- main script
	├─ Makefile # ----------------------- builds test binary and dependencies
	├─ src # ---------------------------- .c files
	|	├─ main.c
	|	├─ tester.c
	|	├─ malloc_mock.c
	|	└─ ...
	└─ inc # ---------------------------- header files
	 	├─ tester.h
	 	└─ ...
```


## Usage

From inside the target **get_next_line** repository:

```bash

cd get_next_line
git clone https://github.com/Guirie92/gnl_tester.git # or name it as you wish
cd gnl_tester
./run_tests.sh

```
This will:

- Build and run all tests
- Run with Valgrind to report memory leaks and errors


## Run Specific Tests


To run with a mem-check/valgrind output, use -m (or --mem/--memcheck/--strict):
```bash

./run_tests.sh -m

```
**Do not worry if you don't see green OK marks on each test.
The important output from the mem check is the error trace and the
final Valgrind assessment.**

For instance:
```
=== Valgrind Summary ===
✓ All Valgrind tests passed - No memory issues detected

================================================================
================ TESTING COMPLETED SUCCESSFULLY ================
================================================================
```
or

```
=== Valgrind Summary ===
✗ Valgrind detected issues in 5 buffer size(s)

================================================================
=============== TESTING COMPLETED WITH 10 ERRORS ===============
================================================================
```


To run the bonus, you can use -b:
```bash

./run_tests.sh -b

```
To run with a verbose output, use --verbose:
```bash

./run_tests.sh --verbose

```


## Example Output

```yaml

✗ BUFFER_SIZE=10000000: ==24204== ERROR SUMMARY: 130 errors from 17 contexts (suppressed: 0 from 0)
run_tester.sh: line 164: Show: command not found

Sample error contexts:
==24204== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==24204== Command: ./gnl_tester
==24204== 
==24204== Conditional jump or move depends on uninitialised value(s)
==24204==    at 0x10AB96: cleanup (in /home/guille/Dev/42madrid/delete/deleeeet/gnl_tester_nrepo/gnl_tester)
==24204==    by 0x10A4CE: get_next_line (in /home/guille/Dev/42madrid/delete/deleeeet/gnl_tester_nrepo/gnl_tester)
==24204==    by 0x10A399: test_invalid_fd (in /home/guille/Dev/42madrid/delete/deleeeet/gnl_tester_nrepo/gnl_tester)
--
==24204==  Uninitialised value was created by a stack allocation
==24204==    at 0x10A3E2: get_next_line (in /home/guille/Dev/42madrid/delete/deleeeet/gnl_tester_nrepo/gnl_tester)
==24204== 
==24204== Conditional jump or move depends on uninitialised value(s)
==24204==    at 0x484B235: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==24204==    by 0x10ABA6: cleanup (in /home/guille/Dev/42madrid/delete/deleeeet/gnl_tester_nrepo/gnl_tester)
==24204==    by 0x10A4CE: get_next_line (in /home/guille/Dev/42madrid/delete/deleeeet/gnl_tester_nrepo/gnl_tester)
--
==24204==  Uninitialised value was created by a stack allocation
==24204==    at 0x10A3E2: get_next_line (in /home/guille/Dev/42madrid/delete/deleeeet/gnl_tester_nrepo/gnl_tester)
==24204== 
==24204== Invalid free() / delete / delete[] / realloc()
==24204==    at 0x484B27F: free (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==24204==    by 0x10ABA6: cleanup (in /home/guille/Dev/42madrid/delete/deleeeet/gnl_tester_nrepo/gnl_tester)
==24204==    by 0x10A4CE: get_next_line (in /home/guille/Dev/42madrid/delete/deleeeet/gnl_tester_nrepo/gnl_tester)
--
[BUFFER_SIZE = 10000000]
Invalid fd: OK
[Mandatory]:
empty: ==24204== Conditional jump or move depends on uninitialised value(s)
==24204==    at 0x10AB96: cleanup (in /home/guille/Dev/42madrid/delete/deleeeet/gnl_tester_nrepo/gnl_tester)
==24204==    by 0x10A52F: get_next_line (in /home/guille/Dev/42madrid/delete/deleeeet/gnl_tester_nrepo/gnl_tester)


=== Valgrind Summary ===
✗ Valgrind detected issues in 5 buffer size(s)
Run with --verbose for detailed information


================================================================
============== TESTING COMPLETED WITH 695 ERRORS ===============
================================================================

```

```yaml

[BUFFER_SIZE = 10000000]
Invalid fd: OK
[Mandatory]:
empty:  [ALL_OK]
nl:  1.OK [ALL_OK]
41_no_nl:  1.OK [ALL_OK]
41_with_nl:  1.OK [ALL_OK]
42_no_nl:  1.OK [ALL_OK]
42_with_nl:  1.OK [ALL_OK]
43_no_nl:  1.OK [ALL_OK]
43_with_nl:  1.OK [ALL_OK]
multiple_nlx5:  1.OK 2.OK 3.OK 4.OK 5.OK [ALL_OK]
multiple_line_no_nl:  1.OK 2.OK 3.OK 4.OK 5.OK [ALL_OK]
multiple_line_with_nl:  1.OK 2.OK 3.OK 4.OK 5.OK [ALL_OK]
alternate_line_nl_no_nl:  1.OK 2.OK 3.OK [ALL_OK]
alternate_line_nl_with_nl:  1.OK 2.OK 3.OK 4.OK 5.OK [ALL_OK]
big_line_no_nl:  1.OK [ALL_OK]
big_line_with_nl:  1.OK [ALL_OK]
stdin: SKIPPED (needs pipe implementation)

=== ALL TESTS PASSED ===

==39054== 
==39054== HEAP SUMMARY:
==39054==     in use at exit: 0 bytes in 0 blocks
==39054==   total heap usage: 175 allocs, 175 frees, 530,139,545 bytes allocated
==39054== 
==39054== All heap blocks were freed -- no leaks are possible
==39054== 
==39054== For lists of detected and suppressed errors, rerun with: -s
==39054== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
✓ BUFFER_SIZE=10000000: No memory errors
run_tester.sh: line 164: Show: command not found

=== Valgrind Summary ===
✓ All Valgrind tests passed - No memory issues detected


================================================================
================ TESTING COMPLETED SUCCESSFULLY ================
================================================================

```


## Requirements

- Bash shell (on macOS/Linux or WSL)


## License

MIT License (free to use and modify)