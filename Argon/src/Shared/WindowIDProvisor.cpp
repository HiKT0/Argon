#include "windowIDProvisor.h"

int WindowIDProvisor::getNext()
{
    return counter++;
}

int WindowIDProvisor::counter = 200;