#************************************************************
#
#  makefile for GNU C++
#
#************************************************************

#************************************************************
#
# CC: name of C++ compiler
#
#************************************************************

CC = g++

#************************************************************
#
# library and include path names
#
#************************************************************


#************************************************************
#
# c++ source files
#
#************************************************************

CCFILES  = main.cpp \
           freetype/CGL/CGL_Freetype.cpp \
           freetype/CGL/CGL_Texture.cpp \
           freetype/CGL/CGL_FrameBuffer.cpp \
           freetype/CGL/NGLprimitives.cpp \
           freetype/lib/CUtf.cpp \
           freetype/lib/NUtf.cpp \
           freetype/lib/NUtf2.cpp \
           freetype/lib/NStringTool.cpp \
           freetype/lib/CDataRect.cpp \
           freetype/lib/CBlockOperations.cpp \
           freetype/lib/write_png.cpp
           
CFILES =   freetype/lib/PnglibInterface.c         
           
           


#************************************************************
#
# library files
#
#************************************************************

# LINUX
#LINKLIBS    = -lGL `pkg-config glfw3 --static --libs` -lfreetype -lGLEW -lpng16 -lz

# MSYS
LINKLIBS    = -lopengl32 `pkg-config glfw3 --static --libs` -lfreetype -lglew32 -lpng -lz



#************************************************************
#
# C++ flags
#
# -c  = compile only
# -O2 = optimize level 2
# -s  = remove symboltables
#
#************************************************************
INCCMD = -I/mingw64/include/freetype2 -I./freetype/include
#CCFLAGS  = -mthreads -mtune=i686 -O3 -s -I$(LIBINCLUDE) -L$(LIBBASE)
CFLAGS   = -O2 $(INCCMD)
CCFLAGS  = -O2 $(INCCMD) -I/usr/include/freetype2

#************************************************************
#
# Rule for making .o-file from .cpp-file
#
# $@ = Name of the target (.o)
# $< = Name of dependency (.cpp)
#
#************************************************************

.cpp.o:
	$(CC) -c $(CCFLAGS) -o$@ $<
.c.o:
	gcc -c $(CFLAGS) -o$@ $<

	
	
#************************************************************
#
# List of object-files
#
#************************************************************

COBJECTS   = $(CFILES:.c=.o)
CCOBJECTS  = $(CCFILES:.cpp=.o)

#************************************************************
#
# Name of target
#
#************************************************************

TARGET  = createfonttableau_opengl

#************************************************************
#
# Rule for default-target
#
#************************************************************

default: $(TARGET)

#************************************************************
#
# Rule for clean-up
#
#************************************************************

clean:
	rm $(CCOBJECTS) $(COBJECTS)

#************************************************************
#
# Rule for creating target from objects
#
#************************************************************

$(TARGET): $(CCOBJECTS) $(COBJECTS) makefile
	$(CC) -s -o $(TARGET) -L./lib-extern/linux $(CCOBJECTS) $(COBJECTS) $(LINKLIBS)

