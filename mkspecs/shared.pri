defineReplace(findFile) {
    FILE=$$1

    # LIST=$$(PATH)
    # for a reason beyond my knowledge, if i get path list from $$(PATH) and replace all :'s with space, 
    # i cant iterate the entries with qmake's for loop. 
    # SO, here's a list of predefined places where to look for executable files..
    # Also, i cant rely to find doxygen via system() call since doxygen returns error every time its called
    # if it cant find Doxyfile =( 

    LIST  = /bin/ /usr/bin /usr/local/bin /usr/X11R6/bin ~/bin .
    LIST += /scratchbox/devkits/cputransp/bin /scratchbox/devkits/maemo3-tools/bin 
    LIST += /scratchbox/devkits/debian-etch/bin /scratchbox/devkits/doctools/bin
    LIST += /scratchbox/devkits/perl/bin /scratchbox/tools/bin /targets/links/arch_tools/bin
    LIST += /host_usr/bin /scratchbox/compilers/bin

    LOCATED=
    for(path,LIST) {
        exists($${path}/$${FILE}): {
            isEmpty(LOCATED):LOCATED=$${path}/$${FILE}
        }
    }

    return($${LOCATED})
}
