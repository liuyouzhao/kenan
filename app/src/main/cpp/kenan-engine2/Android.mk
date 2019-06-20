LOCAL_PATH:= $(call my-dir)


include $(CLEAR_VARS)

LOCAL_CFLAGS := -D__LINUX__ -D__ANDROID__ -DIN_ANDROID

LOCAL_LDLIBS := -llog -lEGL -lGLESv2 -lcutils -lui

LOCAL_SRC_FILES:= \
	effect.cpp \
	graphics2d.cpp \
	loader.cpp \
	random.cpp \
	shaders.cpp \
	sprite2d.cpp \
	system.cpp \
	timer.cpp \
	TransformationMatrix.cpp
#	AliGuiJni.cpp	\
#	desktop_wave.cpp	\

# enable this if we turn on _DEBUG, otherwise we exceed our prelink budget
LOCAL_PRELINK_MODULE := false

LOCAL_SHARED_LIBRARIES := \
	libandroid_runtime	\
	libcutils \
    	libui	\
	libEGL \
        libGLESv2 \
	liblog	\
	libskia
	

LOCAL_C_INCLUDES := \
	$(JNI_H_INCLUDE) \
	external/skia/include/core	\
	external/skia/include/images	\
	external/freetype/include



LOCAL_MODULE:= libaligl2

#include $(BUILD_EXECUTABLE)
include $(BUILD_STATIC_LIBRARY)



