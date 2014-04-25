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

#include "IPv4ControlInfo.h"
#include "deviceConfigurator.h"
#include "AnsaIPv4Route.h"

#include "OSPFv3ProcessIPv6.h"


#define DEBUG

Define_Module(OSPFv3ProcessIPv6);

namespace osfpfv3
{

// User message codes
enum UserMsgCodes
{
  M_OK = 0,             // no message
  M_DISABLED_ON_IF,     // EIGRP is disabled on interface
  M_NEIGH_BAD_AS,       // neighbor has bad AS number
  M_NEIGH_BAD_KVALUES,  // neighbor has bad K-values
  M_NEIGH_BAD_SUBNET,   // neighbor isn't on common subnet
  M_HELLO_SEND,         // send Hello message
  M_UPDATE_SEND,        // send Update message
  M_QUERY_SEND,        // send Query message
  M_REPLY_SEND,        // send Query message
};

// User messages
const char *UserMsgs[] =
{
  // M_OK
  "OK",
  // M_DISABLED_ON_IF
  "EIGRP process isn't enabled on interface",
  // M_NEIGH_BAD_AS
  "AS number is different",
  // M_NEIGH_BAD_KVALUES
  "K-value mismatch",
  // M_NEIGH_BAD_SUBNET
  "not on the common subnet",
  // M_HELLO_SEND
  "send Hello message",
  // M_UPDATE_SEND
  "send Update message",
  // M_QUERY_SEND
  "send Query message",
  // M_REPLY_SEND
  "send Reply message",
};

}; // end of namespace ospfv3

/**
 * Output operator for IPv6Network class.
 */


OSPFv3ProcessIPv6::OSPFv3ProcessIPv6() : OSPFV3_IPV6_MULT(IPv6Address("fe02::5")),OSPFV3_IPV6_MULT_DR(IPv6Address("fe02::6"))
{
    OSPFV3_OUT_GW = "IPv6Out";
    routerID = IPv4Address("0.0.0.0");
    processID = 1;

}

OSPFv3ProcessIPv6::~OSPFv3ProcessIPv6()
{
    //delete this->linkTable;
    //delete this->eigrpIftDisabled;
    //delete this->eigrpDual;
}

void OSPFv3ProcessIPv6::initialize(int stage)
{
    // in stage 0 interfaces are registrated
    // in stage 2 interfaces and routing table
    // in stage 3
    if (stage == 3)
    {
        this->OSPFv3Ift = ModuleAccess<OSPFv3InterfaceTableIPv6>("OSPFv3InterfaceTableIPv6").get();
        this->OSPFv3Nt  = ModuleAccess<OSPFv3NeighborTableIPv6>("OSPFv3NeighborTableIPv6").get();
        this->OSPFv3Tt  = ModuleAccess<OSPFv3TopologyTableIPv6>("OSPFv3TopologyTableIPv6").get();

//        this->rt6 = ModuleAccess<ANSARoutingTable6>("routingTable6").get();
        this->rt6 = ANSARoutingTable6Access().get();
        this->ift = InterfaceTableAccess().get();
        this->nb = NotificationBoardAccess().get();

        // subscribe for changes in the device
        nb->subscribe(this, NF_INTERFACE_STATE_CHANGED);
        nb->subscribe(this, NF_INTERFACE_CONFIG_CHANGED);
        nb->subscribe(this, NF_IPv4_ROUTE_DELETED);
        // TODO in future NF_INTERFACE_STATE_CHANGED for BW and DLY change

//        this->eigrpIftDisabled = new EigrpDisabledInterfaces();
     //   this->linkTable = new OSPFv3LinkTableIPv6();
//        this->eigrpDual = new EigrpDual(this);
//        this->eigrpMetric = new EigrpMetricHelper();

        // load configuration
        DeviceConfigurator *conf = ModuleAccess<DeviceConfigurator>("deviceConfigurator").get();
        conf->loadOSPFv3ConfigIPv6(this);
    }
}

void OSPFv3ProcessIPv6::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage())
    { // Timer
        this->processTimer(msg);
    }
    else
    { // OSPFv3 message

        // Get source IP address and interface ID
        IPv6ControlInfo *ctrlInfo =check_and_cast<IPv6ControlInfo *>(msg->getControlInfo());
        IPv6Address srcAddr = ctrlInfo->getSrcAddr();
        int ifaceId = ctrlInfo->getInterfaceId();

 //       EV<< endl<< endl<< endl<<"OSPFv3 příchozí paket Hurá!"<< endl<< endl<< endl;

        // Find neighbor if exists
 /*       EigrpNeighbor<IPv4Address> *neigh;
        if ((neigh = eigrpNt->findNeighbor(srcAddr)) != NULL)
        { // Reset hold timer
            resetTimer(neigh->getHoldTimer(), neigh->getHoldInt());
        }

        if (dynamic_cast<EigrpHello*>(msg))
        {
            EV << "Received Hello message from " << srcAddr << endl;
            processHelloMsg(msg, srcAddr, ifaceId, neigh);
        }
        else if (dynamic_cast<EigrpUpdate*>(msg) && neigh != NULL)
        {
            EV << "Received Update message from " << srcAddr << endl;
            processUpdateMsg(msg, srcAddr, ifaceId, neigh);
        }
        else if (dynamic_cast<EigrpQuery*>(msg) && neigh != NULL)
        {
            EV << "Received Query message from " << srcAddr << endl;
            processQueryMsg(msg, srcAddr, ifaceId, neigh);
        }
        else if (dynamic_cast<EigrpReply*>(msg) && neigh != NULL)
        {
            EV << "Received Reply message from " << srcAddr << endl;
            processReplyMsg(msg, srcAddr, ifaceId, neigh);
        }
        else if (neigh != NULL)
        {
            EV << "Received message of unknown type, skipped" << endl;
        }
        else
        {
            EV << "Received message from "<< srcAddr <<" that is not neighbor" << endl;
        }
*/
        delete msg;
        msg = NULL;
    }
}

void OSPFv3ProcessIPv6::receiveChangeNotification(int category, const cObject *details)
{
    // ignore notifications during initialization
    if (simulation.getContextType() == CTX_INITIALIZE)
        return;

    Enter_Method_Silent();
    printNotificationBanner(category, details);

    if (category == NF_INTERFACE_STATE_CHANGED)
    {
//        InterfaceEntry *iface = check_and_cast<InterfaceEntry*>(details);
//        processIfaceStateChange(iface);
    }
    else if (category == NF_INTERFACE_CONFIG_CHANGED)
    {
/*        InterfaceEntry *iface = check_and_cast<InterfaceEntry*>(details);
//        EigrpInterface *eigrpIface = getInterfaceById(iface->getInterfaceId());
        double ifParam;

        if (eigrpIface == NULL)
            return;

        ifParam = iface->getBandwidth();
        if (ifParam != eigrpIface->getBandwidth())
        { // Bandwidth
            eigrpIface->setBandwidth(ifParam);
            if (eigrpIface->isEnabled())
                processIfaceConfigChange(iface, eigrpIface);
        }
        ifParam = iface->getDelay();
        if (ifParam != eigrpIface->getDelay())
        { // Delay
            eigrpIface->setDelay(ifParam);
            if (eigrpIface->isEnabled())
                processIfaceConfigChange(iface, eigrpIface);
        }
*/    }
}

void OSPFv3ProcessIPv6::processTimer(cMessage *msg)
{
    OSPFv3Timer *timer = check_and_cast<OSPFv3Timer*>(msg);
    char timerKind = timer->getTimerKind();

    OSPFv3InterfaceIPv6 *OSPFv3Iface;
    InterfaceEntry *iface;
    //IPv4Address ifMask;
    //OSPFv3NeighborIPv6<IPv6Address> *neigh;
    cObject *contextBasePtr;

    int ifaceId;

    switch (timerKind)
    {
    case OSPFV3_HELLO_TIMER:
        // get interface that belongs to timer
        contextBasePtr = (cObject*)timer->getContextPointer();
        OSPFv3Iface = check_and_cast<OSPFv3InterfaceIPv6 *>(contextBasePtr);

        // schedule Hello timer
        scheduleAt(simTime() + (OSPFv3Iface->getHelloInt()/* - uniform(0, 0.4)*/), timer);

        // send Hello message
        sendHelloPacket(OSPFv3Iface->getInterfaceId());
        break;

/*    case EIGRP_HOLD_TIMER:
        // get neighbor from context
        contextBasePtr = (cObject*)timer->getContextPointer();
        neigh = check_and_cast<EigrpNeighbor<IPv4Address> *>(contextBasePtr);
        ifaceId = neigh->getIfaceId();

        // remove neighbor
        EV << "Neighbor " << neigh->getIPAddress() <<" is down, holding time expired" << endl;
        iface = ift->getInterfaceById(ifaceId);
        ifMask = iface->ipv4Data()->getNetmask();
        removeNeighbor(neigh, ifMask);
        neigh = NULL;

        // Send goodbye and restart Hello timer
        eigrpIface = eigrpIft->findInterfaceById(ifaceId);
        resetTimer(eigrpIface->getHelloTimer(), eigrpIface->getHelloInt());
        sendGoodbyeMsg(ifaceId, eigrpIface->getHoldInt());
        break;
*/
    default:
        EV << "Timer with unknown kind was skipped" << endl;
        delete timer;
        timer = NULL;
        break;
    }
}

void OSPFv3ProcessIPv6::sendHelloPacket(int ifaceId)
{
    OSPFv3Packet *pkt = createHelloPacket(ifaceId);
    if (pkt!=NULL){
        send(pkt, OSPFV3_OUT_GW);
        EV << /*eigrp::UserMsgs[eigrp::M_HELLO_SEND] << */" to interface " << ifaceId << endl;
    }
}


OSPFv3HelloPacket *OSPFv3ProcessIPv6::createHelloPacket(int ifaceId)
{
    OSPFv3InterfaceIPv6 *ife= OSPFv3Ift->getInterfaceById(ifaceId);
    OSPFv3HelloPacket *pkt = new OSPFv3HelloPacket("OSPFv3_Hello");
    if (ife!=NULL){
        addOSPFv3Header(pkt, OSPFV3_HELLO, routerID,(IPv4Address)ife->getAreaId(),ife->getInstanceId());
        addIPv6CtrInfo(pkt, ifaceId, OSPFV3_IPV6_MULT, ife->getHopLimit());

        pkt->setInterfaceID(ifaceId);
        pkt->setRouterPriority(ife->getRouterPriorityId());
        pkt->setHelloInterval(ife->getHelloInt());
        pkt->setRouterDeadInterval(ife->getDeadInt());

        return pkt;
    }
    return NULL;
}

void OSPFv3ProcessIPv6::addOSPFv3Header(OSPFv3Packet *pkt, char type, IPv4Address routerID, IPv4Address areaID, char instanceID)
{
    pkt->setType(type);
    pkt->setRouterID(routerID);
    pkt->setAreaID(areaID);
    pkt->setInstanceID(instanceID);
}


void OSPFv3ProcessIPv6::addIPv6CtrInfo(OSPFv3Packet *pkt, int ifaceId, const IPv6Address &destAddress, short hopLimit)
{
   // create and fill in control info
   IPv6ControlInfo *controlInfo = new IPv6ControlInfo();
   controlInfo->setProtocol(89);
//   controlInfo->setSrcAddr(ift->getInterfaceById(ifaceId)->ipv6Data()->getLinkLocalAddress());
   controlInfo->setDestAddr(destAddress);
//   controlInfo->setTrafficClass();
   controlInfo->setHopLimit(hopLimit);
   controlInfo->setInterfaceId(ifaceId);

   pkt->setControlInfo(controlInfo);
}

//
// interface IOSPFv3Module
//

/*OSPFv3LinkIPv6 *OSPFv3ProcessIPv6::addLink(IPv6Address address, int mask)
{
    // TODO duplicity control
    return linkTable->addLink(address, mask);
}*/
/*
void OSPFv3ProcessIPv6::setHelloInt(int interval, int ifaceId)
{
    EigrpInterface *iface = getInterfaceById(ifaceId);

    if (iface == NULL)
    {
        iface = addInterfaceToEigrp(ifaceId, EigrpNetworkTable::UNSPEC_NETID, false);
    }
    iface->setHelloInt(interval);
}

void OSPFv3ProcessIPv6::setDeadInt(int interval, int ifaceId)
{
    EigrpInterface *iface = getInterfaceById(ifaceId);

    if (iface == NULL)
    { iface = addInterfaceToEigrp(ifaceId, EigrpNetworkTable::UNSPEC_NETID, false); }
    iface->setDeadInt(interval);
}*/

void OSPFv3ProcessIPv6::enableInterface(InterfaceEntry *iface, OSPFv3InterfaceIPv6 *OSPFv3Iface/*, IPv4Address& ifAddress, IPv4Address& ifMask*//*, int linkId*/)
{
    OSPFv3Timer* hellot;
    int ifaceId = OSPFv3Iface->getInterfaceId();
//    EigrpRouteSource<IPv4Address> *src;
//    EigrpMetricPar metricPar;
//    bool newSourceResult;

    EV << "OSPFv3 enabled on interface " << ifaceId << endl;

    // Remove interface from EIGRP interface table
//    eigrpIftDisabled->removeInterface(eigrpIface);
    OSPFv3Ift->addInterface(OSPFv3Iface);

    // Start Hello timer on interface
    if ((hellot = OSPFv3Iface->getHelloTimer()) == NULL)
    {
        hellot = createTimer(OSPFV3_HELLO_TIMER, OSPFv3Iface);
        OSPFv3Iface->setHelloTimerPtr(hellot);
    }
    scheduleAt(simTime() + uniform(0, OSPFv3Iface->getHelloInt()), hellot);

    // Create route
//    src = eigrpTt->findOrCreateRoute(ifAddress, ifMask, ifaceId, EigrpRouteSource<IPv4Address>::CONNECTED_ID, newSourceResult);

//    metricPar = eigrpMetric->getParam(eigrpIface, iface);
    // Compute metric
//    src->setMetricParams(metricPar);
//    uint32_t metric = eigrpMetric->computeMetric(metricPar, this->kValues);
//    src->setMetric(metric);

    // Notify DUAL about event
//    eigrpDual->processEvent(EigrpDual::NEW_NETWORK, src, true);
}

OSPFv3InterfaceIPv6 *OSPFv3ProcessIPv6::addInterfaceToOSPFv3(int ifaceId, int area,/*int linkId,*/ bool enabled)
{
    InterfaceEntry *iface = ift->getInterfaceById(ifaceId);
//    IPv4Address ifAddress, ifMask;
    // create OSPFv3 interface and hello timer
    OSPFv3InterfaceIPv6 *OSPFv3Iface = new OSPFv3InterfaceIPv6(iface, area,/*linkId,*/ enabled);

    if (enabled)
    {
        // Get address and mask of interface
 //       ifMask = iface->ipv4Data()->getNetmask();
 //       ifAddress = iface->ipv4Data()->getIPAddress().doAnd(ifMask);

        enableInterface(iface, OSPFv3Iface/*, ifAddress, ifMask*//*, linkId*/);
    }
    else
    {
//        eigrpIftDisabled->addInterface(eigrpIface);
    }

    return OSPFv3Iface;
}

OSPFv3Timer *OSPFv3ProcessIPv6::createTimer(char timerKind, void *context)
{
    OSPFv3Timer *timer = new OSPFv3Timer();
    timer->setTimerKind(timerKind);
    timer->setContextPointer(context);

    return timer;
}
