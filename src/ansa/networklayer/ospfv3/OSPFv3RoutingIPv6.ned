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

package inet.ansa.networklayer.ospfv3;

import inet.ansa.networklayer.ospfv3.processIPv6.OSPFv3ProcessIPv6;
import inet.ansa.networklayer.ospfv3.tablesIPv6.OSPFv3InterfaceTableIPv6;
import inet.ansa.networklayer.ospfv3.tablesIPv6.OSPFv3NeighborTableIPv6;
import inet.ansa.networklayer.ospfv3.tablesIPv6.OSPFv3TopologyTableIPv6;


module OSPFv3RoutingIPv6
{
    parameters:
		@display("i=block/network2");
        @display("bgb=263,229");
    gates:
        input IPv6In;
        output IPv6Out;

    submodules:
        // vlevo nazev, vpravo typ
        OSPFv3ProcessIPv6: OSPFv3ProcessIPv6 {
            @display("p=70,70");
        }
        OSPFv3InterfaceTableIPv6: OSPFv3InterfaceTableIPv6 {
            @display("p=200,175");
        }
        OSPFv3NeighborTableIPv6: OSPFv3NeighborTableIPv6 {
            @display("p=200,35");
        }
        OSPFv3TopologyTableIPv6: OSPFv3TopologyTableIPv6 {
            @display("p=200,105");
        }
    connections:
        IPv6In --> OSPFv3ProcessIPv6.IPv6In;
        IPv6Out <-- OSPFv3ProcessIPv6.IPv6Out;
}
