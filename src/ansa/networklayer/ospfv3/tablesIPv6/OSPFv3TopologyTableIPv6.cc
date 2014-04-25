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

#include "IPv4Address.h"

#include <algorithm>

#include "OSPFv3TopologyTableIPv6.h"
//#include "EigrpMessage_m.h"

Define_Module(OSPFv3TopologyTableIPv6);


/*std::ostream& operator<<(std::ostream& os, const EigrpRouteSource<IPv4Address>& source)
{
    //const char *state = route.isActive() ? "active" : "passive";
    //const char *source = route.isInternal()() ? "internal" : "external";

    EigrpRoute<IPv4Address> *routeInfo = source.getRouteInfo();

    const char *conn = "Connected";

    os << routeInfo->getRouteAddress() << "/" << routeInfo->getRouteMask().getNetmaskLength();
    os << ", FD is " << routeInfo->getFd() << ", via ";
    if (source.getNextHop().isUnspecified())
        os << conn;
    else
        os << source.getNextHop();

    os << " (" << source.getMetric() << "/" << source.getRd() << "), IF ID = " << source.getIfaceId();
    if (source.isSuccessor()) os << ", is successor";

    return os;
}*/

void OSPFv3TopologyTableIPv6::initialize()
{
//    WATCH_PTRVECTOR(routeVec);
}

void OSPFv3TopologyTableIPv6::handleMessage(cMessage *msg)
{
    throw cRuntimeError("This module does not process messages");
}

OSPFv3TopologyTableIPv6::~OSPFv3TopologyTableIPv6()
{
    int cnt = routeVec.size();
    EigrpRouteSource<IPv4Address> *rt;

    for (int i = 0; i < cnt; i++)
    {
        rt = routeVec[i];
        routeVec[i] = NULL;
        delete rt;
    }
    routeVec.clear();
}
