# Signaling Game in CyberSecurity
### **Signaling Games in Cybersecurity: Theory, mathematics, and Examples**  

Signal games are a class of dynamic games with incomplete information, where one player (the Sender) sends a signal, and the other (the Receiver) interprets it and makes a decision. In the context of cybersecurity, they are used to:  
- **Deception of attackers** (disinformation, honeypots),  
- **Detection of intruders** (analysis of suspicious behavior),  
- **Optimization of protection** (alarm about the "strength" of the system).  

---

## **1. The formal model of the signal game**  

### **1.1. Basic components**  
The game is defined by the tuple:
$$\[G = \langle T, M, A, P, u_S, u_R \rangle\]$$  
where:  
- $$\(T\)$$ — **Sender type** (for example, "attacker" or "legitimate user").  
- $$\(M\)$$ — **multiple signals** (for example, SYN packets, HTTP requests).  
- $$\(A\)$$ — **Recipient's actions** (for example, "block", "skip").  
- $$\(P\)$$ — probability distribution over \(T\) (Recipient's a priori beliefs).  
- $$\(u_S, u_R\)$$ — **utility functions** of the Sender and Recipient.  

### **1.2. The course of the game**  
1. Nature chooses the type $$\(t\in T\)$$ for the Sender according to $$\(P(t)\)$$.  
2. The sender observes $$\(t\)$$ and sends a signal $$\(m\in M\)$$.  
3. The recipient sees $$\(m\)$$ (but not $$\(t\)$$) and chooses the action $$\(a\in A\)$$.  
4. The winnings $$\(u_S(t, m, a)\)$$ and $$\(u_R(t, m, a)\)$$ are realized.  

---

## **2. Balance in signal games**  

## 2.1. Perfect Bayesian Equilibrium (PBE)**  
PBE is a set of strategies and beliefs where:  
1. **Strategies are optimal** under given beliefs.  
2. **Beliefs are consistent** with observed signals (updated according to Bayes' rule).  

**Formally:**  
- **Sender's Strategy:** $$\(\sigma(m|t)\)$$ — the probability of sending $$\(m\)$$ with the type $$\(t\)$$.  
- **Recipient's Strategy:** $$\(\alpha(a | m) \)$$ — the probability of selecting $$\(a\)$$ after the signal $$\(m\)$$.  
- **Recipient's Beliefs:** $$\(\mu(t|m)\)$$ — the probability that the type $$\(t\)$$ sent $$\(m\)$$.  

**PBE Terms and Conditions:**  
1. **Optimal Sender:**  
   $$\[\forall t, \sigma(m | t) > 0 \implies m \in \argmax_{m'} u_S(t, m', \alpha(\cdot | m')).\]$$  
2. **Optimal Recipient:**  
   $$\[\forall m, \alpha(a | m) > 0 \implies a \in \argmax_{a'} \sum_t \mu(t | m) u_R(t, m, a').\]$$  
3. **Consistency of beliefs:**  
   $$\[\mu(t | m) = \frac{P(t) \sigma(m | t)}{\sum_{t'} P(t') \sigma(m | t')}.\]$$  

---

## **3. Examples in cybersecurity**  

### **3.1. Example 1: A Honeypot as a signal game**  
- **Sender Types:**
- $$\( t_1 \)$$ — the attacker,  
  - $$\(t_2\)$$ — legitimate user.  
- **Signals:**
- $$\(m_1\)$$ — port scanning (suspicious behavior),
- $$\(m_2\)$$ — normal HTTP request.  
- **Recipient's Actions:**  
  - $$\( a_1 \)$$ — block,  
  - $$\(a_2\)$$ — skip.  

**Winnings:**  
- Attacker: $$\(u_S(t_1, m_1, a_2) = +1\)$$ (attack success), $$\(u_S(t_1, m_1, a_1) = -1\)$$.
- Recipient: $$\(u_R(t_1, m_1, a_1) = +1\)$$ (protection worked).  

** Balance:**  
- If $$\(P(t_1)\)$$ is low, the Recipient ignores $$\(m_1\)$$ → the attackers abuse.  
- If the defender implements **honeypot**, then $$\(\mu(t_1| m_1) \)$$ increases → blocking.  

### **3.2. Example 2: Cheating a DDoS attacker**  
- **Sender Types:**
- $$\(t_1\)$$ is a bot (DDoS),
- $$\(t_2\)$$ is a real user.  
- **Signals:**  
  - $$\(m_1\)$$ — high frequency of requests,
- $$\(m_2\)$$ — normal traffic.  
- **Recipient's Actions:**
- $$\( a_1 \)$$ — restrict traffic,  
  - $$\(a_2\)$$ — skip.  

**Dynamics:**  
- If the bots disguise themselves as $$\(m_2\)$$, the defender uses **statistical analysis** to update $$\(\mu(t|m)\)$$.  

---

## **4. Mathematical subtleties**  

### **4.1. Dividing and unifying equilibria**  
- **Separating Equilibrium:**  
  Each type sends a **unique signal**. For example:  
  - Attackers always send $$\(m_1\)$$, legitimate users — $$\(m_2\)$$.  
  - Condition: $$\(u_S(t_1, m_1, a) > u_S(t_1, m_2, a) \)$$ and vice versa for $$\(t_2\)$$.  

- **Pooling Equilibrium:**  
  All types send **the same signal**. For example:
- Everyone sends $$\(m_1\)$$, the receiver ignores the signal.  
  - It is possible if $$\(P(t_1)\)$$ is very small.  

### **4.2. The Crawford-Sobel model (Crawford & Sobel, 1982)**  
- Signals ** are distorted** (for example, attackers fake traffic).  
- Enter **The "cost of lying"** $$\(c(t, m)\)$$:
$$\[u_S(t, m, a) = v_S(a) - c(t, m).\]$$  
- Example: if it is more difficult for a bot to simulate $$\(m_2\)$$, then $$\(c(t_1, m_2)\)$$ is large.  

---

## **5. Complex articles and links**  

1. **Classics:**  
   - Crawford, V., & Sobel, J. (1982). ["Strategic Information Transmission"](https://www.jstor.org/stable/1913390). *Econometrica*.  
     *(A formal model of information distortion.)*  

2. **Applications are safe:**  
   - Zhu, Q., et al. (2013). ["A Signaling Game Model for Moving Target Defense"](https://ieeexplore.ieee.org/document/6547101). *IEEE INFOCOM*.  
     *(Alarm games for dynamic protection.)*  

3. **Deep analysis:**  
   - Fudenberg, D., & Tirole, J. (1991). *Game Theory*. MIT Press. (Chapter 8: "Signaling Games").  

4. **Modern works:**  
   - Pawlick, J., et al. (2019). ["Deception by Design: Evidence-Based Signaling Games for Network Defense"](https://arxiv.org/abs/1803.07151). *ACM WiSec*.  
     *(Examples with honeypots and deception of attackers.)*  

5. **Mathematical aspects:**  
   - Cho, I.-K., & Kreps, D. (1987). ["Signaling Games and Stable Equilibria"](https://www.jstor.org/stable/1912081). *QJE*.  
     *(Criteria of intuitive stability.)*  

---

## **6. Conclusion**  
Signal games are a powerful tool for:  
- **Deception of attackers** (honeypots, false vulnerabilities),  
- **Detection optimization** (traffic signal analysis),  
- **Behavior modeling** (how attackers react to protection).

# Additional materials
- [Signaling Game (Wiki)](https://en.wikipedia.org/wiki/Signaling_game)
    - [Bayesian game (type)](https://en.wikipedia.org/wiki/Bayesian_game)
