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

package inet.examples.ansa.ospfv3.Simple;

import inet.ansa.networklayer.ospfv3.OSPFv3RouterIPv6;
import inet.nodes.ethernet.Eth100M;


//
// TODO auto-generated type
//
network Simple_test
{
    submodules:
        R1: OSPFv3RouterIPv6 {
            @display("p=58,105");
        }
        R2: OSPFv3RouterIPv6 {
            @display("p=217,105");
        }
    connections:
        R1.ethg++ <--> Eth100M <--> R2.ethg++;
}
