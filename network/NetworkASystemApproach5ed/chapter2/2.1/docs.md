# Classes of Links

```mermaid
%%{init: {'theme': 'base', 'themeVariables': {'clusterBkg': '#f0f0f0'}}}%%
graph TD
    classDef frequency fill:#e6f3ff,stroke:#333
    classDef spectrum fill:#ffe6e6,stroke:#333
    classDef apps fill:#e6f3ff,stroke:#333

    A["Electromagnetic Spectrum"]:::spectrum
    B["Frequency (Hz)"]:::frequency
    C["Applications"]:::apps

    B --> 10^0
    B --> 10^2
    B --> 10^4
    B --> 10^6
    B --> 10^8
    B --> 10^10
    B --> 10^12
    B --> 10^14
    B --> 10^16
    B --> 10^18
    B --> 10^20
    B --> 10^22
    B --> 10^24

    A --> Radio
    A --> Microwave
    A --> Infrared
    A --> UV
    A --> Xray["X-ray"]
    A --> Gammaray["Gamma ray"]

    C --> Coax
    C --> AM
    C --> FM
    C --> TV
    C --> Terrestrial
    C --> Fiber
```