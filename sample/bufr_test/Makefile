#
#                                 Makefile for example programs
A64 = A64
R64 = R64
ARCH = linux
CNAME = _gfortran
#
PLACE = ../
LIB = bufr
#
include $(PLACE)config/config.$(ARCH)$(CNAME)$(R64)$(A64)
#
include $(PLACE)options/options_$(ARCH)
#
TARGDIR  = ./
#
#
TARGETS  = all clean
EXECS    = decode_bufr decode_bufr_image bufr_decode_all tdexp tdexp create_bufr bufr_decode bufr_synop
#
#
all :$(EXECS)

decode_bufr: decode_bufr.o 
	$(FC) $(FFLAGS) -o $@ decode_bufr.o -L$(PLACE) -l$(LIB)$(R64)

bufr_decode_all: bufr_decode_all.o 
	$(FC) $(FFLAGS) -o $@ bufr_decode_all.o -L$(PLACE) -l$(LIB)$(R64) 

bufr2crex   : bufr2crex.o
	$(FC) $(FFLAGS) -o $@ bufr2crex.o -L$(PLACE) -l$(LIB)$(R64) -lcrex$(R64)

crex2bufr   : crex2bufr.o
	$(FC) $(FFLAGS) -o $@ crex2bufr.o -L$(PLACE) -l$(LIB)$(R64) -lcrex$(R64)

create_bufr: create_bufr.o
	$(FC) $(FFLAGS) -o $@ create_bufr.o -L$(PLACE) -l$(LIB)$(R64)

tdexp: tdexp.o
	$(FC) $(FFLAGS) -o $@ tdexp.o -L$(PLACE) -l$(LIB)$(R64)

decode_bufr_image: decode_bufr_image.o
	$(FC) $(FFLAGS) -o $@ decode_bufr_image.o -L$(PLACE) -l$(LIB)$(R64)

bufr_decode: bufr_decode.o 
	$(FC) $(FFLAGS) -g -ggdb -o $@ bufr_decode.o -L$(PLACE) -l$(LIB)$(R64)

bufr_decode2: bufr_decode2.o 
	$(FC) $(FFLAGS) -g -ggdb -o $@ bufr_decode2.o -L$(PLACE) -l$(LIB)$(R64)

bufr_synop: bufr_synop.o 
	$(FC) $(FFLAGS) -g -ggdb -o $@ bufr_synop.o -L$(PLACE) -l$(LIB)$(R64)

hello: hello.o 
	$(FC) $(FFLAGS) -o $@ hello.o -L$(PLACE) -l$(LIB)$(R64)

clean   :
	@for name in $(EXECS); do\
	(rm -f $$name *.o ); \
	done 
