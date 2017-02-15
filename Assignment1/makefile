#Macro Definitions
CC = gcc
CFLAG = -Wall

all: asn1

asn1: asn1.o libpnm.o
	$(CC) $(CFLAG) -o asn1 asn1.o libpnm.o -lm

asn1.o: asn1.c libpnm.h
	$(CC) $(CFLAG) -c asn1.c -lm

libpnm.o: libpnm.c libpnm.h
	$(CC) $(CFLAG) -c libpnm.c

test: asn1
	#./asn1 2 45 43 "IamError.pnm" 1 #Uncomment this to test errors
	./asn1 1 360 360 "square.pbm" 1
	./asn1 1 300 360 "rectH.pbm" 1
	./asn1 1 360 300 "rectW.pbm" 1
	./asn1 1 120 4 "rectwide.pbm" 1
	./asn1 1 4 120 "recthigh.pbm" 1
	./asn1 2 360 360 "square.pgm" 1
	./asn1 2 300 360 "rectH.pgm" 1
	./asn1 2 360 300 "rectW.pgm" 1
	./asn1 2 120 4 "rectwide.pgm" 1
	./asn1 2 4 120 "recthigh.pgm" 1

clean:
	rm -f *.o asn1
