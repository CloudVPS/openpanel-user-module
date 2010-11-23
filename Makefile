# This file is part of OpenPanel - The Open Source Control Panel
# OpenPanel is free software: you can redistribute it and/or modify it 
# under the terms of the GNU General Public License as published by the Free 
# Software Foundation, using version 3 of the License.
#
# Please note that use of the OpenPanel trademark may be subject to additional 
# restrictions. For more information, please visit the Legal Information 
# section of the OpenPanel website on http://www.openpanel.com/

include makeinclude

OBJ	= main.o version.o

all: module.xml usermodule.exe down_prefs.png down_system.png down_user.png
	grace mkapp usermodule 

down_prefs.png: prefs.png
	convert -modulate 50,100,100 prefs.png down_prefs.png

down_system.png: system.png
	convert -modulate 50,100,100 system.png down_system.png

down_user.png: user.png
	convert -modulate 50,100,100 user.png down_user.png

module.xml: module.def
	mkmodulexml < module.def > module.xml

version.cpp:
	grace mkversion version.cpp

usermodule.exe: $(OBJ)
	$(LD) $(LDFLAGS) -o usermodule.exe $(OBJ) $(LIBS)  \
	/usr/lib/openpanel-core/libcoremodule.a

install:
	mkdir -p ${DESTDIR}/var/openpanel/modules/User.module
	mkdir -p ${DESTDIR}/var/openpanel/conf/staging/User
	cp -rf ./usermodule.app    ${DESTDIR}/var/openpanel/modules/User.module/
	ln -sf usermodule.app/exec ${DESTDIR}/var/openpanel/modules/User.module/action
	cp     module.xml          ${DESTDIR}/var/openpanel/modules/User.module/module.xml
	cp     *.png               ${DESTDIR}/var/openpanel/modules/User.module/
	install -m 755 verify      ${DESTDIR}/var/openpanel/modules/User.module/verify

clean:
	rm -f *.o *.exe
	rm -rf usermodule.app
	rm -f usermodule

SUFFIXES: .cpp .o
.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c -g $<
