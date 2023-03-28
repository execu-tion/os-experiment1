CC = gcc
CFLAGS += -g
LDFLAGS +=  -pthread
##
include .config
obj-$(CONFIG_E1) += exper1.o
obj-$(CONFIG_E2) += exper2.o
target = $(subst .o,.out,$(obj-y))
build = ../build
##

all :  $(obj-y) .config
	$(CC) $(build)/$(obj-y) $(CFLAGS) $(LDFLAGS) -o $(build)/$(target) 

exper1.o : demo
demo : demo.c
	$(CC) $< $(CFLAGS) -o $@

$(obj-y) : %.o:%.c
	$(CC) -c $(CFLAGS) $<  -o $(build)/$@



.config : Kconfig
	kconfig-mconf Kconfig
menuconfig : Kconfig
	kconfig-mconf Kconfig

.PHONY : clean
clean :
ifneq (,$(wildcard $(build)/*.o))
	-@rm $(build)/*.o
endif
ifneq (,$(wildcard $(build)/demo))
	-@rm $(build)/demo
endif
ifneq (,$(wildcard $(build)/$(target)))
	-@rm $(build)/$(target)
endif

