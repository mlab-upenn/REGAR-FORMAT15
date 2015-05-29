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

OBJECTS = SmallSolver.o

point.dynamic: ${LIBOUT}/point.so
${LIBOUT}/point.so: point.so
	@mv point.so ${LIBOUT} 	

libsmall.a: ${OBJECTS}
	${AR} rucs  libsmall.a ${OBJECTS}

small.static: libsmall.a
	@mv libsmall.a ${LIBOUTSTAT} 

trace.o: SmallSolver.cpp
trace.o: trace.cpp 
