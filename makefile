.POSIX:

TOPDIR	= .
SRC		= $(TOPDIR)/src
SRCMAIN = $(SRC)/main
SRCM	= $(SRC)/modules
SRCT	= $(SRC)/tests
OUT		= $(TOPDIR)/executables
CC		= gcc
CFLAGS	= -Wall -iquote $(SRCM) -iquote $(SRCT) $(ADD_CFLAGS)
LDFLAGS	=
LDLIBS	= -lm -lgsl -lgslcblas

.PHONY: all cleanall clean

# shortcuts to executables, so you can use target without paths
membrane-sim: $(OUT)/membrane-sim;
test_nneighbour: $(OUT)/test_nneighbour;
test_functions: $(OUT)/test_functions;
test_integrators: $(OUT)/test_integrators;
test_integrators1: $(OUT)/test_integrators1;
test_dpolynom: $(OUT)/test_dpolynom;
test_potential: $(OUT)/test_potential;
test_vol_pyr: $(OUT)/test_vol_pyr;

# linking of executables
$(OUT)/membrane-sim: $(SRCMAIN)/membrane-sim.o $(SRCM)/geometry.o $(SRCM)/integrators.o $(SRCM)/functions.o $(SRCM)/potential.o $(SRCM)/initial_cond.o $(SRCM)/observables.o
	$(CC) $(LDFLAGS) -o $@ $(SRCMAIN)/membrane-sim.o $(SRCM)/geometry.o $(SRCM)/integrators.o $(SRCM)/functions.o $(SRCM)/potential.o $(SRCM)/initial_cond.o $(SRCM)/observables.o $(LDLIBS)

$(OUT)/test_nneighbour: $(SRCT)/test_nneighbour.o $(SRCM)/geometry.o
	$(CC) $(LDFLAGS) -o $@ $(SRCT)/test_nneighbour.o $(SRCM)/geometry.o $(LDLIBS)
$(OUT)/test_functions: $(SRCT)/test_functions.o $(SRCM)/geometry.o $(SRCM)/functions.o
	$(CC) $(LDFLAGS) -o $@ $(SRCT)/test_functions.o $(SRCM)/geometry.o $(SRCM)/functions.o $(LDLIBS)
$(OUT)/test_dpolynom: $(SRCT)/test_dpolynom.o $(SRCM)/geometry.o $(SRCM)/functions.o
	$(CC) $(LDFLAGS) -o $@ $(SRCT)/test_dpolynom.o $(SRCM)/geometry.o $(SRCM)/functions.o  $(LDLIBS) 
$(OUT)/test_integrators: $(SRCT)/test_integrators.o $(SRCM)/geometry.o $(SRCM)/functions.o $(SRCM)/integrators.o $(SRCM)/potential.o $(SRCM)/initial_cond.o $(SRCM)/observables.o
	$(CC) $(LDFLAGS) -o $@ $(SRCT)/test_integrators.o $(SRCM)/geometry.o $(SRCM)/functions.o $(SRCM)/integrators.o $(SRCM)/potential.o $(SRCM)/initial_cond.o $(SRCM)/observables.o $(LDLIBS)
$(OUT)/test_integrators1: $(SRCT)/test_integrators1.o $(SRCM)/geometry.o $(SRCM)/functions.o $(SRCM)/integrators.o $(SRCM)/potential.o
	$(CC) $(LDFLAGS) -o $@ $(SRCT)/test_integrators1.o $(SRCM)/geometry.o $(SRCM)/functions.o $(SRCM)/integrators.o $(SRCM)/potential.o $(LDLIBS)
$(OUT)/test_potential: $(SRCT)/test_potential.o $(SRCM)/potential.o
	$(CC) $(LDFLAGS) -o $@ $(SRCT)/test_potential.o $(SRCM)/potential.o $(LDLIBS)
$(OUT)/test_vol_pyr: $(SRCT)/test_vol_pyr.o $(SRCM)/geometry.o $(SRCM)/functions.o $(SRCM)/initial_cond.o 
	$(CC) $(LDFLAGS) -o $@ $(SRCT)/test_vol_pyr.o $(SRCM)/geometry.o $(SRCM)/functions.o $(SRCM)/initial_cond.o $(LDLIBS)

# creating object files for main programs
$(SRCMAIN)/membrane-sim.o: $(SRCMAIN)/membrane-sim.c $(SRCM)/global.h $(SRCM)/geometry.h $(SRCM)/integrators.h $(SRCM)/functions.h $(SRCM)/potential.h $(SRCM)/initial_cond.h $(SRCM)/observables.h
	$(CC) -c $(CFLAGS) $(SRCMAIN)/membrane-sim.c -o $@


# creating object files for testing
$(SRCT)/test_nneighbour.o: $(SRCT)/test_nneighbour.c $(SRCM)/global.h $(SRCM)/geometry.h
	$(CC) -c $(CFLAGS) $(SRCT)/test_nneighbour.c -o $@
$(SRCT)/test_functions.o: $(SRCT)/test_functions.c $(SRCM)/global.h $(SRCM)/geometry.h $(SRCM)/functions.h
	$(CC) -c $(CFLAGS) $(SRCT)/test_functions.c -o $@
$(SRCT)/test_dpolynom.o: $(SRCT)/test_dpolynom.c $(SRCM)/global.h $(SRCM)/geometry.h $(SRCM)/functions.h
	$(CC) -c $(CFLAGS) $(SRCT)/test_dpolynom.c -o $@
$(SRCT)/test_integrators.o: $(SRCT)/test_integrators.c $(SRCM)/global.h $(SRCM)/geometry.h $(SRCM)/functions.h $(SRCM)/geometry.h $(SRCM)/potential.h $(SRCM)/initial_cond.h
	$(CC) -c $(CFLAGS) $(SRCT)/test_integrators.c -o $@
$(SRCT)/test_integrators1.o: $(SRCT)/test_integrators1.c $(SRCM)/global.h $(SRCM)/geometry.h $(SRCM)/functions.h $(SRCM)/geometry.h $(SRCM)/potential.h
	$(CC) -c $(CFLAGS) $(SRCT)/test_integrators1.c -o $@
$(SRCT)/test_potential.o: $(SRCT)/test_potential.c
	$(CC) -c $(CFLAGS) $(SRCT)/test_potential.c -o $@
$(SRCT)/test_vol_pyr.o: $(SRCT)/test_vol_pyr.c $(SRCM)/global.h $(SRCM)/geometry.h $(SRCM)/initial_cond.h
	$(CC) -c $(CFLAGS) $(SRCT)/test_vol_pyr.c -o $@


# creating modules object files
$(SRCM)/geometry.o: $(SRCM)/geometry.c $(SRCM)/global.h
	$(CC) -c $(CFLAGS) $(SRCM)/geometry.c -o $@
$(SRCM)/functions.o: $(SRCM)/functions.c $(SRCM)/global.h $(SRCM)/geometry.h
	$(CC) -c $(CFLAGS) $(SRCM)/functions.c -o $@
$(SRCM)/integrators.o: $(SRCM)/integrators.c $(SRCM)/global.h $(SRCM)/functions.h $(SRCM)/potential.h
	$(CC) -c $(CFLAGS) $(SRCM)/integrators.c -o $@
$(SRCM)/potential.o: $(SRCM)/potential.c
	$(CC) -c $(CFLAGS) $(SRCM)/potential.c -o $@
$(SRCM)/initial_cond.o: $(SRCM)/initial_cond.c $(SRCM)/global.h $(SRCM)/geometry.h $(SRCM)/functions.h
	$(CC) -c $(CFLAGS) $(SRCM)/initial_cond.c -o $@
$(SRCM)/observables.o: $(SRCM)/observables.c $(SRCM)/global.h $(SRCM)/functions.h
	$(CC) -c $(CFLAGS) $(SRCM)/observables.c -o $@


# phony recipes
all: membrane-sim test_nneighbour test_functions test_dpolynom test_integrators test_potential test_vol_pyr test_integrators1
cleanall: clean
	rm -f $(OUT)/*
clean:
	rm -f $(SRCM)/*.o $(SRCT)/*.o $(SRCMAIN)/*.o
