CXX=clang++

CXXFLAGS= -std=c++11 -Wall -Wextra -Wno-unused-parameter -Wno-unused-private-field -g

#change to this before final submission
#CXXFLAGS= -std=c++11 -Wall -Wextra -Werror

BINARIES=build_index

all: ${BINARIES}

build_index: src/main.o src/fileReader.o
	${CXX} $^ -o $@


clean:
	/bin/rm -f ${BINARIES} *.0