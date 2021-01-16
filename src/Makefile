CXX = g++
CXXFLAGS = -O3 -std=c++11 -ffast-math -DNDEBUG -g -W -Wall -Wno-deprecated 
LINKFLAGS = -lm 

SRCS = \
	Hash.cpp \
	ESP.cpp \
	SFM.cpp \
	SFMEDM.cpp \
	Main.cpp

OBJS = $(SRCS:%.cpp=%.o)

all: sfmedm
sfmedm: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OTHERFLAGS) $(OBJS) $(LINKFLAGS) -o sfmedm

debug:
	make all CXXFLAGS="-ggdb -W -Wall -pedantic"

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(OTHERFLAGS) -c $<

clean:
	rm -f sfmedm *.o *~
