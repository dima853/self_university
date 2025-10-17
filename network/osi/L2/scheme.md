Here's the **deepest Mermaid.js scheme** explaining Layer 2 networking operations in chronological order:

```mermaid
flowchart TD
    Start([Host Network Interface Initialization]) --> PhysLink
    
    subgraph PhysLink [Physical Layer Establishment]
        L1[Layer 1: Physical Link] --> LinkStatus{Link Detected?}
        LinkStatus -- Yes --> L1Up[Physical Link UP]
        LinkStatus -- No --> L1Down[Physical Link DOWN<br>Check cable/interface]
    end

    L1Up --> MACInit
    
    subgraph MACInit [MAC Layer Initialization]
        MACLoad[Load Burned-in MAC Address<br>OUI: 24-bit + NIC: 24-bit] --> MACCheck{MAC Valid?}
        MACCheck -- Yes --> MACReady[MAC Layer Ready]
        MACCheck -- No --> MACFallback[Fallback: Software MAC]
    end

    MACReady --> L2Protocols
    
    subgraph L2Protocols [Layer 2 Protocol Stack]
        STP[Spanning Tree Protocol<br>BPDU Exchange] --> STPConverge{STP Converged?}
        STPConverge -- Yes --> TopoStable[Loop-Free Topology]
        STPConverge -- No --> Blocking[Ports in Blocking State]
        
        LACP[LACP - Link Aggregation<br>Member Port Negotiation] --> LAGGroup[LAG Group Formation]
        
        CDP[CDP/LLDP Discovery<br>Neighbor Information Exchange] --> NeighborTable[Build Neighbor Table]
    end

    TopoStable --> FrameProcessing
    
    subgraph FrameProcessing [Frame Processing Logic]
        RxFrame[Frame Received] --> FCS{FCS Check}
        FCS -- Pass --> DestCheck[Check Destination MAC]
        FCS -- Fail --> FrameDrop1[Drop Frame - FCS Error]
        
        DestCheck --> DestType{Destination Type?}
        
        DestType -- Unicast --> UniCheck{Unicast MAC = Our MAC?}
        UniCheck -- Yes --> PassUp[Pass to Upper Layers]
        UniCheck -- No --> ForwardCheck{Forwarding Enabled?}
        
        DestType -- Broadcast --> BcastProc[Broadcast Processing<br>FF:FF:FF:FF:FF:FF]
        
        DestType -- Multicast --> McastCheck{Multicast Registered?}
        McastCheck -- Yes --> McastProc[Multicast Processing]
        McastCheck -- No --> FrameDrop2[Drop Frame - Not Registered]
        
        ForwardCheck -- Yes --> L2Forwarding[Layer 2 Forwarding]
        ForwardCheck -- No --> FrameDrop3[Drop Frame - Not Our MAC]
    end

    BcastProc --> ProtocolHandlers
    
    subgraph ProtocolHandlers [Broadcast/Multicast Protocol Handlers]
        ARPHandler[ARP Handler<br>Opcode: 1=Request, 2=Reply] --> ARPTable[Update ARP Table]
        
        DHCPHandler[DHCP Handler<br>Discover/Offer/Request/ACK] --> IPConfig[IP Address Assignment]
        
        IPv6ND[IPv6 Neighbor Discovery<br>Router Solicitation/Advertisement] --> IPv6Config[IPv6 Address Auto-config]
        
        mDNS[mDNS Handler<br>.local name resolution] --> ServiceDisc[Service Discovery]
    end

    PassUp --> UpperLayer[Pass to Network Layer (L3)<br>EtherType: 0x0800=IPv4, 0x86DD=IPv6]
    
    L2Forwarding --> SwitchingLogic
    
    subgraph SwitchingLogic [Switch Forwarding Logic]
        MACLearning[MAC Learning Process<br>Source MAC + Port → MAC Table] --> TableUpdate[Update MAC Table]
        
        TableUpdate --> Lookup[Lookup Destination MAC] --> FoundInTable{MAC in Table?}
        
        FoundInTable -- Yes --> OutPort[Forward to Specific Port]
        FoundInTable -- No --> Flood[Flood to All Ports<br>Except Ingress]
        
        OutPort --> VLANCheck{VLAN Tag Present?}
        VLANCheck -- Yes --> VLANVerify[Verify VLAN Membership]
        VLANCheck -- No --> NativeVLAN[Apply Native VLAN]
        
        VLANVerify -- Allowed --> ForwardFrame[Forward Frame]
        VLANVerify -- Blocked --> VLANDrop[Drop - VLAN Violation]
    end

    ForwardFrame --> QoS
    
    subgraph QoS [Quality of Service Processing]
        PktClass[Packet Classification<br>Based on DSCP/CoS] --> PriorityMap[Map to Queue]
        PriorityMap --> Scheduling[Queue Scheduling<br>Strict Priority/WRR]
        Scheduling --> TxQueue[Transmission Queue]
    end

    TxQueue --> Egress
    
    subgraph Egress [Egress Processing]
        RateLimit[Rate Limiting/Policing] --> StormControl[Broadcast Storm Control]
        StormControl --> WithinLimit{Within Limits?}
        WithinLimit -- Yes --> Transmit[Transmit Frame]
        WithinLimit -- No --> StormDrop[Drop - Storm Control]
    end

    Transmit --> Statistics
    
    subgraph Statistics [Statistics & Monitoring]
        Counters[Update Counters<br>Frames In/Out, Errors] --> PortStats[Port Statistics]
        PortStats --> Monitoring[SNMP/NetFlow Monitoring]
    end

    Monitoring --> End([Continuous Operation])
    
    %% Style definitions
    classDef process fill:#e1f5fe,stroke:#01579b,stroke-width:2px
    classDef decision fill:#fff3e0,stroke:#ef6c00,stroke-width:2px
    classDef protocol fill:#e8f5e8,stroke:#2e7d32,stroke-width:2px
    classDef error fill:#ffebee,stroke:#c62828,stroke-width:2px
    
    class PhysLink,MACInit,FrameProcessing,SwitchingLogic,QoS,Egress,Statistics process
    class LinkStatus,STPConverge,MACCheck,DestType,UniCheck,ForwardCheck,McastCheck,FoundInTable,VLANCheck,WithinLimit decision
    class L2Protocols,ProtocolHandlers protocol
    class L1Down,FrameDrop1,FrameDrop2,FrameDrop3,VLANDrop,StormDrop error
```

## Key L2 Concepts in Order of Operation:

### 1. **Physical Layer (L1)**
- **Auto-negotiation**: Speed/duplex negotiation
- **Link pulses**: Physical link detection

### 2. **MAC Layer Initialization**
- **Burned-in MAC**: Hardware address loading
- **OUI (Organizationally Unique Identifier)**: First 24 bits
- **NIC-specific**: Last 24 bits

### 3. **Control Protocols**
- **STP (802.1D)**: Prevents switching loops
- **LACP (802.3ad)**: Link aggregation
- **CDP/LLDP**: Neighbor discovery

### 4. **Frame Processing**
- **FCS (Frame Check Sequence)**: Error detection
- **MAC address types**: Unicast, Broadcast, Multicast
- **EtherType**: Protocol identification (IPv4=0x0800, IPv6=0x86DD)

### 5. **Switching Logic**
- **MAC learning**: Source address + port mapping
- **Forwarding**: Exact port or flooding
- **Aging**: MAC table entry timeout

### 6. **VLAN Processing (802.1Q)**
- **VLAN tagging**: 4-byte header insertion
- **Native VLAN**: Untagged frame handling
- **VLAN pruning**: Restricted VLAN propagation

### 7. **Quality of Service**
- **CoS (Class of Service)**: 802.1p priority bits
- **Queue management**: Multiple output queues
- **Scheduling algorithms**: Strict priority, WRR

### 8. **Egress Processing**
- **Rate limiting**: Traffic policing
- **Storm control**: Broadcast/multicast suppression
- **Error checking**: Final validation before transmission

This sequence represents the complete L2 packet journey from physical connection to final transmission, including all major protocols and decision points in chronological order.