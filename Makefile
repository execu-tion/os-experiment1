#
#
include .config
dest-$(CONFIG_EXPER1) += e1
dest-$(CONFIG_EXPER2) += e2
dest-$(CONFIG_EXPER3) += e3
#
#
all: .config menuconfig
	make -C ./$(dest-y) 

#menuconfig : .config
#	make -C ./$(dest-y) $@
menuconfig:
	kconfig-mconf Kconfig
	
.config : Kconfig
	kconfig-mconf Kconfig
.PHONY : clean

clean :
	make -C ./$(dest-y) $@
