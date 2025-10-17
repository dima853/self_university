

```mermaid
flowchart TD
    A[Host Physical Link UP<br>PHY Sublayer Active] --> B[MAC Layer Initialization<br>EUI-48/64 Address Loading]
    
    B --> C[Control Protocols Startup]
    
    subgraph C [Control Protocols Startup]
        C1[STP: Blocking State] --> C2[STP: Listen/Learn]
        C2 --> C3[STP: Forwarding]
        C3 --> C4[LACP Negotiation]
        C4 --> C5[LLDP Discovery<br>LLDPDU with TLVs]
    end
    
    C --> D[Frame Reception<br>MAU/MDI Processing]
    
    subgraph D [Frame Reception Process]
        D1[Raw Bit Reception<br>PMD Sublayer] --> D2[Frame Delineation<br>Preamble/SFD]
        D2 --> D3[Dest MAC Check<br>DA vs SA Analysis]
        D3 --> D4[FCS Validation<br>CRC-32 Check]
    end
    
    D --> E{Dest MAC Type?<br>Unicast/Broadcast/Multicast}
    
    E -->|Unicast| F{Our MAC or BSSID?}
    E -->|Broadcast| G[Broadcast Processing<br>FF:FF:FF:FF:FF:FF]
    E -->|Multicast| H{Multicast Registered?<br>IGMP Snooping}
    
    F -->|Yes| I[Pass to Upper Layers<br>LLC/LSAP Processing]
    F -->|No| J[Forwarding Decision<br>MAC Table Lookup]
    
    G --> K[Essential Broadcast Protocols]
    
    subgraph K [Essential Broadcast Protocols]
        K1[ARP Processing] --> K2[DHCP Processing<br>IANA Address Assignment]
        K2 --> K3[IPv6 NDP<br>EUI-64 Addressing]
        K3 --> K4[mDNS/SSDP<br>Service Discovery]
    end
    
    H -->|Yes| L[Multicast Group Handling<br>GMRP/IGMP]
    H -->|No| M[Drop Frame]
    
    I --> N[Protocol Demultiplexing<br>EPD via EtherType]
    
    subgraph N [Protocol Demultiplexing]
        N1{EtherType Field<br>IANA Registered}
        N1 -->|0x0800| N2[IPv4 Handler]
        N1 -->|0x86DD| N3[IPv6 Handler]
        N1 -->|0x0806| N4[ARP Handler]
        N1 -->|0x8100| N5[VLAN Tagging<br>PVID/PPVID Processing]
        N1 -->|0xAAAA| N6[SNAP Processing]
    end
    
    J --> O[Switching Forwarding]
    
    subgraph O [Switching Forwarding Process]
        O1[MAC Learning<br>Src MAC + Port → MIB] --> O2[MAC Table Lookup<br>DA Search]
        O2 --> O3{Dest MAC Known?<br>Forwarding Database}
        O3 -->|Yes| O4[Forward to Specific Port<br>VID/PVID Check]
        O3 -->|No| O5[Flood to All Ports<br>Except Ingress]
        O4 --> O6[VLAN Verification<br>VID Membership Check]
        O5 --> O7[VLAN-Aware Flooding<br>Within same VID]
    end
    
    O6 --> P{QoS Required?<br>CoS/ToS Fields}
    O7 --> P
    
    P -->|Yes| Q[Quality of Service Processing]
    P -->|No| R[Egress Processing]
    
    subgraph Q [Quality of Service Processing]
        Q1[CoS/DSCP Classification<br>IEEE 802.1p] --> Q2[Priority Queue Assignment<br>Multiple Egress Queues]
        Q2 --> Q3[Queue Scheduling<br>SP/WRR/WFQ Algorithms]
    end
    
    Q --> R
    
    subgraph R [Egress Processing]
        R1[Rate Limiting Check<br>Ingress/Egress Policers] --> R2[Storm Control Check<br>Broadcast/Multicast/Unicast]
        R2 --> R3{Within Limits?<br>Threshold Enforcement}
        R3 -->|Yes| R4[Frame Transmission<br>PHY Encoding]
        R3 -->|No| R5[Drop - Policy Violation]
    end
    
    R4 --> S[Update Statistics<br>SNMP MIB Counters]
    
    S --> T[Continuous Operation Loop<br>NMS Monitoring]
    
    %% Security & Management Paths
    U[Security Processing] --> U1[KaY/MACsec Entity<br>SecY Security Processing]
    U1 --> U2[802.1X Authentication<br>Port-Based NAC]
    
    %% Power over Ethernet
    V[PoE System] --> V1[PSE Power Sourcing<br>Power Management]
    V1 --> V2[PD Powered Device<br>Classification]
    
    %% Error paths
    D4 -->|FCS Fail| X[Drop - Frame Error]
    O6 -->|VLAN Violation| Y[Drop - VLAN Access Denied<br>VID Authorization Failed]
    
    classDef process fill:#e1f5fe,stroke:#01579b
    classDef decision fill:#fff3e0,stroke:#ef6c00
    classDef protocol fill:#e8f5e8,stroke:#2e7d32
    classDef error fill:#ffebee,stroke:#c62828
    classDef security fill:#f3e5f5,stroke:#4a148c
    classDef power fill:#e8f8f5,stroke:#00695c
    
    class A,B,D,I,J,O,P,R,S,T process
    class E,F,H,N1,O3,P,R3 decision
    class C,K,N,Q protocol
    class M,X,Y,R5 error
    class U,U1,U2 security
    class V,V1,V2 power
```

## **Enhanced Components Added:**

### **1. Physical Layer (PHY)**
- **PMD** (Physical Media Dependent): Actual media encoding
- **MAU** (Medium Attachment Unit): Transceiver functionality
- **MDI** (Media Dependent Interface): Physical connector interface

### **2. Addressing & Identification**
- **EUI-48/64** (Extended Unique Identifier): MAC address formats
- **BSSID** (Basic Service Set Identification): Wireless AP identification
- **OUI** (Organizationally Unique Identifier): Manufacturer prefix in MAC

### **3. Protocol Processing**
- **LLC** (Logical Link Control): Upper sublayer of L2
- **LSAP** (Link Service Access Point): Service access points
- **SNAP** (Subnetwork Access Protocol): Protocol encapsulation
- **EPD** (Ethertype Protocol Discrimination): Protocol identification

### **4. Discovery & Management**
- **LLDPDU** (LLDP Data Unit): Discovery protocol frames
- **TLV** (Type, Length, Value): Data encoding format
- **MIB** (Management Information Base): SNMP data structure
- **NMS** (Network Management System): Central management

### **5. Security Framework**
- **KaY** (Key Agreement Entity): MACsec key management
- **SecY** (Security Entity): MACsec enforcement
- **802.1X**: Port-based network access control

### **6. Power Systems**
- **PSE** (Power Sourcing Equipment): Switch/Injector side
- **PD** (Powered Device): Powered endpoint side

### **7. VLAN Advanced Concepts**
- **PVID** (Port VLAN ID): Default VLAN for untagged frames
- **PPVID** (Port and Protocol VLAN ID): Protocol-based VLAN assignment

<<<<<<< HEAD
### **8. Standards Bodies**
- **IANA** (Internet Assigned Numbers Authority): Ethertype assignments
- **IEEE** (Institute of Electrical and Electronics Engineers): Standards development
 
=======
### **Phase 8: Maintenance & Monitoring**
22. **Counters & Statistics** (RMON, port counters, error tracking)
23. **Aging Processes** (MAC table entry timeout)
24. **Topology Updates** (Dynamic response to network changes)

## **Key L2 Protocols & Standards:**
- **MAC Addressing**: IEEE 802.3 (Ethernet)
- **VLANs**: IEEE 802.1Q
- **STP**: IEEE 802.1D
- **LACP**: IEEE 802.3ad
- **LLDP**: IEEE 802.1AB
- **QoS**: IEEE 802.1p

This sequence represents the complete lifecycle of a frame at Layer 2, from physical reception through processing to final transmission, including all critical decision points and protocol interactions.
>>>>>>> ec1c315587d6dea9cdf0fe41d8a04e7bcf1b0263
