# Makefile for Writing Make Files Example

# *****************************************************
# Variables to control Makefile operation

CXX = g++
CXXFLAGS = -Wall -g

# ****************************************************
# Targets needed to bring the executable up to date

main: main.o recorder.o exception_handler.o
	$(CXX) $(CXXFLAGS) -o main main.o recorder.o exception_handler.o -lportaudio

# The main.o target can be written more simply

recorder.o: recorder.cc
	$(CXX) $(CXXFLAGS) -c recorder.cc

main.o: main.cc wav_info.h wav_writer.h
	$(CXX) $(CXXFLAGS) -c main.cc

exception_handler.o: exception_handler.cc
	$(CXX) $(CXXFLAGS) -c exception_handler.cc