//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef __INET_OSPFV3AREATABLEIPV6_H_
#define __INET_OSPFV3AREATABLEIPV6_H_

#include <omnetpp.h>

//#include "EigrpRoute.h"
#include "OSPFv3Area.h"
//#include "OSPFv3ProcessIPv6.h"
//#include "OSPFv3Interface.h"
//#include "IOSPFv3Module.h"

//namespace OSPFv3 {

class OSPFv3Area;

class OSPFv3AreaTableIPv6 : public cSimpleModule
{
  private:
    typedef typename std::vector<OSPFv3Area*> OSPFv3AreaVec;

    OSPFv3AreaVec OSPFv3Areas;    /**< Table with areas. */

//    int routeIdCounter;         /**< Counter for route ID */

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);

  public:
    OSPFv3AreaTableIPv6() { /*routeIdCounter = 1;*/ }
    virtual ~OSPFv3AreaTableIPv6();

    void addArea(OSPFv3Area* area);
    OSPFv3Area *getAreaById(const IPv4Address AreaId);

};
//} // namespace OSPFv3
#endif // __INET_OSPFV3AREATABLEIPV6_H_
