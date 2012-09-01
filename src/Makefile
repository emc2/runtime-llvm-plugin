INCLUDEFLAGS=-I/usr/local/include -I../include
CFLAGS=-O2 $(INCLUDEFLAGS)
CXXFLAGS=-O2 $(INCLUDEFLAGS) -fno-rtti -fno-exceptions
SRCS=GCType.cpp GCTypeVisitors.cpp ParseMetadataPass.cpp

OBJS=${SRCS:N*.h:R:S/$/.o/g}
DEPEND=.depend

all: depend $(OBJS);

.cpp.o:
	$(CXX) $(CXXFLAGS) -c -o $(.OBJDIR)/$(.TARGET) $(.IMPSRC)

$(DEPEND):
	mkdep $(CFLAGS) $(SRCS)

clean:
	rm $(DEPEND)
