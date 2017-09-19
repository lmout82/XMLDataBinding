//**********************************************************************************************
//                                        modelfactory.h
//
// Author(s): lmout82
// Version 1.x beta
// Licence: MIT License
// Link: https://github.com/lmout82/XMLDataBinding
// Creation date: August 2017
//**********************************************************************************************

#ifndef ModelFactory_H
#define ModelFactory_H


#include "models.h"


class IModelMaker {
 public:
  virtual ModelBase* Create() = 0;
  virtual ~IModelMaker() {};
};


template<typename T>
class ModelMaker : public IModelMaker
{
 public:
  virtual ModelBase* Create()
  {
    return new T;
  }
};


typedef std::map<std::string, IModelMaker*> MakerMap;
typedef std::vector<ModelBase*> PointerVec;

class ModelFactory {
 public:
  static MakerMap InitMap();
  static PointerVec InitVec();

  static void Register(const std::string, IModelMaker*);

  static ModelBase* Create(const char*);
  static void Destroy();

 private:
  static  MakerMap m_Makers;
  static  PointerVec m_Pointers;
};


#endif
