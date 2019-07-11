LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_LDLIBS := -llog
LOCAL_MODULE    := substrate
LOCAL_SRC_FILES := hde64.c Debug.cpp Hooker.cpp PosixMemory.cpp
include $(BUILD_SHARED_LIBRARY)
