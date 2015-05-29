
CPP = g++
BISON = bison
LEX = flex 
AR = ar


LIBOUT = ../lib
UPPAALINCLUDE = /usr/local/uppaal/include
OURINCLUDE = ../include

CXXFLAGS = -I${UPPAALINCLUDE} -I${OURINCLUDE} `xml2-config --cflags` 
OURINCLUDEXML = ${OURINCLUDE}/xmlloader

INCLUDE =${OURINCLUDEXML}/bison.tab.hpp ${OURINCLUDEXML}/Visitor.hpp  ${OURINCLUDEXML}/bisonHead.hpp  ${OURINCLUDEXML}/Visitable.hpp
OBJECTS =XMLLoader.o bison.tab.o lex.o Visitor.o Visitable.o arithmetic.tab.o  lexarith.o

xmlloader.lib: libxmlloader.a
		@mv libxmlloader.a ${LIBOUT}	


libxmlloader.a: ${OBJECTS}
	${AR} rucs libxmlloader.a ${OBJECTS}

XMLLoader.o: XMLLoader.cpp ../include/xmlloader/XMLLoader.hpp

bison.tab.o: bison.tab.cpp ${INCLUDE}
lex.o: lex.guard.c ${OURINCLUDEXML}/bison.tab.hpp 
	${CPP} -c ${CXXFLAGS} lex.guard.c -o lex.o


Visitor.o: Visitor.cpp ${INCLUDE}
Visitable.o: Visitable.cpp ${INCLUDE}
bison.tab.cpp: bison.ypp
	${BISON}  -pguard bison.ypp

bison.tab.hpp: bison.ypp
	${BISON} -pguard -d  bison.ypp

lex.guard.c: lex.l
	${LEX} lex.l

lex.arith.c: arithmetic.l
	${LEX} arithmetic.l
lexarith.o: lex.arith.c ${OURINCLUDEXML}/arithmetic.tab.hpp 
	${CPP} -c ${CXXFLAGS} lex.arith.c -o lexarith.o


arithmetic.tab.o: arithmetic.tab.cpp ${INCLUDE}
arithmetic.tab.cpp: arithmetic.ypp
	${BISON}  -parith arithmetic.ypp

arithmetic.tab.hpp: arithmetic.ypp
	${BISON} -parith -d  arithmetic.ypp


${OURINCLUDEXML}/bison.tab.hpp: bison.tab.hpp
	@mv bison.tab.hpp ${OURINCLUDEXML}/bison.tab.hpp


${OURINCLUDEXML}/arithmetic.tab.hpp: arithmetic.tab.hpp
	@mv arithmetic.tab.hpp ${OURINCLUDEXML}/arithmetic.tab.hpp



