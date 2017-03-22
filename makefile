#target: dependencies


CC = gcc
CXX = g++
CXXFLAGS += .Wall 
CXXFLAGS = -std=c++0x

OBJS =  ftserver.o

SRCS = ftserver.c

PROGS = ftserver

all: ${PROGS} 

ftserver: ${OBJS} ${HEADERS}
	${CXX} ${CXXFLAGS} ${SRCS} ${HEADERS} -o ftserver

${OBJS}: ${SRCS}
	${CXX} ${CXXFLAGS} -c $(@:.o=.c) 

clean:
	rm - ${PROGS} *.o *~


