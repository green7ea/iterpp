CXXFLAGS += -O2 -g -pipe -Wall -Wextra -march=native -std=c++17
PROGS = primes
ASM = primes.asm

.PHONY: all clean test asm

all: $(PROGS) asm

test: $(PROGS)
	./$(PROGS) 432143

asm: $(ASM)

%.asm: %
	objdump -ldC $(PROGS) > $(ASM)

%: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f $(OBJS) $(PROGS)
