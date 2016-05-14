################################################################################
#
# Makefile for Wave11
#
# Copyright (c) 2012-2016 Sakura-zen soft. All rights reserved.
#
################################################################################

# 出力名
TARGET = ./Wave11.exe

# ビルド対象パス
VPATH = ./

# ビルド対象
OBJS = RiffWavReader.o \
		RiffWavWriter.o \
		main.o

# インクルードフォルダ
# INCLUDE = -I../include

# ライブラリフォルダ
# LDFLAGS = ーL../lib

# ライブラリ
LDLIBS = -lm

# 中間ファイルフォルダ
BUILD_DIR = obj

include ../Makefile.in

CPPFLAGS += -std=c++11
# CFLAGS += D_XX_

dependtmp = $(subst .o,.d,$(OBJS))
dependencies = $(patsubst %,obj/%,$(dependtmp))

$(TARGET): $(patsubst %,obj/%,$(OBJS))
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

$(BUILD_DIR)/%.o : %.cpp
	$(CC) $(CPPFLAGS) $(INCLUDE) -c -o $@ $<

$(BUILD_DIR)/%.o : %.c
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<

.PHONY: .clean

clean:
	$(RM) $(TARGET) $(OBJS) $(dependencies)
	$(RM) -r $(BUILD_DIR)

ifneq "$(MAKECMDGOALS)" "clean"
include $(dependencies)
endif

$(BUILD_DIR)/%.d : %.cpp
	$(MKDIR) $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(INCLUDE) $(TARGET_ARCH) -M $< | \
	$(SED) 's,\($(notdir $*)\.o\) *:,$(dir $@)\1 $@: ,' >$@.tmp
	$(MV) $@.tmp $@

$(BUILD_DIR)/%.d : %.c
	$(MKDIR) $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(INCLUDE) $(TARGET_ARCH) -M $< | \
	$(SED) 's,\($(notdir $*)\.o\) *:,$(dir $@)\1 $@: ,' >$@.tmp
	$(MV) $@.tmp $@

