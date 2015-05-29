CPP = g++
BISON = bison
LEX = flex 
AR = ar


LIBOUT = ../lib
UPPAALINCLUDE = /usr/local/uppaal/include
OURINCLUDE = ../include

CXXFLAGS =  -I${OURINCLUDE} 
OURINCLUDEDBM = ${OURINCLUDE}/realDBM

INCLUDE =${OURINCLUDEDBM}/dbm.hpp ${OURINCLUDEDBM}/bound.hpp
OBJECTS =bound.o dbm.o

clean: lta.lib
	rm *.o
	rm ${LIBOUT}/libdbm.a

dbm.lib: ${LIBOUT}/libdbm.a

libdbm.a: ${OBJECTS}  
	${AR} rucs  libdbm.a ${OBJECTS}

${LIBOUT}/libdbm.a: libdbm.a
	@mv libdbm.a ${LIBOUT}

bound.o: bound.cpp ${INCLUDE} 
dbm.o: dbm.cpp ${INCLUDE}





