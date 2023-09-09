TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ../../src

DEFINES += CONFIG_AT_CMD_SUPPORT CONFIG_UART_ECHO QT_RUN

SOURCES += \
        ../../src/app_AT_cmd.c \
        ../../src/app_AT_cmd_comm.c \
        ../../src/app_custom_AT_cmd.c \
        ../../src/app_sys_AT_cmd.c \
        ../../src/cli.c \
        ../../src/command.c \
        main.c

HEADERS += \
    ../../src/app_AT_cmd.h \
    ../../src/at_def.h \
    ../../src/at_main.h \
    ../../src/cli.h \
    ../../src/command.h
