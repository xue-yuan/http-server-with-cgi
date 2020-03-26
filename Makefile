all: web

web:
	gcc web-server/web.c web-server/lib/*.c -o host 

cgi:
	gcc cgi/cgi.c -o scgi 

test: web-test cgi-test

web-test: web web-run

cgi-test: cgi cgi-run

web-run:
	@./host

cgi-run:
	@./scgi

clean:
	rm -rf host scgi