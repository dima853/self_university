# 🖥️ **Clients in distributed systems: a complete analysis**  

These protocols ❗(FOR DESKTOP APP)❗

## **1. Problems of X servers and their solutions**  

### **Problem: "Slow Internet interface"**  
**Why?**  
- The **X Window Protocol** (the basis of the GUI in Unix/Linux) was originally developed for local area networks.  
- It transmits ** a lot of small messages** (for example, "draw a button here", "change text").  
- In wide area networks (WAN) with high latency, this leads to **brakes**.  

---

### **Solution 1: NX protocol (improved X)**  
**How does it work?**  
1. **Message caching**:  
   - The client and server remember which commands have already been transmitted.  
   - If the command is repeated (for example, text update), only **difference** is transmitted.  
   - Example:
```python
     # It was: "Draw the 'OK' button in (x=10, y=20)"
# It was: "Change the text of the button from ID=5 to 'Cancel'"
```  
2. **Data compression**:
- Even if the command is new, it is compressed (saving traffic by **4-1000 times**).  

**Result**:  
- You can work with the graphical interface even via a 9600-bit/s modem!  

---

### **Solution 2: THINC (alternative X)**  
**The idea**:  
- Intercepts application commands (for example, "draw window") and ** optimizes them**.  
- Optimization example:
- **Batching**: Combining several commands into one.  
    ```python
    # It was: 
    #1. Draw a rectangle (x=0, y=0, w=100, h=50)
#2. Fill it with the color #FF0000
    # Became: 
    # 1. Draw a shaded red rectangle (x=0, y=0, w=100, h=50)
``
- **Clipping excess**: If new commands overwrite the old ones (for example, updating the screen), the old ones can ** not be sent**.  

**Advantages**:
- Less latency than in X.  
- The server is simpler (suitable for embedded devices).  

---

## **2. "Thin clients" vs "Thick clients"**  

### **Option 1: Thick Client (Raw Pixel Streaming)**  
** How does it work?**  
- The application **draws the interface locally** → sends the finished frames** to the server (as a video).  
- Example: **Remote Desktop**.  

**Problems**:
- **Requires powerful compression**:  
  - Without video compression 320x240 @ 30 FPS = **53 Mbps** (unrealistic for mobile networks).  
- **No flexibility**:
- If the user enlarges the font, the server must forward ** the entire new screen**.  

---

### **Option 2: Thin Client (Compound Documents)**  
** How does it work?**  
- The client **only displays the data**, and the logic is executed on the server.  
- Example: **Google Docs**.  

**Features**:
- **Drag-and-drop**:
- Dragged the file to the trash → the server received the "delete file" command.  
- **In-place editing**:
- Clicked on the graph in the document → opened the **Excel interface** directly in Word.  

**Advantages**:
- Minimal client load.  
- All data is stored on the server (securely).  

---

## **3. Transparency for the client**  

### **What is it?**  
The goal is to make sure that the client ** does not notice** that he is working with a remote server.  

### **Examples**:
1. **Access Transparency**:
- The client invokes server methods **in the same way** as local functions.  
   - Implementation: **RPC (Remote Procedure Call)**.  
     ```java
     // Client code (looks like a local call)
int result = server.calculate(10, 20);
     ```  

2. **Replication Transparency**:
- There may be **several** servers (for reliability), but the client sees **one**.  
   - How it works:
``mermaid
     graph LR
         Client -->|Request| LoadBalancer
         LoadBalancer -->|Request| Server1
         LoadBalancer -->|Request| Server2
         Server1 -->|Response| Client
         Server2 -->|Response| Client
     ```  

3. **Failure Transparency**:
- If the server is down, the client **automatically reconnects** to the backup.  
   - Example: **WhatsApp** (messages are cached and sent when communication is restored).  

---

## **Result**  
1. **For graphics**:
- **NX** and **THINC** accelerate X servers.  
   - **Thin clients** (Compound Documents) is a modern trend.  
2. **For transparency**:
- The client should not know where the server is running (locally or in the cloud).  
   - Solutions: RPC, replication, caching.  

**The main thing**:  
> The client should be **as simple as possible**, and all the complexity is hidden in the backend and middleware! 🚀  


---
### **1. X Window (X11) — The "Grandfather's way"**  
** What is it?**  
An old system for graphical user interface in Linux/Unix. It works according to the principle:  
- **The application** (for example, Firefox) sends commands: _"draw a button here", "print text there"_.  
- **The X server** (on your computer) executes these commands.  

**Problems**:
- **Slow on the internet**: Each button contains dozens of messages. It's like ordering a pizza, naming the ingredients one at a time: "Dough... Tomatoes... cheese..." 🐢  
- **Loads the processor**: The server must process every small command.  

---

### **2. NX — The "Magic Compressor"**  
** What is it?**  
A patch for X11 that **compresses traffic** by 1000 times. As if you were transmitting emojis instead of words: 🍕 = "Bring pizza with pepperoni."  

** How does it work?**  
1. **Caching**:  
   - If Firefox draws the button twice, NX sends only the changes (for example, "change the text from 'OK' to 'Cancel'").
2.**Compression**:  
   - Even new teams shrink 4 times.  

**Advantages**:
- Works even on dial-up (56 kbit/s modem)!  
- Does not require changes in applications.  

**Disadvantages**:  
- Still need an X server.  

---

### **3. THINC is an "Easy alternative"**  
** What is it?**  
The X11 replacement, which:  
- Intercepts application commands and **simplifies them** (for example, "draw the entire window at once, not in parts").  
- Does not require a full-fledged X server — suitable for phones and IoT.  

**Example**:  
```python
# Regular X11:
1. Draw a window frame
2. Draw the "OK" button
3. Draw the text "Hello"
# THINC:
1. Draw the entire window with one command
```  

**Advantages**:  
- Faster than NX by 2-3 times.  
- Less load on the client.  

---

### **Comparison**  
| Technology | Where it is used                          | Pros                                        | Cons                     |
| ---------- | ----------------------------------------- | ------------------------------------------- | ------------------------ |
| **X11**    | Old Linux systems                         | Standard, supported everywhere              | Slow, consumes resources |
| **NX**     | Remote Desktops                           | Traffic compression, works on weak channels | Still difficult          |
| **THINC**  | Integrated systems (ATMs, cash registers) | Very lightweight, Low latency               | Fewer features           |

---

### **What about now?**  
Now everyone is switching to **Wayland** (X11 replacement), but for remote access they use:  
- **RDP** (Windows) / **VNC** (cross-platform) — how to "transfer video" of the screen.  
- **Web interfaces** (for example, VS Code in the browser) — do not require X11 at all.  

---

**Conclusion**:
NX and THINC are "crutches" for speeding up the old X11 ❗(FOR DESKTOP APP)❗. Modern systems work differently, but if you administer Linux servers, it's useful to know about them!