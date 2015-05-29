CPP = g++
BISON = bison
LEX = flex 
AR = ar


LIBOUT = ../solvers
LIBOUTSTAT = ../lib
UPPAALINCLUDE = /usr/local/uppaal/include
UPPAALLIB = /usr/local/uppaal/lib
OURINCLUDE = ../include/


CXXFLAGS =  -I${OURINCLUDE} -I${UPPAALINCLUDE} -fPIC
OURINCLUDEPOINT = ${OURINCLUDE}/pointInDBM

INCLUDE = ${OURINCLUDEPOINT}/DCCreator.hpp
OBJECTS = Solver.o SolutionFinder.o DCCreator.o

point.dynamic: ${LIBOUT}/point.so
${LIBOUT}/point.so: point.so
	@mv point.so ${LIBOUT} 	
point.so: ${OBJECTS} ../lib/liblta.a
	${CPP}  -L${UPPAALLIB} -L../lib -shared *.o -o point.so -llta -ludbm -Bstatic -lgmp -lgmpxx

libpoint.a: ${OBJECTS}
	${AR} rucs  libpoint.a ${OBJECTS}

point.static: libpoint.a
	@mv libpoint.a ${LIBOUTSTAT} 

SolutionFinder.o: SolutionFinder.cpp 
DCCreator.o: DCCreator.cpp ${INCLUDE}
Solver.o: Solver.cpp

clean: point.dynamic point.static
	rm *.o
	rm ${LIBOUT}/point.so
	rm ${LIBOUTSTAT}/libpoint.a