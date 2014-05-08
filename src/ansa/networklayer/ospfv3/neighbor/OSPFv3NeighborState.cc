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


#include "OSPFv3NeighborState.h"
/*
#include "OSPFArea.h"
#include "OSPFInterface.h"
#include "OSPFRouter.h"
*/

void OSPFv3NeighborState::changeState(OSPFv3Neighbor* neighbor,
        int/*OSPFv3Neighbor::NeighborStateType*/ newState, int/*OSPFv3Neighbor::NeighborStateType*/ currentState)
{
    prevState=currentState;
    OSPFv3Neighbor::NeighborStateType oldState = (OSPFv3Neighbor::NeighborStateType) prevState;
    state=newState;
    OSPFv3Neighbor::NeighborStateType nextState = (OSPFv3Neighbor::NeighborStateType) state;
    bool shouldRebuildRoutingTable = false;


    if ((oldState == OSPFv3Neighbor::STATE_FULL) || (nextState == OSPFv3Neighbor::STATE_FULL)) {
        IPv4Address routerID = neighbor->getInterface()->getArea()->getProcess()->getRouterID();
 //       OSPF::RouterLSA* routerLSA = neighbor->getInterface()->getArea()->findRouterLSA(routerID);

/*        if (routerLSA != NULL) {
            long sequenceNumber = routerLSA->getHeader().getLsSequenceNumber();
            if (sequenceNumber == MAX_SEQUENCE_NUMBER) {
                routerLSA->getHeader().setLsAge(MAX_AGE);
                neighbor->getInterface()->getArea()->floodLSA(routerLSA);
                routerLSA->incrementInstallTime();
            } else {
                OSPF::RouterLSA* newLSA = neighbor->getInterface()->getArea()->originateRouterLSA();

                newLSA->getHeader().setLsSequenceNumber(sequenceNumber + 1);
                shouldRebuildRoutingTable |= routerLSA->update(newLSA);
                delete newLSA;

                neighbor->getInterface()->getArea()->floodLSA(routerLSA);
            }
 */       }

        if (neighbor->getInterface()->getState() == OSPFv3InterfaceIPv6::STATE_DESIGNATED_ROUTER) {
/*            OSPF::NetworkLSA* networkLSA = neighbor->getInterface()->getArea()->findNetworkLSA(neighbor->getInterface()->getAddressRange().address);

            if (networkLSA != NULL) {
                long sequenceNumber = networkLSA->getHeader().getLsSequenceNumber();
                if (sequenceNumber == MAX_SEQUENCE_NUMBER) {
                    networkLSA->getHeader().setLsAge(MAX_AGE);
                    neighbor->getInterface()->getArea()->floodLSA(networkLSA);
                    networkLSA->incrementInstallTime();
                } else {
                    OSPF::NetworkLSA* newLSA = neighbor->getInterface()->getArea()->originateNetworkLSA(neighbor->getInterface());

                    if (newLSA != NULL) {
                        newLSA->getHeader().setLsSequenceNumber(sequenceNumber + 1);
                        shouldRebuildRoutingTable |= networkLSA->update(newLSA);
                        delete newLSA;
                    } else {    // no neighbors on the network -> old NetworkLSA must be flushed
                        networkLSA->getHeader().setLsAge(MAX_AGE);
                        networkLSA->incrementInstallTime();
                    }

                    neighbor->getInterface()->getArea()->floodLSA(networkLSA);
                }
            }
        }
*/    }

    if (shouldRebuildRoutingTable) {
//        neighbor->getInterface()->getArea()->getProcess()->rebuildRoutingTable();
    }
}

void OSPFv3NeighborState::processEvent(OSPFv3Neighbor* neighbor, int/*OSPFv3Neighbor::NeighborEventType*/ event)
{
    switch (state) {
        case OSPFv3Neighbor::STATE_ATTEMPT:
            processEventAttempt(neighbor,event);break;
        case OSPFv3Neighbor::STATE_DOWN:
            processEventDown(neighbor,event);break;
        case OSPFv3Neighbor::STATE_EXCHANGE:
            processEventExchange(neighbor,event);break;
        case OSPFv3Neighbor::STATE_EXCHANGE_START:
            processEventExchangeStart(neighbor,event);break;
        case OSPFv3Neighbor::STATE_FULL:
            processEventFull(neighbor,event);break;
        case OSPFv3Neighbor::STATE_INIT:
            processEventInit(neighbor,event);break;
        case OSPFv3Neighbor::STATE_LOADING:
            processEventLoading(neighbor,event);break;
        case OSPFv3Neighbor::STATE_TWOWAY:
            processEventTwoWay(neighbor,event);break;
    }
}
void OSPFv3NeighborState::processEventAttempt(OSPFv3Neighbor* neighbor, int/*OSPFv3Neighbor::NeighborEventType*/ event)
{
    if ((event == OSPFv3Neighbor::KILL_NEIGHBOR) || (event == OSPFv3Neighbor::LINK_DOWN)) {
        OSPFv3MessageHandler* messageHandler = neighbor->getInterface()->getArea()->getProcess()->getMessageHandler();
        neighbor->reset();
        messageHandler->clearTimer(neighbor->getInactivityTimer());
        changeState(neighbor, OSPFv3Neighbor::STATE_DOWN, state);
    }
    if (event == OSPFv3Neighbor::INACTIVITY_TIMER) {
        neighbor->reset();
        if (neighbor->getInterface()->getType() == OSPFv3InterfaceIPv6::NBMA) {
            OSPFv3MessageHandler* messageHandler = neighbor->getInterface()->getArea()->getProcess()->getMessageHandler();
            messageHandler->startTimer(neighbor->getPollTimer(), neighbor->getInterface()->getPollInt());
        }
        changeState(neighbor, OSPFv3Neighbor::STATE_DOWN, state);
    }
    if (event == OSPFv3Neighbor::HELLO_RECEIVED) {
        OSPFv3MessageHandler* messageHandler = neighbor->getInterface()->getArea()->getProcess()->getMessageHandler();
        messageHandler->clearTimer(neighbor->getInactivityTimer());
        messageHandler->startTimer(neighbor->getInactivityTimer(), neighbor->getRouterDeadInterval());
        changeState(neighbor, OSPFv3Neighbor::STATE_INIT, state);
    }
}

void OSPFv3NeighborState::processEventDown(OSPFv3Neighbor* neighbor, int/*OSPFv3Neighbor::NeighborEventType*/ event)
{
    if (event == OSPFv3Neighbor::START) {
        OSPFv3MessageHandler* messageHandler = neighbor->getInterface()->getArea()->getProcess()->getMessageHandler();
        messageHandler->clearTimer(neighbor->getPollTimer());
        neighbor->getInterface()->sendHelloPacket(neighbor->getNeighborAddress(), neighbor->getInterface()->getHopLimit());
        messageHandler->startTimer(neighbor->getInactivityTimer(), neighbor->getRouterDeadInterval());
        changeState(neighbor, OSPFv3Neighbor::STATE_ATTEMPT, state);
    }
    if (event == OSPFv3Neighbor::HELLO_RECEIVED) {
        OSPFv3MessageHandler* messageHandler = neighbor->getInterface()->getArea()->getProcess()->getMessageHandler();
        messageHandler->clearTimer(neighbor->getPollTimer());
        messageHandler->startTimer(neighbor->getInactivityTimer(), neighbor->getRouterDeadInterval());
        changeState(neighbor, OSPFv3Neighbor::STATE_INIT, state);
    }
    if (event == OSPFv3Neighbor::POLL_TIMER) {
        neighbor->getInterface()->sendHelloPacket(neighbor->getNeighborAddress(), neighbor->getInterface()->getHopLimit());
        OSPFv3MessageHandler* messageHandler = neighbor->getInterface()->getArea()->getProcess()->getMessageHandler();
        messageHandler->startTimer(neighbor->getPollTimer(), neighbor->getInterface()->getPollInt());
    }
}
void OSPFv3NeighborState::processEventExchange(OSPFv3Neighbor* neighbor, int/*OSPFv3Neighbor::NeighborEventType*/ event)
{
    if ((event == OSPFv3Neighbor::KILL_NEIGHBOR) || (event == OSPFv3Neighbor::LINK_DOWN)) {
        OSPFv3MessageHandler* messageHandler = neighbor->getInterface()->getArea()->getProcess()->getMessageHandler();
        neighbor->reset();
        messageHandler->clearTimer(neighbor->getInactivityTimer());
        changeState(neighbor, OSPFv3Neighbor::STATE_DOWN, state);
    }
    if (event == OSPFv3Neighbor::INACTIVITY_TIMER) {
        neighbor->reset();
        if (neighbor->getInterface()->getType() == OSPFv3InterfaceIPv6::NBMA) {
            OSPFv3MessageHandler* messageHandler = neighbor->getInterface()->getArea()->getProcess()->getMessageHandler();
            messageHandler->startTimer(neighbor->getPollTimer(), neighbor->getInterface()->getPollInt());
        }
        changeState(neighbor, OSPFv3Neighbor::STATE_DOWN, state);
    }
    if (event == OSPFv3Neighbor::ONEWAY_RECEIVED) {
        neighbor->reset();
        changeState(neighbor, OSPFv3Neighbor::STATE_INIT, state);
    }
    if (event == OSPFv3Neighbor::HELLO_RECEIVED) {
        OSPFv3MessageHandler* messageHandler = neighbor->getInterface()->getArea()->getProcess()->getMessageHandler();
        messageHandler->clearTimer(neighbor->getInactivityTimer());
        messageHandler->startTimer(neighbor->getInactivityTimer(), neighbor->getRouterDeadInterval());
    }
    if (event == OSPFv3Neighbor::IS_ADJACENCY_OK) {
/*        if (!neighbor->needAdjacency()) {
            neighbor->reset();
            changeState(neighbor, OSPFv3Neighbor::STATE_TWOWAY, state);
        }
*/    }
    if ((event == OSPFv3Neighbor::SEQUENCE_NUMBER_MISMATCH) || (event == OSPFv3Neighbor::BAD_LINK_STATE_REQUEST)) {
        OSPFv3MessageHandler* messageHandler = neighbor->getInterface()->getArea()->getProcess()->getMessageHandler();
        neighbor->reset();
//        neighbor->incrementDDSequenceNumber();
//        neighbor->sendDatabaseDescriptionPacket(true);
//        messageHandler->startTimer(neighbor->getDDRetransmissionTimer(), neighbor->getInterface()->getRetransmissionInterval());
        changeState(neighbor, OSPFv3Neighbor::STATE_EXCHANGE_START, state);
    }
    if (event == OSPFv3Neighbor::EXCHANGE_DONE) {
/*        if (neighbor->isLinkStateRequestListEmpty()) {
            OSPFv3MessageHandler* messageHandler = neighbor->getInterface()->getArea()->getProcess()->getMessageHandler();
            messageHandler->startTimer(neighbor->getDDRetransmissionTimer(), neighbor->getRouterDeadInterval());
            neighbor->clearRequestRetransmissionTimer();
            changeState(neighbor, OSPFv3Neighbor::STATE_FULL, state);
        } else {
*/            OSPFv3MessageHandler* messageHandler = neighbor->getInterface()->getArea()->getProcess()->getMessageHandler();
            messageHandler->startTimer(neighbor->getDDRetransmissionTimer(), neighbor->getRouterDeadInterval());
            changeState(neighbor, OSPFv3Neighbor::STATE_LOADING, state);
//        }
    }
    if (event == OSPFv3Neighbor::UPDATE_RETRANSMISSION_TIMER) {
//        neighbor->retransmitUpdatePacket();
//        neighbor->startUpdateRetransmissionTimer();
    }
    if (event == OSPFv3Neighbor::REQUEST_RETRANSMISSION_TIMER) {
//        neighbor->sendLinkStateRequestPacket();
//        neighbor->startRequestRetransmissionTimer();
    }
}

void OSPFv3NeighborState::processEventExchangeStart(OSPFv3Neighbor* neighbor, int/*OSPFv3Neighbor::NeighborEventType*/ event)
{
    if ((event == OSPFv3Neighbor::KILL_NEIGHBOR) || (event == OSPFv3Neighbor::LINK_DOWN)) {
        OSPFv3MessageHandler* messageHandler = neighbor->getInterface()->getArea()->getProcess()->getMessageHandler();
        neighbor->reset();
        messageHandler->clearTimer(neighbor->getInactivityTimer());
        changeState(neighbor, OSPFv3Neighbor::STATE_DOWN, state);
    }
    if (event == OSPFv3Neighbor::INACTIVITY_TIMER) {
        neighbor->reset();
        if (neighbor->getInterface()->getType() == OSPFv3InterfaceIPv6::NBMA) {
            OSPFv3MessageHandler* messageHandler = neighbor->getInterface()->getArea()->getProcess()->getMessageHandler();
            messageHandler->startTimer(neighbor->getPollTimer(), neighbor->getInterface()->getPollInt());
        }
        changeState(neighbor, OSPFv3Neighbor::STATE_DOWN, state);
    }
    if (event == OSPFv3Neighbor::ONEWAY_RECEIVED) {
        neighbor->reset();
        changeState(neighbor, OSPFv3Neighbor::STATE_INIT, state);
    }
    if (event == OSPFv3Neighbor::HELLO_RECEIVED) {
        OSPFv3MessageHandler* messageHandler = neighbor->getInterface()->getArea()->getProcess()->getMessageHandler();
        messageHandler->clearTimer(neighbor->getInactivityTimer());
        messageHandler->startTimer(neighbor->getInactivityTimer(), neighbor->getRouterDeadInterval());
    }
    if (event == OSPFv3Neighbor::IS_ADJACENCY_OK) {
/*        if (!neighbor->needAdjacency()) {
            neighbor->reset();
            changeState(neighbor, OSPFv3Neighbor::STATE_TWOWAY, state);
        }
 */   }
    if (event == OSPFv3Neighbor::DD_RETRANSMISSION_TIMER) {
        OSPFv3MessageHandler* messageHandler = neighbor->getInterface()->getArea()->getProcess()->getMessageHandler();
//        neighbor->retransmitDatabaseDescriptionPacket();
//        messageHandler->startTimer(neighbor->getDDRetransmissionTimer(), neighbor->getInterface()->getRetransmissionInterval());
    }
    if (event == OSPFv3Neighbor::NEGOTIATION_DONE) {
//        neighbor->createDatabaseSummary();
//        neighbor->sendDatabaseDescriptionPacket();
        OSPFv3MessageHandler* messageHandler = neighbor->getInterface()->getArea()->getProcess()->getMessageHandler();
        messageHandler->clearTimer(neighbor->getDDRetransmissionTimer());
        changeState(neighbor, OSPFv3Neighbor::STATE_EXCHANGE, state);
    }
}

void OSPFv3NeighborState::processEventFull(OSPFv3Neighbor* neighbor, int/*OSPFv3Neighbor::NeighborEventType*/ event)
{
    if ((event == OSPFv3Neighbor::KILL_NEIGHBOR) || (event == OSPFv3Neighbor::LINK_DOWN)) {
        OSPFv3MessageHandler* messageHandler = neighbor->getInterface()->getArea()->getProcess()->getMessageHandler();
        neighbor->reset();
        messageHandler->clearTimer(neighbor->getInactivityTimer());
        changeState(neighbor, OSPFv3Neighbor::STATE_DOWN, state);
    }
    if (event == OSPFv3Neighbor::INACTIVITY_TIMER) {
        neighbor->reset();
        if (neighbor->getInterface()->getType() == OSPFv3InterfaceIPv6::NBMA) {
            OSPFv3MessageHandler* messageHandler = neighbor->getInterface()->getArea()->getProcess()->getMessageHandler();
            messageHandler->startTimer(neighbor->getPollTimer(), neighbor->getInterface()->getPollInt());
        }
        changeState(neighbor, OSPFv3Neighbor::STATE_DOWN, state);
    }
    if (event == OSPFv3Neighbor::ONEWAY_RECEIVED) {
        neighbor->reset();
        changeState(neighbor, OSPFv3Neighbor::STATE_INIT, state);
    }
    if (event == OSPFv3Neighbor::HELLO_RECEIVED) {
        OSPFv3MessageHandler* messageHandler = neighbor->getInterface()->getArea()->getProcess()->getMessageHandler();
        messageHandler->clearTimer(neighbor->getInactivityTimer());
        messageHandler->startTimer(neighbor->getInactivityTimer(), neighbor->getRouterDeadInterval());
    }
    if (event == OSPFv3Neighbor::IS_ADJACENCY_OK) {
/*        if (!neighbor->needAdjacency()) {
            neighbor->reset();
            changeState(neighbor, OSPFv3Neighbor::STATE_TWOWAY, state);
        }
*/    }
    if ((event == OSPFv3Neighbor::SEQUENCE_NUMBER_MISMATCH) || (event == OSPFv3Neighbor::BAD_LINK_STATE_REQUEST)) {
        OSPFv3MessageHandler* messageHandler = neighbor->getInterface()->getArea()->getProcess()->getMessageHandler();
        neighbor->reset();
//        neighbor->incrementDDSequenceNumber();
//        neighbor->sendDatabaseDescriptionPacket(true);
//        messageHandler->startTimer(neighbor->getDDRetransmissionTimer(), neighbor->getInterface()->getRetransmissionInterval());
        changeState(neighbor, OSPFv3Neighbor::STATE_EXCHANGE_START, state);
    }
    if (event == OSPFv3Neighbor::UPDATE_RETRANSMISSION_TIMER) {
//        neighbor->retransmitUpdatePacket();
//        neighbor->startUpdateRetransmissionTimer();
    }
    if (event == OSPFv3Neighbor::DD_RETRANSMISSION_TIMER) {
//        neighbor->deleteLastSentDDPacket();
    }
}

void OSPFv3NeighborState::processEventInit(OSPFv3Neighbor* neighbor, int/*OSPFv3Neighbor::NeighborEventType*/ event)
{
    if ((event == OSPFv3Neighbor::KILL_NEIGHBOR) || (event == OSPFv3Neighbor::LINK_DOWN)) {
        OSPFv3MessageHandler* messageHandler = neighbor->getInterface()->getArea()->getProcess()->getMessageHandler();
        neighbor->reset();
        messageHandler->clearTimer(neighbor->getInactivityTimer());
        changeState(neighbor, OSPFv3Neighbor::STATE_DOWN, state);
    }
    if (event == OSPFv3Neighbor::INACTIVITY_TIMER) {
        neighbor->reset();
        if (neighbor->getInterface()->getType() == OSPFv3InterfaceIPv6::NBMA) {
            OSPFv3MessageHandler* messageHandler = neighbor->getInterface()->getArea()->getProcess()->getMessageHandler();
            messageHandler->startTimer(neighbor->getPollTimer(), neighbor->getInterface()->getPollInt());
        }
        changeState(neighbor, OSPFv3Neighbor::STATE_DOWN, state);
    }
    if (event == OSPFv3Neighbor::HELLO_RECEIVED) {
        OSPFv3MessageHandler* messageHandler = neighbor->getInterface()->getArea()->getProcess()->getMessageHandler();
        messageHandler->clearTimer(neighbor->getInactivityTimer());
        messageHandler->startTimer(neighbor->getInactivityTimer(), neighbor->getRouterDeadInterval());
    }
    if (event == OSPFv3Neighbor::TWOWAY_RECEIVED) {
/*        if (neighbor->needAdjacency()) {
            OSPFv3MessageHandler* messageHandler = neighbor->getInterface()->getArea()->getProcess()->getMessageHandler();
            if (!(neighbor->isFirstAdjacencyInited())) {
                neighbor->initFirstAdjacency();
            } else {
                neighbor->incrementDDSequenceNumber();
            }
            neighbor->sendDatabaseDescriptionPacket(true);
            messageHandler->startTimer(neighbor->getDDRetransmissionTimer(), neighbor->getInterface()->getRetransmissionInterval());
            changeState(neighbor, OSPFv3Neighbor::STATE_EXCHANGE_START, state);
        } else {
*/            changeState(neighbor, OSPFv3Neighbor::STATE_TWOWAY, state);
//        }
    }
}

void OSPFv3NeighborState::processEventLoading(OSPFv3Neighbor* neighbor, int/*OSPFv3Neighbor::NeighborEventType*/ event)
{
    if ((event == OSPFv3Neighbor::KILL_NEIGHBOR) || (event == OSPFv3Neighbor::LINK_DOWN)) {
        OSPFv3MessageHandler* messageHandler = neighbor->getInterface()->getArea()->getProcess()->getMessageHandler();
        neighbor->reset();
        messageHandler->clearTimer(neighbor->getInactivityTimer());
        changeState(neighbor, OSPFv3Neighbor::STATE_DOWN, state);
    }
    if (event == OSPFv3Neighbor::INACTIVITY_TIMER) {
        neighbor->reset();
        if (neighbor->getInterface()->getType() == OSPFv3InterfaceIPv6::NBMA) {
            OSPFv3MessageHandler* messageHandler = neighbor->getInterface()->getArea()->getProcess()->getMessageHandler();
            messageHandler->startTimer(neighbor->getPollTimer(), neighbor->getInterface()->getPollInt());
        }
        changeState(neighbor, OSPFv3Neighbor::STATE_DOWN, state);
    }
    if (event == OSPFv3Neighbor::ONEWAY_RECEIVED) {
        neighbor->reset();
        changeState(neighbor, OSPFv3Neighbor::STATE_INIT, state);
    }
    if (event == OSPFv3Neighbor::HELLO_RECEIVED) {
        OSPFv3MessageHandler* messageHandler = neighbor->getInterface()->getArea()->getProcess()->getMessageHandler();
        messageHandler->clearTimer(neighbor->getInactivityTimer());
        messageHandler->startTimer(neighbor->getInactivityTimer(), neighbor->getRouterDeadInterval());
    }
    if (event == OSPFv3Neighbor::LOADING_DONE) {
//        neighbor->clearRequestRetransmissionTimer();
        changeState(neighbor, OSPFv3Neighbor::STATE_FULL, state);
    }
    if (event == OSPFv3Neighbor::IS_ADJACENCY_OK) {
/*        if (!neighbor->needAdjacency()) {
            neighbor->reset();
            changeState(neighbor, OSPFv3Neighbor::STATE_TWOWAY, state);
        }
*/    }
    if ((event == OSPFv3Neighbor::SEQUENCE_NUMBER_MISMATCH) || (event == OSPFv3Neighbor::BAD_LINK_STATE_REQUEST)) {
        OSPFv3MessageHandler* messageHandler = neighbor->getInterface()->getArea()->getProcess()->getMessageHandler();
        neighbor->reset();
//        neighbor->incrementDDSequenceNumber();
//        neighbor->sendDatabaseDescriptionPacket(true);
//        messageHandler->startTimer(neighbor->getDDRetransmissionTimer(), neighbor->getInterface()->getRetransmissionInterval());
        changeState(neighbor, OSPFv3Neighbor::STATE_EXCHANGE_START, state);
    }
    if (event == OSPFv3Neighbor::REQUEST_RETRANSMISSION_TIMER) {
//        neighbor->sendLinkStateRequestPacket();
//        neighbor->startRequestRetransmissionTimer();
    }
    if (event == OSPFv3Neighbor::UPDATE_RETRANSMISSION_TIMER) {
//        neighbor->retransmitUpdatePacket();
//        neighbor->startUpdateRetransmissionTimer();
    }
    if (event == OSPFv3Neighbor::DD_RETRANSMISSION_TIMER) {
//        neighbor->deleteLastSentDDPacket();
    }
}

void OSPFv3NeighborState::processEventTwoWay(OSPFv3Neighbor* neighbor, int/*OSPFv3Neighbor::NeighborEventType*/ event)
{
    if ((event == OSPFv3Neighbor::KILL_NEIGHBOR) || (event == OSPFv3Neighbor::LINK_DOWN)) {
        OSPFv3MessageHandler* messageHandler = neighbor->getInterface()->getArea()->getProcess()->getMessageHandler();
        neighbor->reset();
        messageHandler->clearTimer(neighbor->getInactivityTimer());
        changeState(neighbor, OSPFv3Neighbor::STATE_DOWN, state);
    }
    if (event == OSPFv3Neighbor::INACTIVITY_TIMER) {
        neighbor->reset();
        if (neighbor->getInterface()->getType() == OSPFv3InterfaceIPv6::NBMA) {
            OSPFv3MessageHandler* messageHandler = neighbor->getInterface()->getArea()->getProcess()->getMessageHandler();
            messageHandler->startTimer(neighbor->getPollTimer(), neighbor->getInterface()->getPollInt());
        }
        changeState(neighbor, OSPFv3Neighbor::STATE_DOWN, state);
    }
    if (event == OSPFv3Neighbor::ONEWAY_RECEIVED) {
        neighbor->reset();
        changeState(neighbor, OSPFv3Neighbor::STATE_INIT, state);
    }
    if (event == OSPFv3Neighbor::HELLO_RECEIVED) {
        OSPFv3MessageHandler* messageHandler = neighbor->getInterface()->getArea()->getProcess()->getMessageHandler();
        messageHandler->clearTimer(neighbor->getInactivityTimer());
        messageHandler->startTimer(neighbor->getInactivityTimer(), neighbor->getRouterDeadInterval());
    }
    if (event == OSPFv3Neighbor::IS_ADJACENCY_OK) {
/*        if (neighbor->needAdjacency()) {
            OSPFv3MessageHandler* messageHandler = neighbor->getInterface()->getArea()->getProcess()->getMessageHandler();
            if (!(neighbor->isFirstAdjacencyInited())) {
                neighbor->initFirstAdjacency();
            } else {
                neighbor->incrementDDSequenceNumber();
            }
            neighbor->sendDatabaseDescriptionPacket(true);
            messageHandler->startTimer(neighbor->getDDRetransmissionTimer(), neighbor->getInterface()->getRetransmissionInterval());
            changeState(neighbor, OSPFv3Neighbor::STATE_EXCHANGE_START, state);
        }
*/    }
}

