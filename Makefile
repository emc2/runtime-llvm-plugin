OBJDIR=$(.CURDIR)/obj/
SRCDIR=$(.CURDIR)/src/
MAKEOBJDIR=${OBJDIR}

all: #${OBJS} .PHONY

clean: .PHONY
	rm -rf $(OBJDIR)
	make clean -C $(SRCDIR)

obj: .PHONY
	if [ ! -d $(OBJDIR) ]; then mkdir $(.CURDIR)/obj/; fi

objs: obj .PHONY
	make -C $(SRCDIR)
