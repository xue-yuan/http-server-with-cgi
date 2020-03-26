all: webc cgic

webc:
	gcc -Wall web-server/web.c web-server/lib/*.c -o hostd 

cgic:
	gcc -Wall cgi/cgi.c cgi/backend/*.c -o cgid 

test: web-test cgi-test

web-test: webc web-run

cgi-test: cgic cgi-run

web-run:
	@./host

cgi-run:
	@./scgi

clean:
	rm -rf hostd cgid a.out