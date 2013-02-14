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
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/observables/TimeSliceObservable.o \
	${OBJECTDIR}/src/main.o \
	${OBJECTDIR}/src/moves/FlipMove.o \
	${OBJECTDIR}/src/moves/AlexanderMove.o \
	${OBJECTDIR}/src/moves/Move.o \
	${OBJECTDIR}/src/moves/InversePinchingMove.o \
	${OBJECTDIR}/src/moves/InverseAlexanderMove.o \
	${OBJECTDIR}/src/moves/PinchingMove.o \
	${OBJECTDIR}/src/observables/SpectralDimensionObservable.o \
	${OBJECTDIR}/src/moves/InverseCollapseMove.o \
	${OBJECTDIR}/src/Simulation.o \
	${OBJECTDIR}/src/observables/ShapeObservable.o \
	${OBJECTDIR}/src/Utils.o \
	${OBJECTDIR}/src/observables/VolumeProfileObservable.o \
	${OBJECTDIR}/src/moves/MoveFactory.o \
	${OBJECTDIR}/src/Triangle.o \
	${OBJECTDIR}/src/moves/CollapseMove.o \
	${OBJECTDIR}/src/Vertex.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

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

${OBJECTDIR}/src/observables/TimeSliceObservable.o: src/observables/TimeSliceObservable.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/observables
	${RM} $@.d
	$(COMPILE.cc) -O2 -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/observables/TimeSliceObservable.o src/observables/TimeSliceObservable.cpp

${OBJECTDIR}/src/main.o: src/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/main.o src/main.cpp

${OBJECTDIR}/src/moves/FlipMove.o: src/moves/FlipMove.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/moves
	${RM} $@.d
	$(COMPILE.cc) -O2 -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/moves/FlipMove.o src/moves/FlipMove.cpp

${OBJECTDIR}/src/moves/AlexanderMove.o: src/moves/AlexanderMove.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/moves
	${RM} $@.d
	$(COMPILE.cc) -O2 -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/moves/AlexanderMove.o src/moves/AlexanderMove.cpp

${OBJECTDIR}/src/moves/Move.o: src/moves/Move.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/moves
	${RM} $@.d
	$(COMPILE.cc) -O2 -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/moves/Move.o src/moves/Move.cpp

${OBJECTDIR}/src/moves/InversePinchingMove.o: src/moves/InversePinchingMove.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/moves
	${RM} $@.d
	$(COMPILE.cc) -O2 -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/moves/InversePinchingMove.o src/moves/InversePinchingMove.cpp

${OBJECTDIR}/src/moves/InverseAlexanderMove.o: src/moves/InverseAlexanderMove.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/moves
	${RM} $@.d
	$(COMPILE.cc) -O2 -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/moves/InverseAlexanderMove.o src/moves/InverseAlexanderMove.cpp

${OBJECTDIR}/src/moves/PinchingMove.o: src/moves/PinchingMove.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/moves
	${RM} $@.d
	$(COMPILE.cc) -O2 -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/moves/PinchingMove.o src/moves/PinchingMove.cpp

${OBJECTDIR}/src/observables/SpectralDimensionObservable.o: src/observables/SpectralDimensionObservable.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/observables
	${RM} $@.d
	$(COMPILE.cc) -O2 -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/observables/SpectralDimensionObservable.o src/observables/SpectralDimensionObservable.cpp

${OBJECTDIR}/src/moves/InverseCollapseMove.o: src/moves/InverseCollapseMove.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/moves
	${RM} $@.d
	$(COMPILE.cc) -O2 -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/moves/InverseCollapseMove.o src/moves/InverseCollapseMove.cpp

${OBJECTDIR}/src/Simulation.o: src/Simulation.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Simulation.o src/Simulation.cpp

${OBJECTDIR}/src/observables/ShapeObservable.o: src/observables/ShapeObservable.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/observables
	${RM} $@.d
	$(COMPILE.cc) -O2 -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/observables/ShapeObservable.o src/observables/ShapeObservable.cpp

${OBJECTDIR}/src/Utils.o: src/Utils.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Utils.o src/Utils.cpp

${OBJECTDIR}/src/observables/VolumeProfileObservable.o: src/observables/VolumeProfileObservable.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/observables
	${RM} $@.d
	$(COMPILE.cc) -O2 -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/observables/VolumeProfileObservable.o src/observables/VolumeProfileObservable.cpp

${OBJECTDIR}/src/moves/MoveFactory.o: src/moves/MoveFactory.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/moves
	${RM} $@.d
	$(COMPILE.cc) -O2 -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/moves/MoveFactory.o src/moves/MoveFactory.cpp

${OBJECTDIR}/src/Triangle.o: src/Triangle.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Triangle.o src/Triangle.cpp

${OBJECTDIR}/src/moves/CollapseMove.o: src/moves/CollapseMove.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/moves
	${RM} $@.d
	$(COMPILE.cc) -O2 -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/moves/CollapseMove.o src/moves/CollapseMove.cpp

${OBJECTDIR}/src/Vertex.o: src/Vertex.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -Iinclude -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Vertex.o src/Vertex.cpp

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
