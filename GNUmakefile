#****************************************************************************
#
# GNUmakefile for building SPKRID on Debian 13 / GCC.
#
# Prerequisites:
#   sudo apt install g++ make libncurses-dev libasound2-dev
#
# Targets: admincp  userapp
#
# Usage:
#   make -f GNUmakefile
#   make -f GNUmakefile clean
#
#****************************************************************************

TARGET  = bin
CXX     = g++
CC      = gcc
CXXFLAGS = -std=c++14 -Wall -I.
CFLAGS   = -Wall -I.
LDFLAGS  = -lm -lncurses -lasound

all: $(TARGET)/admincp $(TARGET)/userapp
	mkdir -p $(TARGET)/profiles
	mkdir -p $(TARGET)/uidata/menus
	mkdir -p $(TARGET)/uidata/dialogs
	mkdir -p $(TARGET)/uidata/audio
	cp classes/uinterface/audio/*.wav $(TARGET)/uidata/audio/
	cp classes/uinterface/menugeneric/adminmainmenu/*.dlg $(TARGET)/uidata/dialogs/
	cp classes/uinterface/menugeneric/adminmainmenu/*.mnu $(TARGET)/uidata/menus/
	cp classes/uinterface/menugeneric/usermainmenu/*.dlg $(TARGET)/uidata/dialogs/
	cp classes/uinterface/menugeneric/usermainmenu/*.mnu $(TARGET)/uidata/menus/

#****************************************************************************
#                   Build the Administrator Control Panel                   *
#****************************************************************************
ADMINCP_OBJS = \
	admincp.o \
	adminmainmenu.o \
	menugeneric.o \
	dialogbox.o \
	infobox.o \
	speechprofile.o \
	profilemanager.o \
	silenceremover.o \
	alsawavereader.o \
	medianfilter.o \
	lpanalyzer.o \
	linearpredictor.o \
	pitchdetector.o \
	pitchextractor.o \
	impulsegenerator.o \
	firfilter.o \
	signal.o \
	waveletanalyzer.o \
	daub2analyzer.o \
	downsampler.o \
	readkey.o \
	drawbox.o \
	expand.o \
	conio_compat.o \
	audioreader.o \
	audiowriter.o \
	playaudio.o \
	alsawavewriter.o \
	wavereader.o \
	wavepcmreader.o

mkdir_target:
	mkdir -p $(TARGET)

$(TARGET)/admincp: mkdir_target $(ADMINCP_OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET)/admincp $(ADMINCP_OBJS) $(LDFLAGS)

#****************************************************************************
#                    Build the User Application Module                      *
#****************************************************************************
USERAPP_OBJS = \
	userapp.o \
	usermainmenu.o \
	menugeneric.o \
	dialogbox.o \
	infobox.o \
	speechprofile.o \
	profilemanager.o \
	silenceremover.o \
	alsawavereader.o \
	medianfilter.o \
	lpanalyzer.o \
	linearpredictor.o \
	pitchdetector.o \
	pitchextractor.o \
	impulsegenerator.o \
	firfilter.o \
	signal.o \
	waveletanalyzer.o \
	daub2analyzer.o \
	downsampler.o \
	readkey.o \
	drawbox.o \
	expand.o \
	conio_compat.o \
	audioreader.o \
	audiowriter.o \
	playaudio.o \
	alsawavewriter.o \
	wavereader.o \
	wavepcmreader.o

$(TARGET)/userapp: mkdir_target $(USERAPP_OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET)/userapp $(USERAPP_OBJS) $(LDFLAGS)

#****************************************************************************
#                         Compilation rules                                 *
#****************************************************************************
admincp.o: admincp.cpp
	$(CXX) $(CXXFLAGS) -c admincp.cpp

userapp.o: userapp.cpp
	$(CXX) $(CXXFLAGS) -c userapp.cpp

adminmainmenu.o: classes/uinterface/menugeneric/adminmainmenu/adminmainmenu.cpp
	$(CXX) $(CXXFLAGS) -c classes/uinterface/menugeneric/adminmainmenu/adminmainmenu.cpp -o adminmainmenu.o

usermainmenu.o: classes/uinterface/menugeneric/usermainmenu/usermainmenu.cpp
	$(CXX) $(CXXFLAGS) -c classes/uinterface/menugeneric/usermainmenu/usermainmenu.cpp -o usermainmenu.o

menugeneric.o: classes/uinterface/menugeneric/menugeneric.cpp
	$(CXX) $(CXXFLAGS) -c classes/uinterface/menugeneric/menugeneric.cpp -o menugeneric.o

dialogbox.o: classes/uinterface/dialogbox/dialogbox.cpp
	$(CXX) $(CXXFLAGS) -c classes/uinterface/dialogbox/dialogbox.cpp -o dialogbox.o

infobox.o: classes/uinterface/infobox/infobox.cpp
	$(CXX) $(CXXFLAGS) -c classes/uinterface/infobox/infobox.cpp -o infobox.o

speechprofile.o: classes/speechprofile/speechprofile.cpp
	$(CXX) $(CXXFLAGS) -c classes/speechprofile/speechprofile.cpp -o speechprofile.o

profilemanager.o: classes/profilemanager/profilemanager.cpp
	$(CXX) $(CXXFLAGS) -c classes/profilemanager/profilemanager.cpp -o profilemanager.o

silenceremover.o: classes/silenceremover/silenceremover.cpp
	$(CXX) $(CXXFLAGS) -c classes/silenceremover/silenceremover.cpp -o silenceremover.o

alsawavereader.o: classes/audioreader/alsawavereader/alsawavereader.cpp
	$(CXX) $(CXXFLAGS) -c classes/audioreader/alsawavereader/alsawavereader.cpp -o alsawavereader.o

medianfilter.o: classes/filter/median/medianfilter.cpp
	$(CXX) $(CXXFLAGS) -c classes/filter/median/medianfilter.cpp -o medianfilter.o

lpanalyzer.o: classes/lpanalyzer/lpanalyzer.cpp
	$(CXX) $(CXXFLAGS) -c classes/lpanalyzer/lpanalyzer.cpp -o lpanalyzer.o

linearpredictor.o: classes/lpanalyzer/lpredictor/linearpredictor.cpp
	$(CXX) $(CXXFLAGS) -c classes/lpanalyzer/lpredictor/linearpredictor.cpp -o linearpredictor.o

pitchdetector.o: classes/pitch/rgold/pitchdetector.cpp
	$(CXX) $(CXXFLAGS) -c classes/pitch/rgold/pitchdetector.cpp -o pitchdetector.o

pitchextractor.o: classes/pitch/rgold/pitchextractor/pitchextractor.cpp
	$(CXX) $(CXXFLAGS) -c classes/pitch/rgold/pitchextractor/pitchextractor.cpp -o pitchextractor.o

impulsegenerator.o: classes/pitch/rgold/impulsegenerator/impulsegenerator.cpp
	$(CXX) $(CXXFLAGS) -c classes/pitch/rgold/impulsegenerator/impulsegenerator.cpp -o impulsegenerator.o

firfilter.o: classes/filter/fir/firfilter.cpp
	$(CXX) $(CXXFLAGS) -c classes/filter/fir/firfilter.cpp -o firfilter.o

waveletanalyzer.o: classes/waveletbox/analyzer/waveletanalyzer.cpp
	$(CXX) $(CXXFLAGS) -c classes/waveletbox/analyzer/waveletanalyzer.cpp -o waveletanalyzer.o

daub2analyzer.o: classes/waveletbox/analyzer/daub2/daub2analyzer.cpp
	$(CXX) $(CXXFLAGS) -c classes/waveletbox/analyzer/daub2/daub2analyzer.cpp -o daub2analyzer.o

downsampler.o: classes/waveletbox/analyzer/downsampler/downsampler.cpp
	$(CXX) $(CXXFLAGS) -c classes/waveletbox/analyzer/downsampler/downsampler.cpp -o downsampler.o

audioreader.o: classes/audioreader/audioreader.cpp
	$(CXX) $(CXXFLAGS) -c classes/audioreader/audioreader.cpp -o audioreader.o

audiowriter.o: classes/audiowriter/audiowriter.cpp
	$(CXX) $(CXXFLAGS) -c classes/audiowriter/audiowriter.cpp -o audiowriter.o

signal.o: classes/signal/signal.cpp
	$(CXX) $(CXXFLAGS) -c classes/signal/signal.cpp -o signal.o

readkey.o: classes/uinterface/console/readkey.c
	$(CC) $(CFLAGS) -c classes/uinterface/console/readkey.c -o readkey.o

expand.o: classes/uinterface/console/expand.c
	$(CC) $(CFLAGS) -c classes/uinterface/console/expand.c -o expand.o

drawbox.o: classes/uinterface/console/drawbox.c
	$(CC) $(CFLAGS) -c classes/uinterface/console/drawbox.c -o drawbox.o

conio_compat.o: classes/uinterface/console/conio_compat.cpp
	$(CXX) $(CXXFLAGS) -c classes/uinterface/console/conio_compat.cpp -o conio_compat.o

playaudio.o: classes/audiowriter/playaudio.cpp
	$(CXX) $(CXXFLAGS) -c classes/audiowriter/playaudio.cpp -o playaudio.o

alsawavewriter.o: classes/audiowriter/alsawavewriter/alsawavewriter.cpp
	$(CXX) $(CXXFLAGS) -c classes/audiowriter/alsawavewriter/alsawavewriter.cpp -o alsawavewriter.o

wavereader.o: classes/audioreader/wavereader/wavereader.cpp
	$(CXX) $(CXXFLAGS) -c classes/audioreader/wavereader/wavereader.cpp -o wavereader.o

wavepcmreader.o: classes/audioreader/wavereader/pcm/wavepcmreader.cpp
	$(CXX) $(CXXFLAGS) -c classes/audioreader/wavereader/pcm/wavepcmreader.cpp -o wavepcmreader.o

#****************************************************************************
clean:
	rm -f *.o
	rm -f $(TARGET)/admincp $(TARGET)/userapp
