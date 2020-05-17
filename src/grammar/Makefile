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
		  error.cc \
		  idtable.cc

INCPATH += -I../grammar -I../symbol -I../lex

DEFINES += -DLEX -DGRAMMAR -DPRINT_GRAMMAR_TREE

include ../Makefile.common

endif # MAKEFILE_GRAMMAR