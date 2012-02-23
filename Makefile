# trackingdemo Makefile

CC = g++
CFLAGS = -g -W -Wall -Werror -Wno-unused -O2
LFLAGS = -lcwiid -lGL -lbluetooth -lm -lglut
V = @

all: trackingdemo


trackingdemo.o: trackingdemo.cpp
	@echo + cc trackingdemo.cpp
	$(V)$(CC) $(CFLAGS) -c trackingdemo.cpp

wiiheadtracking.o: wiiheadtracking.cpp
		@echo + cc wiiheadtracking.cpp
		$(V)$(CC) $(CFLAGS) -c wiiheadtracking.cpp

trackingdemo: trackingdemo.o wiiheadtracking.o
	@echo + link main 
	$(V)$(CC) $(CFLAGS) $(LFLAGS) -o $@ trackingdemo.o wiiheadtracking.o

clean:
	@echo + clean
	$(V)rm -rf *.o trackingdemo

