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


#include "OSPFv3MessageHandler.h"
/*
#include "ICMPMessage.h"
#include "OSPFRouter.h"

*/
OSPFv3MessageHandler::OSPFv3MessageHandler(OSPFv3ProcessIPv6* containingProcess/*, cSimpleModule* containingModule*/)/* :
    IOSPFv3MessageHandler(containingProcess),*/
//    ospfModule(containingModule),
/*    helloHandler(containingProcess),
    ddHandler(containingProcess),
    lsRequestHandler(containingProcess),
    lsUpdateHandler(containingProcess),
    lsAckHandler(containingProcess)
*/{
    process = containingProcess;
    helloHandler= new OSPFv3HelloHandler(containingProcess);
}
OSPFv3MessageHandler::~OSPFv3MessageHandler(){
    delete helloHandler;
}

void OSPFv3MessageHandler::messageReceived(cMessage* message)
{
    // Get source IP address and interface ID
//    IPv6ControlInfo *ctrlInfo =check_and_cast<IPv6ControlInfo *>(msg->getControlInfo());
//    IPv6Address srcAddr = ctrlInfo->getSrcAddr();
//    int ifaceId = ctrlInfo->getInterfaceId();
    if (message->isSelfMessage()) {
        handleTimer(message);
    } else if (dynamic_cast<ICMPMessage *>(message)) {
        EV << "ICMP error received -- discarding\n";
        delete message;
    } else {
        OSPFv3Packet* packet = check_and_cast<OSPFv3Packet*> (message);
        EV << "Received packet: (" << packet->getClassName() << ")" << packet->getName() << "\n";
        if (packet->getRouterID() == IPv4Address(process->getRouterID())) {
            EV << "This packet is from ourselves, discarding.\n";
            delete message;
        } else {
            processPacket(packet);
        }
    }
}

void OSPFv3MessageHandler::handleTimer(cMessage* timer)
{
    OSPFv3InterfaceIPv6 *intf;
    OSPFv3Neighbor* neighbor;
    switch (timer->getKind()) {
        case OSPFV3_HELLO_TIMER:
            {
                if (! (intf = reinterpret_cast <OSPFv3InterfaceIPv6*> (timer->getContextPointer()))) {
                    // should not reach this point
                    EV << "Discarding invalid InterfaceHelloTimer.\n";
                    delete timer;
                } else {
                    printEvent("Hello Timer expired", intf);
                    intf->processEvent(OSPFv3InterfaceIPv6::HELLO_TIMER);
                }
            }
            break;
        case OSPFV3_WAIT_TIMER:
            {
                if (! (intf = reinterpret_cast <OSPFv3InterfaceIPv6*> (timer->getContextPointer()))) {
                    // should not reach this point
                    EV << "Discarding invalid InterfaceWaitTimer.\n";
                    delete timer;
                } else {
                    printEvent("Wait Timer expired", intf);
                    intf->processEvent(OSPFv3InterfaceIPv6::WAIT_TIMER);
                }
            }
            break;
       case OSPFV3_ACKNOWLEDGEMENT_TIMER:
            {
                if (! (intf = reinterpret_cast <OSPFv3InterfaceIPv6*> (timer->getContextPointer()))) {
                    // should not reach this point
                    EV << "Discarding invalid InterfaceAcknowledgementTimer.\n";
                    delete timer;
                } else {
                    printEvent("Acknowledgement Timer expired", intf);
                    intf->processEvent(OSPFv3InterfaceIPv6::ACKNOWLEDGEMENT_TIMER);
                }
            }
            break;
        case OSPFV3_NEIGHBOR_INACTIVITY_TIMER:
            {
                if (! (neighbor = reinterpret_cast <OSPFv3Neighbor*> (timer->getContextPointer()))) {
                    // should not reach this point
                    EV << "Discarding invalid NeighborInactivityTimer.\n";
                    delete timer;
                } else {
                    printEvent("Inactivity Timer expired", neighbor->getInterface(), neighbor);
                    neighbor->processEvent(OSPFv3Neighbor::INACTIVITY_TIMER);
                }
            }
            break;
        case OSPFV3_NEIGHBOR_POLL_TIMER:
            {
                if (! (neighbor = reinterpret_cast <OSPFv3Neighbor*> (timer->getContextPointer()))) {
                    // should not reach this point
                    EV << "Discarding invalid NeighborInactivityTimer.\n";
                    delete timer;
                } else {
                    printEvent("Poll Timer expired", neighbor->getInterface(), neighbor);
                    neighbor->processEvent(OSPFv3Neighbor::POLL_TIMER);
                }
            }
            break;
        case OSPFV3_NEIGHBOR_DD_RETRANSMISSION_TIMER:
            {
                if (! (neighbor = reinterpret_cast <OSPFv3Neighbor*> (timer->getContextPointer()))) {
                    // should not reach this point
                    EV << "Discarding invalid NeighborDDRetransmissionTimer.\n";
                    delete timer;
                } else {
                    printEvent("Database Description Retransmission Timer expired", neighbor->getInterface(), neighbor);
                    neighbor->processEvent(OSPFv3Neighbor::DD_RETRANSMISSION_TIMER);
                }
            }
            break;
        case OSPFV3_NEIGHBOR_UPDATE_RETRANSMISSION_TIMER:
            {
                if (! (neighbor = reinterpret_cast <OSPFv3Neighbor*> (timer->getContextPointer()))) {
                    // should not reach this point
                    EV << "Discarding invalid NeighborUpdateRetransmissionTimer.\n";
                    delete timer;
                } else {
                    printEvent("Update Retransmission Timer expired", neighbor->getInterface(), neighbor);
                    neighbor->processEvent(OSPFv3Neighbor::UPDATE_RETRANSMISSION_TIMER);
                }
            }
            break;
        case OSPFV3_NEIGHBOR_REQUEST_RETRANSMISSION_TIMER:
            {
                if (! (neighbor = reinterpret_cast <OSPFv3Neighbor*> (timer->getContextPointer()))) {
                    // should not reach this point
                    EV << "Discarding invalid NeighborRequestRetransmissionTimer.\n";
                    delete timer;
                } else {
                    printEvent("Request Retransmission Timer expired", neighbor->getInterface(), neighbor);
                    neighbor->processEvent(OSPFv3Neighbor::REQUEST_RETRANSMISSION_TIMER);
                }
            }
            break;
        case OSPFV3_DATABASE_AGE_TIMER:
            {
                printEvent("Ageing the database");
 //               router->ageDatabase();
            }
            break;
        default:
            {
                EV << "Timer with unknown kind was skipped" << endl;
                delete timer;
                timer = NULL;
            }
            break;
    }
}

void OSPFv3MessageHandler::processPacket(OSPFv3Packet* packet, OSPFv3InterfaceIPv6* unused1, OSPFv3Neighbor* unused2)
{
    // see RFC 2328 8.2

    // packet version must be OSPF version 2
    if (packet->getVersion() == 3) {
        IPv6ControlInfo* controlInfo = check_and_cast<IPv6ControlInfo *> (packet->getControlInfo());
        int interfaceId = controlInfo->getInterfaceId();
        IPv4Address areaID = packet->getAreaID();
        OSPFv3Area*   area = process->getOSPFv3AreaById(areaID);
        if (area != NULL) {
            // packet Area ID must either match the Area ID of the receiving interface or...
            OSPFv3InterfaceIPv6* intf = process->getOSPFv3InterfaceById(interfaceId);

            if (intf == NULL) {
                // it must be the backbone area and...
                if (areaID == NULL_IPV4ADDRESS) {
/*                    if (router->getAreaCount() > 1) {
                        // it must be a virtual link and the source router's router ID must be the endpoint of this virtual link and...
                        intf = area->findVirtualLink(packet->getRouterID());

                        if (intf != NULL) {
                            OSPF::Area* virtualLinkTransitArea = router->getAreaByID(intf->getTransitAreaID());

                            if (virtualLinkTransitArea != NULL) {
                                // the receiving interface must attach to the virtual link's configured transit area
                                OSPFv3InterfaceIPv6* virtualLinkInterface = virtualLinkTransitArea->getInterface(interfaceId);

                                if (virtualLinkInterface == NULL) {
                                    intf = NULL;
                                }
                            } else {
                                intf = NULL;
                            }
                        }
                    }*/
                }
            }
            if (intf != NULL) {
                IPv6Address destinationAddress = controlInfo->getDestAddr();
                IPv6Address allDRouters = process->OSPFV3_IPV6_MULT_DR;
                OSPFv3InterfaceIPv6::InterfaceStateType interfaceState = intf->getState();

                // if destination address is ALL_D_ROUTERS the receiving interface must be in DesignatedRouter or Backup state
                if (
                    ((destinationAddress == allDRouters) &&
                     (
                      (interfaceState == OSPFv3InterfaceIPv6::STATE_DESIGNATED_ROUTER) ||
                      (interfaceState == OSPFv3InterfaceIPv6::STATE_BACKUP)
                     )
                    ) ||
                    (destinationAddress != allDRouters)
                   )
                {
                    // packet authentication
//                    if (authenticatePacket(packet)) {
                        OSPFv3PacketType packetType = static_cast<OSPFv3PacketType> (packet->getType());
                        OSPFv3Neighbor* neighbor = NULL;

                        // all packets except HelloPackets are sent only along adjacencies, so a Neighbor must exist
                        if (packetType != OSPFV3_HELLO) {
                            switch (intf->getType()) {
                                case OSPFv3InterfaceIPv6::BROADCAST:
                                case OSPFv3InterfaceIPv6::NBMA:
                                case OSPFv3InterfaceIPv6::POINTTOMULTIPOINT:
//                                    neighbor = intf->getNeighborByAddress(controlInfo->getSrcAddr());
                                    break;
                                case OSPFv3InterfaceIPv6::POINTTOPOINT:
                                case OSPFv3InterfaceIPv6::VIRTUAL:
//                                    neighbor = intf->getNeighborByID(packet->getRouterID());
                                    break;
                                default: break;
                            }
                        }
                        switch (packetType) {
                            case OSPFV3_HELLO:
                                helloHandler->processPacket(packet, intf);
                                break;
                            case OSPFV3_DATABASE_DESCRIPTION:
                                if (neighbor != NULL) {
//                                    ddHandler.processPacket(packet, intf, neighbor);
                                }
                                break;
/*                            case LINKSTATE_REQUEST_PACKET:
                                if (neighbor != NULL) {
                                    lsRequestHandler.processPacket(packet, intf, neighbor);
                                }
                                break;
                            case LINKSTATE_UPDATE_PACKET:
                                if (neighbor != NULL) {
                                    lsUpdateHandler.processPacket(packet, intf, neighbor);
                                }
                                break;
                            case LINKSTATE_ACKNOWLEDGEMENT_PACKET:
                                if (neighbor != NULL) {
                                    lsAckHandler.processPacket(packet, intf, neighbor);
                                }
                                break;
*/                            default: break;
                        }
                    //}
                }
            }
        }
    }
    delete packet;
}

void OSPFv3MessageHandler::sendPacket(OSPFv3Packet* packet, IPv6Address destination, int outputIfId, short hopLimit)
{
    IPv6ControlInfo *ipControlInfo = new IPv6ControlInfo();
    ipControlInfo->setProtocol(IP_PROT_OSPF);
    ipControlInfo->setDestAddr(destination);
    ipControlInfo->setHopLimit(hopLimit);
    ipControlInfo->setInterfaceId(outputIfId);
    packet->setControlInfo(ipControlInfo);

    switch (packet->getType()) {
        case OSPFV3_HELLO:
            {
                packet->setKind(OSPFV3_HELLO);
                packet->setName("OSPFv3_Hello");

                OSPFv3HelloPacket* helloPacket = check_and_cast<OSPFv3HelloPacket*> (packet);
                printHelloPacket(helloPacket, destination, outputIfId);
            }
            break;
/*        case DATABASE_DESCRIPTION_PACKET:
            {
                packet->setKind(DATABASE_DESCRIPTION_PACKET);
                packet->setName("OSPF_DDPacket");

                OSPFDatabaseDescriptionPacket* ddPacket = check_and_cast<OSPFDatabaseDescriptionPacket*> (packet);
                printDatabaseDescriptionPacket(ddPacket, destination, outputIfIndex);
            }
            break;
        case LINKSTATE_REQUEST_PACKET:
            {
                packet->setKind(LINKSTATE_REQUEST_PACKET);
                packet->setName("OSPF_LSReqPacket");

                OSPFLinkStateRequestPacket* requestPacket = check_and_cast<OSPFLinkStateRequestPacket*> (packet);
                printLinkStateRequestPacket(requestPacket, destination, outputIfIndex);
            }
            break;
        case LINKSTATE_UPDATE_PACKET:
            {
                packet->setKind(LINKSTATE_UPDATE_PACKET);
                packet->setName("OSPF_LSUpdPacket");

                OSPFLinkStateUpdatePacket* updatePacket = check_and_cast<OSPFLinkStateUpdatePacket*> (packet);
                printLinkStateUpdatePacket(updatePacket, destination, outputIfIndex);
            }
            break;
        case LINKSTATE_ACKNOWLEDGEMENT_PACKET:
            {
                packet->setKind(LINKSTATE_ACKNOWLEDGEMENT_PACKET);
                packet->setName("OSPF_LSAckPacket");

                OSPFLinkStateAcknowledgementPacket* ackPacket = check_and_cast<OSPFLinkStateAcknowledgementPacket*> (packet);
                printLinkStateAcknowledgementPacket(ackPacket, destination, outputIfIndex);
            }
            break;
*/        default: break;
    }

    process->sendOSPFv3Packet(packet);
}

void OSPFv3MessageHandler::clearTimer(cMessage* timer)
{
    process->cancelEvent(timer);
}

void OSPFv3MessageHandler::startTimer(cMessage* timer, simtime_t delay)
{
    process->scheduleAt(simTime() + delay, timer);
}
OSPFv3Timer *OSPFv3MessageHandler::createTimer(short timerKind, void *context)
{
    OSPFv3Timer *timer = new OSPFv3Timer();
    timer->setKind(timerKind);
    timer->setContextPointer(context);

    return timer;
}

void OSPFv3MessageHandler::printEvent(const char* eventString, const OSPFv3InterfaceIPv6 *intf, const OSPFv3Neighbor* forNeighbor) const
{
    EV << eventString;
    if ((intf != NULL) || (forNeighbor != NULL)) {
        EV << ": ";
    }
    if (forNeighbor != NULL) {
        EV << "neighbor["
           << forNeighbor->getNeighborID()
           << "] (state: "
           << forNeighbor->getStateString(forNeighbor->getState())
           << "); ";
    }
    if (intf != NULL) {
        EV << "interface["
           << static_cast <short> (intf->getInterfaceId())
           << "] ";
        switch (intf->getType()) {
            case OSPFv3InterfaceIPv6::POINTTOPOINT:      EV << "(PointToPoint)";
                                                     break;
            case OSPFv3InterfaceIPv6::BROADCAST:         EV << "(Broadcast)";
                                                     break;
            case OSPFv3InterfaceIPv6::NBMA:              EV << "(NBMA).\n";
                                                     break;
            case OSPFv3InterfaceIPv6::POINTTOMULTIPOINT: EV << "(PointToMultiPoint)";
                                                     break;
            case OSPFv3InterfaceIPv6::VIRTUAL:           EV << "(Virtual)";
                                                     break;
            default:                                 EV << "(Unknown)";
                                                     break;
        }
        EV << " (state: "
           << intf->getStateString(intf->getState())
           << ")";
    }
    EV << ".\n";
}

void OSPFv3MessageHandler::printHelloPacket(const OSPFv3HelloPacket* helloPacket, IPv6Address destination, int outputInterfaceId) const
{
    EV << "Sending Hello packet to " << destination << " on interface[" << outputInterfaceId << "] with contents:"<<endl;
    EV << "  DR=" << helloPacket->getDesignatedRouter() << endl;
    EV << "  BDR=" << helloPacket->getBackupDesignatedRouter() << endl;

    EV << "  neighbors:\n";

    unsigned int neighborCount = helloPacket->getNeighborIDArraySize();
    for (unsigned int i = 0; i < neighborCount; i++) {
        EV << "    " << helloPacket->getNeighborID(i) << "\n";
    }
}
/*
void OSPF::MessageHandler::printDatabaseDescriptionPacket(const OSPFDatabaseDescriptionPacket* ddPacket, IPv4Address destination, int outputIfIndex) const
{
    EV << "Sending Database Description packet to " << destination << " on interface[" << outputIfIndex << "] with contents:\n";

    const OSPFDDOptions& ddOptions = ddPacket->getDdOptions();
    EV << "  ddOptions="
       << ((ddOptions.I_Init) ? "I " : "_ ")
       << ((ddOptions.M_More) ? "M " : "_ ")
       << ((ddOptions.MS_MasterSlave) ? "MS" : "__")
       << "\n";
    EV << "  seqNumber=" << ddPacket->getDdSequenceNumber() << "\n";
    EV << "  LSA headers:\n";

    unsigned int lsaCount = ddPacket->getLsaHeadersArraySize();
    for (unsigned int i = 0; i < lsaCount; i++) {
        EV << "    ";
        printLSAHeader(ddPacket->getLsaHeaders(i), ev.getOStream());
        EV << "\n";
    }
}

void OSPF::MessageHandler::printLinkStateRequestPacket(const OSPFLinkStateRequestPacket* requestPacket, IPv4Address destination, int outputIfIndex) const
{
    EV << "Sending Link State Request packet to " << destination << " on interface[" << outputIfIndex << "] with requests:\n";

    unsigned int requestCount = requestPacket->getRequestsArraySize();
    for (unsigned int i = 0; i < requestCount; i++) {
        const LSARequest& request = requestPacket->getRequests(i);
        EV << "  type=" << request.lsType
           << ", LSID=" << request.linkStateID
           << ", advertisingRouter=" << request.advertisingRouter << "\n";
    }
}

void OSPF::MessageHandler::printLinkStateUpdatePacket(const OSPFLinkStateUpdatePacket* updatePacket, IPv4Address destination, int outputIfIndex) const
{
    EV << "Sending Link State Update packet to " << destination << " on interface[" << outputIfIndex << "] with updates:\n";

    unsigned int i = 0;
    unsigned int updateCount = updatePacket->getRouterLSAsArraySize();

    for (i = 0; i < updateCount; i++) {
        const OSPFRouterLSA& lsa = updatePacket->getRouterLSAs(i);
        EV << "  ";
        printLSAHeader(lsa.getHeader(), ev.getOStream());
        EV << "\n";

        EV << "  bits="
           << ((lsa.getV_VirtualLinkEndpoint()) ? "V " : "_ ")
           << ((lsa.getE_ASBoundaryRouter()) ? "E " : "_ ")
           << ((lsa.getB_AreaBorderRouter()) ? "B" : "_")
           << "\n";
        EV << "  links:\n";

        unsigned int linkCount = lsa.getLinksArraySize();
        for (unsigned int j = 0; j < linkCount; j++) {
            const Link& link = lsa.getLinks(j);
            EV << "    ID=" << link.getLinkID();
            EV << ", data="
               << link.getLinkData() << " (" << IPv4Address(link.getLinkData()) << ")"
               << ", type=";
            switch (link.getType()) {
                case POINTTOPOINT_LINK:  EV << "PointToPoint";   break;
                case TRANSIT_LINK:       EV << "Transit";        break;
                case STUB_LINK:          EV << "Stub";           break;
                case VIRTUAL_LINK:       EV << "Virtual";        break;
                default:                 EV << "Unknown";        break;
            }
            EV << ", cost=" << link.getLinkCost() << "\n";
        }
    }

    updateCount = updatePacket->getNetworkLSAsArraySize();
    for (i = 0; i < updateCount; i++) {
        const OSPFNetworkLSA& lsa = updatePacket->getNetworkLSAs(i);
        EV << "  ";
        printLSAHeader(lsa.getHeader(), ev.getOStream());
        EV << "\n";

        EV << "  netMask=" << lsa.getNetworkMask() << "\n";
        EV << "  attachedRouters:\n";

        unsigned int routerCount = lsa.getAttachedRoutersArraySize();
        for (unsigned int j = 0; j < routerCount; j++) {
            EV << "    " << lsa.getAttachedRouters(j) << "\n";
        }
    }

    updateCount = updatePacket->getSummaryLSAsArraySize();
    for (i = 0; i < updateCount; i++) {
        const OSPFSummaryLSA& lsa = updatePacket->getSummaryLSAs(i);
        EV << "  ";
        printLSAHeader(lsa.getHeader(), ev.getOStream());
        EV << "\n";
        EV << "  netMask=" << lsa.getNetworkMask() << "\n";
        EV << "  cost=" << lsa.getRouteCost() << "\n";
    }

    updateCount = updatePacket->getAsExternalLSAsArraySize();
    for (i = 0; i < updateCount; i++) {
        const OSPFASExternalLSA& lsa = updatePacket->getAsExternalLSAs(i);
        EV << "  ";
        printLSAHeader(lsa.getHeader(), ev.getOStream());
        EV << "\n";

        const OSPFASExternalLSAContents& contents = lsa.getContents();
        EV << "  netMask=" <<  contents.getNetworkMask() << "\n";
        EV << "  bits=" << ((contents.getE_ExternalMetricType()) ? "E\n" : "_\n");
        EV << "  cost=" << contents.getRouteCost() << "\n";
        EV << "  forward=" << contents.getForwardingAddress() << "\n";
    }
}

void OSPF::MessageHandler::printLinkStateAcknowledgementPacket(const OSPFLinkStateAcknowledgementPacket* ackPacket, IPv4Address destination, int outputIfIndex) const
{
    EV << "Sending Link State Acknowledgement packet to " << destination << " on interface[" << outputIfIndex << "] with acknowledgements:\n";

    unsigned int lsaCount = ackPacket->getLsaHeadersArraySize();
    for (unsigned int i = 0; i < lsaCount; i++) {
        EV << "    ";
        printLSAHeader(ackPacket->getLsaHeaders(i), ev.getOStream());
        EV << "\n";
    }
}
*/
