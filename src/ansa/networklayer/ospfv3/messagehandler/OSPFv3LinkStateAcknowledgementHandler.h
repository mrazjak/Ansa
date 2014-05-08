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

#ifndef __INET_OSPFV3LINKSTATEACKNOWLEDGEMENTHANDLER_H
#define __INET_OSPFV3LINKSTATEACKNOWLEDGEMENTHANDLER_H

#include "IOSPFv3MessageHandler.h"

//namespace OSPFv3 {

class OSPFv3LinkStateAcknowledgementHandler /*: public IOSPFv3MessageHandler*/ {
protected:
    OSPFv3ProcessIPv6* process;
public:
    OSPFv3LinkStateAcknowledgementHandler(OSPFv3ProcessIPv6* containingProcess);

    void processPacket(OSPFv3Packet* packet, OSPFv3InterfaceIPv6* intf, OSPFv3Neighbor* neighbor);
};

//} // namespace OSPFv3

#endif // __INET_OSPFV3LINKSTATEACKNOWLEDGEMENTHANDLER_H

