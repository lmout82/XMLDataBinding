//**********************************************************************************************
//                                        models.cpp
//
// Author(s): lmout82
// Version 1.x beta
// Licence: MIT License
// Link: https://github.com/lmout82/XMLDataBinding
// Creation date: August 2017
//**********************************************************************************************

#include "models.h"


ModelAttr::ModelAttr() {};

ModelAttr::~ModelAttr() {
  for (AttrType::iterator it = m_Attr.begin(); it != m_Attr.end(); ++it)
    if (it->second)
      delete it->second;
  m_Attr.clear();
};

void ModelAttr::AddAttr(const char* name, const char* value) {
  if (name) {
    AttrType::iterator it = m_Attr.find(name);
    if (it == m_Attr.end())
      m_Attr.insert(AttrType::value_type(name, value ? new std::string(value) : NULL));
  }
};

const char* ModelAttr::GetAttr(const char* name) {
  if (name) {
    AttrType::iterator it = m_Attr.find(name); 
    if (it != m_Attr.end())
      return it->second ? it->second->c_str() : NULL;
    else
      return NULL;
  }
};


ModelElement::ModelElement(const char* text) {
  m_Text = text ? text : "";
};

ModelElement::~ModelElement() {};

const char* ModelElement::GetText() {
  return m_Text.c_str();
};


ModelBase::ModelBase() {
  m_MapIt= m_Map.begin();
};

ModelBase::~ModelBase() {
  for (ModelMap::iterator it = m_Map.begin(); it != m_Map.end(); ++it)
    if(it->second)
      delete it->second;
  m_Map.clear();
};

// Checks whether each inserted element has a key equivalent to the one of an element already in the
// container, and if so, the element is not inserted. Default value is NULL.
void ModelBase::Add(std::string name) {
  m_Map.insert(ModelMap::value_type(name, NULL));
};

// Only declared elements with Add(...) method are settable: text->text inside the element and
// name is the key associated with the text (generaly the element's name)
void ModelBase::Set(const char* name, const char* text) {
  if (name && text) {
    ModelMap::iterator it = m_Map.find(name);
    if (it != m_Map.end() && !it->second)
      it->second = new ModelElement(text);
  }
};

ModelElement* ModelBase::Get(const char* name) {
  if (name) {
    ModelMap::iterator it = m_Map.find(name); 
    if (it != m_Map.end())
      return it->second;
  }
  return NULL;
};

void ModelBase::GetFirst(char** name, ModelElement** elt) {
  m_MapIt = m_Map.begin();
  if (m_MapIt != m_Map.end()) {
    *name  = const_cast<char*>((m_MapIt->first).c_str());
    *elt   = m_MapIt->second;
  }
  else {
    *name  = NULL;
    *elt   = NULL;
  }
};

void ModelBase::GetNext(char** name, ModelElement** elt) {
  m_MapIt++;
  if (m_MapIt != m_Map.end()) {
    *name  = const_cast<char*>((m_MapIt->first).c_str());
    *elt   = m_MapIt->second;      
  }
  else {
    *name  = NULL;
    *elt   = NULL;
  }
};
