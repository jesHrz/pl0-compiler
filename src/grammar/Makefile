ifndef MAKEFILE_GRAMMAR
define MAKEFILE_GRAMMAR
endef

CCFILES = main.cc \
		  grammar.cc \
		  lex.cc \
		  number.cc \
		  operator.cc \
		  symbol.cc \
		  word.cc \

INCPATH += -I../symbol -I../lex

DEFINES += -DLEX -DGRAMMAR

include ../Makefile.common

endif # MAKEFILE_GRAMMAR