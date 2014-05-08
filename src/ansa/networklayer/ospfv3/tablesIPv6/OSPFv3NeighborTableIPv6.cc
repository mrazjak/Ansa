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

#include "OSPFv3NeighborTableIPv6.h"
#include "IPv4Address.h"

Define_Module(OSPFv3NeighborTableIPv6);

/*std::ostream& operator<<(std::ostream& os, const EigrpNeighbor<IPv4Address>& neigh)
{
    const char *state = neigh.getState() ? "up" : "pending";

    os << "If ID = " << neigh.getIfaceId() << ", IP = " << neigh.getIPAddress();
    os << ", Hold = " << neigh.getHoldInt() << ", state is " << state;
    return os;
}*/

// Must be there (cancelHoldTimer method)
OSPFv3NeighborTableIPv6::~OSPFv3NeighborTableIPv6()
{
    int cnt = OSPFv3Neighbors.size();
    OSPFv3Neighbor*neigh;

    for (int i = 0; i < cnt; i++)
    {
        neigh = OSPFv3Neighbors[i];

        //cancelHoldTimer(neigh);

        OSPFv3Neighbors[i] = NULL;
        delete neigh;
    }
    OSPFv3Neighbors.clear();
}

void OSPFv3NeighborTableIPv6::initialize()
{
    //WATCH_PTRVECTOR(neighborVec);
}

void OSPFv3NeighborTableIPv6::handleMessage(cMessage *msg)
{
    throw cRuntimeError("This module does not process messages");
}
void OSPFv3NeighborTableIPv6::addNeighbor(OSPFv3Neighbor *neigh)
{
    //TODO check duplicity
    OSPFv3Neighbors.push_back(neigh);
}
OSPFv3Neighbor* OSPFv3NeighborTableIPv6::getNeighborByIntfIdAndNeigId(IPv4Address nId,int intfID) {
    OSPFv3NeighborVector::iterator it;

    for (it = OSPFv3Neighbors.begin(); it != OSPFv3Neighbors.end(); it++)
    {
        if (((*it)->getNeighborID() == nId)&&((*it)->getInterface()->getInterfaceId() == intfID))
        {
            return *it;
        }
    }
    return NULL;
}
OSPFv3Neighbor* OSPFv3NeighborTableIPv6::getNeighborByIntfIdAndNeigAddress(IPv6Address nAdd,int intfID) {
    OSPFv3NeighborVector::iterator it;

    for (it = OSPFv3Neighbors.begin(); it != OSPFv3Neighbors.end(); it++)
    {
        if (((*it)->getNeighborAddress() == nAdd)&&((*it)->getInterface()->getInterfaceId() == intfID))
        {
            return *it;
        }
    }
    return NULL;
}
int OSPFv3NeighborTableIPv6::getNeighborsByIntfId(int intfID,OSPFv3NeighborVector* neighbors) {
    OSPFv3NeighborVector::iterator it;
    int count = 0;

    for (it = OSPFv3Neighbors.begin(); it != OSPFv3Neighbors.end(); it++)
    {
        if ((*it)->getInterface()->getInterfaceId() == intfID)
        {
            neighbors->push_back((*it));
            count++;
        }
    }
    return count;
}

