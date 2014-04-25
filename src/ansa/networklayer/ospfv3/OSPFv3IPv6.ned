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

import inet.ansa.networklayer.ipSplitter.IpSplitter;
import inet.ansa.networklayer.ospfv3.OSPFv3RoutingIPv6;


module OSPFv3IPv6
{
    parameters:
        @display("i=block/network2");
        @node;
        @labels(node,ethernet-node);
        xml routingTable = default(xml("<routingTable/>"));  // Routing table is filled by deviceConfigurator
        bool IPForward = default(true);
        string configFile = default("config.xml");           // file name of the XML file which contains configuration for the router
        string deviceId;                                     // needed for proper load of the device XML configuration
    gates:
        input IPv4In;  // @labels(IPv4ControlInfo/up);
        output IPv4Out;// @labels(IPv4ControlInfo/down);
        input IPv6In;  // @labels(IPv6ControlInfo/up);
        output IPv6Out;// @labels(IPv6ControlInfo/down);


    submodules:

        OSPFv3RoutingIPv6: OSPFv3RoutingIPv6 {
            @display("p=92,47");

        }

        ipSplitter6: IpSplitter {
            @display("p=92,158");
            gates:
                ipv6In[1];//[sizeof(ethg)+sizeof(pppg)+sizeof(radioIn)];
                ipv6Out[1];//[sizeof(ethg)+sizeof(pppg)+sizeof(radioIn)];
                ifIn[1];//[sizeof(ethg)+sizeof(pppg)+sizeof(radioIn)];
                ifOut[1];//[sizeof(ethg)+sizeof(pppg)+sizeof(radioIn)];
        }
    connections allowunconnected:
        //OSPFv3RoutingIPv6.IPv6Out --> udp.appIn[0];
        //OSPFv3RoutingIPv6.IPv6In <-- udp.appOut[0];


        ipSplitter6.ipv6Out[0] --> OSPFv3RoutingIPv6.IPv6In;
        OSPFv3RoutingIPv6.IPv6Out --> ipSplitter6.ipv6In[0];

        IPv6In --> ipSplitter6.ifIn[0];
        ipSplitter6.ifOut[0] --> IPv6Out;
}
