CC=gcc

penguen: penguen.o http.o error.o http_message.o parser.o lexer.o router.o conf.o log.o
	$(CC) $(MACROS) -g -o $@ penguen.o parser.o lexer.o router.o http.o error.o http_message.o conf.o log.o -lm

parser.c parser.h: parser.y
	bison -d -o parser.c parser.y

lexer.c lexer.h: lexer.l
	flex --header-file=lexer.h -o lexer.c lexer.l

lexer.o: lexer.c lexer.h
	$(CC) $(MACROS) -g -c -o $@ $<

parser.o: parser.c parser.h lexer.h
	$(CC) $(MACROS) -g -c -o $@ $<

error.o: error.c error.h
	$(CC) $(MACROS) -g -c -o $@ $<

http.o: http.c http.h error.h http_message.h conf.h
	$(CC) $(MACROS) -g -c -o $@ $<

http_message.o: http_message.c http_message.h error.h
	$(CC) $(MACROS) -g -c -o $@ $<

router.o: router.c router.h http_message.h
	$(CC) $(MACROS) -g -c -o $@ $<

conf.o: conf.c conf.h router.h
	$(CC) $(MACROS) -g -c -o $@ $<

log.o: log.c log.h
	$(CC) $(MACROS) -g -c -o $@ $<

penguen.o: penguen.c http.h conf.h
	$(CC) $(MACROS) -g -c -o $@ $<

clean:
	rm *.o *.i *.s