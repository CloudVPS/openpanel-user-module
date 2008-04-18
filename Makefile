include makeinclude

OBJ	= main.o version.o

all: module.xml usermodule.exe
	mkapp usermodule 

module.xml: module.def
	mkmodulexml < module.def > module.xml

version.cpp:
	mkversion version.cpp

usermodule.exe: $(OBJ)
	$(LD) $(LDFLAGS) -o usermodule.exe $(OBJ) $(LIBS)  \
	../opencore/api/c++/lib/libcoremodule.a

clean:
	rm -f *.o *.exe
	rm -rf usermodule.app
	rm -f usermodule

SUFFIXES: .cpp .o
.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -I../opencore/api/c++/include -c -g $<
