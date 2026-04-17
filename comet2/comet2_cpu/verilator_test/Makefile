VERILATOR ?= verilator
VERILATOR_FLAGS := -Wno-WIDTHTRUNC --cc --exe --build
TOP := ../comet2_cpu
TESTBENCH := main.cpp
OBJ_DIR := obj_dir
BIN := $(OBJ_DIR)/Vcomet2_cpu

.PHONY: test clean

test:
	$(VERILATOR) $(VERILATOR_FLAGS) $(TOP).v $(TESTBENCH)
	./$(BIN)

clean:
	rm -rf $(OBJ_DIR)
