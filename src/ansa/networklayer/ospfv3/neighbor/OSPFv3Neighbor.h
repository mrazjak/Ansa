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

#ifndef __INET_OSPFV3NEIGHBOR_H
#define __INET_OSPFV3NEIGHBOR_H

//#include <list>

//#include "LSA.h"
//#include "OSPFcommon.h"
#include "OSPFv3PacketIPv6_m.h"
#include "OSPFv3Timer_m.h"
#include "OSPFv3ProcessIPv6.h"
#include "OSPFv3Interface.h"
#include "OSPFv3NeighborState.h"
#include "OSPFv3Area.h"

//namespace OSPFv3 {

class OSPFv3ProcessIPv6;
class OSPFv3NeighborState;
class OSPFv3Area;
//class Interface;

class OSPFv3Neighbor {

//friend class NeighborState;

public:
    enum NeighborEventType {
        HELLO_RECEIVED               = 0,
        START                       = 1,
        TWOWAY_RECEIVED              = 2,
        NEGOTIATION_DONE             = 3,
        EXCHANGE_DONE                = 4,
        BAD_LINK_STATE_REQUEST         = 5,
        LOADING_DONE                 = 6,
        IS_ADJACENCY_OK               = 7,
        SEQUENCE_NUMBER_MISMATCH      = 8,
        ONEWAY_RECEIVED              = 9,
        KILL_NEIGHBOR                = 10,
        INACTIVITY_TIMER             = 11,
        POLL_TIMER                   = 12,
        LINK_DOWN                    = 13,
        DD_RETRANSMISSION_TIMER       = 14,
        UPDATE_RETRANSMISSION_TIMER   = 15,
        REQUEST_RETRANSMISSION_TIMER  = 16
    };

    enum NeighborStateType {
        STATE_DOWN          = 0,
        STATE_ATTEMPT       = 1,
        STATE_INIT          = 2,
        STATE_TWOWAY        = 4,
        STATE_EXCHANGE_START = 8,
        STATE_EXCHANGE      = 16,
        STATE_LOADING       = 32,
        STATE_FULL          = 64
    };

    enum DatabaseExchangeRelationshipType {
        MASTER = 0,
        SLAVE = 1
    };

/*    struct DDPacketID {
        OSPFDDOptions   ddOptions;
        OSPFOptions     options;
        unsigned long   sequenceNumber;
    };
*/
private:
/*    struct TransmittedLSA {
        LSAKeyType      lsaKey;
        unsigned short  age;
    };
*/
private:
    OSPFv3InterfaceIPv6*                parentInterface;
    OSPFv3Area*                         parentArea;

    OSPFv3NeighborState*                state;
//    NeighborState*                      previousState;
    IPv4Address                         neighborID;
    IPv6Address                         neighborAddress;
    unsigned char                       neighborPriority;
    IPv4Address                         neighborsDesignatedRouter;
    IPv4Address                         neighborsBackupDesignatedRouter;
    bool                                designatedRoutersSetUp;
    short                               neighborsRouterDeadInterval;
    OSPFv3Timer*                        inactivityTimer;
    OSPFv3Timer*                        pollTimer;
    OSPFv3Timer*                        ddRetransmissionTimer;
    OSPFv3Timer*                        updateRetransmissionTimer;
    bool                                updateRetransmissionTimerActive;
    OSPFv3Timer*                        requestRetransmissionTimer;
    bool                                requestRetransmissionTimerActive;
/*    DatabaseExchangeRelationshipType    databaseExchangeRelationship;
    bool                                firstAdjacencyInited;
    unsigned long                       ddSequenceNumber;
    DDPacketID                          lastReceivedDDPacket;
    IPv4Address                         neighborIPAddress;
    OSPFOptions                         neighborOptions;
    std::list<OSPFLSA*>                 linkStateRetransmissionList;
    std::list<OSPFLSAHeader*>           databaseSummaryList;
    std::list<OSPFLSAHeader*>           linkStateRequestList;
    std::list<TransmittedLSA>           transmittedLSAs;
    OSPFDatabaseDescriptionPacket*      lastTransmittedDDPacket;
*/


    // TODO: Should come from a global unique number generator module.
//    static unsigned long                ddSequenceNumberInitSeed;

private:
    //void changeState(NeighborState* newState, NeighborState* currentState);

public:
    OSPFv3Neighbor(OSPFv3Area *parentArea,OSPFv3InterfaceIPv6 *pIntf,
            IPv4Address neighbor = IPv4Address(0,0,0,0),//NULL_IPV4ADDRESS
            IPv6Address nAddress = IPv6Address("0::0"));//NULL_IPV6ADDRESS
    virtual ~OSPFv3Neighbor();

    void                processEvent(NeighborEventType event);
    void                reset();
//    void                initFirstAdjacency();
    NeighborStateType   getState() const;
    static const char*  getStateString(NeighborStateType stateType);
/*    void                sendDatabaseDescriptionPacket(bool init = false);
    bool                retransmitDatabaseDescriptionPacket();
    void                createDatabaseSummary();
    void                sendLinkStateRequestPacket();
    void                retransmitUpdatePacket();
    bool                needAdjacency();
    void                addToRetransmissionList(OSPFLSA* lsa);
    void                removeFromRetransmissionList(LSAKeyType lsaKey);
    bool                isLinkStateRequestListEmpty(LSAKeyType lsaKey) const;
    OSPFLSA*            findOnRetransmissionList(LSAKeyType lsaKey);
    void                startUpdateRetransmissionTimer();
    void                clearUpdateRetransmissionTimer();
    void                addToRequestList(OSPFLSAHeader* lsaHeader);
    void                removeFromRequestList(LSAKeyType lsaKey);
    bool                isLSAOnRequestList(LSAKeyType lsaKey) const;
    OSPFLSAHeader*      findOnRequestList(LSAKeyType lsaKey);
    void                startRequestRetransmissionTimer();
    void                clearRequestRetransmissionTimer();
    void                addToTransmittedLSAList(LSAKeyType lsaKey);
    bool                isOnTransmittedLSAList(LSAKeyType lsaKey) const;
    void                ageTransmittedLSAList();
    unsigned long       getUniqueULong();
    void                deleteLastSentDDPacket();
*/
    void                setNeighborID(IPv4Address id)  { neighborID = id; }
    IPv4Address         getNeighborID() const  { return neighborID; }
    void                setNeighborAddress(IPv6Address address)  { neighborAddress = address; }
    IPv6Address         getNeighborAddress() const  { return neighborAddress; }
    void                setPriority(unsigned char priority)  { neighborPriority = priority; }
    unsigned char       getPriority() const  { return neighborPriority; }
/*    void                setAddress(IPv4Address address)  { neighborIPAddress = address; }
    IPv4Address         getAddress() const  { return neighborIPAddress; }
*/    void                setDesignatedRouter(IPv4Address routerID)  { neighborsDesignatedRouter = routerID; }
    IPv4Address         getDesignatedRouter() const  { return neighborsDesignatedRouter; }
    void                setBackupDesignatedRouter(IPv4Address routerID)  { neighborsBackupDesignatedRouter = routerID; }
    IPv4Address         getBackupDesignatedRouter() const  { return neighborsBackupDesignatedRouter; }
    void                setRouterDeadInterval(short interval)  { neighborsRouterDeadInterval = interval; }
    short               getRouterDeadInterval() const  { return neighborsRouterDeadInterval; }
 /*   void                setDDSequenceNumber(unsigned long sequenceNumber)  { ddSequenceNumber = sequenceNumber; }
    unsigned long       getDDSequenceNumber() const  { return ddSequenceNumber; }
    void                setOptions(OSPFOptions options)  { neighborOptions = options; }
    OSPFOptions         getOptions() const  { return neighborOptions; }
    void                setLastReceivedDDPacket(DDPacketID packetID)  { lastReceivedDDPacket = packetID; }
    DDPacketID          getLastReceivedDDPacket() const  { return lastReceivedDDPacket; }

    void                                setDatabaseExchangeRelationship(DatabaseExchangeRelationshipType relation) { databaseExchangeRelationship = relation; }
    DatabaseExchangeRelationshipType    getDatabaseExchangeRelationship() const  { return databaseExchangeRelationship; }
*/
    void                          setInterface(OSPFv3InterfaceIPv6* intf)  { parentInterface = intf; }
    OSPFv3InterfaceIPv6*          getInterface()  { return parentInterface; }
    const OSPFv3InterfaceIPv6*    getInterface() const  { return parentInterface; }

    OSPFv3Timer*        getInactivityTimer()  { return inactivityTimer; }
    OSPFv3Timer*        getPollTimer()  { return pollTimer; }
    OSPFv3Timer*        getDDRetransmissionTimer()  { return ddRetransmissionTimer; }
    OSPFv3Timer*        getUpdateRetransmissionTimer()  { return updateRetransmissionTimer; }
    bool                isUpdateRetransmissionTimerActive() const  { return updateRetransmissionTimerActive; }
    bool                isRequestRetransmissionTimerActive() const  { return requestRetransmissionTimerActive; }
//    bool                isFirstAdjacencyInited() const  { return firstAdjacencyInited; }
    bool                designatedRoutersAreSetUp() const  { return designatedRoutersSetUp; }
    void                setUpDesignatedRouters(bool setUp)  { designatedRoutersSetUp = setUp; }
/*    unsigned long       getDatabaseSummaryListCount() const  { return databaseSummaryList.size(); }

    void incrementDDSequenceNumber()  { ddSequenceNumber++; }
    bool isLinkStateRequestListEmpty() const { return linkStateRequestList.empty(); }
    bool isLinkStateRetransmissionListEmpty() const { return linkStateRetransmissionList.empty(); }
    void popFirstLinkStateRequest()  { linkStateRequestList.pop_front(); }
*/};

//} // namespace OSPFv3

/*inline bool operator==(OSPF::Neighbor::DDPacketID leftID, OSPF::Neighbor::DDPacketID rightID)
{
    return ((leftID.ddOptions.I_Init == rightID.ddOptions.I_Init) &&
            (leftID.ddOptions.M_More == rightID.ddOptions.M_More) &&
            (leftID.ddOptions.MS_MasterSlave == rightID.ddOptions.MS_MasterSlave) &&
            (leftID.options == rightID.options) &&
            (leftID.sequenceNumber == rightID.sequenceNumber));
}*/

/*inline bool operator!=(OSPF::Neighbor::DDPacketID leftID, OSPF::Neighbor::DDPacketID rightID)
{
    return (!(leftID == rightID));
}*/

#endif // __INET_OSPFV3NEIGHBOR_H

