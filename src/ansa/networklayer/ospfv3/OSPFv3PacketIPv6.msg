// Copyright (C) 2013 Brno University of Technology (http://nes.fit.vutbr.cz/ansa)
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
// @file RIPngMessage.msg
// @author Jiri Trhlik (mailto:jiritm@gmail.com), Vladimir Vesely (mailto:ivesely@fit.vutbr.cz)
// @brief RIPng message
// @detail RIPng protocol message (same format as is defined in RFC 2080)


cplusplus {{
#include "IPv4Address.h"
}}


class noncobject IPv4Address;

enum OSPFv3PacketType
{
    OSPFV3_HELLO = 1;
    OSPFV3_DATABASE_DESCRIPTION = 2;
    OSPFV3_LINK_STATE_REQUEST = 3;
    OSPFV3_LINK_STATE_UPDATE = 4;
    OSPFV3_LINK_STATE_ACKNOWLEDGEMENT= 5;
};

// should be a byte long bitfield
struct OSPFv3Options
{
    bool unused_0;
    bool unused_1;
    bool unused_2;
    bool unused_3;
    bool unused_4;
    bool unused_5;
    bool unused_6;
    bool unused_7;
    bool unused_8;
    bool unused_9;
    bool unused_10;
    bool unused_11;
    bool unused_12;
    bool unused_13;
    bool unused_14;
    bool unused_15;
    bool OSPFv2_extention_1;
    bool OSPFv2_extention_2;
    bool DC_DemandCircuits;
    bool R_Router;
    bool N_NSSA;
    bool x_MOSPF;
    bool E_ExternalRoutingCapability;
    bool V6_IPv6;
};

//
// Represents an OSPFv3 packet header
//
packet OSPFv3Packet
{
    unsigned char version = 3;                   //  8 bit
    unsigned char type enum(OSPFv3PacketType) = OSPFV3_HELLO; // 8  bit
    unsigned short packetLenght;                 // 16 bit
    IPv4Address routerID;               // 32 bit
    IPv4Address areaID;                 // 32 bit
    unsigned short checksum;                     // 16 bit
    unsigned char instanceID;                    //  8 bit
    unsigned char zerro;                         //  8 bit
};

//
// Represents an OSPFv3 Hello packet
//
packet OSPFv3HelloPacket extends OSPFv3Packet
{
    int interfaceID;                    // 32 bit
    unsigned char routerPriority;       //  8 bit
    OSPFv3Options options;              // 24 bit
    unsigned short helloInterval = 10;           // 16 bit
    unsigned short routerDeadInterval = 40;      // 16 bit
    IPv4Address designatedRouter;       // 32 bit
    IPv4Address backupDesignatedRouter; // 32 bit
    IPv4Address neighborID[];           //  ~ bit
};


//enum LSAType
//{
//    ROUTERLSA_TYPE = 1;
//    NETWORKLSA_TYPE = 2;
//    SUMMARYLSA_NETWORKS_TYPE = 3;
//    SUMMARYLSA_ASBOUNDARYROUTERS_TYPE = 4;
//    AS_EXTERNAL_LSA_TYPE = 5;
//}

//
// Represents an OSPFv3 LSA header
//
//class OSPFv3LSAHeader
//{
//    unsigned short lsAge = 0;
//    OSPFv3Options lsOptions;
//    char lsType enum(LSAType) = ROUTERLSA_TYPE;
//    IPv6Address linkStateID;
//    IPv6Address advertisingRouter;
//    long lsSequenceNumber = 0;
//    unsigned short lsaLength = 0;
//}

//
// common ancestor type for all LSAs
//
//class OSPFv3LSA
//{
//    OSPFv3LSAHeader header;
//}

//enum LinkType
//{
//    POINTTOPOINT_LINK = 1;
//    TRANSIT_LINK = 2;
//    STUB_LINK = 3;
//    VIRTUAL_LINK = 4;
//}

//struct TOSData
//{
//    unsigned char tos;
//    unsigned char tosMetric[3];
//}

//class Link
//{
//    IPv6Address linkID;
//    unsigned long linkData = 0;
//    unsigned char type enum(LinkType) = POINTTOPOINT_LINK;
//    unsigned char numberOfTOS = 0;
//    unsigned long linkCost = 1;
//    TOSData tosData[];
//}

//
// Represents an OSPFv3 Router LSA
//
//class OSPFv3RouterLSA extends OSPFv3LSA
//{
//    bool V_VirtualLinkEndpoint = false;
//    bool E_ASBoundaryRouter = false;
//    bool B_AreaBorderRouter = false;
//    unsigned short numberOfLinks = 0;
//    Link links[];
//}

//
// Represents an OSPFv3 Network LSA
//
//class OSPFv3NetworkLSA extends OSPFv3LSA
//{
//    IPv6Address networkMask;
//    IPv6Address attachedRouters[];
//}

//
// Represents an OSPFv3 Summary LSA
//
//class OSPFv3SummaryLSA extends OSPFv3LSA
//{
//    IPv6Address networkMask;
//    unsigned long routeCost = 1;
//    TOSData tosData[];
//}

//struct ExternalTOSInfo
//{
//    TOSData tosData;
//    bool E_ExternalMetricType;
//    IPv6Address forwardingAddress;
//    long externalRouteTag;
//}

//
// Represents the contents of an OSPFv3 AS External LSA
//
//class OSPFv3ASExternalLSAContents
//{
//    IPv6Address networkMask;
//    bool E_ExternalMetricType = false;
//    unsigned long routeCost = 1;
//    IPv6Address forwardingAddress;
//    long externalRouteTag = 0;
//    ExternalTOSInfo externalTOSInfo[];
//}

//
// Represents an OSPFv3 AS External LSA
//
//class OSPFv3ASExternalLSA extends OSPFv3LSA
//{
//    OSPFv3ASExternalLSAContents contents;
//}


// should be a byte long bitfield
//struct OSPFv3DDOptions
//{
//    bool unused_1;
//    bool unused_2;
//    bool unused_3;
//    bool unused_4;
//    bool unused_5;
//    bool I_Init;
//    bool M_More;
//    bool MS_MasterSlave;
//}

//
// Represents an OSPFv3 Database Description packet
//
//packet OSPFv3DatabaseDescriptionPacket extends OSPFv3Packet
//{
//    unsigned short interfaceMTU;
//    OSPFv3Options options;
//    OSPFv3DDOptions ddOptions;
//    unsigned long ddSequenceNumber;
//    OSPFv3LSAHeader lsaHeaders[];
//}

//struct LSARequest
//{
//    unsigned long lsType;
//    IPv6Address linkStateID;
//    IPv6Address advertisingRouter;
//}

//
// Represents an OSPFv3 Link State Request packet
//
//packet OSPFv3LinkStateRequestPacket extends OSPFv3Packet
//{
//    LSARequest requests[];
//}

//
// Represents an OSPFv3 Link State Update packet
//
//packet OSPFv3LinkStateUpdatePacket extends OSPFv3Packet
//{
//    unsigned long numberOfLSAs;
//    OSPFv3RouterLSA routerLSAs[];
//    OSPFv3NetworkLSA networkLSAs[];
//    OSPFv3SummaryLSA summaryLSAs[];
//    OSPFv3ASExternalLSA asExternalLSAs[];
//}

//
// Represents an OSPFv3 Link State Acknowledgement packet
//
//packet OSPFv3LinkStateAcknowledgementPacket extends OSPFv3Packet
//{
//    OSPFv3LSAHeader lsaHeaders[];
//}