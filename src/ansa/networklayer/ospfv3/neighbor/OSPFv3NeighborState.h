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

#ifndef __INET_OSPFV3NEIGHBORSTATE_H
#define __INET_OSPFV3NEIGHBORSTATE_H

#include "OSPFv3Neighbor.h"

//namespace OSPFv3 {
class OSPFv3Neighbor;

class OSPFv3NeighborState {
protected:
    int/*OSPFv3Neighbor::NeighborStateType*/ state;
    int/*OSPFv3Neighbor::NeighborStateType*/ prevState;
    void changeState(OSPFv3Neighbor* neighbor,
            int/*OSPFv3Neighbor::NeighborStateType*/ newState,
            int/*OSPFv3Neighbor::NeighborStateType*/ currentState);

public:
    OSPFv3NeighborState(){state=prevState=0/*OSPFv3Neighbor::STATE_DOWN*/;}
    virtual ~OSPFv3NeighborState() {}

    int/*OSPFv3Neighbor::NeighborStateType*/ getState() const {return state;}
    void processEvent(OSPFv3Neighbor* neighbor, int/*OSPFv3Neighbor::NeighborEventType*/ event);
private:
    void processEventAttempt(OSPFv3Neighbor* neighbor, int/*OSPFv3Neighbor::NeighborEventType*/ event);
    void processEventDown(OSPFv3Neighbor* neighbor, int/*OSPFv3Neighbor::NeighborEventType*/ event);
    void processEventExchange(OSPFv3Neighbor* neighbor, int/*OSPFv3Neighbor::NeighborEventType*/ event);
    void processEventExchangeStart(OSPFv3Neighbor* neighbor, int/*OSPFv3Neighbor::NeighborEventType*/ event);
    void processEventFull(OSPFv3Neighbor* neighbor, int/*OSPFv3Neighbor::NeighborEventType*/ event);
    void processEventInit(OSPFv3Neighbor* neighbor, int/*OSPFv3Neighbor::NeighborEventType*/ event);
    void processEventLoading(OSPFv3Neighbor* neighbor, int/*OSPFv3Neighbor::NeighborEventType*/ event);
    void processEventTwoWay(OSPFv3Neighbor* neighbor, int/*OSPFv3Neighbor::NeighborEventType*/ event);
};

//} // namespace OSPF

#endif // __INET_OSPFV3NEIGHBORSTATE_H

