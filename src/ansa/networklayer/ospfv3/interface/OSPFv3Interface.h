//
// Copyright (C) 2006 Andras Babos and Andras Varga
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, see <http://www.gnu.org/licenses/>.
//

#ifndef __INET_OSPFV3INTERFACE_H
#define __INET_OSPFV3INTERFACE_H

#include <map>
#include <vector>
#include <list>

#include <omnetpp.h>
#include "INETDefs.h"

//#include "OSPFcommon.h"
//#include "OSPFNeighbor.h"
#include "OSPFv3Timer_m.h"
#include "InterfaceEntry.h"
#include "OSPFv3InterfaceState.h"
#include "OSPFv3Area.h"
#include "IPv4Address.h"
#include "OSPFv3ProcessIPv6.h"
#include "OSPFv3PacketIPv6_m.h"
#include "OSPFv3Neighbor.h"

//namespace OSPFv3 {

class OSPFv3InterfaceState;
class OSPFv3Area;
class OSPFv3Neighbor;

//const IPv4Address NULL_IPV4ADDRESS(0,0,0,0);

class OSPFv3InterfaceIPv6: public cObject
{
public:

    enum InterfaceStateType {
        STATE_DOWN                  = 0,
        STATE_LOOPBACK              = 1,
        STATE_WAITING               = 2,
        STATE_POINTTOPOINT          = 3,
        STATE_NOT_DESIGNATED_ROUTER = 4,
        STATE_BACKUP                = 5,
        STATE_DESIGNATED_ROUTER     = 6
    };
    enum OSPFv3InterfaceType {
        UNKNOWN_TYPE       = 0,
        POINTTOPOINT      = 1,
        BROADCAST         = 2,
        NBMA              = 3,
        POINTTOMULTIPOINT = 4,
        VIRTUAL           = 5
    };
    enum InterfaceEventType {
        INTERFACE_UP          = 0,
        HELLO_TIMER           = 1,
        WAIT_TIMER            = 2,
        ACKNOWLEDGEMENT_TIMER = 3,
        BACKUP_SEEN           = 4,
        NEIGHBOR_CHANGE       = 5,
        LOOP_INDICATION       = 6,
        UNLOOP_INDICATION     = 7,
        INTERFACE_DOWN        = 8
    };

  protected:
    int interfaceId;        /**< ID of interface*/
    char instanceId;
    OSPFv3Area *parentArea;             /**< Area*/
    short helloInt;
    short pollInt;
    short deadInt;
    short acknowledgementDelay;
//    int helloInt;           /**< Hello interval in seconds (<1-65535>) */
//    int deadInt;            /**< Router's Dead interval in seconds (<1-65535>) */
    int routerPriority;     /**< Router Priority (0-255)*/
    int hopLimit;           /**< Hop Limit typically 1 and different virtual line*/
    OSPFv3Timer *helloTimer;    /**< pointer to hello timer */
    OSPFv3Timer *waitTimer;    /**< pointer to wait timer */
    OSPFv3Timer *ackTimer;    /**< pointer to acknowlegement timer */
    bool enabled;           /**< true, if OSPFv3 is enabled on interface */
    OSPFv3InterfaceType     ifType;
    OSPFv3InterfaceState*   state;
    IPv4Address designatedRouter;
    IPv4Address backupDesignatedRouter;

    double bandwidth; /**< Bandwidth in Kbps (<1-10 000 000>) */
    double delay;     /**< Delay in us (<1-16 777 215>) */
    int reliability; /**< Reliability in percent (<1-255>) */
    int load;      /**< Load in percent (<1-255>) */

  public:
    OSPFv3InterfaceIPv6(InterfaceEntry* iface, OSPFv3Area *area, bool enabled);
    ~OSPFv3InterfaceIPv6();

    bool operator==(const OSPFv3InterfaceIPv6& iface) const
    {
        return interfaceId == iface.getInterfaceId();
    }

    void processEvent(InterfaceEventType event);
    void reset();

    /**< Set identifier of interface. */
    void setInterfaceId(int id) { interfaceId = id; }
    /**< Set pointer to hello timer. */
    void setHelloTimerPtr(OSPFv3Timer *timer) { this->helloTimer = timer; }
    void setHelloWaitPtr (OSPFv3Timer *timer) { this->waitTimer  = timer; }
    void setHelloAckPtr  (OSPFv3Timer *timer) { this->ackTimer   = timer; }
    void setHelloInt(short helloInt) { this->helloInt = helloInt; }
    void setDeadInt(short deadInt) { this->deadInt = deadInt; }
    void setPollInt(short interval)  { pollInt = interval; }
    void setAcknowledgementDelay(short delay)  { acknowledgementDelay = delay; }
    void setEnabling(bool enabled) { this->enabled = enabled; }
    void setInstanceId(int instId) { this->instanceId= instId; }
//    void setAreaId(int areaId) { this->areaId = areaId; }
    void setRouterPriority(int rp) { this->routerPriority = rp; }
    void setHopLimit(int hplmt) { this->hopLimit = hplmt; }
    void setType(OSPFv3InterfaceIPv6::OSPFv3InterfaceType type, int hopLim = 1);

    void setBackupDesignatedRouter(IPv4Address rID) {backupDesignatedRouter = rID;}
    void setDesignatedRouter(IPv4Address rID) {designatedRouter = rID;}

    void setBandwidth(int bw) { this->bandwidth = bw; }
    void setDelay(int dly) { this->delay = dly; }
    void setReliability(int rel) { this->reliability = rel; }
    void setLoad(int load) { this->load = load; }


    /**< Return identifier of interface. */
    int getInterfaceId() const { return interfaceId; }
    /**< Return pointer to hello timer. */
    OSPFv3Timer *getHelloTimer() const { return this->helloTimer; }
    OSPFv3Timer *getWaitTimer() const { return this->waitTimer; }
    OSPFv3Timer *getAckTimer() const { return this->ackTimer; }
    short getHelloInt() const { return this->helloInt; }
    short getDeadInt() const { return this->deadInt; }
    short getPollInt() const  { return pollInt; }
    short getAcknowledgementDelay() const  { return acknowledgementDelay; }
    bool isEnabled() const { return this->enabled; }
    int getInstanceId() const { return this->instanceId; }
    OSPFv3Area *getArea() const { return parentArea; }
    IPv4Address getAreaId() const;// { return this->parentArea->getAreaID(); }
    int getRouterPriority() const {return this->routerPriority; }
    int getHopLimit() const { return this->hopLimit; }
    OSPFv3InterfaceType getType() const{return this->ifType; }
    InterfaceStateType  getState() const;
    static const char*  getStateString(InterfaceStateType stateType);

    IPv4Address getDesignatedRouter() const  { return designatedRouter; }
    IPv4Address getBackupDesignatedRouter() const  { return backupDesignatedRouter; }

    void sendHelloPacket(IPv6Address destination, short ttl = 1);

    unsigned int getBandwidth() const { return bandwidth; }
    unsigned int getDelay() const { return delay; }
    unsigned int getReliability() const { return reliability; }
    unsigned int getLoad() const { return load; }
  private:
      friend class InterfaceState;
      OSPFv3Timer *createTimer(char timerKind);
      OSPFv3Packet *addOSPFv3Header(OSPFv3Packet *pkt, char type, IPv4Address routerID, IPv4Address areaID, char instanceID);
};
/*

private:
    unsigned short                                                      mtu;
    IPv4AddressRange                                                    interfaceAddressRange;
    AreaID                                                              areaID;
    AreaID                                                              transitAreaID;
//    short                                                               helloInterval;
//    short                                                               pollInterval;
//    short                                                               routerDeadInterval;
    short                                                               interfaceTransmissionDelay;
    unsigned char                                                       routerPriority;
    cMessage*                                                           helloTimer;
    cMessage*                                                           waitTimer;
    cMessage*                                                           acknowledgementTimer;
    std::map<RouterID, Neighbor*>                                       neighboringRoutersByID;
    std::map<IPv4Address, Neighbor*>                                    neighboringRoutersByAddress;
    std::vector<Neighbor*>                                              neighboringRouters;
    std::map<IPv4Address, std::list<OSPFLSAHeader> >                    delayedAcknowledgements;
//    DesignatedRouterID                                                  designatedRouter;
//    DesignatedRouterID                                                  backupDesignatedRouter;
    Metric                                                              interfaceOutputCost;
    short                                                               retransmissionInterval;
//    short                                                               acknowledgementDelay;
    AuthenticationType                                                  authenticationType;
    AuthenticationKeyType                                               authenticationKey;

    Area*                                                               parentArea;
private:
//    friend class InterfaceState;
//    void changeState(InterfaceState* newState, InterfaceState* currentState);

public:
    Interface(OSPFInterfaceType ifType = UNKNOWN_TYPE);
    virtual ~Interface();

//    void                processEvent(InterfaceEventType event);
//    void                reset();
//    void                sendHelloPacket(IPv4Address destination, short ttl = 1);
    void                sendLSAcknowledgement(OSPFLSAHeader* lsaHeader, IPv4Address destination);
    Neighbor*           getNeighborByID(RouterID neighborID);
    Neighbor*           getNeighborByAddress(IPv4Address address);
    void                addNeighbor(Neighbor* neighbor);
//    InterfaceStateType  getState() const;
//    static const char*  getStateString(InterfaceStateType stateType);
    bool                hasAnyNeighborInStates(int states) const;
    void                removeFromAllRetransmissionLists(LSAKeyType lsaKey);
    bool                isOnAnyRetransmissionList(LSAKeyType lsaKey) const;
    bool                floodLSA(OSPFLSA* lsa, Interface* intf = NULL, Neighbor* neighbor = NULL);
    void                addDelayedAcknowledgement(OSPFLSAHeader& lsaHeader);
    void                sendDelayedAcknowledgements();
    void                ageTransmittedLSALists();

    OSPFLinkStateUpdatePacket* createUpdatePacket(OSPFLSA* lsa);

    void                    setType(OSPFInterfaceType ifType)  { interfaceType = ifType; }
    OSPFInterfaceType       getType() const  { return interfaceType; }
    void                    setIfIndex(int index);
    int                     getIfIndex() const  { return ifIndex; }
    void                    setMTU(unsigned short ifMTU)  { mtu = ifMTU; }
    unsigned short          getMTU() const  { return mtu; }
    void                    setAreaID(AreaID areaId)  { areaID = areaId; }
    AreaID                  getAreaID() const  { return areaID; }
    void                    setTransitAreaID(AreaID areaId)  { transitAreaID = areaId; }
    AreaID                  getTransitAreaID() const  { return transitAreaID; }
    void                    setOutputCost(Metric cost)  { interfaceOutputCost = cost; }
    Metric                  getOutputCost() const  { return interfaceOutputCost; }
    void                    setRetransmissionInterval(short interval)  { retransmissionInterval = interval; }
    short                   getRetransmissionInterval() const  { return retransmissionInterval; }
    void                    setTransmissionDelay(short delay)  { interfaceTransmissionDelay = delay; }
    short                   getTransmissionDelay() const  { return interfaceTransmissionDelay; }
    void                    setAcknowledgementDelay(short delay)  { acknowledgementDelay = delay; }
    short                   getAcknowledgementDelay() const  { return acknowledgementDelay; }
    void                    setRouterPriority(unsigned char priority)  { routerPriority = priority; }
    unsigned char           getRouterPriority() const  { return routerPriority; }
    void                    setHelloInterval(short interval)  { helloInterval = interval; }
    short                   getHelloInterval() const  { return helloInterval; }
    void                    setPollInterval(short interval)  { pollInterval = interval; }
    short                   getPollInterval() const  { return pollInterval; }
    void                    setRouterDeadInterval(short interval)  { routerDeadInterval = interval; }
    short                   getRouterDeadInterval() const  { return routerDeadInterval; }
    void                    setAuthenticationType(AuthenticationType type)  { authenticationType = type; }
    AuthenticationType      getAuthenticationType() const  { return authenticationType; }
    void                    setAuthenticationKey(AuthenticationKeyType key) { authenticationKey = key; }
    AuthenticationKeyType   getAuthenticationKey() const  { return authenticationKey; }
    void                    setAddressRange(IPv4AddressRange range)  { interfaceAddressRange = range; }
    IPv4AddressRange        getAddressRange() const  { return interfaceAddressRange; }

    cMessage*               getHelloTimer()  { return helloTimer; }
    cMessage*               getWaitTimer()  { return waitTimer; }
    cMessage*               getAcknowledgementTimer()  { return acknowledgementTimer; }
//    DesignatedRouterID      getDesignatedRouter() const  { return designatedRouter; }
//    DesignatedRouterID      getBackupDesignatedRouter() const  { return backupDesignatedRouter; }
    unsigned long           getNeighborCount() const  { return neighboringRouters.size(); }
    Neighbor*               getNeighbor(unsigned long i)  { return neighboringRouters[i]; }
    const Neighbor*         getNeighbor(unsigned long i) const  { return neighboringRouters[i]; }

    void                    setArea(Area* area)  { parentArea = area; }
    Area*                   getArea()  { return parentArea; }
    const Area*             getArea() const  { return parentArea; }
};

*/
//} // namespace OSPFv3

#endif // __INET_OSPFV3INTERFACE_H

