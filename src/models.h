//**********************************************************************************************
//                                        models.h
//
// Author(s): lmout82
// Version 1.x beta
// Licence: MIT License
// Link: https://github.com/lmout82/XMLDataBinding
// Creation date: August 2017
//**********************************************************************************************

#ifndef Models_H
#define Models_H

#include <vector>
#include <map>
#include <string>


template<class T>
class ModelList {
 public:
  ModelList() { m_Index = 0; };
  ~ModelList() {};

  void Add(T* model) {
    if (model)
      m_List.push_back(model);
  };

  T* GetFirst() {
    if (m_List.size()>0) {
      return m_List.at(0);
      m_Index = 0;
    }
    else
      return NULL;
  };
  T* GetNext() {
    m_Index++;
    if (m_Index<m_List.size()) {
      return m_List.at(m_Index);
    }
    else
      return NULL;
  };

 private:
  size_t m_Index;
  std::vector<T*> m_List;
};


class ModelAttr {
 public:
  ModelAttr();
  ~ModelAttr();

  void AddAttr(const char*, const char*);
  const char* GetAttr(const char*);

 protected:
  typedef std::map<std::string, std::string*> AttrType;
  AttrType m_Attr;
};


class ModelElement: public ModelAttr {
 public:
  ModelElement(const char*);
  ~ModelElement();

  const char* GetText();

 private:
  std::string m_Text;  
};


class ModelBase: public ModelAttr {
 public:
  ModelBase();
  virtual ~ModelBase();

  void Add(std::string);
  void Set(const char*, const char*);
  virtual void Set(const char*, ModelBase*) {};
  ModelElement* Get(const char*);

  void GetFirst(char**, ModelElement**);
  void GetNext(char**, ModelElement**);

  virtual void Show() {};

 protected:
  typedef std::map<std::string, ModelElement*> ModelMap;
  ModelMap::iterator m_MapIt;
  ModelMap m_Map;
};


#define DECLARE_MODEL_REF(classname,varname)		\
  classname* varname;					\
  void set_##classname(ModelBase* model) {		\
    if(!varname)					\
      varname = dynamic_cast<classname*>(model);	\
  };

#define DECLARE_MODEL_LIST(classname,varname)			\
  ModelList<classname> varname;					\
  void set_##classname(ModelBase* model) {			\
    classname* p##varname = dynamic_cast<classname*>(model);	\
    if (p##varname)						\
      varname.Add(p##varname);					\
  };


#define BEGIN_MODEL_SETTERS()				\
  void Set(const char* name, ModelBase* model) {

#define ADD_SETTER_MODEL(classname)		\
  set_##classname(model);

#define END_MODEL_SETTERS()			\
  };


#define MODELLIST_FOREACH(varname_eltname,varname_eltmodel,modellist)	\
  char* varname_eltname = NULL;						\
  ModelElement* varname_eltmodel = NULL;				\
  for ((modellist)->GetFirst(&varname_eltname, &varname_eltmodel); varname_eltname != NULL; \
       (modellist)->GetNext(&varname_eltname, &varname_eltmodel))

#define MODEL_FOREACH(modelname,varname_model,model)			\
  modelname* varname_model = NULL;					\
  for ( varname_model = (model).GetFirst(); varname_model != NULL;	\
	varname_model = (model).GetNext() ) 


#endif
