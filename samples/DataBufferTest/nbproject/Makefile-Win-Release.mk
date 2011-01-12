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
CND_CONF=Win-Release
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/main.o


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
LDLIBSOPTIONS=../../dist/Win-Release/Cygwin_4.x-Windows/libmini-net.a

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-Win-Release.mk dist/Win-Release/Cygwin-Windows/databuffertest.exe

dist/Win-Release/Cygwin-Windows/databuffertest.exe: ../../dist/Win-Release/Cygwin_4.x-Windows/libmini-net.a

dist/Win-Release/Cygwin-Windows/databuffertest.exe: ${OBJECTFILES}
	${MKDIR} -p dist/Win-Release/Cygwin-Windows
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/databuffertest ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I../../include -MMD -MP -MF $@.d -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:
	cd ../.. && ${MAKE}  -f Makefile CONF=Win-Release

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Win-Release
	${RM} dist/Win-Release/Cygwin-Windows/databuffertest.exe

# Subprojects
.clean-subprojects:
	cd ../.. && ${MAKE}  -f Makefile CONF=Win-Release clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
