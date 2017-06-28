# Phylogentics
INCLUDEPATH += ../pbdmms

SOURCES += \
    $$PWD/phyg_species_id.cpp \
    phyg_species.cpp \
    phyg_id.cpp \
    phyg_helper.cpp

HEADERS += \
    $$PWD/phyg.h \
    $$PWD/phyg_create_reconstructed.h \
    $$PWD/phyg_remove_self_loops.h \
    $$PWD/phyg_species_id.h \
    $$PWD/phyg_has_ancestor.h \
    phyg_species.h \
    phyg_id.h \
    phyg_helper.h
