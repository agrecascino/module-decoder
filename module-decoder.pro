TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

include(deployment.pri)
qtcAddDeployment()

QMAKE_CXXFLAGS += -std=c++11
unix: CONFIG += link_pkgconfig

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../Downloads/libopenmpt-0.2.5602/bin/release/ -lopenmpt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../Downloads/libopenmpt-0.2.5602/bin/debug/ -lopenmpt
else:unix: LIBS += -L$$PWD/../Downloads/libopenmpt-0.2.5602/bin/ -lopenmpt

INCLUDEPATH += $$PWD/../Downloads/libopenmpt-0.2.5602/libopenmpt
DEPENDPATH += $$PWD/../Downloads/libopenmpt-0.2.5602/libopenmpt

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../Downloads/libopenmpt-0.2.5602/bin/release/libopenmpt.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../Downloads/libopenmpt-0.2.5602/bin/debug/libopenmpt.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../Downloads/libopenmpt-0.2.5602/bin/release/openmpt.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../Downloads/libopenmpt-0.2.5602/bin/debug/openmpt.lib
else:unix: PRE_TARGETDEPS += $$PWD/../Downloads/libopenmpt-0.2.5602/bin/libopenmpt.a

unix|win32: LIBS += -lportaudiocpp

unix|win32: LIBS += -lportaudio
