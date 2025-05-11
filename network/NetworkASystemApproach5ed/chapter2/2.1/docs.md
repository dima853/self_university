# Classes of Links

```mermaid
%%{init: {'theme': 'base', 'themeVariables': { 'clusterBkg': '#f0f0f0'}, 'config': {'rankSpacing': 5}}}%%
graph LR
    subgraph Frequency[Frequency (Hz)]
        direction TB
        10^0 --> 10^2 --> 10^4 --> 10^6 --> 10^8 --> 10^{10} --> 10^{12} --> 10^{14} --> 10^{16} --> 10^{18} --> 10^{20} --> 10^{22} --> 10^{24}
    end

    subgraph Spectrum[Electromagnetic Spectrum]
        direction TB
        Radio --> Microwave --> Infrared --> UV --> X-ray --> Gamma_ray[Gamma ray]
    end

    subgraph Applications[Applications]
        direction TB
        Coax --> AM --> FM --> TV --> Terrestrial_microwave[Terrestrial microwave] --> Fiber_optics[Fiber optics]
    end

    %% Связи для визуального соответствия изображению
    Frequency -.- Spectrum
    Frequency -.- Applications

    %% Стилизация
    style Frequency fill:#e6f3ff,stroke:#333
    style Spectrum fill:#ffe6e6,stroke:#333
    style Applications fill:#e6f3ff,stroke:#333
```