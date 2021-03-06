CC = clang
CXX = clang++

CFLAGS := -Ofast -Wall -flto
CFLAGS += -fno-asynchronous-unwind-tables -fno-unwind-tables -fmerge-all-constants

CXXFLAGS := -fno-exceptions -fno-rtti

LDFLAGS := -L /usr/local/lib/ -s -Wl,-icf=all,--gc-sections -flto -Wl,--plugin-opt=O3 -fuse-ld=lld
LIBS := -lbenchmark -lpthread

## Objects to build
C_SRCS   := $(wildcard *.c)
CXX_SRCS := $(wildcard *.cc)
OUTS := $(C_SRCS:.c=.out) $(CXX_SRCS:.cc=.out)

benchmark: $(OUTS)
	./run_benchmark.sh "$(OUTS)"

%.out: %.c ../aspawn.h ../syscall/syscall.h ../libaspawn.so Makefile
	$(MAKE) -C ../
	$(CC) -std=c11 $(CFLAGS) $(LDFLAGS) $(LIBS) $< ../libaspawn.so -o $@

%.out: %.cc ../aspawn.h ../syscall/syscall.h ../libaspawn.so Makefile
	$(MAKE) -C ../
	$(CXX) -std=c++17 $(CFLAGS) $(CXXFLAGS) $(LDFLAGS) $(LIBS) $< ../libaspawn.so -o $@

clean:
	rm -f $(OUTS)

.PHONY: benchmark clean
