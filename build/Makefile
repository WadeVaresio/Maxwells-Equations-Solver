PROJECT_DEPENDENCIES=../src/Point.o ../src/PointManager.o ../src/InitialVoltageCalculator.o \
			../src/DevicePointImporter.o ../src/Coordinates.o ../src/CoordinateHasher.o ../src/FieldVector.o ../src/FieldSolver.o
CXX=g++
STANDARD=c++11
MAIN_TARGET=main
TEST_ROD_CURRENT_FLOW=TestRodCurrentFlow
CXXFLAGS= -std=${STANDARD}

all: ../src/main.o ../test/testRodCurrentFlow.o ${PROJECT_DEPENDENCIES}

${MAIN_TARGET}: ../src/main.o ${PROJECT_DEPENDENCIES}
	${CXX} $^ -o $@

${TEST_ROD_CURRENT_FLOW}: ../test/testRodCurrentFlow.o ${PROJECT_DEPENDENCIES}
	${CXX} $^ -o $@

clean:
	/bin/rm -f ../src/*.o
	/bin/rm -f ../test/*.o
	/bin/rm -f ${MAIN_TARGET}
	/bin/rm -f ${TEST_ROD_CURRENT_FLOW}
