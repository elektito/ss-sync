FLAGS := $(shell pkg-config --cflags --libs gstreamer-1.0 gstreamer-rtsp-server-1.0)
FLAGS += -lgstrtp-1.0
FLAGS += -g

SOURCES := ss-sync.c \
	   gstrtpmytimestamp.c \
	   my-factory.c

HEADERS := my-factory.h \
	   gstrtpmytimestamp.h

ss-sync: $(SOURCES) $(HEADERS) Makefile
	cc $(SOURCES) $(FLAGS) -o ss-sync

clean:
	rm ss-sync
