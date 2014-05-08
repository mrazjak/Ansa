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


#include "OSPFv3LinkStateAcknowledgementHandler.h"

//#include "OSPFRouter.h"


OSPFv3LinkStateAcknowledgementHandler::OSPFv3LinkStateAcknowledgementHandler(OSPFv3ProcessIPv6* containingProcess) /*:
    IOSPFv3MessageHandler(containingProcess)*/
{
    process = containingProcess;
}

void OSPFv3LinkStateAcknowledgementHandler::processPacket(OSPFv3Packet* packet, OSPFv3InterfaceIPv6* intf, OSPFv3Neighbor* neighbor)
{
/*    router->getMessageHandler()->printEvent("Link State Acknowledgement packet received", intf, neighbor);

    if (neighbor->getState() >= OSPF::Neighbor::EXCHANGE_STATE) {
        OSPFLinkStateAcknowledgementPacket* lsAckPacket = check_and_cast<OSPFLinkStateAcknowledgementPacket*> (packet);

        int lsaCount = lsAckPacket->getLsaHeadersArraySize();

        EV << "  Processing packet contents:\n";

        for (int i = 0; i < lsaCount; i++) {
            OSPFLSAHeader& lsaHeader = lsAckPacket->getLsaHeaders(i);
            OSPFLSA* lsaOnRetransmissionList;
            OSPF::LSAKeyType lsaKey;

            EV << "    ";
            printLSAHeader(lsaHeader, ev.getOStream());
            EV << "\n";

            lsaKey.linkStateID = lsaHeader.getLinkStateID();
            lsaKey.advertisingRouter = lsaHeader.getAdvertisingRouter();

            if ((lsaOnRetransmissionList = neighbor->findOnRetransmissionList(lsaKey)) != NULL) {
                if (operator==(lsaHeader, lsaOnRetransmissionList->getHeader())) {
                    neighbor->removeFromRetransmissionList(lsaKey);
                } else {
                    EV << "Got an Acknowledgement packet for an unsent Update packet.\n";
                }
            }
        }
        if (neighbor->isLinkStateRetransmissionListEmpty()) {
            neighbor->clearUpdateRetransmissionTimer();
        }
    }
*/}
