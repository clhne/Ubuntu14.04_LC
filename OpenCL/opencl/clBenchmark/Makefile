# image concolution benchmark Makefile
# ==================================================
CXX=g++
CXXFLAGS=-g

# some direct
MACHINE=$(shell uname -s)
ARCH=$(shell uname -p)

# includes
# =========
SDL_INCLUDE=$(shell sdl-config --cflags)
OPENCL_INCLUDE=
# Libs
# =========

GPU_UTILS_LIB=\
	/Developer/GPU\ Computing/shared/obj/x86_64/release/shrUtils.cpp.o \
	/Developer/GPU\ Computing/shared/obj/x86_64/release/cmd_arg_reader.cpp.o \
	/Developer/GPU\ Computing/shared/obj/x86_64/release/rendercheckGL.cpp.o \
	/Developer/GPU\ Computing/OpenCL/common/lib/liboclUtil_x86_64.a
OPENCL_LIB=

SDL_LIB=$(shell sdl-config --libs) -ljpeg -lpng -ltiff
ifeq ($(MACHINE), Darwin)
SDL_LIB+= /opt/local/lib/libSDL_image.a -lGLEW
OPENCL_LIB+=-framework OpenCL -framework AppKit -lXmu
OPENCL_INCLUDE+=\
	-I/Developer/GPU\ Computing/OpenCL/common/inc		\
	-I/Developer/GPU\ Computing/shared/inc			\
	-F OpenCL

else
SDL_INCLUDE+= -I/home/evl/kreda/devlib/include
SDL_LIB+= -lGLU /home/evl/kreda/devlib/lib/libSDL_image.a /home/evl/kreda/devlib/lib/libGLEW.a
endif

# everything together
INCLUDE=-I/opt/local/include $(SDL_INCLUDE) $(OPENCL_INCLUDE) $(shell freetype-config --cflags)
LIB=$(SDL_LIB) $(OPENCL_LIB) $(GPU_UTILS_LIB) $(shell freetype-config --libs)

# add additional headers for MacOSX
ifeq ($(MACHINE), Darwin)
INCLUDE+= -F OpenGL -I/opt/local/include
LIB+= -framework OpenGL -framework OpenCL
endif

# Object files
# =================================================
OBJ=					\
	graphics.o			\
	timer.o				\
	texture_unit.o			\
	vector_math.o			\
	misc.o				\
	common.o				\
	vis.o				\
	ocl.o				\
	benchmark.o			\
	batch_test.o			\
	FreeType.o			\
	main.o

# Header files
# =================================================
HEADER=					\
	graphics.h			\
	timer.h				\
	texture_unit.h			\
	vector_math.h			\
	misc.h				\
	ocl.h				\
	benchmark.h			\
	batch_test.h			\
	common.h				\
	FreeType.h

TARGET=benchmark

$(TARGET):	$(OBJ)
	g++ -g $(OBJ) $(LIB) -o $(TARGET)

%.o: %.cpp $(HEADER)
	$(CXX) -c $(CXXFLAGS) $(INCLUDE) -o $@ $<

all:	$(TARGET)

clean:
	rm -rf $(OBJ)
	rm -rf $(TARGET)

