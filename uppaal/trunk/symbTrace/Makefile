CPP = g++
BISON = bison
LEX = flex 
AR = ar


LIBOUT = ../lib
UPPAALINCLUDE = /usr/local/uppaal/include
OURINCLUDE = ../include

CXXFLAGS =  -I${OURINCLUDE} -I${UPPAALINCLUDE}
OURINCLUDELTA = ${OURINCLUDE}/symbTrace

INCLUDE =${OURINCLUDELTA}/DCConstraint.hpp ${OURINCLUDELTA}/Edge.hpp  ${OURINCLUDELTA}/Location.hpp  ${OURINCLUDELTA}/lta.hpp  ${OURINCLUDELTA}/Node.hpp
OBJECTS =Node.o Location.o Edge.o DCConstraint.o lta.o

clean: symbTrace.lib
	rm *.o
	rm ${LIBOUT}/libsymbTrace.a

symbTrace.lib: ${LIBOUT}/libsymbTrace.a

libsymbTrace.a: ${OBJECTS}  
	${AR} rucs  libsymbTrace.a ${OBJECTS}

${LIBOUT}/libsymbTrace.a: libsymbTrace.a
	@mv libsymbTrace.a ${LIBOUT}

DCConstraint.o: DCConstraint.cpp ${INCLUDE}
Node.o: Node.cpp ${INCLUDE}
Edge.o: Edge.cpp ${INCLUDE}
Location.o: Location.cpp ${INCLUDE}
lta.o: lta.cpp ${INCLUDE}





