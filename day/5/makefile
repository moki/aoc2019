TARGET = main.out
SRCS = $(wildcard ./*.cpp) $(wildcard ./*/*.cpp)
OBJS = $(SRCS: ./*/*.cpp=*.o)
INPUT = input

# CC = gcc
CXX = g++

DEPDIR := .d
$(shell mkdir -p $(DEPDIR) >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td
# CFLAGS = -std=c99 -D_POSIX_C_SOURCE -Wshadow -Wall -pedantic -Wextra -g -O3 -flto -pthread
CXXFLAGS = -std=c++1z -Wshadow -Wall -pedantic -Wextra -g -O3 -flto
# -pthread
TARGET_ARCH =-march=native
# LDFLAGS = -lpthread
# LDFLAGS =
# LDLIBS = -lm
LDLIBS =

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LDFLAGS) $(LDLIBS) -o $@

COMPILE.cpp = $(CXX) $(DEPFLAGS) $(CXXFLAGS) $(TARGET_ARCH) -c
POSTCOMPILE = @mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d && touch $@

%.o : %.cpp
%.o : %.cpp $(DEPDIR)/%.d
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<
	$(POSTCOMPILE)

clean:
	-rm -f ./*.o ./*/*.o
	-rm -f ./*.out

test:
	make clean
	make
	cat $(INPUT) | valgrind --tool=memcheck --leak-check=full --track-origins=yes ./main.out

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

include $(wildcard $(patsubst %,$(DEPDIR)/%.d,$(basename $(SRCS))))
