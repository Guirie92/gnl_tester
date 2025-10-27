#!/bin/bash

# Colors
CLR_GREEN='\033[0;32m'
CLR_RED='\033[0;31m'
CLR_YELLOW='\033[0;33m'
CLR_CYAN='\x1b[36m'
CLR_MGT='\033[0;35m'
CLR_RESET='\033[0m'

# Default values
VALGRIND_MODE="verbose"  # can be "verbose" or "simple"

# Parse command line arguments
for arg in "$@"; do
    case $arg in
        --simple|-s)
            VALGRIND_MODE="simple"
            shift
            ;;
        --verbose|-v)
            VALGRIND_MODE="verbose"
            shift
            ;;
    esac
done

# Buffer sizes to test
BUFFER_SIZES="1 42 100 9999 10000000"

# Test files directory
TEST_FILES="files"

# Create test files directory if it doesn't exist
mkdir -p $TEST_FILES

echo -e "${CLR_GREEN}=== GNL Comprehensive Tester ===${CLR_RESET}"
echo "Valgrind mode: $VALGRIND_MODE"
echo

# Check if we're in the right directory
if [ ! -f "../get_next_line.c" ]; then
    echo -e "${CLR_RED}Error: get_next_line.c not found in parent directory${CLR_RESET}"
    echo "Current directory: $(pwd)"
    echo "Please run this script from the tester/ directory"
    exit 1
fi

echo -e "${CLR_GREEN}GNL files found in parent directory${CLR_RESET}"
echo

# Build the tester
make -s fclean
echo -e "Building tester..."
make

if [ $? -ne 0 ]; then
    echo -e "${CLR_RED}Build failed!${CLR_RESET}"
    exit 1
fi

echo -e "${CLR_GREEN}Build successful!${CLR_RESET}"
echo

# Check for bonus
if [ "$1" = "bonus" ]; then
    if [ -f "../get_next_line_bonus.c" ]; then
        echo -e "Testing bonus version..."
        make bonus
    else
        echo -e "${CLR_RED}Bonus files not found! Testing mandatory only.${CLR_RESET}"
    fi
fi

# Check for verbose test output
if [ "$VALGRIND_MODE" = "verbose" ]; then
    TEST_VERBOSE_FLAG="-v"
else
    TEST_VERBOSE_FLAG=""
fi

# Run tests for each buffer size by recompiling
total_errors=0
for size in $BUFFER_SIZES; do
    echo -e "$Testing with BUFFER_SIZE=$size"
    
    # Clean and recompile with the specific buffer size
    make BUFFER_SIZE=$size fclean > /dev/null 2>&1
    make BUFFER_SIZE=$size > /dev/null 2>&1
    
    if [ $? -ne 0 ]; then
        echo -e "${CLR_RED}Compilation failed for BUFFER_SIZE=$size${CLR_RESET}"
        total_errors=$((total_errors + 1))
        continue
    fi
    
    # Run the tests
    ./gnl_tester $TEST_VERBOSE_FLAG
    exit_code=$?
    total_errors=$((total_errors + exit_code))
    echo
done


# Final test summary
echo -e "=== TEST SUMMARY ==="
if [ $total_errors -eq 0 ]; then
    echo -e "${CLR_GREEN}✓ All tests passed!\n${CLR_RESET}"
else
    echo -e "${CLR_RED}✗ $total_errors test groups failed${CLR_RESET}"
fi
echo -e "\n${CLR_CYAN}PROCEEDING WITH A MEMORY CHECK, RUNNING VALGRIND${CLR_RESET}\n"



# Valgrind check
echo -e "Running Valgrind memory check..."
if command -v valgrind >/dev/null 2>&1; then
    # Buffer sizes to test with Valgrind (remove large tests if performance hurts)
    VALGRIND_BUFFER_SIZES="1 42 100 9999 10000000"
    valgrind_errors=0
    
    for size in $VALGRIND_BUFFER_SIZES; do
        echo -e "${CLR_CYAN}Testing Valgrind with BUFFER_SIZE=$size${CLR_RESET}"
        
        # Recompile with specific buffer size
        echo -e "Compiling with BUFFER_SIZE=$size..."
        make BUFFER_SIZE=$size fclean > /dev/null 2>&1
        make BUFFER_SIZE=$size > /dev/null 2>&1
        
        if [ $? -ne 0 ]; then
            echo -e "${CLR_RED}Compilation for Valgrind failed for BUFFER_SIZE=$size${CLR_RESET}"
            valgrind_errors=$((valgrind_errors + 1))
            continue
        fi
        
        if [ "$VALGRIND_MODE" = "verbose" ]; then
            # Verbose Valgrind
            TEMP_VALGRIND_FILE=$(mktemp)
            
            echo -e "Running Valgrind..."
            valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1 ./gnl_tester 2>&1 | tee "$TEMP_VALGRIND_FILE" || true
            
            # Extract ERROR SUMMARY
            ERROR_SUMMARY=$(grep "ERROR SUMMARY:" "$TEMP_VALGRIND_FILE" | head -1)
            ERROR_COUNT=$(echo "$ERROR_SUMMARY" | grep -oE '[0-9]+ errors' | grep -oE '[0-9]+' || echo "0")
            
            if [ -n "$ERROR_SUMMARY" ]; then
                if [ "$ERROR_COUNT" -eq 0 ]; then
                    echo -e "${CLR_GREEN}✓ BUFFER_SIZE=$size: No memory errors${CLR_RESET}"
                else
                    echo -e "${CLR_RED}✗ BUFFER_SIZE=$size: $ERROR_SUMMARY${CLR_RESET}"
                    valgrind_errors=$((valgrind_errors + 1))
                    
                    # Show brief details
                    if grep -q "definitely lost:" "$TEMP_VALGRIND_FILE"; then
                        echo -e "${CLR_YELLOW}  Leaks: " $(grep "definitely lost:" "$TEMP_VALGRIND_FILE" | head -1) "${CLR_RESET}"
                    fi
                    if grep -q "Invalid read\|Invalid write" "$TEMP_VALGRIND_FILE"; then
                        echo -e "${CLR_YELLOW}  Memory errors detected${CLR_RESET}"
                    fi
                    # Show some error contexts
                    echo
                    echo -e "${CLR_YELLOW}Sample error contexts:"
                    grep -B3 -A3 "==.*==.*Conditional\|==.*==.*Invalid" "$TEMP_VALGRIND_FILE" | head -30
                    echo -e "${CLR_RESET}"
                fi
            fi
            
            rm -f "$TEMP_VALGRIND_FILE"
        else
            # Simple Valgrind
            valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --error-exitcode=1 ./gnl_tester > /dev/null 2>&1
            
            if [ $? -eq 0 ]; then
                echo -e "${CLR_GREEN}✓ BUFFER_SIZE=$size: No memory issues${CLR_RESET}"
            else
                echo -e "${CLR_RED}✗ BUFFER_SIZE=$size: Memory issues detected${CLR_RESET}"
                valgrind_errors=$((valgrind_errors + 1))
            fi
        fi
        echo
    done
    
    # Valgrind summary
    echo -e "=== Valgrind Summary ==="
    if [ $valgrind_errors -eq 0 ]; then
        echo -e "${CLR_GREEN}✓ All Valgrind tests passed - No memory issues detected${CLR_RESET}"
    else
        echo -e "${CLR_RED}✗ Valgrind detected issues in $valgrind_errors buffer size(s)${CLR_RESET}"
    fi
else
    echo -e "${CLR_YELLOW}Valgrind not found, skipping memory checks${CLR_RESET}"
fi

echo
if [ $total_errors -eq 0 ]; then
    echo -e "${CLR_GREEN}================================================================"
    echo -e "================ TESTING COMPLETED SUCCESSFULLY ================"
    echo -e "================================================================${CLR_RESET}"
else
    echo -e "${CLR_RED}================================================================"
    echo -e "      ====== TESTING COMPLETED WITH $total_errors ERRORS ======"
    echo -e "================================================================${CLR_RESET}"
fi