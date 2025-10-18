## 📋 Header Overview

```c
#pragma once
#ifndef SNMP_H
#define SNMP_H

#ifdef __cplusplus
extern "C"
{
#endif
```

**Purpose:** Standard C header guards to prevent multiple inclusions and ensure C++ compatibility.

**This approach is widely considered a best practice.**

The combination of `#pragma once` and the classic `#ifndef` provides a balanced approach to protect against multiple inclusion of header files, though the actual benefits may vary depending on the specific use case.

### Why this approach is commonly recommended:

1. **`#pragma once` - Potential speed and convenience**
    * **May be faster in some scenarios**: On projects with complex include hierarchies, some compilers can optimize file processing using filesystem metadata.
    * **Convenience factor**: Avoids the need to maintain unique macro names across the codebase.
    * **Reduced naming conflicts**: Eliminates potential macro name collisions between different headers.

2. **`#ifndef` / `#define` - Portability and reliability**
    * **Wide portability**: The `#ifndef` directive is a C/C++ language standard that should work on any compliant compiler.
    * **Fallback protection**: Provides a reliable alternative when `#pragma once` support is limited or encounters unusual filesystem situations.

### Performance Observations from Testing:

Our limited testing showed mixed results that may not be representative of all scenarios:

- **Small to medium files**: Performance differences were generally minor (0.01-0.05s)
- **Larger inclusion scenarios**: Some tests showed `#pragma once` with modest improvements
- **Combined approach**: Performance was comparable to either method alone in most cases

**Note**: The actual performance impact likely depends on many factors including compiler implementation, filesystem characteristics, project structure, and build environment. The benefits may be more significant in very large projects with complex include graphs.

### Practical Considerations:

* **Modern compiler behavior**: Most contemporary compilers (GCC, Clang, MSVC) recognize `#pragma once` while maintaining `#ifndef` compatibility
* **Compiler variations**: Some compilers may optimize the combined approach differently than others
* **Edge case handling**: The dual approach may provide additional safety in unusual environments like network filesystems or with symbolic links

### Conservative Recommendation:

This combined approach represents a reasonable compromise that prioritizes compatibility while potentially offering performance benefits in some situations. For maximum portability across diverse build environments, the additional `#ifndef` guard provides insurance against `#pragma once` limitations in edge cases.

The minimal overhead of including both directives appears to be acceptable for most practical purposes, though individual projects should consider their specific requirements and target environments.

# Implementation Details & Testing - https://github.com/dima853/self_university/tree/main/network/c/compatibility/ifndef_pragmaonce

---

## 🌐 Network Constants

```c
1) #define SNMP_PORT 161      /* standard UDP port for SNMP agents \
                            * to receive requests messages */
2) #define SNMP_TRAP_PORT 162 /* standard UDP port for SNMP      \
                            * managers to receive notificaion \
                            * (trap and inform) messages */
```

**Purpose:** Standard IANA-assigned ports for SNMP operations.

| Port    | Direction       | Purpose                    |
| ------- | --------------- | -------------------------- |
| **161** | Manager → Agent | GET/SET requests to agents |
| **162** | Agent → Manager | Traps/Informs to managers  |


## ✅ **OFFICIALLY CONFIRMED BY IANA:**

**Service Name: [snmp (Port 161)](https://www.iana.org/assignments/service-names-port-numbers/service-names-port-numbers.xhtml?search=161)**
```
Port Number: 161
Transport Protocol: tcp, udp  
Description: SNMP
```

**Service Name: [snmptrap (Port 162)](https://www.iana.org/assignments/service-names-port-numbers/service-names-port-numbers.xhtml?search=162)** 
```
Port Number: 162
Transport Protocol: tcp, udp
Description: SNMPTRAP
Assignee: Marshall Rose
```

---

```c
1) #define SNMP_MAX_LEN 1500    /* typical maximum message size */
2) #define SNMP_MIN_MAX_LEN 484 /* minimum maximum message size */
3) #define SNMP_MAX_PACKET_LEN (0x7fffffff)
```

## 1) 📄 From RFC 894 (IP over Ethernet):

**Page 1:**
> "The maximum length of an IP datagram sent over an Ethernet is 1500 octets."

**This corresponds to:** `#define SNMP_MAX_LEN 1500`


## 2) 📄 From RFC 3416 (SNMP Protocol Operations):

**Section 4.2:**
> "The maximum size of an SNMP message is limited to the minimum of: (a) 484 octets (b) the maximum message size that the destination can accept"

**This corresponds to:** `#define SNMP_MIN_MAX_LEN 484`
## 3) 📄 From RFC 3416 (SNMP Protocol Operations):

**Page 9, section 4.1:**
> "A compliant implementation must support as many variable bindings in a PDU or BulkPDU as fit into the overall maximum message size limit of the SNMP engine, but no more than 2147483647 variable bindings."

**This corresponds to:** `#define SNMP_MAX_PACKET_LEN (0x7fffffff)` = 2,147,483,647

**Purpose:** Message size limits for memory allocation and buffer management.

---

## 🔄 SNMP Version

```c
#define SNMP_VERSION_3 3
```

**Purpose:** This header focuses on **SNMPv3** (the modern, secure version).

---

## 📨 PDU Types - SNMP Protocol Data Units

### Core Operations (RFC 3416)
```c
#define SNMP_MSG_GET (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x0)      /* a0=160 */
#define SNMP_MSG_GETNEXT (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x1)  /* a1=161 */
#define SNMP_MSG_RESPONSE (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x2) /* a2=162 */
#ifndef NETSNMP_NO_WRITE_SUPPORT
#define SNMP_MSG_SET (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x3) /* a3=163 */
#endif
```

**ASN.1 Encoding Explanation:**
- `ASN_CONTEXT`: Context-specific tag
- `ASN_CONSTRUCTOR`: Constructed type (contains other elements)
- `0x0-0x3`: PDU type identifier

| PDU Type     | Value | Purpose                            |
| ------------ | ----- | ---------------------------------- |
| **GET**      | 160   | Retrieve specific variable values  |
| **GETNEXT**  | 161   | Retrieve next variable in sequence |
| **RESPONSE** | 162   | Response to any request            |
| **SET**      | 163   | Modify variable values             |

### Advanced Operations (SNMPv2c/v3)
```c
#define SNMP_MSG_GETBULK (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x5) /* a5=165 */
#define SNMP_MSG_INFORM (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x6)  /* a6=166 */
#define SNMP_MSG_TRAP2 (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x7)   /* a7=167 */
#define SNMP_MSG_REPORT (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x8) /* a8=168 */
```

| PDU Type    | Value | Purpose                         |
| ----------- | ----- | ------------------------------- |
| **GETBULK** | 165   | Efficient bulk data retrieval   |
| **INFORM**  | 166   | Acknowledged notification       |
| **TRAPv2**  | 167   | Unacknowledged notification     |
| **REPORT**  | 168   | Error reporting between engines |

---

## ⚙️ Internal Processing States

These are **NOT** network protocol values - they're internal state machine states for SET operation processing:

```c
#define SNMP_MSG_INTERNAL_SET_BEGIN -1
#define SNMP_MSG_INTERNAL_SET_RESERVE1 0
#define SNMP_MSG_INTERNAL_SET_RESERVE2 1
#define SNMP_MSG_INTERNAL_SET_ACTION 2
#define SNMP_MSG_INTERNAL_SET_COMMIT 3
#define SNMP_MSG_INTERNAL_SET_FREE 4
#define SNMP_MSG_INTERNAL_SET_UNDO 5
```

**SET Operation State Flow:**
```
RESERVE1 → RESERVE2 → ACTION → COMMIT → FREE
                    ↘ UNDO (if error)
```

**Advanced Internal States:**
```c
#define SNMP_MSG_INTERNAL_CHECK_VALUE 17
#define SNMP_MSG_INTERNAL_ROW_CREATE 18
#define SNMP_MSG_INTERNAL_UNDO_SETUP 19
// ... etc
```

**Purpose:** These implement the **transaction-like behavior** for SNMP SET operations, allowing rollback on failure.

---

## ✅ Confirmed Class Macro

```c
#define SNMP_CMD_CONFIRMED(c) (c == SNMP_MSG_INFORM || c == SNMP_MSG_GETBULK || \
                               c == SNMP_MSG_GETNEXT || c == SNMP_MSG_GET ||    \
                               c == SNMP_MSG_SET)
```

**Purpose:** Identifies PDUs that **require a response** from the receiver.

| Confirmed             | Unconfirmed            |
| --------------------- | ---------------------- |
| GET, GETNEXT, GETBULK | TRAP                   |
| SET, INFORM           | RESPONSE               |
| (Expect response)     | (No response expected) |

---

## ❌ Exception Values (SNMPv2/v3)

```c
#define SNMP_NOSUCHOBJECT (ASN_CONTEXT | ASN_PRIMITIVE | 0x0)   /* 80=128 */
#define SNMP_NOSUCHINSTANCE (ASN_CONTEXT | ASN_PRIMITIVE | 0x1) /* 81=129 */
#define SNMP_ENDOFMIBVIEW (ASN_CONTEXT | ASN_PRIMITIVE | 0x2)   /* 82=130 */
```

**Purpose:** Special return values for variable binding exceptions.

| Exception          | Meaning                          |
| ------------------ | -------------------------------- |
| **noSuchObject**   | Requested object doesn't exist   |
| **noSuchInstance** | Requested instance doesn't exist |
| **endOfMibView**   | No more variables in MIB tree    |

---

## 🚨 Error Status Codes

### Basic Errors (SNMPv1)
```c
#define SNMP_ERR_NOERROR (0)
#define SNMP_ERR_TOOBIG (1)
#define SNMP_ERR_NOSUCHNAME (2)
#define SNMP_ERR_BADVALUE (3)
#define SNMP_ERR_READONLY (4)
#define SNMP_ERR_GENERR (5)
```

### Extended Errors (SNMPv2/v3)
```c
#define SNMP_ERR_NOACCESS (6)
#define SNMP_ERR_WRONGTYPE (7)
#define SNMP_ERR_WRONGLENGTH (8)
#define SNMP_ERR_WRONGENCODING (9)
// ... up to SNMP_ERR_INCONSISTENTNAME (18)
```

**Key Error Explanations:**
- `noAccess`: Authentication/authorization failure
- `authorizationError`: VACM access control denial  
- `inconsistentName`: Row creation semantic error

```c
#define SNMP_VALIDATE_ERR(x) ((x > MAX_SNMP_ERR) ? SNMP_ERR_GENERR : (x < SNMP_ERR_NOERROR) ? SNMP_ERR_GENERR : x)
```

**Purpose:** Sanitizes error codes to prevent invalid values.

---

## 🗃️ Row Status Values (For Table Operations)

```c
#define SNMP_ROW_NONEXISTENT 0
#define SNMP_ROW_ACTIVE 1
#define SNMP_ROW_NOTINSERVICE 2
#define SNMP_ROW_NOTREADY 3
#define SNMP_ROW_CREATEANDGO 4
#define SNMP_ROW_CREATEANDWAIT 5
#define SNMP_ROW_DESTROY 6
```

**Purpose:** Implements **dynamic row creation/deletion** in SNMP tables.

**Usage Flow:**
```mermaid
graph LR
    A[createAndGo] --> B[active]
    C[createAndWait] --> D[notInService] --> B
    B --> E[destroy] --> F[nonExistent]
```

---

## 💾 Storage Types

```c
#define SNMP_STORAGE_NONE 0
#define SNMP_STORAGE_OTHER 1
#define SNMP_STORAGE_VOLATILE 2
#define SNMP_STORAGE_NONVOLATILE 3
#define SNMP_STORAGE_PERMANENT 4
#define SNMP_STORAGE_READONLY 5
```

**Purpose:** Defines data persistence characteristics for MIB objects.

---

## 🛡️ Security Definitions (SNMPv3)

### Message Processing Models
```c
#define SNMP_MP_MODEL_SNMPv3 3
```

### Security Models
```c
#define SNMP_SEC_MODEL_ANY 0
#define SNMP_SEC_MODEL_USM 3  /* User-based Security Model */
#define SNMP_SEC_MODEL_TSM 4  /* Transport Security Model */
```

### Security Levels
```c
#define SNMP_SEC_LEVEL_NOAUTH 1        /* noAuthNoPriv */
#define SNMP_SEC_LEVEL_AUTHNOPRIV 2    /* authNoPriv */
#define SNMP_SEC_LEVEL_AUTHPRIV 3      /* authPriv */
```

### Message Flags
```c
#define SNMP_MSG_FLAG_AUTH_BIT 0x01    /* Message is authenticated */
#define SNMP_MSG_FLAG_PRIV_BIT 0x02    /* Message is encrypted */
#define SNMP_MSG_FLAG_RPRT_BIT 0x04    /* Message is reportable */
```

**Security Combinations:**
```c
// No security
flags = 0

// Authentication only  
flags = SNMP_MSG_FLAG_AUTH_BIT

// Authentication + Encryption
flags = SNMP_MSG_FLAG_AUTH_BIT | SNMP_MSG_FLAG_PRIV_BIT

// Reportable message
flags = SNMP_MSG_FLAG_RPRT_BIT
```

---

## 🎛️ Control Flags (Implementation-Specific)

```c
#define UCD_MSG_FLAG_RESPONSE_PDU 0x100
#define UCD_MSG_FLAG_EXPECT_RESPONSE 0x200
#define UCD_MSG_FLAG_FORCE_PDU_COPY 0x400
#define UCD_MSG_FLAG_ALWAYS_IN_VIEW 0x800
```

**Purpose:** Internal implementation controls for message processing.

---

## 🌳 OID Base Definitions

```c
#define SNMP_OID_INTERNET 1, 3, 6, 1
#define SNMP_OID_ENTERPRISES SNMP_OID_INTERNET, 4, 1
#define SNMP_OID_MIB2 SNMP_OID_INTERNET, 2, 1
#define SNMP_OID_SNMPV2 SNMP_OID_INTERNET, 6
```

**OID Tree Structure:**
```
1.3.6.1 (internet)
├── 2.1 (mib-2)
├── 4.1 (enterprises) 
└── 6 (snmpV2)
```

---

## 🔧 Function Prototypes

### Utility Functions
```c
char *uptime_string(u_long, char *);           // Convert uptime to string
void xdump(const void *, size_t, const char *); // Hex dump utility
```

### ASN.1 Parsing/Building (Core Protocol Engine)
```c
u_char *snmp_parse_var_op(u_char *, oid *, size_t *, u_char *,
                          size_t *, u_char **, size_t *);
u_char *snmp_build_var_op(u_char *, const oid *, size_t *, u_char,
                          size_t, const void *, size_t *);
```

**Purpose:** These functions implement the **ASN.1 BER encoding/decoding** for SNMP protocol data units.

---

## 🏁 Footer
```c
#ifdef __cplusplus
}
#endif
#endif /* SNMP_H */
```

**Purpose:** Close the C++ extern "C" block and header guard.

## 📊 Summary

This header provides:

1. **Protocol Constants** - PDU types, error codes, ports
2. **SNMPv3 Security** - Security models, levels, flags  
3. **Internal Processing** - State machines for SET operations
4. **ASN.1 Utilities** - Encoding/decoding functions
5. **MIB Management** - Row status, storage types, OID bases
---

# How It Works? (Super Briefly)
<img width="18804" height="2464" alt="Untitled diagram-2025-10-18-115602" src="https://github.com/user-attachments/assets/905f9d59-bed6-468b-ab1a-08045f185ea2" />


## 🛡️ **Security Levels (SNMPv3 Security Levels)**

### **L31: noAuthNoPriv (Level 1)**
```c
#define SNMP_SEC_LEVEL_NOAUTH 1
```
**What is it:** Without authentication and encryption  
**How it works:** 
- 🤡 Anonymous access
- Messages are not subscribed
- 🔓 Data is transmitted in clear text
- 💀 **DANGEROUS!** Only for test networks

### **L32: authNoPriv (Level 2)** 
```c
#define SNMP_SEC_LEVEL_AUTHNOPRIV 2
```
**What is it:** Authentication without encryption
**How it works:**
- ✅ User verification (login/password)
- 🔐 HMAC-MD5 or HMAC-SHA for signature
- 📨 Data is NOT encrypted
- Protection against spoofing, but the data is visible

### **L33: authPriv (Level 3)**
```c
#define SNMP_SEC_LEVEL_AUTHPRIV 3
```
**What is it:** Authentication + Encryption
**How it works:**
- ✅ User verification
- 🔐 Message signature
- 🚫 Data encryption (DES, AES)
- 💪 **MAXIMUM PROTECTION**

---

## 📨 **PDU Types (Message Types)**

### **Core Operations**
- **E11: GET (160)** - "Give the value of this variable"
- **E12: GETNEXT (161)** - "Give the following variable in the MIB tree"
- **E14: SET (163)** - "Change the value of a variable"
- **E13: RESPONSE (162)** - "Reply to any request"

### **Advanced Operations**
- **E21: GETBULK (165)** - "Give a lot of data at once" (optimization)
- **E22: INFORM (166)** - "Confirmation Notification"
- **E23: TRAPv2 (167)** - "Notification without confirmation"
- **E24: REPORT (168)** - "Error message between engines"

---

## ⚙️ **Internal Processing (Internal SET Processing)**

**F1: SET State Machine** - as a database transaction:

```
BEGIN → RESERVE1 → RESERVE2 → ACTION → COMMIT → FREE
                               UNDO (if an error occurs)
```

**Why do I need to:**
- `RESERVE1/RESERVE2` - booking resources
- `ACTION` - making the change
- `COMMIT` - confirm
- `UNDO` - rollback if an error occurs
- `FREE` - freeing up resources

---

## ❌ **Exception Values**

**When the variable is not found:**
- **H1: noSuchObject (128)** - "There is no such object"
- **H2: noSuchInstance (129)** - "There is no such instance"  
- **H3: endOfMibView (130)** - "There is nothing further in the tree"

---

## 🚨 **Error Status Codes**

### **Basic Errors**
- **I11: noError (0)** - Everything is ok
- **I12: tooBig (1)** - The response is too big
- **I13: noSuchName (2)** - Variable not found
- **I14: BadValue (3)** - Incorrect value
- **I15: readOnly (4)** - Attempt to change read-only
- **I16: genErr(5)** - Common error

### **Extended Errors**
- **I21: noAccess (6)** - No access rights
- **I22: wrongType (7)** - Incorrect data type
- **I23: wrongLength (8)** - Wrong length
- ... etc. up to 18

---

## 🗃️ **Row Status (Row statuses of tables)**

**For dynamic table management:**
```c
nonExistent → createAndWait → notInService → active → destroy
               createAndGo ────────────────┘
```

- **J1: nonExistent** - The row does not exist
- **J4/J5: createAndWait/createAndGo** - Create a string
- **J3: notInService** - Created but not active  
- **J2: active** - Active and running
- **J7: destroy** - Delete line

---

## 💾 **Storage Types**

**How long does the data persist?:**
- **K1: none** - Is not saved at all
- **K3: volatile** - In memory only (disappears after reboot)
- **K4: nonVolatile** - Persists between reboots
- **K5: permanent** - Cannot be deleted
- **K6: readonly** - Read-only

---

## 🌳 **OID Structure (Object Tree)**

```
1.3.6.1 (internet)
├── 2.1 (mib-2) # Standard MIB objects
├── 4.1 (enterprises)     # Vendor extensions
6 (SNMPv2) # SNMPv2 features
```

**Each variable in the network has a unique OID!**

---

## 🔧 **Core Functions**

- **N11: uptime_string()** - Converts time to beautiful text
- **N12: xdump()** - Shows data in hex format
- **N21: snmp_parse_var_op()** - Parses ASN.1 data
- **N22: snmp_build_var_op()** - Creates an ASN.1 data