LOCAL_PATH:=$(call my-dir)

include $(CLEAR_VARS)

LOCAL_LDLIBS  := -llog
APP_STL :=  c++_static

#module name and source files
LOCAL_MODULE := adpcm

#LOCAL_C_INCLUDES := adpcmlib.h
LOCAL_SRC_FILES := pcm_jni.c





include $(BUILD_SHARED_LIBRARY)