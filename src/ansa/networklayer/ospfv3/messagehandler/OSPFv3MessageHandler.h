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

#ifndef __INET_OSPFV3MESSAGEHANDLER_H
#define __INET_OSPFV3MESSAGEHANDLER_H

#include "OSPFv3DatabaseDescriptionHandler.h"
#include "OSPFv3HelloHandler.h"
#include "IOSPFv3MessageHandler.h"
#include "IPv4ControlInfo.h"
#include "OSPFv3LinkStateAcknowledgementHandler.h"
#include "OSPFv3LinkStateRequestHandler.h"
#include "OSPFv3LinkStateUpdateHandler.h"
#include "OSPFv3Interface.h"
#include "OSPFv3PacketIPv6_m.h"
//#include "OSPFTimer.h"
#include "OSPFv3ProcessIPv6.h"
#include "OSPFv3Interface.h"
#include "ICMPMessage.h"

//namespace OSPFv3 {

class OSPFv3InterfaceIPv6;
class OSPFv3HelloHandler;

class OSPFv3MessageHandler /*: public */ {
protected:
    OSPFv3ProcessIPv6* process;
private:
//    cSimpleModule*                  ospfModule;

    OSPFv3HelloHandler              *helloHandler;
//    DatabaseDescriptionHandler      ddHandler;
//    LinkStateRequestHandler         lsRequestHandler;
//    LinkStateUpdateHandler          lsUpdateHandler;
//    LinkStateAcknowledgementHandler lsAckHandler;

public:
    OSPFv3MessageHandler(OSPFv3ProcessIPv6* containingProcess/*, cSimpleModule* containingModule*/);
    ~OSPFv3MessageHandler();

    void    messageReceived(cMessage* message);
    void    handleTimer(cMessage*  timer);

    void    processPacket(OSPFv3Packet* packet, OSPFv3InterfaceIPv6* unused1 = NULL, OSPFv3Neighbor* unused2 = NULL);

    void    sendPacket(OSPFv3Packet* packet, IPv6Address destination, int outputIfId, short hopLimit = 1);
    void    clearTimer(cMessage*  timer);
    void    startTimer(cMessage*  timer, simtime_t delay);
    OSPFv3Timer *createTimer(short timerKind, void *context);

    void    printEvent(const char* eventString, const OSPFv3InterfaceIPv6 *intf = NULL, const OSPFv3Neighbor* forNeighbor = NULL) const;
    void    printHelloPacket(const OSPFv3HelloPacket* helloPacket, IPv6Address destination, int outputInterfaceId) const;
//    void    printDatabaseDescriptionPacket(const OSPFDatabaseDescriptionPacket* ddPacket, IPv4Address destination, int outputIfIndex) const;
//    void    printLinkStateRequestPacket(const OSPFLinkStateRequestPacket* requestPacket, IPv4Address destination, int outputIfIndex) const;
//    void    printLinkStateUpdatePacket(const OSPFLinkStateUpdatePacket* updatePacket, IPv4Address destination, int outputIfIndex) const;
//    void    printLinkStateAcknowledgementPacket(const OSPFLinkStateAcknowledgementPacket* ackPacket, IPv4Address destination, int outputIfIndex) const;

    // Authentication not implemented
//    bool    authenticatePacket(OSPFPacket* packet)  { return true; }
};

//} // namespace OSPFv3

#endif // __INET_OSPFV3MESSAGEHANDLER_H

