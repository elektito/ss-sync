FLAGS := $(shell pkg-config --cflags --libs gstreamer-1.0 gstreamer-rtsp-server-1.0)
FLAGS += -lgstnet-1.0 -lgstrtp-1.0
FLAGS += -g

SOURCES := ss-sync.c \
	   gstrtpxyztimestamp.c \
	   my-factory.c

HEADERS := my-factory.h \
	   gstrtpxyztimestamp.h

ss-sync: $(SOURCES) $(HEADERS) Makefile
	cc $(SOURCES) $(FLAGS) -o ss-sync
