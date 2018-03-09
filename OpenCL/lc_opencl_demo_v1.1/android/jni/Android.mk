LOCAL_PATH := $(call my-dir)

#--------
# fir_float

include $(CLEAR_VARS)
LOCAL_MODULE    := fir_float
LOCAL_C_INCLUDES := $(LOCAL_PATH)/inc
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../common
LOCAL_LDLIBS    := -lm -llog -lGLES_mali -L$(LOCAL_PATH)/lib
LOCAL_CFLAGS += -fPIE
LOCAL_LDFLAGS += -fPIE -pie -Wno-unused-parameter
LOCAL_SRC_FILES :=  \
	./../../common/common.cpp \
	./../../common/image.cpp \
	../../tests/fir_float/fir_float.cpp
include $(BUILD_EXECUTABLE)

#--------
# sobel

include $(CLEAR_VARS)
LOCAL_MODULE    := sobel
LOCAL_C_INCLUDES := $(LOCAL_PATH)/inc
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../common
LOCAL_LDLIBS    := -lm -llog -lGLES_mali -L$(LOCAL_PATH)/lib
LOCAL_CFLAGS += -fPIE
LOCAL_LDFLAGS += -fPIE -pie -Wno-unused-parameter
LOCAL_SRC_FILES :=  \
	./../../common/common.cpp \
	./../../common/image.cpp \
	../../tests/sobel/sobel.cpp
include $(BUILD_EXECUTABLE)

#--------
# sgemm

include $(CLEAR_VARS)
LOCAL_MODULE    := sgemm
LOCAL_C_INCLUDES := $(LOCAL_PATH)/inc
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../common
LOCAL_LDLIBS    := -lm -llog -lGLES_mali -L$(LOCAL_PATH)/lib
LOCAL_CFLAGS += -fPIE
LOCAL_LDFLAGS += -fPIE -pie -Wno-unused-parameter
LOCAL_SRC_FILES :=  \
	./../../common/common.cpp \
	./../../common/image.cpp \
	../../tests/sgemm/sgemm.cpp
include $(BUILD_EXECUTABLE)
#--------
# mandelbrot

include $(CLEAR_VARS)
LOCAL_MODULE    := mandelbrot
LOCAL_C_INCLUDES := $(LOCAL_PATH)/inc
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../common
LOCAL_LDLIBS    := -lm -llog -lGLES_mali -L$(LOCAL_PATH)/lib
LOCAL_CFLAGS += -fPIE
LOCAL_LDFLAGS += -fPIE -pie -Wno-unused-parameter
LOCAL_SRC_FILES :=  \
	./../../common/common.cpp \
	./../../common/image.cpp \
	../../tests/mandelbrot/mandelbrot.cpp
include $(BUILD_EXECUTABLE)

#--------
# image_scaling

include $(CLEAR_VARS)
LOCAL_MODULE    := image_scaling
LOCAL_C_INCLUDES := $(LOCAL_PATH)/inc
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../common
LOCAL_LDLIBS    := -lm -llog -lGLES_mali -L$(LOCAL_PATH)/lib
LOCAL_CFLAGS += -fPIE
LOCAL_LDFLAGS += -fPIE -pie -Wno-unused-parameter
LOCAL_SRC_FILES :=  \
	./../../common/common.cpp \
	./../../common/image.cpp \
	../../tests/image_scaling/image_scaling.cpp
include $(BUILD_EXECUTABLE)

#--------
# sobel_no_vectors

include $(CLEAR_VARS)
LOCAL_MODULE    := sobel_no_vectors
LOCAL_C_INCLUDES := $(LOCAL_PATH)/inc
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../common
LOCAL_LDLIBS    := -lm -llog -lGLES_mali -L$(LOCAL_PATH)/lib
LOCAL_CFLAGS += -fPIE
LOCAL_LDFLAGS += -fPIE -pie -Wno-unused-parameter
LOCAL_SRC_FILES :=  \
	./../../common/common.cpp \
	./../../common/image.cpp \
	../../tests/sobel_no_vectors/sobel_no_vectors.cpp
include $(BUILD_EXECUTABLE)

