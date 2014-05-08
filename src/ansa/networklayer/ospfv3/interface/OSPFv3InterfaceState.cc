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
/*

#include <map>
*/
#include "OSPFv3InterfaceState.h"
/*
#include "OSPFArea.h"
#include "OSPFInterface.h"
#include "OSPFInterfaceStateBackup.h"
#include "OSPFInterfaceStateDesignatedRouter.h"
#include "OSPFInterfaceStateNotDesignatedRouter.h"
#include "OSPFRouter.h"

*/

void OSPFv3InterfaceState::changeState(
//        OSPFv3InterfaceIPv6* intf, OSPFv3InterfaceState* newState, OSPFv3InterfaceState* currentState)
        OSPFv3InterfaceIPv6* intf, int/*OSPFv3InterfaceIPv6::InterfaceStateType*/ next_State,
        int/*OSPFv3InterfaceIPv6::InterfaceStateType*/ old_State)
{
    prevState=old_State;
    OSPFv3InterfaceIPv6::InterfaceStateType oldState = (OSPFv3InterfaceIPv6::InterfaceStateType) prevState;
    state=next_State;
    OSPFv3InterfaceIPv6::InterfaceStateType nextState = (OSPFv3InterfaceIPv6::InterfaceStateType) state;
    OSPFv3InterfaceIPv6::OSPFv3InterfaceType intfType = intf->getType();
    bool shouldRebuildRoutingTable = false;

    if ((oldState == OSPFv3InterfaceIPv6::STATE_DOWN) ||
        (nextState == OSPFv3InterfaceIPv6::STATE_DOWN) ||
        (oldState == OSPFv3InterfaceIPv6::STATE_LOOPBACK) ||
        (nextState == OSPFv3InterfaceIPv6::STATE_LOOPBACK) ||
        (oldState == OSPFv3InterfaceIPv6::STATE_DESIGNATED_ROUTER) ||
        (nextState == OSPFv3InterfaceIPv6::STATE_DESIGNATED_ROUTER) ||
        ((intfType == OSPFv3InterfaceIPv6::POINTTOPOINT) &&
         ((oldState == OSPFv3InterfaceIPv6::STATE_POINTTOPOINT) ||
          (nextState == OSPFv3InterfaceIPv6::STATE_POINTTOPOINT))) ||
        (((intfType == OSPFv3InterfaceIPv6::BROADCAST) ||
          (intfType == OSPFv3InterfaceIPv6::NBMA)) &&
         ((oldState == OSPFv3InterfaceIPv6::STATE_WAITING) ||
          (nextState == OSPFv3InterfaceIPv6::STATE_WAITING))))
    {
/*        OSPF::RouterLSA* routerLSA = intf->getArea()->findRouterLSA(intf->getArea()->getRouter()->getRouterID());

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
        } else {  // (lsa == NULL) -> This must be the first time any interface is up...
            OSPF::RouterLSA* newLSA = intf->getArea()->originateRouterLSA();

            shouldRebuildRoutingTable |= intf->getArea()->installRouterLSA(newLSA);

            routerLSA = intf->getArea()->findRouterLSA(intf->getArea()->getRouter()->getRouterID());

            intf->getArea()->setSPFTreeRoot(routerLSA);
            intf->getArea()->floodLSA(newLSA);
            delete newLSA;
        }
*/    }

    if (nextState == OSPFv3InterfaceIPv6::STATE_DESIGNATED_ROUTER) {
 /*       OSPF::NetworkLSA* newLSA = intf->getArea()->originateNetworkLSA(intf);
        if (newLSA != NULL) {
            shouldRebuildRoutingTable |= intf->getArea()->installNetworkLSA(newLSA);

            intf->getArea()->floodLSA(newLSA);
            delete newLSA;
        } else {    // no neighbors on the network -> old NetworkLSA must be flushed
            OSPF::NetworkLSA* oldLSA = intf->getArea()->findNetworkLSA(intf->getAddressRange().address);

            if (oldLSA != NULL) {
                oldLSA->getHeader().setLsAge(MAX_AGE);
                intf->getArea()->floodLSA(oldLSA);
                oldLSA->incrementInstallTime();
            }
        }
*/    }

    if (oldState == OSPFv3InterfaceIPv6::STATE_DESIGNATED_ROUTER) {
  /*      OSPF::NetworkLSA* networkLSA = intf->getArea()->findNetworkLSA(intf->getAddressRange().address);

        if (networkLSA != NULL) {
            networkLSA->getHeader().setLsAge(MAX_AGE);
            intf->getArea()->floodLSA(networkLSA);
            networkLSA->incrementInstallTime();
        }
*/    }

    if (shouldRebuildRoutingTable) {
 //       intf->getArea()->getRouter()->rebuildRoutingTable();
    }
}

void OSPFv3InterfaceState::calculateDesignatedRouter(OSPFv3InterfaceIPv6* intf)
{
    IPv4Address routerID = intf->getArea()->getProcess()->getRouterID();
    IPv4Address currentDesignatedRouter = intf->getDesignatedRouter();
    IPv4Address currentBackupRouter = intf->getBackupDesignatedRouter();

    std::vector<OSPFv3Neighbor*> neighbors;
    intf->getArea()->getProcess()->getNeighborsByIntfId(intf->getInterfaceId(),&neighbors);
    unsigned int neighborCount = neighbors.size();
    unsigned char repeatCount = 0;
    unsigned int i;

    IPv4Address declaredBackup;
    unsigned char declaredBackupPriority;
    IPv4Address declaredBackupID;
    bool backupDeclared;

    IPv4Address declaredDesignatedRouter;
    unsigned char declaredDesignatedRouterPriority;
    IPv4Address declaredDesignatedRouterID;
    bool designatedRouterDeclared;

    do {
        // calculating backup designated router
        declaredBackup = NULL_IPV4ADDRESS;
        declaredBackupPriority = 0;
        declaredBackupID = NULL_IPV4ADDRESS;
        backupDeclared = false;

        IPv4Address highestRouter = NULL_IPV4ADDRESS;
        unsigned char highestPriority = 0;
        IPv4Address highestID = NULL_IPV4ADDRESS;

        for (i = 0; i < neighborCount; i++) {
            OSPFv3Neighbor* neighbor = neighbors[i];
            unsigned char neighborPriority = neighbor->getPriority();

            if (neighbor->getState() < OSPFv3Neighbor::STATE_TWOWAY) {
                continue;
            }
            if (neighborPriority == 0) {
                continue;
            }

            IPv4Address neighborID = neighbor->getNeighborID();
            IPv4Address neighborsDesignatedRouter = neighbor->getDesignatedRouter();
            IPv4Address neighborsBackupDesignatedRouter = neighbor->getBackupDesignatedRouter();

            if (neighborsDesignatedRouter != neighborID) {
                if (neighborsBackupDesignatedRouter == neighborID) {
                    if ((neighborPriority > declaredBackupPriority) ||
                        ((neighborPriority == declaredBackupPriority) &&
                         (neighborID > declaredBackup)))
                    {
                        declaredBackup = neighborsBackupDesignatedRouter;
                        declaredBackupPriority = neighborPriority;
                        declaredBackupID = neighborID;
                        backupDeclared = true;
                    }
                }
                if (!backupDeclared) {
                    if ((neighborPriority > highestPriority) ||
                        ((neighborPriority == highestPriority) &&
                         (neighborID > highestRouter)))
                    {
                        highestRouter = neighborID;
 //                       highestRouter.ipInterfaceAddress = neighbor->getAddress();
                        highestPriority = neighborPriority;
                        highestID = neighborID;
                    }
                }
            }
        }
        // also include the router itself in the calculations
        if (intf->getRouterPriority() > 0) {
            if (currentDesignatedRouter != routerID) {
                if (currentBackupRouter == routerID) {
                    if ((intf->getRouterPriority() > declaredBackupPriority) ||
                        ((intf->getRouterPriority() == declaredBackupPriority) &&
                         (routerID > declaredBackupID)))
                    {
                        declaredBackup = routerID;
//                        declaredBackup.ipInterfaceAddress = intf->interfaceAddressRange.address;
                        declaredBackupPriority = intf->getRouterPriority();
                        declaredBackupID = routerID;
                        backupDeclared = true;
                    }

                }
                if (!backupDeclared) {
                    if ((intf->getRouterPriority() > highestPriority) ||
                        ((intf->getRouterPriority() == highestPriority) &&
                         (routerID > highestRouter)))
                    {
                        declaredBackup = routerID;
//                        declaredBackup.ipInterfaceAddress = intf->interfaceAddressRange.address;
                        declaredBackupPriority = intf->getRouterPriority();
                        declaredBackupID = routerID;
                        backupDeclared = true;
                    } else {
                        declaredBackup = highestRouter;
                        declaredBackupPriority = highestPriority;
                        declaredBackupID = highestID;
                        backupDeclared = true;
                    }
                }
            }
        }

        // calculating backup designated router
        declaredDesignatedRouter = NULL_IPV4ADDRESS;
        declaredDesignatedRouterPriority = 0;
        declaredDesignatedRouterID = NULL_IPV4ADDRESS;
        designatedRouterDeclared = false;

        for (i = 0; i < neighborCount; i++) {
            OSPFv3Neighbor* neighbor = neighbors[i];
            unsigned char neighborPriority = neighbor->getPriority();

            if (neighbor->getState() < OSPFv3Neighbor::STATE_TWOWAY) {
                continue;
            }
            if (neighborPriority == 0) {
                continue;
            }

            IPv4Address neighborID = neighbor->getNeighborID();
            IPv4Address neighborsDesignatedRouter = neighbor->getDesignatedRouter();
            IPv4Address neighborsBackupDesignatedRouter = neighbor->getBackupDesignatedRouter();

            if (neighborsDesignatedRouter == neighborID) {
                if ((neighborPriority > declaredDesignatedRouterPriority) ||
                    ((neighborPriority == declaredDesignatedRouterPriority) &&
                     (neighborID > declaredDesignatedRouterID)))
                {
                    declaredDesignatedRouter = neighborsDesignatedRouter;
                    declaredDesignatedRouterPriority = neighborPriority;
                    declaredDesignatedRouterID = neighborID;
                    designatedRouterDeclared = true;
                }
            }
        }
        // also include the router itself in the calculations
        if (intf->getRouterPriority() > 0) {
            if (currentDesignatedRouter == routerID) {
                if ((intf->getRouterPriority() > declaredDesignatedRouterPriority) ||
                    ((intf->getRouterPriority() == declaredDesignatedRouterPriority) &&
                     (routerID > declaredDesignatedRouterID)))
                {
                    declaredDesignatedRouter = routerID;
//                    declaredDesignatedRouter.ipInterfaceAddress = intf->interfaceAddressRange.address;
                    declaredDesignatedRouterPriority = intf->getRouterPriority();
                    declaredDesignatedRouterID = routerID;
                    designatedRouterDeclared = true;
                }

            }
        }
        if (!designatedRouterDeclared) {
            declaredDesignatedRouter = declaredBackup;
            declaredDesignatedRouterPriority = declaredBackupPriority;
            declaredDesignatedRouterID = declaredBackupID;
            designatedRouterDeclared = true;
        }

        // if the router is any kind of DR or is no longer one of them, then repeat
        if (
            (
                (declaredDesignatedRouter != NULL_IPV4ADDRESS) &&
                ((
                    (currentDesignatedRouter == routerID) &&
                    (declaredDesignatedRouter != routerID)
                ) ||
                (
                    (currentDesignatedRouter != routerID) &&
                    (declaredDesignatedRouter == routerID)
                ))
            ) ||
            (
                (declaredBackup != NULL_IPV4ADDRESS) &&
                ((
                    (currentBackupRouter == routerID) &&
                    (declaredBackup != routerID)
                ) ||
                (
                    (currentBackupRouter != routerID) &&
                    (declaredBackup == routerID)
                ))
            )
        )
        {
            currentDesignatedRouter = declaredDesignatedRouter;
            currentBackupRouter = declaredBackup;
            repeatCount++;
        } else {
            repeatCount += 2;
        }

    } while (repeatCount < 2);

    IPv4Address routersOldDesignatedRouterID = intf->getDesignatedRouter();
    IPv4Address routersOldBackupID = intf->getBackupDesignatedRouter();

    intf->setDesignatedRouter(declaredDesignatedRouter);
    intf->setBackupDesignatedRouter(declaredBackup);

    bool wasBackupDesignatedRouter = (routersOldBackupID == routerID);
    bool wasDesignatedRouter = (routersOldDesignatedRouterID == routerID);
    bool wasOther = (intf->getState() == OSPFv3InterfaceIPv6::STATE_NOT_DESIGNATED_ROUTER);
    bool wasWaiting = (!wasBackupDesignatedRouter && !wasDesignatedRouter && !wasOther);
    bool isBackupDesignatedRouter = (declaredBackup == routerID);
    bool isDesignatedRouter = (declaredDesignatedRouter == routerID);
    bool isOther = (!isBackupDesignatedRouter && !isDesignatedRouter);

    if (wasBackupDesignatedRouter) {
        if (isDesignatedRouter) {
            changeState(intf, OSPFv3InterfaceIPv6::STATE_DESIGNATED_ROUTER, state);
        }
        if (isOther) {
            changeState(intf, OSPFv3InterfaceIPv6::STATE_NOT_DESIGNATED_ROUTER, state);
        }
    }
    if (wasDesignatedRouter) {
        if (isBackupDesignatedRouter) {
            changeState(intf, OSPFv3InterfaceIPv6::STATE_BACKUP, state);
        }
        if (isOther) {
            changeState(intf, OSPFv3InterfaceIPv6::STATE_NOT_DESIGNATED_ROUTER, state);
        }
    }
    if (wasOther) {
        if (isDesignatedRouter) {
            changeState(intf, OSPFv3InterfaceIPv6::STATE_DESIGNATED_ROUTER, state);
        }
        if (isBackupDesignatedRouter) {
            changeState(intf, OSPFv3InterfaceIPv6::STATE_BACKUP, state);
        }
    }
    if (wasWaiting) {
        if (isDesignatedRouter) {
            changeState(intf, OSPFv3InterfaceIPv6::STATE_DESIGNATED_ROUTER, state);
        }
        if (isBackupDesignatedRouter) {
            changeState(intf, OSPFv3InterfaceIPv6::STATE_BACKUP, state);
        }
        if (isOther) {
            changeState(intf, OSPFv3InterfaceIPv6::STATE_NOT_DESIGNATED_ROUTER, state);
        }
    }

    for (i = 0; i < neighborCount; i++) {
        if ((intf->getType() == OSPFv3InterfaceIPv6::NBMA) &&
            ((!wasBackupDesignatedRouter && isBackupDesignatedRouter) ||
             (!wasDesignatedRouter && isDesignatedRouter)))
        {
            if (neighbors[i]->getPriority() == 0) {
                neighbors[i]->processEvent(OSPFv3Neighbor::START);
            }
        }
        if ((declaredDesignatedRouter != routersOldDesignatedRouterID) ||
            (declaredBackup != routersOldBackupID))
        {
            if (neighbors[i]->getState() >= OSPFv3Neighbor::STATE_TWOWAY) {
                neighbors[i]->processEvent(OSPFv3Neighbor::IS_ADJACENCY_OK);
            }
        }
    }
}

void OSPFv3InterfaceState::processEvent(OSPFv3InterfaceIPv6* intf,
        int/*OSPFv3InterfaceIPv6::InterfaceEventType*/ event)
{
    switch (state) {
        case OSPFv3InterfaceIPv6::STATE_DOWN:
            processEventDown(intf,event);break;
        case OSPFv3InterfaceIPv6::STATE_LOOPBACK:
            processEventLoopback(intf,event);break;
        case OSPFv3InterfaceIPv6::STATE_WAITING:
            processEventWaiting(intf,event);break;
        case OSPFv3InterfaceIPv6::STATE_POINTTOPOINT:
            processEventPointToPoint(intf,event);break;
        case OSPFv3InterfaceIPv6::STATE_NOT_DESIGNATED_ROUTER:
            processEventNotDesignatedRouter(intf,event);break;
        case OSPFv3InterfaceIPv6::STATE_BACKUP:
            processEventBackup(intf,event);break;
        case OSPFv3InterfaceIPv6::STATE_DESIGNATED_ROUTER:
            processEventDesignatedRouter(intf,event);break;
    }
}

void OSPFv3InterfaceState::processEventDown(OSPFv3InterfaceIPv6* intf,
        int/*OSPFv3InterfaceIPv6::InterfaceEventType*/ event)
{
    if (event == OSPFv3InterfaceIPv6::INTERFACE_UP) {
        OSPFv3MessageHandler* messageHandler = intf->getArea()->getProcess()->getMessageHandler();
        messageHandler->startTimer(intf->getHelloTimer(), truncnormal(0.1, 0.01)); // add some deviation to avoid startup collisions
//        messageHandler->startTimer(intf->getAcknowledgementTimer(), intf->getAcknowledgementDelay());
        switch (intf->getType()) {
            case OSPFv3InterfaceIPv6::POINTTOPOINT:
            case OSPFv3InterfaceIPv6::POINTTOMULTIPOINT:
            case OSPFv3InterfaceIPv6::VIRTUAL:
                changeState(intf, OSPFv3InterfaceIPv6::STATE_POINTTOPOINT, state);
                break;

            case OSPFv3InterfaceIPv6::NBMA:
                if (intf->getRouterPriority() == 0) {
                    changeState(intf, OSPFv3InterfaceIPv6::STATE_NOT_DESIGNATED_ROUTER, state);
                } else {
                    changeState(intf, OSPFv3InterfaceIPv6::STATE_WAITING, state);
                    messageHandler->startTimer(intf->getWaitTimer(), intf->getDeadInt());
                    std::vector<OSPFv3Neighbor*> neighbors;
                    intf->getArea()->getProcess()->getNeighborsByIntfId(intf->getInterfaceId(),&neighbors);
                    long neighborCount = neighbors.size();
                    for (long i = 0; i < neighborCount; i++) {
                        if (neighbors[i]->getPriority() > 0) {
                            neighbors[i]->processEvent(OSPFv3Neighbor::START);
                        }
                    }
                }
                break;

            case OSPFv3InterfaceIPv6::BROADCAST:
                if (intf->getRouterPriority() == 0) {
                    changeState(intf, OSPFv3InterfaceIPv6::STATE_NOT_DESIGNATED_ROUTER,state);
                } else {
                    changeState(intf, OSPFv3InterfaceIPv6::STATE_WAITING, state);
                    messageHandler->startTimer(intf->getWaitTimer(), intf->getDeadInt());
                }
                break;

            default:
                break;
        }
    }
    if (event == OSPFv3InterfaceIPv6::LOOP_INDICATION) {
        intf->reset();
        changeState(intf, OSPFv3InterfaceIPv6::STATE_LOOPBACK, state);
    }
}
void OSPFv3InterfaceState::processEventLoopback(OSPFv3InterfaceIPv6* intf,
        int/*OSPFv3InterfaceIPv6::InterfaceEventType*/ event)
{
    if (event == OSPFv3InterfaceIPv6::INTERFACE_DOWN) {
        intf->reset();
        changeState(intf, OSPFv3InterfaceIPv6::INTERFACE_DOWN, state);
    }
    if (event == OSPFv3InterfaceIPv6::UNLOOP_INDICATION) {
        changeState(intf, OSPFv3InterfaceIPv6::INTERFACE_DOWN, state);
    }
}

void OSPFv3InterfaceState::processEventWaiting(OSPFv3InterfaceIPv6* intf,
        int/*OSPFv3InterfaceIPv6::InterfaceEventType*/ event)
{
    if ((event == OSPFv3InterfaceIPv6::BACKUP_SEEN) ||
        (event == OSPFv3InterfaceIPv6::WAIT_TIMER))
    {
        calculateDesignatedRouter(intf);
    }
    if (event == OSPFv3InterfaceIPv6::INTERFACE_DOWN) {
        intf->reset();
        changeState(intf, OSPFv3InterfaceIPv6::STATE_DOWN, state);
    }
    if (event == OSPFv3InterfaceIPv6::LOOP_INDICATION) {
        intf->reset();
        changeState(intf, OSPFv3InterfaceIPv6::STATE_LOOPBACK, state);
    }
    if (event == OSPFv3InterfaceIPv6::HELLO_TIMER) {
        if (intf->getType() == OSPFv3InterfaceIPv6::BROADCAST) {
            intf->sendHelloPacket(intf->getArea()->getProcess()->OSPFV3_IPV6_MULT);
        } else {    // OSPFv3InterfaceIPv6::NBMA
            std::vector<OSPFv3Neighbor*> neighbors;
            intf->getArea()->getProcess()->getNeighborsByIntfId(intf->getInterfaceId(),&neighbors);
            long neighborCount = neighbors.size();
            for (long i = 0; i < neighborCount; i++) {
                if (neighbors[i]->getPriority() > 0) {
                    intf->sendHelloPacket(neighbors[i]->getNeighborAddress(), intf->getHopLimit());
                }
            }
        }
        intf->getArea()->getProcess()->getMessageHandler()->startTimer(
                intf->getHelloTimer(), intf->getHelloInt());
    }
    if (event == OSPFv3InterfaceIPv6::ACKNOWLEDGEMENT_TIMER) {
//        intf->sendDelayedAcknowledgements();
    }
}

void OSPFv3InterfaceState::processEventPointToPoint(OSPFv3InterfaceIPv6* intf,
        int/*OSPFv3InterfaceIPv6::InterfaceEventType*/ event)
{
    if (event == OSPFv3InterfaceIPv6::INTERFACE_DOWN) {
        intf->reset();
        changeState(intf, OSPFv3InterfaceIPv6::STATE_DOWN, state);
    }
    if (event == OSPFv3InterfaceIPv6::LOOP_INDICATION) {
        intf->reset();
        changeState(intf, OSPFv3InterfaceIPv6::STATE_LOOPBACK, state);
    }
    if (event == OSPFv3InterfaceIPv6::HELLO_TIMER) {
        if (intf->getType() == OSPFv3InterfaceIPv6::VIRTUAL) {
            std::vector<OSPFv3Neighbor*> neighbors;
            intf->getArea()->getProcess()->getNeighborsByIntfId(intf->getInterfaceId(),&neighbors);
            long neighborCount = neighbors.size();
            if (neighborCount > 0) {
                intf->sendHelloPacket(neighbors[0]->getNeighborAddress(), intf->getHopLimit());
            }
        } else {
            intf->sendHelloPacket(IPv6Address(intf->getArea()->getProcess()->OSPFV3_IPV6_MULT));
        }
        intf->getArea()->getProcess()->getMessageHandler()->startTimer(
                intf->getHelloTimer(), intf->getHelloInt());
    }
    if (event == OSPFv3InterfaceIPv6::ACKNOWLEDGEMENT_TIMER) {
//        intf->sendDelayedAcknowledgements();
    }
}

void OSPFv3InterfaceState::processEventNotDesignatedRouter(OSPFv3InterfaceIPv6* intf,
        int/*OSPFv3InterfaceIPv6::InterfaceEventType*/ event)
{
    if (event == OSPFv3InterfaceIPv6::NEIGHBOR_CHANGE) {
        calculateDesignatedRouter(intf);
    }
    if (event == OSPFv3InterfaceIPv6::INTERFACE_DOWN) {
        intf->reset();
        changeState(intf, OSPFv3InterfaceIPv6::STATE_DOWN, state);
    }
    if (event == OSPFv3InterfaceIPv6::LOOP_INDICATION) {
        intf->reset();
        changeState(intf, OSPFv3InterfaceIPv6::STATE_LOOPBACK, state);
    }
    if (event == OSPFv3InterfaceIPv6::HELLO_TIMER) {
        if (intf->getType() == OSPFv3InterfaceIPv6::BROADCAST) {
            intf->sendHelloPacket(intf->getArea()->getProcess()->OSPFV3_IPV6_MULT);
        } else {    // OSPFv3InterfaceIPv6::NBMA
            std::vector<OSPFv3Neighbor*> neighbors;
            intf->getArea()->getProcess()->getNeighborsByIntfId(intf->getInterfaceId(),&neighbors);
            long neighborCount = neighbors.size();
            for (long i = 0; i < neighborCount; i++) {
                if (intf->getRouterPriority() > 0) {
                    if (neighbors[i]->getPriority() > 0) {
                        intf->sendHelloPacket(neighbors[i]->getNeighborAddress(), intf->getHopLimit());
                    }
                }
                else{
                    if ((intf->getDesignatedRouter()==neighbors[i]->getNeighborID())
                            ||(intf->getBackupDesignatedRouter()==neighbors[i]->getNeighborID()))
                    {
                        intf->sendHelloPacket(neighbors[i]->getNeighborAddress());
                    }
                }
            }
        }
        intf->getArea()->getProcess()->getMessageHandler()->startTimer(
                intf->getHelloTimer(), intf->getHelloInt());
    }
    if (event == OSPFv3InterfaceIPv6::ACKNOWLEDGEMENT_TIMER) {
//        intf->sendDelayedAcknowledgements();
    }
}

void OSPFv3InterfaceState::processEventBackup(OSPFv3InterfaceIPv6* intf,
        int/*OSPFv3InterfaceIPv6::InterfaceEventType*/ event){
    if (event == OSPFv3InterfaceIPv6::NEIGHBOR_CHANGE) {
        calculateDesignatedRouter(intf);
    }
    if (event == OSPFv3InterfaceIPv6::INTERFACE_DOWN) {
        intf->reset();
        changeState(intf, OSPFv3InterfaceIPv6::STATE_DOWN, state);
    }
    if (event == OSPFv3InterfaceIPv6::LOOP_INDICATION) {
        intf->reset();
        changeState(intf, OSPFv3InterfaceIPv6::STATE_LOOPBACK, state);
    }
    if (event == OSPFv3InterfaceIPv6::HELLO_TIMER) {
        if (intf->getType() == OSPFv3InterfaceIPv6::BROADCAST) {
            intf->sendHelloPacket(intf->getArea()->getProcess()->OSPFV3_IPV6_MULT);
        } else {    // OSPFv3InterfaceIPv6::NBMA
             std::vector<OSPFv3Neighbor*> neighbors;
             intf->getArea()->getProcess()->getNeighborsByIntfId(intf->getInterfaceId(),&neighbors);
             long neighborCount = neighbors.size();
             for (long i = 0; i < neighborCount; i++) {
                 if (neighbors[i]->getPriority() > 0) {
                     intf->sendHelloPacket(neighbors[i]->getNeighborAddress(), intf->getHopLimit());
                 }
             }
        }
        intf->getArea()->getProcess()->getMessageHandler()->startTimer(
                intf->getHelloTimer(), intf->getHelloInt());
    }
    if (event == OSPFv3InterfaceIPv6::ACKNOWLEDGEMENT_TIMER) {
//        intf->sendDelayedAcknowledgements();
    }
}

void OSPFv3InterfaceState::processEventDesignatedRouter(OSPFv3InterfaceIPv6* intf,
        int/*OSPFv3InterfaceIPv6::InterfaceEventType*/ event)
{
    if (event == OSPFv3InterfaceIPv6::NEIGHBOR_CHANGE) {
        calculateDesignatedRouter(intf);
    }
    if (event == OSPFv3InterfaceIPv6::INTERFACE_DOWN) {
        intf->reset();
        changeState(intf, OSPFv3InterfaceIPv6::STATE_DOWN, state);
    }
    if (event == OSPFv3InterfaceIPv6::LOOP_INDICATION) {
        intf->reset();
        changeState(intf, OSPFv3InterfaceIPv6::STATE_LOOPBACK, state);
    }
    if (event == OSPFv3InterfaceIPv6::HELLO_TIMER) {
        if (intf->getType() == OSPFv3InterfaceIPv6::BROADCAST) {
            intf->sendHelloPacket(intf->getArea()->getProcess()->OSPFV3_IPV6_MULT);
        } else {    // OSPFv3InterfaceIPv6::NBMA
            std::vector<OSPFv3Neighbor*> neighbors;
             intf->getArea()->getProcess()->getNeighborsByIntfId(intf->getInterfaceId(),&neighbors);
             long neighborCount = neighbors.size();
             for (long i = 0; i < neighborCount; i++) {
                 if (neighbors[i]->getPriority() > 0) {
                     intf->sendHelloPacket(neighbors[i]->getNeighborAddress(), intf->getHopLimit());
                 }
             }
        }
        intf->getArea()->getProcess()->getMessageHandler()->startTimer(
                intf->getHelloTimer(), intf->getHelloInt());
    }
    if (event == OSPFv3InterfaceIPv6::ACKNOWLEDGEMENT_TIMER) {
//        intf->sendDelayedAcknowledgements();
    }
}
