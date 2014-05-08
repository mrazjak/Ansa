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

#ifndef __INET_OSPFV3NEIGHBORTABLEIPV6_H_
#define __INET_OSPFV3NEIGHBORTABLEIPV6_H_

#include <omnetpp.h>

#include "IPv4Address.h"
#include "IPv6Address.h"
#include "OSPFv3Neighbor.h"

//#include "OSPFv3NeighborTable.h"

//namespace OSPFv3 {

class OSPFv3Neighbor;
/**
 * TODO - Generated class
 */
class OSPFv3NeighborTableIPv6 : public cSimpleModule
{
  protected:
    typedef typename std::vector<OSPFv3Neighbor*> OSPFv3NeighborVector;

    OSPFv3NeighborVector OSPFv3Neighbors;    /**< Table with neighbors. */
//    int neighborCounter;        /**< For unique ID of neighbor */

    virtual void initialize();
    virtual void handleMessage(cMessage *msg);


  public:
    OSPFv3NeighborTableIPv6() { OSPFv3Neighbors.clear();/*neighborCounter = 1;*/ }
    virtual ~OSPFv3NeighborTableIPv6();

    void addNeighbor(OSPFv3Neighbor *neigh);
    OSPFv3Neighbor* getNeighborByIntfIdAndNeigId(IPv4Address rId, int intfID) ;
    OSPFv3Neighbor* getNeighborByIntfIdAndNeigAddress(IPv6Address rAdd, int intfID) ;
    int getNeighborsByIntfId(int intfID, OSPFv3NeighborVector* neighbors);
};
//} // namespace OSPFv3
#endif // __INET_OSPFV3NEIGHBORTABLEIPV6_H_
