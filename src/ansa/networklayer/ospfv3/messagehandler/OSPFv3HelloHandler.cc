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

#include "OSPFv3HelloHandler.h"
/*
#include "IPv4ControlInfo.h"
#include "OSPFArea.h"
#include "OSPFInterface.h"
#include "OSPFNeighbor.h"
#include "OSPFRouter.h"
*/

OSPFv3HelloHandler::OSPFv3HelloHandler(OSPFv3ProcessIPv6* containingProcess) /*:
    IOSPFv3MessageHandler(containingProcess)*/
{
    process = containingProcess;
}

void OSPFv3HelloHandler::processPacket(OSPFv3Packet* packet, OSPFv3InterfaceIPv6* intf, OSPFv3Neighbor* unused)
{
    OSPFv3HelloPacket* helloPacket = check_and_cast<OSPFv3HelloPacket*> (packet);
    bool shouldRebuildRoutingTable = false;

    /* The values of the HelloInterval,
       and RouterDeadInterval fields in the received Hello packet must
       be checked against the values configured for the receiving
       interface.  Any mismatch causes processing to stop and the
       packet to be dropped.
     */
    if ((intf->getHelloInt() == helloPacket->getHelloInterval()) &&
        (intf->getDeadInt() == helloPacket->getRouterDeadInterval()))
    {
        OSPFv3InterfaceIPv6::OSPFv3InterfaceType interfaceType = intf->getType();

            /* The setting of the E-bit found in the Hello Packet's Options field must match this area's
               ExternalRoutingCapability.
             */
//            if (intf->getArea()->getExternalRoutingCapability() == helloPacket->getOptions().E_ExternalRoutingCapability) {
                IPv6ControlInfo* controlInfo = check_and_cast<IPv6ControlInfo *> (helloPacket->getControlInfo());
                IPv6Address srcAddress = controlInfo->getSrcAddr();
                bool neighborChanged = false;
                bool neighborsDRStateChanged = false;
                bool drChanged = false;
                bool backupSeen = false;
                OSPFv3Neighbor* neighbor;

                /* If the receiving interface connects to a broadcast, Point-to-
                   MultiPoint or NBMA network the source is identified by the IP
                   source address found in the Hello's IP header.
                 */
                if ((interfaceType == OSPFv3InterfaceIPv6::BROADCAST) ||
                    (interfaceType == OSPFv3InterfaceIPv6::POINTTOMULTIPOINT) ||
                    (interfaceType == OSPFv3InterfaceIPv6::NBMA))
                {
//                    neighbor = intf->getNeighborByAddress(srcAddress);
                    neighbor = process->getNeighborByIntfIdAndNeigAddress(
                            srcAddress,intf->getInterfaceId());

                } else {
                    /* If the receiving interface connects to a point-to-point link or a virtual link,
                       the source is identified by the Router ID found in the Hello's OSPF packet header.
                     */
//                    neighbor = intf->getNeighborByID(helloPacket->getRouterID());
                    neighbor = process->getNeighborByIntfIdAndNeigId(
                            helloPacket->getRouterID(),intf->getInterfaceId());
                }

                if (neighbor != NULL) {
                    process->getMessageHandler()->printEvent("Hello packet received", intf, neighbor);

                    IPv4Address oldDesignated = neighbor->getDesignatedRouter();
                    IPv4Address oldBackup = neighbor->getBackupDesignatedRouter();
                    char newPriority = helloPacket->getRouterPriority();
//                    IPv6Address source = controlInfo->getSrcAddr();
                    IPv4Address sourceRId = helloPacket->getRouterID();
                    IPv4Address newDesignatedRouter = helloPacket->getDesignatedRouter();
                    IPv4Address newBackupRouter = helloPacket->getBackupDesignatedRouter();
                    IPv4Address dRouterID;

                    if ((interfaceType == OSPFv3InterfaceIPv6::VIRTUAL) &&
                        (neighbor->getState() == OSPFv3Neighbor::STATE_DOWN))
                    {
                        neighbor->setPriority(helloPacket->getRouterPriority());
                        neighbor->setRouterDeadInterval(helloPacket->getRouterDeadInterval());
                    }

                    /* If a change in the neighbor's Router Priority field
                       was noted, the receiving interface's state machine is
                       scheduled with the event NEIGHBOR_CHANGE.
                     */
                    if (neighbor->getPriority() != newPriority) {
                        neighborChanged = true;
                    }

                    /* If the neighbor is both declaring itself to be Designated
                       Router(Hello Packet's Designated Router field = Neighbor IP
                       address) and the Backup Designated Router field in the
                       packet is equal to 0.0.0.0 and the receiving interface is in
                       state Waiting, the receiving interface's state machine is
                       scheduled with the event BACKUP_SEEN.
                     */
                   if ((newDesignatedRouter == sourceRId) &&
                        (newBackupRouter == NULL_IPV4ADDRESS) &&
                        (intf->getState() == OSPFv3InterfaceIPv6::STATE_WAITING))
                    {
                        backupSeen = true;
                    } else {
                        /* Otherwise, if the neighbor is declaring itself to be Designated Router and it
                           had not previously, or the neighbor is not declaring itself
                           Designated Router where it had previously, the receiving
                           interface's state machine is scheduled with the event
                           NEIGHBOR_CHANGE.
                         */
                        if (((newDesignatedRouter == sourceRId) &&
                             (newDesignatedRouter != oldDesignated)) ||
                            ((newDesignatedRouter != sourceRId) &&
                             (sourceRId == oldDesignated)))
                        {
                            neighborChanged = true;
                            neighborsDRStateChanged = true;
                        }
                    }

                    /* If the neighbor is declaring itself to be Backup Designated
                       Router(Hello Packet's Backup Designated Router field =
                       Neighbor IP address) and the receiving interface is in state
                       Waiting, the receiving interface's state machine is
                       scheduled with the event BACKUP_SEEN.
                     */
                    if ((newBackupRouter == sourceRId) &&
                        (intf->getState() == OSPFv3InterfaceIPv6::STATE_WAITING))
                    {
                        backupSeen = true;
                    } else {
                        /* Otherwise, if the neighbor is declaring itself to be Backup Designated Router
                           and it had not previously, or the neighbor is not declaring
                           itself Backup Designated Router where it had previously, the
                           receiving interface's state machine is scheduled with the
                           event NEIGHBOR_CHANGE.
                         */
                        if (((newBackupRouter == sourceRId) &&
                             (newBackupRouter != oldBackup)) ||
                            ((newBackupRouter != sourceRId) &&
                             (sourceRId == oldBackup)))
                        {
                            neighborChanged = true;
                        }
                    }

                    neighbor->setNeighborID(helloPacket->getRouterID());
                    neighbor->setPriority(newPriority);
                    neighbor->setNeighborAddress(srcAddress);
                    dRouterID/*.routerID*/ = newDesignatedRouter;
//                    dRouterID.ipInterfaceAddress = newDesignatedRouter;
                    if (newDesignatedRouter != oldDesignated) {
                        oldDesignated = dRouterID;//.ipInterfaceAddress;
                        drChanged = true;
                    }
                    neighbor->setDesignatedRouter(dRouterID);
                    dRouterID/*.routerID*/ = newBackupRouter;
 //                   dRouterID.ipInterfaceAddress = newBackupRouter;
                    if (newBackupRouter != oldBackup) {
                        oldBackup = dRouterID;//.ipInterfaceAddress;
                        drChanged = true;
                    }
                    neighbor->setBackupDesignatedRouter(dRouterID);
                    if (drChanged) {
                        neighbor->setUpDesignatedRouters(false);
                    }

                    /* If the neighbor router's Designated or Backup Designated Router
                       has changed it's necessary to look up the Router IDs belonging to the
                       new addresses.
                     */
                    if (!neighbor->designatedRoutersAreSetUp()) {
 //                       OSPFv3Neighbor* designated = intf->getNeighborByAddress(designatedAddress);
                        OSPFv3Neighbor* designated = process->getNeighborByIntfIdAndNeigId(
                                oldDesignated, intf->getInterfaceId());
 //                       OSPFv3Neighbor* backup = intf->getNeighborByAddress(backupAddress);
                        OSPFv3Neighbor* backup = process->getNeighborByIntfIdAndNeigId(
                                oldBackup, intf->getInterfaceId());

                        if (designated != NULL) {
                            dRouterID/*.routerID*/ = designated->getNeighborID();
 //                           dRouterID.ipInterfaceAddress = designated->getAddress();
                            neighbor->setDesignatedRouter(dRouterID);
                        }
                        if (backup != NULL) {
                            dRouterID/*.routerID*/ = backup->getNeighborID();
 //                           dRouterID.ipInterfaceAddress = backup->getAddress();
                            neighbor->setBackupDesignatedRouter(dRouterID);
                        }
                        if ((designated != NULL) && (backup != NULL)) {
                            neighbor->setUpDesignatedRouters(true);
                        }
                    }
                } else {
 //                   OSPF::DesignatedRouterID dRouterID;
                    IPv4Address dRouterID;
                    bool designatedSetUp = false;
                    bool backupSetUp = false;
                    neighbor = new OSPFv3Neighbor(
                            process->getOSPFv3AreaById(helloPacket->getAreaID()),intf,
                            helloPacket->getRouterID(),srcAddress);
                    neighbor->setPriority(helloPacket->getRouterPriority());
//                    neighbor->setNeighborAddress(srcAddress);
                    neighbor->setRouterDeadInterval(helloPacket->getRouterDeadInterval());
//                    neighbor->setInterface(intf);

                    process->getMessageHandler()->printEvent("Hello packet received", intf, neighbor);

                    dRouterID = helloPacket->getDesignatedRouter();
//                    dRouterID.ipInterfaceAddress = dRouterID.routerID;

//                    OSPFv3Neighbor* designated = intf->getNeighborByAddress(dRouterID);
                    OSPFv3Neighbor* designated = process->getNeighborByIntfIdAndNeigId(dRouterID,intf->getInterfaceId());

                    // Get the Designated Router ID from the corresponding Neighbor Object.
                    if (designated != NULL) {
                        if (designated->getNeighborID() != dRouterID/*.routerID*/) {
                            dRouterID/*.routerID*/ = designated->getNeighborID();
                        }
                        designatedSetUp = true;
                    }
                    neighbor->setDesignatedRouter(dRouterID);

                    dRouterID/*.routerID*/ = helloPacket->getBackupDesignatedRouter();
//                    dRouterID.ipInterfaceAddress = dRouterID.routerID;

                    OSPFv3Neighbor* backup = process->getNeighborByIntfIdAndNeigId(dRouterID,intf->getInterfaceId());

                    // Get the Backup Designated Router ID from the corresponding Neighbor Object.
                    if (backup != NULL) {
                        if (backup->getNeighborID() != dRouterID/*.routerID*/) {
                            dRouterID/*.routerID*/ = backup->getNeighborID();
                        }
                        backupSetUp = true;
                    }
                    neighbor->setBackupDesignatedRouter(dRouterID);
                    if (designatedSetUp && backupSetUp) {
                        neighbor->setUpDesignatedRouters(true);
                    }
                    process->addNeighbor(neighbor);
                }

                neighbor->processEvent(OSPFv3Neighbor::HELLO_RECEIVED);
                if ((interfaceType == OSPFv3InterfaceIPv6::NBMA) &&
                    (intf->getRouterPriority() == 0) &&
                    (neighbor->getState() >= OSPFv3Neighbor::STATE_INIT))
                {
                    intf->sendHelloPacket(neighbor->getNeighborAddress());
                }

//                IPv4Address interfaceAddress = intf->getAddressRange().address;
                unsigned int neighborsNeighborCount = helloPacket->getNeighborIDArraySize();
                unsigned int i;
                /* The list of neighbors contained in the Hello Packet is
                   examined.  If the router itself appears in this list, the
                   neighbor state machine should be executed with the event TWOWAY_RECEIVED.
                 */
                for (i = 0; i < neighborsNeighborCount; i++) {
                    if (helloPacket->getNeighborID(i) == process->getRouterID()) {
                        neighbor->processEvent(OSPFv3Neighbor::TWOWAY_RECEIVED);
                        break;
                    }
                }
                /* Otherwise, the neighbor state machine should
                   be executed with the event ONEWAY_RECEIVED, and the processing
                   of the packet stops.
                 */
                if (i == neighborsNeighborCount) {
                    neighbor->processEvent(OSPFv3Neighbor::ONEWAY_RECEIVED);
                }

                if (neighborChanged) {
                    intf->processEvent(OSPFv3InterfaceIPv6::NEIGHBOR_CHANGE);
                    /* In some cases neighbors get stuck in TwoWay state after a DR
                       or Backup change. (calculateDesignatedRouter runs before the
                       neighbors' signal of DR change + this router does not become
                       neither DR nor backup -> IS_ADJACENCY_OK does not get called.)
                       So to make it work(workaround) we'll call IS_ADJACENCY_OK for
                       all neighbors in TwoWay state from here. This shouldn't break
                       anything because if the neighbor state doesn't have to change
                       then needAdjacency returns false and nothing happnes in
                       IS_ADJACENCY_OK.
                     */
                    std::vector<OSPFv3Neighbor*> neighbors;
                    process->getNeighborsByIntfId(intf->getInterfaceId(),&neighbors);
                    unsigned int neighborCount = neighbors.size();
                    for (i = 0; i < neighborCount; i++) {
                        if (neighbors[i]->getState() == OSPFv3Neighbor::STATE_TWOWAY) {
                            neighbors[i]->processEvent(OSPFv3Neighbor::IS_ADJACENCY_OK);
                        }
                    }

                    if (neighborsDRStateChanged) {
/*                        OSPF::RouterLSA* routerLSA = intf->getArea()->findRouterLSA(router->getRouterID());

                        if (routerLSA != NULL) {
                            long sequenceNumber = routerLSA->getHeader().getLsSequenceNumber();
                            if (sequenceNumber == MAX_SEQUENCE_NUMBER) {
                                routerLSA->getHeader().setLsAge(MAX_AGE);
                                intf->getArea()->floodLSA(routerLSA);
                                routerLSA->incrementInstallTime();
                            } else {
                                OSPF::RouterLSA* newLSA = intf->getArea()->originateRouterLSA();

                                newLSA->getHeader().setLsSequenceNumber(sequenceNumber + 1);
                                shouldRebuildRoutingTable |= routerLSA->update(newLSA);
                                delete newLSA;

                                intf->getArea()->floodLSA(routerLSA);
                            }
                        }
*/                    }
                }

                if (backupSeen) {
                    intf->processEvent(OSPFv3InterfaceIPv6::BACKUP_SEEN);
                }
//            }

    }

    if (shouldRebuildRoutingTable) {
//        router->rebuildRoutingTable();
    }
}

