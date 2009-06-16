include makeinclude

OBJ	= main.o version.o

all: module.xml usermodule.exe down_prefs.png down_system.png
	mkapp usermodule 

down_prefs.png: prefs.png
	convert -modulate 50,100,100 prefs.png down_prefs.png

down_system.png: system.png
	convert -modulate 50,100,100 system.png down_system.png

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
