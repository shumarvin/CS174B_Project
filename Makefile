CXX=clang++

CXXFLAGS= -std=c++11 -Wall -Wextra -Wno-unused-parameter -Wno-unused-private-field -g -Ofast

#change to this before final submission
#CXXFLAGS= -std=c++11 -Wall -Wextra -Werror

BINARIES=build_index removeNonEnglish test run_database

all: ${BINARIES}

build_index: src/main.o src/fileReader.o src/pageWriter.o
	${CXX} $^ -o $@

run_database: src/runDatabase.o src/console.o src/bPlusTreeIndex.o
	${CXX} $^ -o $@

removeNonEnglish: src/removeNonEnglish.o
	${CXX} $^ -o $@

test: src/tester.o
	${CXX} $^ -o $@

clean:
	/bin/rm -f ${BINARIES} src/*.o
