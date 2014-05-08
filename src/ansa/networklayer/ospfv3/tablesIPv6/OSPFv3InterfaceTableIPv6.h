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

#ifndef __INET_OSPFV3INTERFACETABLEIPV6_H_
#define __INET_OSPFV3INTERFACETABLEIPV6_H_

#include <omnetpp.h>

#include "OSPFv3Timer_m.h"
#include "InterfaceEntry.h"
#include "OSPFv3Interface.h"


//namespace OSPFv3 {

class OSPFv3InterfaceIPv6;

class OSPFv3InterfaceTableIPv6 : public cSimpleModule
{
  protected:
    typedef typename std::vector<OSPFv3InterfaceIPv6 *> OSPFv3InterfaceVector;

    OSPFv3InterfaceVector OSPFv3Interfaces;

    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);
    virtual int numInitStages() const { return 4; }

//    void cancelHelloTimer(EigrpInterface *iface);

  public:
    ~OSPFv3InterfaceTableIPv6();

    /**< Adds interface to table. */
    void addInterface(OSPFv3InterfaceIPv6 *interface);
    /**< Removes interface from table */
//    EigrpInterface *removeInterface(EigrpInterface *iface);

    /**< Gets interface from table by interface ID */
    OSPFv3InterfaceIPv6 *getInterfaceById(int ifaceId);
    /**< Gets interface from table by interface name */
//    EigrpInterface *findInterfaceByName(const char *ifaceName);
};
//} // namespace OSPFv3
#endif // __INET_OSPFV3INTERFACETABLEIPV6_H_
