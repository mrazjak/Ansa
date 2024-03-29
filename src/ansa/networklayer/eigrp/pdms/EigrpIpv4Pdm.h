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

#ifndef __INET_EIGRPIPV4PDM_H_
#define __INET_EIGRPIPV4PDM_H_

#include <omnetpp.h>

#include "InterfaceTable.h"
#include "InterfaceEntry.h"
#include "IPv4Address.h"
#include "AnsaRoutingTable.h"
#include "NotificationBoard.h"

#include "EigrpInterfaceTable.h"
#include "EigrpNeighborTable.h"
#include "EigrpIpv4NeighborTable.h"
#include "EigrpRoute.h"
#include "EigrpIpv4TopologyTable.h"

#include "IEigrpModule.h"
#include "IEigrpPdm.h"

#include "EigrpMessage_m.h"
#include "EigrpDisabledInterfaces.h"
#include "EigrpDual.h"
#include "EigrpNetworkTable.h"
#include "EigrpMetricHelper.h"
#include "EigrpMsgReq.h"

/**
 * TODO - Generated class
 */
class EigrpIpv4Pdm : public cSimpleModule, public IEigrpModule, public IEigrpPdm, public INotifiable
{
  protected:
    // TODO predelat na EigrpMsgReq a lokalni frontu predavanou do DUALu
//    struct EigrpMsgRequest
//    { // Used by DUAL to send messages
//        HeaderOpcode type;
//        int destNeighbor;                       /**< ID of neighbor that is destination of message */
//        int destInterface;                      /**< ID of destination interface */
//        int sourceId;                           /**< ID of source for route */
//        bool unreachable;                       /**< Route with maximal metric */
//        bool invalid;                           /**< Route has been sent */
//
//        EigrpMsgRequest() : destNeighbor(0), destInterface(0), invalid(false) {}
//    };
    typedef std::vector<ANSAIPv4Route *> RouteVector;
    typedef std::vector<EigrpMsgReq *> RequestVector;

    const char* SPLITTER_OUTGW;  /**< Output gateway to the RTP module */
    const char* RTP_OUTGW;  /**< Output gateway to the RTP module */
    const IPv4Address EIGRP_IPV4_MULT; /**< Multicast address for EIGRP messages */
    EigrpKValues KVALUES_MAX;    /**< K-values (from K1 to K5) are set to max */
    const IPv4Address EIGRP_SELF_ADDR;  /**< Next hop address 0.0.0.0 (self address) */

    int asNum;                  /**< Autonomous system number */
    EigrpKValues kValues;       /**< K-values for calculation of metric */
    int maximumPath;            /**< Maximum number of parallel routes that EIGRP will support */
    int variance;               /**< Parameter for unequal cost load balancing */
    bool stub;                  /**< Router is a stub */
    unsigned int adminDistInt; /**< Administrative distance */
    bool useClassicMetric;      /**< Use classic metric computation or wide metric computation */
    int ribScale;               /**< */

    IInterfaceTable *ift;
    AnsaRoutingTable *rt;
    NotificationBoard *nb;

    EigrpDual *eigrpDual;
    EigrpMetricHelper *eigrpMetric;
    EigrpInterfaceTable *eigrpIft;                   /**< Table with enabled EIGRP interfaces */
    EigrpDisabledInterfaces *eigrpIftDisabled;       /**< Disabled EIGRP interfaces */
    EigrpIpv4NeighborTable *eigrpNt;                /**< Table with EIGRP neighbors */
    EigrpIpv4TopologyTable *eigrpTt;                /**< Topology table */
    EigrpNetworkTable *routingForNetworks;          /**< Networks included in EIGRP */
    RequestVector reqQueue;                         /**< Requests for sending EIGRP messages from DUAL */

    virtual void initialize(int stage);
    virtual void handleMessage(cMessage *msg);
    /**< Multi-stage initialization. */
    virtual int numInitStages() const { return 4; }
    virtual void receiveChangeNotification(int category, const cObject *details);

    void printSentMsg(int routeCnt, IPv4Address& destAddress, EigrpMsgReq *msgReq);
    void printRecvMsg(EigrpMessage *msg, IPv4Address& addr, int ifaceId);

    EigrpTimer *createTimer(char timerKind, void *context);
    void resetTimer(EigrpTimer *timer, int interval);

    EigrpIpv4Hello *createHelloMsg(int holdInt, EigrpKValues kValues, IPv4Address& destAddress, EigrpMsgReq *msgReq);
    EigrpIpv4Ack *createAckMsg(IPv4Address& destAddress, EigrpMsgReq *msgReq);
    EigrpIpv4Update *createUpdateMsg(IPv4Address& destAddress, EigrpMsgReq *msgReq);
    EigrpIpv4Query *createQueryMsg(IPv4Address& destAddress, EigrpMsgReq *msgReq);
    EigrpIpv4Reply *createReplyMsg(IPv4Address& destAddress, EigrpMsgReq *msgReq);
    void addCtrInfo(EigrpMessage *msg, int ifaceId, const IPv4Address &destAddress);
    void addMessageHeader(EigrpMessage *msg, int opcode, EigrpMsgReq *msgReq);
    void createRouteTlv(EigrpMpIpv4Internal *routeTlv, EigrpRoute<IPv4Address> *route, bool unreachable = false);
    void addRoutesToMsg(EigrpIpv4Message *msg, const EigrpMsgReq *msgReq);
    void setRouteTlvMetric(EigrpWideMetricPar *msgMetric, EigrpWideMetricPar *rtMetric);
    EigrpMsgReq *createMsgReq(HeaderOpcode msgType, int destNeighbor, int destIface);
    void sendAllEigrpPaths(EigrpInterface *eigrpIface, EigrpNeighbor<IPv4Address> *neigh);

    void processTimer(cMessage *msg);
    void processMsgFromNetwork(cMessage *msg);
    void processMsgFromRtp(cMessage *msg);
    void processAckMsg(cMessage *msg, IPv4Address& srcAddress, int ifaceId, EigrpNeighbor<IPv4Address> *neigh);
    void processHelloMsg(cMessage *msg, IPv4Address& srcAddress, int ifaceId, EigrpNeighbor<IPv4Address> *neigh);
    void processUpdateMsg(cMessage *msg, IPv4Address& srcAddress, int ifaceId, EigrpNeighbor<IPv4Address> *neigh);
    void processQueryMsg(cMessage *msg, IPv4Address& srcAddress, int ifaceId, EigrpNeighbor<IPv4Address> *neigh);
    void processReplyMsg(cMessage *msg, IPv4Address& srcAddress, int ifaceId, EigrpNeighbor<IPv4Address> *neigh);
    EigrpRouteSource<IPv4Address> *processInterRoute(EigrpMpIpv4Internal& tlv, IPv4Address& nextHop, int sourceNeighId, EigrpInterface *eigrpIface, bool *notifyDual, bool *isSourceNew);

    void processNewNeighbor(int ifaceId, IPv4Address &srcAddress, EigrpHello *helloMessage);
    int checkNeighborshipRules(int ifaceId, int neighAsNum, IPv4Address &neighAddr,
            const EigrpKValues &neighKValues);
    EigrpNeighbor<IPv4Address> *createNeighbor(EigrpInterface *eigrpIface, IPv4Address& address, uint16_t holdInt);
    void removeNeighbor(EigrpNeighbor<IPv4Address> *neigh);

    void disableInterface(InterfaceEntry *iface, EigrpInterface *eigrpIface, IPv4Address& ifAddress, IPv4Address& ifMask);
    void enableInterface(EigrpInterface *eigrpIface, IPv4Address& ifAddress, IPv4Address& ifMask, int networkId);
    EigrpInterface *getInterfaceById(int ifaceId);
    EigrpInterface *addInterfaceToEigrp(int ifaceId, int networkId, bool enabled);
    void processIfaceStateChange(InterfaceEntry *iface);
    void processIfaceConfigChange(EigrpInterface *eigrpIface);

    /**< Returns next hop address. If next hop in message is 0.0.0.0, then next hop must be replaced by source IP of sender */
    IPv4Address getNextHopAddr(IPv4Address& nextHopAddr, IPv4Address& senderAddr)
        { return (nextHopAddr.isUnspecified()) ? senderAddr : nextHopAddr; }
    bool getDestIpAddress(int destNeigh, IPv4Address *resultAddress);

    ANSAIPv4Route *createRTRoute(EigrpRouteSource<IPv4Address> *successor);
    //bool createOrUpdateRouteInRT(EigrpRoute<IPv4Address> *route, EigrpRouteSource<IPv4Address> *source, bool *checkRTSafe, bool *isRTSafe);
    bool installRouteToRT(EigrpRoute<IPv4Address> *route, EigrpRouteSource<IPv4Address> *source, uint64_t dmin, IPv4Route *rtEntry);
    bool isRTSafeForAdd(EigrpRoute<IPv4Address> *route, unsigned int eigrpAd);
    void setRTRouteMetric(IPv4Route *route, uint64_t metric) { if (!useClassicMetric) { metric = metric / ribScale; } route->setMetric(metric); }

    void msgToAllIfaces(HeaderOpcode msgType, EigrpRouteSource<IPv4Address> *source, bool updateFromS);
    void msgToIface(HeaderOpcode msgType, EigrpRouteSource<IPv4Address> *source, EigrpInterface *eigrpIface,  bool forcePoisonRev = false);
    /** Sends all messages in transimt queue */
    void flushMsgRequests();
    EigrpMsgReq *pushMsgRouteToQueue(HeaderOpcode msgType, int ifaceId, int neighId, const EigrpMsgRoute& msgRt);
    //bool getRoutesFromRequests(RequestVector *msgReqs);

    bool applySplitHorizon(EigrpInterface *destInterface, EigrpRouteSource<IPv4Address> *source, EigrpRoute<IPv4Address> *route);

  public:
    EigrpIpv4Pdm();
    ~EigrpIpv4Pdm();

    // Interface IEigrpModule
    void addInterface(int ifaceId, int networkId, bool enabled) { addInterfaceToEigrp(ifaceId, networkId, enabled); }
    EigrpNetwork *addNetwork(IPv4Address address, IPv4Address mask);
    void setASNum(int asNum) { this->asNum = asNum; }
    void setKValues(const EigrpKValues& kValues) { this->kValues = kValues; }
    void setMaximumPath(int maximumPath) { this->maximumPath = maximumPath; }
    void setVariance(int variance) { this->variance = variance; }
    void setHelloInt(int interval, int ifaceId);
    void setHoldInt(int interval, int ifaceId);
    void setSplitHorizon(bool shenabled, int ifaceId);
    void setPassive(bool passive, int ifaceId);
    //void setStub(bool stub);

    // Interface IEigrpPdm;
    void sendUpdate(int destNeighbor, EigrpRoute<IPv4Address> *route, EigrpRouteSource<IPv4Address> *source, const char *reason);
    void sendQuery(int destNeighbor, EigrpRoute<IPv4Address> *route, EigrpRouteSource<IPv4Address> *source, bool updateFromS = false);
    void sendReply(EigrpRoute<IPv4Address> *route, int destNeighbor, EigrpRouteSource<IPv4Address> *source, bool forcePoisonRev = false, bool isUnreachable = false);
    bool removeRouteFromRT(EigrpRouteSource<IPv4Address> *successor);
    EigrpRouteSource<IPv4Address> *updateRoute(EigrpRoute<IPv4Address> *route, uint64_t dmin, bool *rtableChanged, bool removeUnreach = false);
    //void removeSourceFromTT(EigrpRouteSource<IPv4Address> *source);
    void addSourceToTT(EigrpRouteSource<IPv4Address> *source) { eigrpTt->addRoute(source); }
    uint64_t findRouteDMin(EigrpRoute<IPv4Address> *route) { return eigrpTt->findRouteDMin(route); }
    bool hasFeasibleSuccessor(EigrpRoute<IPv4Address> *route, uint64_t& resultDmin) { return eigrpTt->hasFeasibleSuccessor(route, resultDmin); }
    EigrpRouteSource<IPv4Address> *getFirstSuccessor(EigrpRoute<IPv4Address> *route) { return eigrpTt->getFirstSuccessor(route); }
    int setReplyStatusTable(EigrpRoute<IPv4Address> *route, EigrpRouteSource<IPv4Address> *source, bool updateFromS = false);
    bool hasNeighborForUpdate(EigrpRouteSource<IPv4Address> *source);
    void setDelayedRemove(int neighId, EigrpRouteSource<IPv4Address> *src);
};

/**
 * @brief Class gives access to the PimNeighborTable.
 */
class INET_API EigrpIpv4PdmAccess : public ModuleAccess<EigrpIpv4Pdm>
{
  public:
    EigrpIpv4PdmAccess() : ModuleAccess<EigrpIpv4Pdm>("EigrpIpv4Pdm") {}
};

#endif
