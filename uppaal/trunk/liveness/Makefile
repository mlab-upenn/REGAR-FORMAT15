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
OURINCLUDEPOINT = ${OURINCLUDE}/smallzone

OBJECTS = liveTrace.o liveSolver.o 

liblive.a: ${OBJECTS}
	${AR} rucs  liblive.a ${OBJECTS}

live.static: liblive.a
	@mv liblive.a ${LIBOUTSTAT} 

liveSolver.o: liveSolver.cpp
liveTrace.o: liveTrace.cpp 
simpleLiveness.o: simpleLiveness.cpp 
clean: 
	rm *.o

