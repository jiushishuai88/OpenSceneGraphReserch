#include "datadefine.h"
BasePrimtiveData::BasePrimtiveData(PriMtiveType type):
    baseNormal(0,0,1),
    center(0,0,0),
    normal(0,0,1),
    color(0.5,0.5,0.5,1),
    type(type)
{
}

BasePrimtiveData::~BasePrimtiveData()
{

}


PipeData::PipeData(PriMtiveType type):
    BasePrimtiveData(type)
{

}

PipeData::~PipeData()
{

}

OgivePipeData::OgivePipeData(PriMtiveType type):
    PipeData(type)
{

}

OgivePipeData::~OgivePipeData()
{

}
