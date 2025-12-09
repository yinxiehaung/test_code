CC = gcc
CFLAGS = -g -Wall -I./include # <--【修正】加入 -I./include
AR = ar

# --- 您的「框架」函式庫 ---
FRAMEWORK_LIB = lib/libFramework.a

# --- 您的「框架」原始碼 ---
# (core.c, list.c, bst.c)
FRAMEWORK_SRCS = $(wildcard src/*.c) 
FRAMEWORK_OBJS = $(FRAMEWORK_SRCS:src/%.c=obj/%.o)

# --- 您的「測試程式」 ---
TEST_SRCS = test/test_q.c # (假設您的測試程式在 test/ 目錄)
TEST_BIN = bin/test_queue

all: $(TEST_BIN)

# --- 建立「框架」函式庫 ---
# 1. 建立 libFramework.a，它依賴 "所有" 的 .o 檔案
$(FRAMEWORK_LIB): $(FRAMEWORK_OBJS)
	@mkdir -p lib
	$(AR) rcs $@ $^

# --- 建立「測試程式」 ---
# 2. 建立測試程式，它依賴 "框架" 函式庫
$(TEST_BIN): $(TEST_SRCS) $(FRAMEWORK_LIB)
	@mkdir -p bin
	$(CC) $(CFLAGS) $^ -o $@ 

# --- 模式規則 (Pattern Rule) ---
# 3. 將 .c 編譯到 "obj/" 目錄下
obj/%.o: src/%.c
	@mkdir -p obj
	$(CC) $(CFLAGS) -c -o $@ $<

# 4. (測試程式的 .o)
test/%.o: test/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

# --- 清理 ---
clean:
	rm -f $(FRAMEWORK_LIB) $(FRAMEWORK_OBJS) $(TEST_BIN)
.PHONY: all clean
