all:
	@make -C ./src
ifneq (,$(wildcard ./src/demo))
	@cp ./build/demo ../
endif
	@cp ./build/*.out ../
menuconfig:
	@make -C ./src $@

.PHONY: clean
clean:
ifneq (,$(wildcard ../*.out))
	-@rm ../*.out
endif
ifneq (,$(wildcard ../demo))
	@rm ../demo
endif
	@make -C ./src $@
