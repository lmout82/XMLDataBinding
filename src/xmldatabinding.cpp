//**********************************************************************************************
//                                        xmldatabinding.cpp
//
// Author(s): lmout82
// Version 1.x beta
// Licence: MIT License
// Link: https://github.com/lmout82/XMLDataBinding
// Creation date: August 2017
//**********************************************************************************************

#include "xmldatabinding.h"


XMLDataBinding::XMLDataBinding() {}

XMLDataBinding::~XMLDataBinding() {}

// Load an XML file from disk
bool XMLDataBinding::LoadFile(const char* filename) {
  tinyxml2::XMLError eResult = m_XMLdoc.LoadFile(filename);

  return eResult == tinyxml2::XML_SUCCESS;
}

// Get attributes of an element and save them in the specified model
void XMLDataBinding::GetAttr(tinyxml2::XMLElement* elt, ModelAttr* model) {
  if (!elt || !model)
    return;

  const tinyxml2::XMLAttribute* pAttr = elt->FirstAttribute();
  while (pAttr) {
    model->AddAttr(pAttr->Name(), pAttr->Value());
    pAttr = pAttr->Next();
  }
}

// Copy the attributes of an element in ModelElement
void XMLDataBinding::AttrToElt(tinyxml2::XMLElement* elt, ModelBase* model) {
  if (!elt || !model)
    return;

  ModelElement* modElt = model->Get(elt->Name());
  if (modElt)
    GetAttr(elt, modElt);
}

// Copy the attributes of an element in Model
void XMLDataBinding::AttrToModel(tinyxml2::XMLElement* elt, ModelBase* model) {
  if (!elt || !model)
    return;

  GetAttr(elt, model);
}

char* XMLDataBinding::ConcatPath(const char* str1, const char* str2) {
  std::string buf = std::string(str1 ? str1 : "");
  if (str1 && str2)
    buf += ".";
  buf +=  std::string(str2 ? str2 : "");

  return strdup(buf.c_str());
}

// Parse an XML file from parentNode
ModelBase* XMLDataBinding::ParseElt(tinyxml2::XMLElement* parentElt, unsigned int indent = 0,
				    ModelBase* parentModel = NULL, char* path = NULL) {
  if (!parentElt)
    return NULL;

  // Does parentElt contain text inside ? If so, add it to parentModel
  const char* textElt = parentElt->GetText();
  char* eltFullName = ConcatPath(path, parentElt->Value());
  if (textElt && parentModel) {
    parentModel->Set(eltFullName, textElt);
    AttrToElt(parentElt, parentModel);
  } else {
    // Check for empty element: it seems not possible to distinguish between <infinity/> and
    // <averagedepth></averagedepth> (with or without spaces)
    if(parentModel && !parentElt->GetText() && !parentElt->FirstChildElement()) {
      parentModel->Set(eltFullName, "");
      AttrToElt(parentElt, parentModel);
    }
  }
  delete eltFullName;

  // Is parentElt binded to a model ?
  char* bufPath = NULL;
  ModelBase* model = NULL;
  model = ModelFactory::Create(parentElt->Value());
  if (model)
    AttrToModel(parentElt, model);
  else
    bufPath = ConcatPath(path, parentElt->Value());

  // Go to the next child element (if any)
  tinyxml2::XMLElement* childElt;
  for (childElt = parentElt->FirstChildElement(); childElt != NULL; childElt = childElt->NextSiblingElement()) 
    {
      
      ModelBase* retModel = ParseElt(childElt, indent+1, model == NULL ? parentModel:model, bufPath);
      if (retModel) {
	ModelBase* base = model == NULL ? parentModel:model;
	base->Set(parentElt->Value(), retModel);
      }
    }

  if (bufPath)
    delete bufPath;

  return model;
}

// Parse whole XML file
ModelBase* XMLDataBinding::Parse() {
  tinyxml2::XMLElement* rootElt = m_XMLdoc.RootElement();
  if (rootElt) {
    ParseElt(rootElt);
  }
}
