//http://www.efda-itm.eu/ITM/html/isip_fc2k_cpp.html

#include "UALClasses.h"

typedef struct {
      char **parameters;
      char **default_param;
      char **schema;
}param;

void mycppfunction(
      ItmNs::Itm::summary &sum,
      ItmNs::Itm::antennas &ant,
      ItmNs::Itm::equilibriumArray &eq,
      int &x,
      ItmNs::Itm::limiter &lim,
      ItmNs::Itm::coreimpur &cor,
      ItmNs::Itm::ironmodelArray &iron,
      double *y,
      char *str,
      param &codeparam)
{

      /* display first line of parameters */
      cout << codeparam.parameters[0] << endl;
      cout << codeparam.default_param[0] << endl;
      cout << codeparam.schema[0] << endl;
      /* display content of inputs */
      cout << "x=" << x << endl;
      cout << sum.time << endl;
      cout << sum.datainfo.dataprovider << endl;
      cout << ant.datainfo.dataprovider << endl;
      cout << eq.array(0).datainfo.dataprovider << endl;
      for (int k=0; k<3; k++) {
           for (int i=0; i<4; i++) {
                 cout << eq.array(k).profiles_1d.psi(i)<< " ";
           }
           cout << endl;
      }
      /* fill limiter CPO */
      lim.datainfo.dataprovider.assign("test_limiter");
      lim.position.r.resize(5);     // allocate vector
      for (int i=0; i<5; i++) {
            lim.position.r(i)=(i+1);
      }
      /* fill coreimpur CPO */
      cor.datainfo.dataprovider.assign("test_coreimpur");
      cor.flag.resize(3);          // allocate vector
      for (int i=0; i<3; i++) {
            cor.flag(i)=(i+1)*10;
      }
      cor.time=0; // don't forget to fill time for time-dependent CPOs
      /* fill ironmodel CPO */
      iron.array.resize(3);        // allocate slices
      for (int j=0; j<3; j++) {
            char s[255];
            sprintf(s,"test_ironmodel%d",j);
            iron.array(j).datainfo.dataprovider.assign(s); // allocate vector
            iron.array(j).desc_iron.geom_iron.npoints.resize(3);
            for (int i=0; i<3; i++) {
                  iron.array(j).desc_iron.geom_iron.npoints(i)=j*i;
            }
            iron.array(j).time=j;       // fill time for time-dependent CPOs
      }
      /* assign value to non CPO outputs */
      x=5;
      for (int i=0; i<10; i++) {
            y[i]=i;
      }
      strcpy(str,"This is a test string");
}
