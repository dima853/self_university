
## CONSTRUCTION 3.30
- $$\text{Init: on input } s \in \{0,1\}^n \text{ and } IV \in \{0,1\}^{3n/4}, \text{ output } st = (s, IV, 0).$$
- $$\text{Next}(st): \text{Output } y = F_s(IV \parallel \langle i \rangle) \text{ and } st' = (s, IV, i+1).$$