FILES=napi.c napi.h napi_test.c napif.f napif_test.f napif.inc README.FORTRAN

all : tar zip

tar :
	rm -f napi.tar
	tar cf napi.tar $(FILES)

zip :
	rm -f napi.zip
	zip napi.zip $(FILES)
