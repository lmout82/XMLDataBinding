//**********************************************************************************************
//                                        demo.cpp
//
// Author(s): lmout82
// Version 1.x beta
// Licence: MIT License
// Link: https://github.com/lmout82/XMLDataBinding
// Creation date: August 2017
//**********************************************************************************************

#include <iostream>

#include "../src/xmldatabinding.h"
#include "../src/models.h"
#include "../src/modelfactory.h"


class waypoint: public ModelBase {
public:
  waypoint() {
    Add("depth");
    Add("divetime");
    Add("temperature");
  };
  virtual ~waypoint() {};
};


class dive: public ModelBase {
public:
  dive() {
    Add("informationbeforedive.datetime");
    Add("informationafterdive.greatestdepth");
    Add("informationafterdive.averagedepth");
    Add("informationafterdive.diveduration");
    Add("informationbeforedive.surfaceintervalbeforedive.passedtime");
  };
  virtual ~dive() {};
  
  BEGIN_MODEL_SETTERS()
  ADD_SETTER_MODEL(waypoint)
  END_MODEL_SETTERS()

  DECLARE_MODEL_LIST(waypoint,m_Waypoints)
};


class diver: public ModelBase {
public:
  diver() {
    Add("owner.personal.firstname");
    Add("owner.personal.lastname");
    Add("owner.equipment.divecomputer.name");
  };
  virtual ~diver() {};
};


class uddf: public ModelBase {
public:
  uddf() { m_Diver=NULL; };
  virtual ~uddf() {};

  BEGIN_MODEL_SETTERS()
  ADD_SETTER_MODEL(diver)
  ADD_SETTER_MODEL(dive)
  END_MODEL_SETTERS()

  DECLARE_MODEL_REF(diver,m_Diver)
  DECLARE_MODEL_LIST(dive,m_Dives)
};


int main(int argc, char* argv[])
{
  ModelFactory::Register("uddf", new ModelMaker<uddf>());
  ModelFactory::Register("dive", new ModelMaker<dive>());
  ModelFactory::Register("diver", new ModelMaker<diver>());
  ModelFactory::Register("waypoint", new ModelMaker<waypoint>());

  XMLDataBinding doc;
  if (doc.LoadFile("./demo/test.uddf")) {
    uddf* model = dynamic_cast<uddf*>(doc.Parse());
    if (model) {
      std::cout << "=== Diver ===\n";
      MODELLIST_FOREACH(buf,diver,model->m_Diver)
	{
	  std::cout << buf << ": " << diver->GetText() << "\n";
	}

      std::cout<<"\n=== Dives ===\n";
      size_t index_dive = 0;
      MODEL_FOREACH(dive,divemodel,model->m_Dives)
	{
	  index_dive++;
	  std::cout << "\n== Dive #" << index_dive << " ==\n";

	  MODELLIST_FOREACH(buf,diveelt,divemodel)
	    {
	      std::cout << buf << ": " << ((diveelt && diveelt->GetText()) ? diveelt->GetText() : "NULL") << "\n";
	    }

	  // waypoints
	  std::cout << "= Waypoints =\n";
	  MODEL_FOREACH(waypoint,wp,divemodel->m_Waypoints)
	    {
	      MODELLIST_FOREACH(buf,wpelt,wp)
		{
		  std::cout << buf << ": " << wpelt->GetText() << "\n";
		}
	    }
	}
    }
  }

  ModelFactory::Destroy();

  return 0;
}
