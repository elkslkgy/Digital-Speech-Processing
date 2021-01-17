.PHONY: all clean

CFLAGS+=
LDFLAGS+=-lm     # link to math library

TARGET=test_hmm
TARGET1=train
TARGET2=test
TARGET3=compare

all: $(TARGET) $(TARGET1) $(TARGET2)  $(TARGET3)
# type make/make all to compile test_hmm

clean:
	$(RM) $(TARGET)   # type make clean to remove the compiled file
	$(RM) $(TARGET1)
	$(RM) $(TARGET2)
	$(RM) $(TARGET3)