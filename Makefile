INCLUDEFLAGS=-I/usr/local/include -I./include
SRCS=GCType.cpp GCTypeVisitors.cpp
OBJS=src/GCType.o
CXXFLAGS+=${INCLUDEFLAGS} -fno-rtti -fno-exceptions
CXX=clang++

.cpp.o:
	${CXX} ${CXXFLAGS} -c ${.IMPSRC}

all: ${OBJS};
