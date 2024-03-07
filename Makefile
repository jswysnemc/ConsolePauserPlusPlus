# 编译器
CC := gcc

LDFLAGS+='-L-zrelro -L-znow'

PREFIX = ~/.local/share/pauser

INSTALL_PATH = $(PREFIX)/bin

# 源文件目录
SRC_DIR := src

# 头文件目录
INC_DIR := $(SRC_DIR)/include

# 编译选项
CFLAGS := -I$(INC_DIR) 

# 中间文件目录
OBJ_DIR := build/obj

# 目标文件目录
BIN_DIR := build/bin

# 源文件后缀
SRC_EXT := c
# 中间文件后缀
OBJ_EXT := o

# 获取所有源文件
SRC_FILES := $(wildcard $(SRC_DIR)/*.$(SRC_EXT))
# 生成对应的目标文件列表
OBJ_FILES := $(patsubst $(SRC_DIR)/%.$(SRC_EXT), $(OBJ_DIR)/%.$(OBJ_EXT), $(SRC_FILES))

BIN_NAME := pauser

# 目标可执行文件
TARGET := $(BIN_DIR)/$(BIN_NAME)

# 默认目标，编译可执行文件
all: $(TARGET)

# 生成目标文件
$(OBJ_DIR)/%.$(OBJ_EXT): $(SRC_DIR)/%.$(SRC_EXT)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# 链接目标文件生成可执行文件
$(TARGET): $(OBJ_FILES)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $^ -o $@

# 清理中间文件和目标文件
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# 声明伪目标，防止与文件名冲突
.PHONY: all clean


install: $(TARGET)
	# 创建安装路径（如果不存在）
	mkdir -p $(INSTALL_PATH)
	# 将可执行文件复制到安装路径
	cp $(TARGET) $(INSTALL_PATH)
	sudo -E ln -sf $(INSTALL_PATH)/$(BIN_NAME) /usr/bin/$(BIN_NAME)
	