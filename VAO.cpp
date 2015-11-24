#include "VAO.h"

VAO::VAO() : currentVertexIndex(0), VBOsize(-1)
{
}

int VAO::size()
{
    return VBOsize;
}
