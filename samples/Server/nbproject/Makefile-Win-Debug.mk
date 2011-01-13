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
CND_PLATFORM=Cygwin-Windows
CND_CONF=Win-Debug
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/Server.o


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
LDLIBSOPTIONS=../../dist/Win-Debug/Cygwin-Windows/libmini-net.a

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-Win-Debug.mk dist/Win-Debug/Cygwin-Windows/server.exe

dist/Win-Debug/Cygwin-Windows/server.exe: ../../dist/Win-Debug/Cygwin-Windows/libmini-net.a

dist/Win-Debug/Cygwin-Windows/server.exe: ${OBJECTFILES}
	${MKDIR} -p dist/Win-Debug/Cygwin-Windows
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/server ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I../../include -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/Server.o: Server.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I../../include -MMD -MP -MF $@.d -o ${OBJECTDIR}/Server.o Server.cpp

# Subprojects
.build-subprojects:
	cd ../.. && ${MAKE}  -f Makefile CONF=Win-Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Win-Debug
	${RM} dist/Win-Debug/Cygwin-Windows/server.exe

# Subprojects
.clean-subprojects:
	cd ../.. && ${MAKE}  -f Makefile CONF=Win-Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
