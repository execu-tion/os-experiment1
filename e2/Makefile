include .config
obj-$(CONFIG_E1) += e1
obj-$(CONFIG_E2) += e2
src = ./src
all: .config $(obj-y) menuconfig
	@echo building!...
ifneq (,$(wildcard ./*.out))
	@cp ./*.out ../
endif
	@cp ./*.out ../

e1:	./src/os2.c ./src/os2.h
	@gcc -g -ggdb $(src)/os2.c -o exper3.out
e2:
	@gcc -g -ggdb $(src)/os3.c $(src)/main.c -o exper4.out
	
clean:	
ifneq (,$(wildcard ../*.out))
	-@rm ../*.out
endif
ifneq (,$(wildcard ./*.out))
	@rm ./*.out
endif

.config: Kconfig
	@kconfig-mconf Kconfig
menuconfig: 
	@kconfig-mconf Kconfig
