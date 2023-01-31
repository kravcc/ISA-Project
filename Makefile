FLAGS = -Wall -std=c99

all: 
	gcc $(FLAGS) args_parser.c downloader.c feed_parser.c feedreader.c url_parser.c xml_parser.c -lssl -lcrypto -lxml2 `xml2-config --cflags` `xml2-config --libs` -o feedreader

tar:
	tar -cf xkravc02.tar manual.pdf *.c *.h Makefile README.md