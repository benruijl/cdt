#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/Simulation.o \
	${OBJECTDIR}/src/Triangle.o \
	${OBJECTDIR}/src/Utils.o \
	${OBJECTDIR}/src/Vertex.o \
	${OBJECTDIR}/src/main.o \
	${OBJECTDIR}/src/moves/AlexanderMove.o \
	${OBJECTDIR}/src/moves/CollapseMove.o \
	${OBJECTDIR}/src/moves/FlipMove.o \
	${OBJECTDIR}/src/moves/InverseAlexanderMove.o \
	${OBJECTDIR}/src/moves/InverseCollapseMove.o \
	${OBJECTDIR}/src/moves/InversePinchingMove.o \
	${OBJECTDIR}/src/moves/Move.o \
	${OBJECTDIR}/src/moves/MoveFactory.o \
	${OBJECTDIR}/src/moves/PinchingMove.o \
	${OBJECTDIR}/src/observables/CTCObservable.o \
	${OBJECTDIR}/src/observables/HausdorffObservable.o \
	${OBJECTDIR}/src/observables/ShapeObservable.o \
	${OBJECTDIR}/src/observables/SpectralDimensionObservable.o \
	${OBJECTDIR}/src/observables/TimeSliceObservable.o \
	${OBJECTDIR}/src/observables/TimeSliceObservableCDT.o \
	${OBJECTDIR}/src/observables/VolumeProfileObservable.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-fopenmp -Wall
CXXFLAGS=-fopenmp -Wall

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/cdt

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/cdt: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/cdt ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/src/Simulation.o: src/Simulation.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -O -Iinclude -fopenmp -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Simulation.o src/Simulation.cpp

${OBJECTDIR}/src/Triangle.o: src/Triangle.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -O -Iinclude -fopenmp -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Triangle.o src/Triangle.cpp

${OBJECTDIR}/src/Utils.o: src/Utils.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -O -Iinclude -fopenmp -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Utils.o src/Utils.cpp

${OBJECTDIR}/src/Vertex.o: src/Vertex.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -O -Iinclude -fopenmp -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Vertex.o src/Vertex.cpp

${OBJECTDIR}/src/main.o: src/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -g -O -Iinclude -fopenmp -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/main.o src/main.cpp

${OBJECTDIR}/src/moves/AlexanderMove.o: src/moves/AlexanderMove.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/moves
	${RM} $@.d
	$(COMPILE.cc) -g -O -Iinclude -fopenmp -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/moves/AlexanderMove.o src/moves/AlexanderMove.cpp

${OBJECTDIR}/src/moves/CollapseMove.o: src/moves/CollapseMove.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/moves
	${RM} $@.d
	$(COMPILE.cc) -g -O -Iinclude -fopenmp -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/moves/CollapseMove.o src/moves/CollapseMove.cpp

${OBJECTDIR}/src/moves/FlipMove.o: src/moves/FlipMove.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/moves
	${RM} $@.d
	$(COMPILE.cc) -g -O -Iinclude -fopenmp -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/moves/FlipMove.o src/moves/FlipMove.cpp

${OBJECTDIR}/src/moves/InverseAlexanderMove.o: src/moves/InverseAlexanderMove.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/moves
	${RM} $@.d
	$(COMPILE.cc) -g -O -Iinclude -fopenmp -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/moves/InverseAlexanderMove.o src/moves/InverseAlexanderMove.cpp

${OBJECTDIR}/src/moves/InverseCollapseMove.o: src/moves/InverseCollapseMove.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/moves
	${RM} $@.d
	$(COMPILE.cc) -g -O -Iinclude -fopenmp -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/moves/InverseCollapseMove.o src/moves/InverseCollapseMove.cpp

${OBJECTDIR}/src/moves/InversePinchingMove.o: src/moves/InversePinchingMove.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/moves
	${RM} $@.d
	$(COMPILE.cc) -g -O -Iinclude -fopenmp -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/moves/InversePinchingMove.o src/moves/InversePinchingMove.cpp

${OBJECTDIR}/src/moves/Move.o: src/moves/Move.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/moves
	${RM} $@.d
	$(COMPILE.cc) -g -O -Iinclude -fopenmp -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/moves/Move.o src/moves/Move.cpp

${OBJECTDIR}/src/moves/MoveFactory.o: src/moves/MoveFactory.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/moves
	${RM} $@.d
	$(COMPILE.cc) -g -O -Iinclude -fopenmp -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/moves/MoveFactory.o src/moves/MoveFactory.cpp

${OBJECTDIR}/src/moves/PinchingMove.o: src/moves/PinchingMove.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/moves
	${RM} $@.d
	$(COMPILE.cc) -g -O -Iinclude -fopenmp -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/moves/PinchingMove.o src/moves/PinchingMove.cpp

${OBJECTDIR}/src/observables/CTCObservable.o: src/observables/CTCObservable.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/observables
	${RM} $@.d
	$(COMPILE.cc) -g -O -Iinclude -fopenmp -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/observables/CTCObservable.o src/observables/CTCObservable.cpp

${OBJECTDIR}/src/observables/HausdorffObservable.o: src/observables/HausdorffObservable.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/observables
	${RM} $@.d
	$(COMPILE.cc) -g -O -Iinclude -fopenmp -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/observables/HausdorffObservable.o src/observables/HausdorffObservable.cpp

${OBJECTDIR}/src/observables/ShapeObservable.o: src/observables/ShapeObservable.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/observables
	${RM} $@.d
	$(COMPILE.cc) -g -O -Iinclude -fopenmp -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/observables/ShapeObservable.o src/observables/ShapeObservable.cpp

${OBJECTDIR}/src/observables/SpectralDimensionObservable.o: src/observables/SpectralDimensionObservable.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/observables
	${RM} $@.d
	$(COMPILE.cc) -g -O -Iinclude -fopenmp -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/observables/SpectralDimensionObservable.o src/observables/SpectralDimensionObservable.cpp

${OBJECTDIR}/src/observables/TimeSliceObservable.o: src/observables/TimeSliceObservable.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/observables
	${RM} $@.d
	$(COMPILE.cc) -g -O -Iinclude -fopenmp -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/observables/TimeSliceObservable.o src/observables/TimeSliceObservable.cpp

${OBJECTDIR}/src/observables/TimeSliceObservableCDT.o: src/observables/TimeSliceObservableCDT.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/observables
	${RM} $@.d
	$(COMPILE.cc) -g -O -Iinclude -fopenmp -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/observables/TimeSliceObservableCDT.o src/observables/TimeSliceObservableCDT.cpp

${OBJECTDIR}/src/observables/VolumeProfileObservable.o: src/observables/VolumeProfileObservable.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/observables
	${RM} $@.d
	$(COMPILE.cc) -g -O -Iinclude -fopenmp -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/observables/VolumeProfileObservable.o src/observables/VolumeProfileObservable.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/cdt

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
