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

#ifndef __INET_OSPFV3PROCESSIPV6_H_
#define __INET_OSPFV3PROCESSIPV6_H_

#include <omnetpp.h>

#include "InterfaceTable.h"
#include "InterfaceEntry.h"
#include "IPv4Address.h"
#include "IPv6Address.h"
#include "IPv6ControlInfo.h"
#include "ANSARoutingTable6.h"
#include "ANSARoutingTable6Access.h"
#include "NotificationBoard.h"

#include "IOSPFv3Module.h"
#include "OSPFv3InterfaceTableIPv6.h"
#include "OSPFv3NeighborTableIPv6.h"
#include "OSPFv3TopologyTableIPv6.h"

//#include "OSPFv3LinkTableIPv6.h"

#include "OSPFv3Timer_m.h"
#include "OSPFv3PacketIPv6_m.h"

class OSPFv3ProcessIPv6 : public cSimpleModule, public IOSPFv3Module, public INotifiable
{
  protected:
    typedef std::vector<ANSAIPv4Route *> RouteVector;

    const char* OSPFV3_OUT_GW;  /**< Output gateway to the OSPFv3 module */
    const IPv6Address OSPFV3_IPV6_MULT;    /**< Multicast address for OSPFv3 messages */
    const IPv6Address OSPFV3_IPV6_MULT_DR; /**< Multicast address for OSPFv3 messages for all DR router*/

    int processID;              /**< Autonomous system number */
    IPv4Address routerID;       /**< OSPF Router ID*/


    IInterfaceTable *ift;
    ANSARoutingTable6 *rt6;
    NotificationBoard *nb;

    OSPFv3InterfaceTableIPv6 *OSPFv3Ift;               /**< Table with enabled OSPFv3 interfaces */
    OSPFv3NeighborTableIPv6  *OSPFv3Nt;                /**< Table with OSPFv3 neighbors */
    OSPFv3TopologyTableIPv6  *OSPFv3Tt;                /**< Topology table */

//    OSPFv3LinkTableIPv6 *linkTable;                    /**< Links included in OSPFv3 */

    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);
    /**< Multi-stage initialization. */
    virtual int numInitStages() const { return 5; }

    OSPFv3Timer *createTimer(char timerKind, void *context);

    void processTimer(cMessage *msg);

    OSPFv3InterfaceIPv6 *addInterfaceToOSPFv3(int ifaceId, int area,/*int linkId,*/ bool enabled);
    void enableInterface(InterfaceEntry *iface, OSPFv3InterfaceIPv6 *OSPFv3Iface/*, IPv4Address& ifAddress, IPv4Address& ifMask*//*, int linkId*/);

    void sendHelloPacket(int ifaceId);
    OSPFv3HelloPacket *createHelloPacket(int ifaceId);
    void addOSPFv3Header(OSPFv3Packet *pkt, char type, IPv4Address routerID, IPv4Address areaID, char instanceID);
    void addIPv6CtrInfo(OSPFv3Packet *pkt, int ifaceId, const IPv6Address &destAddress, short hopLimit);

  public:
    OSPFv3ProcessIPv6();
    ~OSPFv3ProcessIPv6();

    virtual void receiveChangeNotification(int category, const cObject *details);

    // Interface IOSPFv3Module
    void addInterface(int ifaceId, int area,/*int linkId,*/ bool enabled)
         { addInterfaceToOSPFv3(ifaceId, area,/*linkId,*/ enabled); }
//    OSPFv3LinkIPv6 *addLink(IPv6Address address, int mask);
    void setProcessID(int prcssID) { this->processID = prcssID; }
    void setRouterID(IPv4Address routerID) { this->routerID = routerID; }

    int getProcessID() const { return this->processID; }
    IPv4Address getRouterID() const { return this->routerID; }

};

/**
 * @brief Class gives access to the OSPFv3ProcessIPv6.
 */
class INET_API OSPFv3ProcessIPv6Access : public ModuleAccess<OSPFv3ProcessIPv6>
{
  public:
    OSPFv3ProcessIPv6Access() : ModuleAccess<OSPFv3ProcessIPv6>("OSPFv3ProcessIPv6") {}
};

#endif // __INET_OSPFV3PROCESSIPV6_H_
