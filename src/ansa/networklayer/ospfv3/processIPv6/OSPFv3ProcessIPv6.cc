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

namespace OSPFv3
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


OSPFv3ProcessIPv6::OSPFv3ProcessIPv6() : OSPFV3_IPV6_MULT(IPv6Address("ff02::5")),OSPFV3_IPV6_MULT_DR(IPv6Address("ff02::6"))
{
    OSPFv3MsgHandl = new OSPFv3MessageHandler(this);
    OSPFV3_OUT_GW = "IPv6Out";
    routerID = IPv4Address("0.0.0.0");
    processID = 1;
}

OSPFv3ProcessIPv6::~OSPFv3ProcessIPv6()
{
    delete OSPFv3MsgHandl;
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
    //    this->OSPFv3Ift = ModuleAccess<OSPFv3InterfaceTableIPv6>("OSPFv3InterfaceTableIPv6").get();
        this->OSPFv3Nt  = ModuleAccess<OSPFv3NeighborTableIPv6>("OSPFv3NeighborTableIPv6").get();
        this->OSPFv3Tt  = ModuleAccess<OSPFv3TopologyTableIPv6>("OSPFv3TopologyTableIPv6").get();
        this->OSPFv3At  = ModuleAccess<OSPFv3AreaTableIPv6>("OSPFv3AreaTableIPv6").get();
        this->OSPFv3Ift = ModuleAccess<OSPFv3InterfaceTableIPv6>("OSPFv3InterfaceTableIPv6").get();

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
    OSPFv3MsgHandl->messageReceived(msg);
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


OSPFv3Area *OSPFv3ProcessIPv6::getOSPFv3AreaById (IPv4Address AreaID) const{
    return (OSPFv3At->getAreaById(AreaID));
}
OSPFv3InterfaceIPv6* OSPFv3ProcessIPv6::getOSPFv3InterfaceById(int intfID) const{
    return (OSPFv3Ift->getInterfaceById(intfID));
}
InterfaceEntry* OSPFv3ProcessIPv6::getInterfaceById(int intfID) const{
    return ift->getInterfaceById(intfID);
}
OSPFv3Neighbor* OSPFv3ProcessIPv6::getNeighborByIntfIdAndNeigId(IPv4Address neigId,int intfID) const{
    return OSPFv3Nt->getNeighborByIntfIdAndNeigId(neigId,intfID);
}
OSPFv3Neighbor* OSPFv3ProcessIPv6::getNeighborByIntfIdAndNeigAddress(IPv6Address neigAdd,int intfID) const{
    return OSPFv3Nt->getNeighborByIntfIdAndNeigAddress(neigAdd,intfID);
}
int OSPFv3ProcessIPv6::getNeighborsByIntfId(int intfID, std::vector<OSPFv3Neighbor*>* neighbors) {
    return OSPFv3Nt->getNeighborsByIntfId(intfID,neighbors);
}
//
// interface IOSPFv3Module
//

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
    int ifaceId = OSPFv3Iface->getInterfaceId();
//    EigrpRouteSource<IPv4Address> *src;
//    EigrpMetricPar metricPar;
//    bool newSourceResult;

    EV << "OSPFv3 enabled on interface " << ifaceId << endl;

    // Remove interface from EIGRP interface table
//    eigrpIftDisabled->removeInterface(eigrpIface);
    OSPFv3Ift->addInterface(OSPFv3Iface);
    joinMulticastGroups(OSPFv3Iface->getInterfaceId());

    OSPFv3Iface->processEvent(OSPFv3InterfaceIPv6::INTERFACE_UP);

}

void OSPFv3ProcessIPv6::joinMulticastGroups(int interfaceId)
{
    InterfaceEntry *ie = ift->getInterfaceById(interfaceId);
    if (!ie)
        throw cRuntimeError("Interface id=%d does not exist", interfaceId);
    IPv6InterfaceData *ipv6Data = ie->ipv6Data();
    if (!ipv6Data)
        throw cRuntimeError("Interface %s (id=%d) does not have IPv6 data", ie->getName(), interfaceId);
    ipv6Data->joinMulticastGroup(OSPFv3ProcessIPv6::OSPFV3_IPV6_MULT);
    ipv6Data->joinMulticastGroup(OSPFv3ProcessIPv6::OSPFV3_IPV6_MULT_DR);
}

OSPFv3InterfaceIPv6 *OSPFv3ProcessIPv6::addInterfaceToOSPFv3( OSPFv3InterfaceIPv6 *OSPFv3Iface)
{
    InterfaceEntry *iface = ift->getInterfaceById(OSPFv3Iface->getInterfaceId());

    if (true/*OSPFv3Intf->getEnabled*/)
    {
        enableInterface(iface, OSPFv3Iface);
    }
    else
    {
//        OSPFv3Ift->addInterface(OSPFv3Iface);
        //TODO přidat rozhraní do OSPF bez Timeru jako passive
//        eigrpIftDisabled->addInterface(eigrpIface);
    }

    return OSPFv3Iface;
}

OSPFv3Area *OSPFv3ProcessIPv6::addArea2( OSPFv3Area *OSPFv3area)
{
    OSPFv3At->addArea(OSPFv3area);
    return OSPFv3area;
}
OSPFv3Neighbor *OSPFv3ProcessIPv6::addNeighbor2( OSPFv3Neighbor *OSPFv3neig)
{
    OSPFv3Nt->addNeighbor(OSPFv3neig);
    return OSPFv3neig;
}

