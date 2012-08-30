INCLUDEFLAGS=-I/usr/local/include -I./include
#SRCS=GCType.cpp GCTypeVisitors.cpp 
OBJS=src/GCType.o src/GCTypeVisitors.o src/ParseMetadataPass.o
CXXFLAGS+=${INCLUDEFLAGS} -fno-rtti -fno-exceptions
CXX=clang++

.cpp.o:
	${CXX} ${CXXFLAGS} -c ${.IMPSRC}

all: ${OBJS};

clean:;
	rm -rf ${OBJS}
