//**********************************************************************************************
//                                        xmldatabinding.h
//
// Author(s): lmout82
// Version 1.x beta
// Licence: MIT License
// Link: https://github.com/lmout82/XMLDataBinding
// Creation date: August 2017
//**********************************************************************************************

#ifndef XMLDataBinding_H
#define XMLDataBinding_H

#include "tinyxml2.h"
#include "models.h"
#include "modelfactory.h"


class XMLDataBinding {
 public:
  XMLDataBinding();
  ~XMLDataBinding();

  bool LoadFile(const char*);

  ModelBase* Parse();
  ModelBase* ParseElt(tinyxml2::XMLElement*, unsigned int, ModelBase*, char*);

  void AttrToElt(tinyxml2::XMLElement*, ModelBase*);
  void AttrToModel(tinyxml2::XMLElement*, ModelBase*);

 private:
  void GetAttr(tinyxml2::XMLElement*, ModelAttr*);
  char* ConcatPath(const char*, const char*);

  tinyxml2::XMLDocument m_XMLdoc;
};


#endif
