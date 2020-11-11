# XMLDataBinding

XML data binding refers to a means of representing information in an XML document as a C++ object. This allows applications to access the data in the XML from the object rather than using the DOM to retrieve the data from a direct representation of the XML itself.

As C++ does not support reflexion natively, this lib makes an extensive use of STL. It does not use a compiler to generate C++ classes (models) as well as XML parsing and serialization code.

This side project was initially created to extract information from the Universal Dive Data Format (UDDF).

Please note that this lib was developed and tested only under linux (Mint 18).

## Important notice

This repository was initially developed when I was learning to code a long long time ago. Please take into account that this work does not reflect my present skills.

**_This repository is not maintained._**

## Example (from demo.cpp)
### UDDF
```
<uddf version="3.2.0">
  (..)
  <diver>
    <owner id="lkjflkdf">
      <personal>
        <firstname>My firstname</firstname>
        <lastname>My lastname</lastname>
      </personal>
      <equipment>
        <divecomputer id="mares_dc_024_id_2">
          <name>Puck pro</name>
          <manufacturer id="mares_dc_024_sn_123456789-456789">
            <name>Mares</name>
          </manufacturer>
          <model>Puck Pro</model>
          <serialnumber>123456789-456789</serialnumber>
        </divecomputer>
      </equipment>
    </owner>
  </diver>
  <profiledata>
    <repetitiongroup id="repgroup_dive_#1_2015-05-11T09:23:00">
      <dive id="#1_2015-05-11T09:23:00">
        <informationbeforedive>
          (...)
          <datetime>2015-05-11T09:23:00</datetime>
          (...)
        </informationbeforedive>
        <samples>
          <waypoint>
            <depth>0</depth>
            <divetime>0</divetime>
            <temperature>290.65</temperature>
          </waypoint>
          (...)
        </samples>
        <informationafterdive>
          (...)
          <greatestdepth>25.4</greatestdepth>
          <diveduration>3100</diveduration>
          (...)
        </informationafterdive>
      </dive>
      (...)
    </repetitiongroup>
    </profiledata>
</uddf>
```

### Models
```
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
```

### How to use it
```
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
```
### Output
```
=== Diver ===
owner.equipment.divecomputer.name: Puck pro
owner.personal.firstname: My firstname
owner.personal.lastname: My lastname

=== Dives ===

== Dive #1 ==
informationafterdive.averagedepth: NULL
informationafterdive.diveduration: 3100
informationafterdive.greatestdepth: 25.4
informationbeforedive.datetime: 2015-05-11T09:23:00
informationbeforedive.surfaceintervalbeforedive.passedtime: NULL
= Waypoints =
depth: 0
divetime: 0
temperature: 290.65
depth: 3
divetime: 5
temperature: 290.55
depth: 3.7
divetime: 10
temperature: 290.55
depth: 4.4
divetime: 15
temperature: 290.45

== Dive #2 ==
informationafterdive.averagedepth: 
informationafterdive.diveduration: 200
informationafterdive.greatestdepth: 6.8
informationbeforedive.datetime: 2015-08-12T17:49:00
informationbeforedive.surfaceintervalbeforedive.passedtime: 24240.0
= Waypoints =
depth: 0
divetime: 0
temperature: 292.15
depth: 2.6
divetime: 5
temperature: 292.15
depth: 2.5
divetime: 10
temperature: 292.05
depth: 2.3
divetime: 15
temperature: 291.95
depth: 2.7
divetime: 20
temperature: 291.95
```

## License
The sources files are now available freely under the MIT license. I hope this work will be useful.
