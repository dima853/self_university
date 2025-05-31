### **[Summary: Distributed Denial of Service (DDoS) attacks and protection methods](https://www.researchgate.net/publication/236153147_A_Survey_of_Defense_Mechanisms_Against_Distributed_Denial_of_Service_DDoS_Flooding_Attacks)**  

#### **1. Introduction**  
**DDoS attacks** are malicious attempts to deprive users of access to network resources by overloading servers or network infrastructure.  
- **The first DDoS attack** was recorded in 1999 (CIAC).  
- Since then, most attacks have been **distributed** (using botnets).  

**The main methods of DDoS attacks:**  
1. **Vulnerabilities in protocols** – sending specially generated packets for software failure.  
2. **Flood attacks** - resource overload:  
   - **Network/transport layer** (for example, SYN flood, UDP flood).  
   - **Application layer** (HTTP flood, database queries).  

**Examples of major DDoS attacks:**  
- 2000: Yahoo! – 2 hours of downtime, loss of income.  
- 2002: Attack on 9 out of 13 root DNS servers.  
- 2004: Mydoom-virus against SCO Group.  
- 2009: Attacks on the websites of South Korea and the United States.  
- 2010: Anonymous vs. Mastercard, PayPal, Visa.  
- 2012: US banks (Bank of America, Citigroup, etc.).  

**Statistics:**
- 75% of organizations have experienced DDoS (2008-2009).  
- 7000 attacks daily (Prolexic).  
- The attack power is growing (up to 100 Gbit/s).  

---  

#### **2. Motivation of attackers**  
1. **Financial gain** (cybercrime).  
2. **Revenge** (disgruntled employees, hacktivists).  
3. **Ideology** (political protests, hacktivism).  
4. **Intellectual challenge** (amateur hackers).  
5. **Cyberwarfare** (government attacks on infrastructure).  

**Research:**  
- Motivation modeling through **game theory** ([Fultz et al., 22]).  
- It is important to study the psychology of hackers in order to develop preventive measures.  

---  

#### **3. Classification of DDoS attacks**  

**A. Network/transport layer attacks**  
- **SYN flood** – connection queue overflow.  
- **UDP flood** – sending UDP packets to random ports.  
- **ICMP flood** (Smurf attack) – amplification through broadcast requests.  
- **DNS enhancement** – using DNS servers to generate traffic.  

**B. Attacks at the application level**  
- **HTTP flood** – massive requests to the web server.  
- **Slowloris** – long HTTP connections to exhaust the limits.  
- **Database attacks** – complex SQL queries.  

---  

#### **4. Botnets and their role in DDoS**  
**Botnet** is a network of infected devices (bots/zombies) controlled by an attacker.  

**Types of botnets:**  
- **Centralized** (IRC, HTTP commands).  
- **Decentralized** (P2P networks, for example, Zeus).  

**Methods of infection:**  
- Through worms, Trojans, software vulnerabilities.  

---  

#### **5. DDoS protection methods**  

**Classification of protection:**  
1. **At the place of detection:**
- **The source of the attack** (filtering on the provider's side).  
   - **Intermediate network** (firewalls, IDS).  
   - **Sacrifice** (request limitation, load balancing).  

2. **By reaction time:**  
   - **Before the attack** (proactive protection, patches).  
   - **During the attack** (traffic analysis, blocking).  
   - **After the attack** (recovery, log analysis).  

**Examples of methods:**  
- **Packet filtering** (for example, BGP Blackholing).  
- **Rate Limiting**.  
- **CAPTCHA** to filter out bots.  
- **Distributed security systems** (e.g. CDN, Cloudflare).  

---  

#### **6. Assessment of protection effectiveness**  
**Criteria:**  
- **Detection rate**.  
- **Accuracy** (minimum false positives).  
- **Scalability**.  
- **Cost of implementation**.  

**Comparison:**  
- **Centralized systems** are simpler, but vulnerable to attacks.  
- **Distributed systems** are more stable, but more difficult to manage.  

---  

#### **7. Cyber Insurance**  
- Compensation for DDoS losses.  
- Includes the cost of recovery and loss of income.  

---  

#### **8. Conclusions and prospects**  
- DDoS remains a serious threat.  
- Comprehensive solutions are needed ****:  
  - Collaboration between providers and companies.  
  - Machine learning for traffic analysis.  
  - International laws against cybercrime.  

**Future research:**  
- Behavioral analysis of attackers.  
- Automated response systems (AI).
- Improvement of distributed protection mechanisms.