#-------------------------------------------------
#
# Project created by QtCreator 2014-11-08T10:54:30
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Babel
TEMPLATE = app


SOURCES += main.cpp\
    ClientWorker.cpp \
    Mainwindow.cpp \
    babelwindow.cpp \
    DisplayQt.cpp \
    data/ClientData.cpp \
    data/Friend.cpp \
    data/User.cpp \
    ../abstraction/QTMutex.cpp \
    Call.cpp \
    data/Callee.cpp \
    call/HostedCall.cpp \
    call/RemoteCall.cpp \
    ../common/PacketHelper.cpp \
    sound/OpusCodecManager.cpp \
    sound/PortAudioSoundManager.cpp \
    sound/SoundBuffer.cpp \
    sound/SoundContainer.cpp \
    sound/SoundMixer.cpp

HEADERS  += \
    ClientWorker.h \
    Mainwindow.h \
    babelwindow.h \
    data/ClientData.h \
    data/Friend.h \
    data/User.h \
    ../abstraction/IMutex.h \
    ../abstraction/myMutex.h \
    ../abstraction/QTMutex.h \
    ../common/TCPPacketHeader.h \
    ../common/constant.h \
    ../common/eCommandId.h \
    Call.h \
    data/Callee.h \
    ../common/eErrorCode.h \
    ../common/Serializer.hpp \
    data/eAuthType.h \
    ../common/eFriendRequestType.h \
    ../common/eFriendStatus.h \
    call/ICall.h \
    call/HostedCall.h \
    ../common/eCalleeStatus.h \
    call/RemoteCall.h \
    ../common/PacketHelper.h \
    ../common/UDPPacketHeader.h \
    sound/ICodecManager.h \
    sound/ISoundManager.h \
    sound/OpusCodecManager.h \
    sound/PortAudioSoundManager.h \
    sound/SoundBuffer.h \
    sound/SoundConstant.h \
    sound/SoundContainer.h \
    sound/SoundMixer.h

INCLUDEPATH += . \
               ../common \
               ../abstraction \
               ./data \
               ./call \
               ./sound \
               ../../lib/portaudio/include \
               ../../lib/opus-1.1/include

DEFINES += "_CLIENT"

FORMS += \
    Mainwindow.ui \
    babelwindow.ui

LIBS += ../../build/Win32/Debug/portaudio.lib \
        ../../build/Win32/Debug/celt.lib \
        ../../build/Win32/Debug/opus.lib \
        ../../build/Win32/Debug/silk_common.lib \
        ../../build/Win32/Debug/silk_fixed.lib \
        ../../build/Win32/Debug/silk_float.lib \
        "C:\Program Files\Microsoft SDKs\Windows\v7.1\Lib\ksguid.lib" \
        winspool.lib \
        shlwapi.lib \
        rpcrt4.lib \
        libEGLd.lib \
        libGLESv2d.lib \
        kernel32.lib \
        shell32.lib \
        uuid.lib \
        comdlg32.lib \
        oleaut32.lib \
        imm32.lib \
        winmm.lib \
        ws2_32.lib \
        ole32.lib \
        advapi32.lib \
        d3d9.lib \
        dxguid.lib \
        gdi32.lib \
        user32.lib \
