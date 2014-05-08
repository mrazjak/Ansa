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

#ifndef __INET_OSPFV3INTERFACESTATE_H
#define __INET_OSPFV3INTERFACESTATE_H

#include <omnetpp.h>
#include <vector>
#include "OSPFv3Interface.h"
//#include "OSPFArea.h"
//#include "OSPFv3InterfaceStateBackup.h"
//#include "OSPFv3InterfaceStateDesignatedRouter.h"
//#include "OSPFv3InterfaceStateNotDesignatedRouter.h"
//#include "OSPFv3ProcessIPv6.h"

//namespace OSPFv3 {
class OSPFv3InterfaceIPv6;
//class InterfaceStateDown;

class OSPFv3InterfaceState {
protected:
    int/*OSPFv3InterfaceIPv6::InterfaceStateType*/ state;
    int/*OSPFv3InterfaceIPv6::InterfaceStateType*/ prevState;

    void changeState(OSPFv3InterfaceIPv6* intf,
            int/*OSPFv3InterfaceIPv6::InterfaceStateType*/ nextState,
            int/*OSPFv3InterfaceIPv6::InterfaceStateType*/ oldState);

    void calculateDesignatedRouter(OSPFv3InterfaceIPv6* intf);

public:
    OSPFv3InterfaceState() {state=prevState=0/*OSPFv3InterfaceIPv6::STATE_DOWN*/;}
    virtual ~OSPFv3InterfaceState() {}

    int/*OSPFv3InterfaceIPv6::InterfaceStateType*/ getState() const {return state;}
    void processEvent(OSPFv3InterfaceIPv6* intf, int/*OSPFv3InterfaceIPv6::InterfaceEventType*/ event);
private:
    void processEventDown(OSPFv3InterfaceIPv6* intf, int/*OSPFv3InterfaceIPv6::InterfaceEventType*/ event);
    void processEventLoopback(OSPFv3InterfaceIPv6* intf, int/*OSPFv3InterfaceIPv6::InterfaceEventType*/ event);
    void processEventWaiting(OSPFv3InterfaceIPv6* intf, int/*OSPFv3InterfaceIPv6::InterfaceEventType*/ event);
    void processEventPointToPoint(OSPFv3InterfaceIPv6* intf, int/*OSPFv3InterfaceIPv6::InterfaceEventType*/ event);
    void processEventNotDesignatedRouter(OSPFv3InterfaceIPv6* intf, int/*OSPFv3InterfaceIPv6::InterfaceEventType*/ event);
    void processEventBackup(OSPFv3InterfaceIPv6* intf, int/*OSPFv3InterfaceIPv6::InterfaceEventType*/ event);
    void processEventDesignatedRouter(OSPFv3InterfaceIPv6* intf, int/*OSPFv3InterfaceIPv6::InterfaceEventType*/ event);
};

//} // namespace OSPFv3

#endif // __INET_OSPFV3INTERFACESTATE_H

