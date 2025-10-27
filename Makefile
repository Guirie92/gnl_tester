# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: guillsan <guillsan@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/27 22:09:51 by guillsan          #+#    #+#              #
#    Updated: 2025/10/27 22:09:53 by guillsan         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Compiler and flags
CC = cc
BUFFER_SIZE ?= 42
CFLAGS = -Wall -Wextra -Werror -D BUFFER_SIZE=$(BUFFER_SIZE)
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes

# Directories
SRC_DIR = src
OBJ_DIR = obj
LIB_DIR = lib
FILES_DIR = files

# Source files
SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/tester.c $(SRC_DIR)/test_cases.c $(SRC_DIR)/test_utils.c
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# GNL files (relative paths to parent directory)
GNL_DIR = ..
GNL_SRCS = $(GNL_DIR)/get_next_line.c $(GNL_DIR)/get_next_line_utils.c
GNL_HEADER = $(GNL_DIR)/get_next_line.h
GNL_BONUS_SRCS = $(GNL_DIR)/get_next_line_bonus.c $(GNL_DIR)/get_next_line_utils_bonus.c
GNL_BONUS_HEADER = $(GNL_DIR)/get_next_line_bonus.h

# Test executable
TARGET = gnl_tester

# Colors for output
GREEN = \033[0;32m
RED = \033[0;31m
YELLOW = \033[0;33m
RESET = \033[0m


all: $(TARGET)

$(TARGET): $(OBJS)
	@echo "Building GNL tester..."
	@if [ -f "$(GNL_DIR)/get_next_line.c" ]; then \
		echo "$(GREEN)Found mandatory GNL files$(RESET)"; \
		$(CC) $(CFLAGS) -I.. -I../.. -o $@ $(OBJS) $(GNL_SRCS); \
	else \
		echo "$(RED)Error: get_next_line.c not found in $(GNL_DIR)$(RESET)"; \
		exit 1; \
	fi

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I.. -I../.. -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

bonus:
	@if [ -f "$(GNL_DIR)/get_next_line_bonus.c" ]; then \
		echo "$(GREEN)Building with bonus files...$(RESET)"; \
		$(CC) $(CFLAGS) -I$(GNL_DIR) -o $(TARGET) $(OBJS) $(GNL_BONUS_SRCS); \
	else \
		echo "$(RED)Bonus files not found, building mandatory only$(RESET)"; \
		$(MAKE) all; \
	fi

test: $(TARGET)
	@./run_tests.sh

test_bonus: bonus
	@./run_tests.sh bonus

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(TARGET)

re: fclean all

# Test with specific buffer sizes
test_1: $(TARGET)
	@echo "$(YELLOW)Testing with BUFFER_SIZE=1$(RESET)"
	@BUFFER_SIZE=1 ./$(TARGET)

test_42: $(TARGET)
	@echo "$(YELLOW)Testing with BUFFER_SIZE=42$(RESET)"
	@BUFFER_SIZE=42 ./$(TARGET)

test_100: $(TARGET)
	@echo "$(YELLOW)Testing with BUFFER_SIZE=100$(RESET)"
	@BUFFER_SIZE=100 ./$(TARGET)

test_9999: $(TARGET)
	@echo "$(YELLOW)Testing with BUFFER_SIZE=9999$(RESET)"
	@BUFFER_SIZE=9999 ./$(TARGET)

test_10000000: $(TARGET)
	@echo "$(YELLOW)Testing with BUFFER_SIZE=10000000$(RESET)"
	@BUFFER_SIZE=10000000 ./$(TARGET)

valgrind: $(TARGET)
	@echo "$(YELLOW)Running with Valgrind (BUFFER_SIZE=42)...$(RESET)"
	@BUFFER_SIZE=42 $(VALGRIND) ./$(TARGET) 2>&1 | (grep -E "(ERROR SUMMARY|definitely lost|indirectly lost|possibly lost)" | grep -v " 0 errors" || true) && echo "$(GREEN)Valgrind: No memory issues$(RESET)" || echo "$(RED)Valgrind: Memory issues detected$(RESET)"

.PHONY: all clean fclean re test bonus test_bonus test_1 test_42 test_100 test_9999 test_10000000 valgrind
.DEFAULT_GOAL = all