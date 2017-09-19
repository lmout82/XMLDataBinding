//**********************************************************************************************
//                                        modelfactory.cpp
//
// Author(s): lmout82
// Version 1.x beta
// Licence: MIT License
// Link: https://github.com/lmout82/XMLDataBinding
// Creation date: August 2017
//**********************************************************************************************

#include "modelfactory.h"


MakerMap ModelFactory::InitMap() {
  MakerMap makers;
  return makers;
};

PointerVec ModelFactory::InitVec() {
  PointerVec pointers;
  return pointers;
};

void ModelFactory::Register(const std::string classname, IModelMaker* maker) {
  if (m_Makers.find(classname) == m_Makers.end())
    m_Makers[classname] = maker;
};

ModelBase* ModelFactory::Create(const char* classname) {
  MakerMap::iterator i = m_Makers.find(classname);
  if (i != m_Makers.end()) {
    IModelMaker* maker = i->second;
    ModelBase* model = maker->Create();
    m_Pointers.push_back(model);
    return model;
  }

  return NULL;
};

// Destroy pointers and makers
void ModelFactory::Destroy() {
  for (MakerMap::iterator it = m_Makers.begin(); it != m_Makers.end(); ++it)
    if (it->second)
      delete it->second;
  m_Makers.clear();

  for (size_t i=0; i<m_Pointers.size(); ++i)
    delete m_Pointers[i];
  m_Pointers.clear();
}

MakerMap ModelFactory::m_Makers = ModelFactory::InitMap();
PointerVec ModelFactory::m_Pointers = ModelFactory::InitVec();
