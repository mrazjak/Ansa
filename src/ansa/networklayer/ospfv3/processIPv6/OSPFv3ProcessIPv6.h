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
#include "IPv6InterfaceData.h"
#include "ANSARoutingTable6.h"
#include "ANSARoutingTable6Access.h"
#include "NotificationBoard.h"

#include "IOSPFv3Module.h"
#include "OSPFv3Interface.h"
#include "OSPFv3InterfaceState.h"
#include "OSPFv3InterfaceTableIPv6.h"
#include "OSPFv3NeighborTableIPv6.h"
#include "OSPFv3TopologyTableIPv6.h"
//#include "OSPFv3Area.h"
#include "OSPFv3AreaTableIPv6.h"

#include "OSPFv3Timer_m.h"
#include "OSPFv3PacketIPv6_m.h"

//#include "IOSPFv3MessageHandler.h"
#include "OSPFv3MessageHandler.h"

//namespace OSPFv3 {

const IPv4Address NULL_IPV4ADDRESS(0,0,0,0);
const IPv6Address NULL_IPV6ADDRESS("0::0");

//class IOSPFv3MessageHandler;
class OSPFv3MessageHandler;
class OSPFv3Area;
//class OSPFv3InterfaceIPv6;
class OSPFv3AreaTableIPv6;
class OSPFv3InterfaceTableIPv6;
class OSPFv3NeighborTableIPv6;
//class InterfaceState;

class OSPFv3ProcessIPv6 : public cSimpleModule, public IOSPFv3Module, public INotifiable
{
  public:
    const char* OSPFV3_OUT_GW;  /**< Output gateway to the OSPFv3 module */
    const IPv6Address OSPFV3_IPV6_MULT;    /**< Multicast address for OSPFv3 messages */
    const IPv6Address OSPFV3_IPV6_MULT_DR; /**< Multicast address for OSPFv3 messages for all DR router*/
  protected:
//    typedef std::vector<ANSAIPv4Route *> RouteVector;
    int processID;              /**< Autonomous system number */
    IPv4Address routerID;       /**< OSPF Router ID*/


    IInterfaceTable   *ift;
    ANSARoutingTable6 *rt6;
    NotificationBoard *nb;

    OSPFv3InterfaceTableIPv6 *OSPFv3Ift;               /**< Table with enabled OSPFv3 interfaces */
    OSPFv3NeighborTableIPv6  *OSPFv3Nt;                /**< Table with OSPFv3 neighbors */
    OSPFv3TopologyTableIPv6  *OSPFv3Tt;                /**< Topology table */
    OSPFv3AreaTableIPv6      *OSPFv3At;                /**< Area table */

    OSPFv3MessageHandler     *OSPFv3MsgHandl;

    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);
    /**< Multi-stage initialization. */
    virtual int numInitStages() const { return 5; }

    OSPFv3InterfaceIPv6 *addInterfaceToOSPFv3(OSPFv3InterfaceIPv6 *intf);
    OSPFv3Area *addArea2(OSPFv3Area *area);
    OSPFv3Neighbor *addNeighbor2(OSPFv3Neighbor *neig);
    void enableInterface(InterfaceEntry *iface, OSPFv3InterfaceIPv6 *OSPFv3Iface);
    void joinMulticastGroups(int interfaceId);

  public:
    OSPFv3ProcessIPv6();
    ~OSPFv3ProcessIPv6();

    virtual void receiveChangeNotification(int category, const cObject *details);

    OSPFv3MessageHandler*          getMessageHandler()  { return OSPFv3MsgHandl; }

    // Interface IOSPFv3Module
    void addInterface(OSPFv3InterfaceIPv6 *intf) { addInterfaceToOSPFv3(intf); }
    void addArea(OSPFv3Area *area) { addArea2(area); }
    void addNeighbor(OSPFv3Neighbor *neig) { addNeighbor2(neig); }
    void setProcessID(int prcssID) { this->processID = prcssID; }
    void setRouterID(IPv4Address routerID) { this->routerID = routerID; }

    void sendOSPFv3Packet(OSPFv3Packet *pkt){ send(pkt,OSPFV3_OUT_GW); }

    int getProcessID() const { return this->processID; }
    IPv4Address             getRouterID() const { return this->routerID; }
    OSPFv3Area*             getOSPFv3AreaById (IPv4Address AreaID) const;
    OSPFv3InterfaceIPv6*    getOSPFv3InterfaceById(int intfID) const;
    InterfaceEntry*         getInterfaceById(int intfID) const;
    OSPFv3Neighbor*         getNeighborByIntfIdAndNeigId(IPv4Address neigId,int intfID) const;
    OSPFv3Neighbor*         getNeighborByIntfIdAndNeigAddress(IPv6Address neigAdd,int intfID) const;
    int                     getNeighborsByIntfId(int intfID, std::vector<OSPFv3Neighbor*>* neighbors) ;
};
//} // namespace OSPFv3
/**
 * @brief Class gives access to the OSPFv3ProcessIPv6.
 */
class INET_API OSPFv3ProcessIPv6Access : public ModuleAccess<OSPFv3ProcessIPv6>
{
  public:
    OSPFv3ProcessIPv6Access() : ModuleAccess<OSPFv3ProcessIPv6>("OSPFv3ProcessIPv6") {}
};

#endif // __INET_OSPFV3PROCESSIPV6_H_
