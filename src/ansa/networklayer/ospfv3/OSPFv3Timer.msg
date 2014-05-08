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

// types of timers
cplusplus {{
#include "IPv6Address.h"
}}

class noncobject IPv6Address;

enum OSPFv3TimerType
{
    OSPFV3_HELLO_TIMER = 0;
    OSPFV3_WAIT_TIMER = 1;
    OSPFV3_ACKNOWLEDGEMENT_TIMER = 3;
    OSPFV3_NEIGHBOR_INACTIVITY_TIMER = 4;
    OSPFV3_NEIGHBOR_POLL_TIMER = 5;
    OSPFV3_NEIGHBOR_DD_RETRANSMISSION_TIMER = 6;
    OSPFV3_NEIGHBOR_UPDATE_RETRANSMISSION_TIMER = 7;
    OSPFV3_NEIGHBOR_REQUEST_RETRANSMISSION_TIMER = 8;
    OSPFV3_DATABASE_AGE_TIMER = 9;
}

//
// general timer structure
packet OSPFv3Timer 
{
    char timerKind enum(OSPFv3TimerType);
}

