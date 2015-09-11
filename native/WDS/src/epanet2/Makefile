# Makefile for EPANET2

objs = epanet.o input1.o input2.o input3.o rules.o output.o report.o \
       inpfile.o hydraul.o smatrix.o quality.o mempool.o hash.o

hdrs = hash.h text.h types.h funcs.h vars.h

epanet2 : $(objs)
	  cc -o epanet2 -lm $(objs)

epanet.o  : $(hdrs) toolkit.h enumstxt.h
input1.o  : $(hdrs)
input2.o  : $(hdrs)
input3.o  : $(hdrs)
rules.o   : $(hdrs)
output.o  : $(hdrs)
report.o  : $(hdrs)
inpfile.o : $(hdrs)
hydraul.o : $(hdrs)
smatrix.o : $(hdrs)
quality.o : $(hdrs) mempool.h
mempool.o : mempool.h
hash.o    : hash.h


