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

**This is the best practice.**

The combination of `#pragma once` and the classic `#ifndef'-sentinel is the most reliable and portable approach to protect against multiple inclusion of header files.

### Why is this the best practice:

1. **`#pragma once` - Speed and convenience**
    * **Faster**: It is easier for the compiler to check whether a file has already been included using its unique path than to compare all macros through the preprocessor.
    * **More convenient**: You don't need to come up with a unique name for the guardian macro (in your case, `SNMP_H`).
    * **Fewer errors**: It is impossible to accidentally use the same macro name in two different files.

2. **`#ifndef` / `#define` - Portability**
* **Absolute portability**: The directive `#ifndef` is a C/C++ language standard and **is guaranteed to work on any compiler** conforming to the standard.
    * **Backup**: `#pragma once` is a non-standard but widespread extension. If you use both methods, and for some reason the compiler does not support `#pragma once`, the sentinel macro will reliably pick up and protect the file.

### How it works in practice:

* **Smart compiler** (GCC, Clang, MSVC and other modern ones): It will see `#pragma once`, remember the file and will not even check the `#ifndef` block, since it already knows that the file is included. This gives maximum performance.
* **An old or exotic compiler** (which does not know `#pragma once`): Ignores this directive (as well as any unknown `pragma`) and proceeds to check the macro `#ifndef SNMP_H'. Since the macro has not yet been defined, the code will be enabled and protected in a standard way.

# Prove - 

---

## 🌐 Network Constants

```c
#define SNMP_PORT 161      /* standard UDP port for SNMP agents \
                            * to receive requests messages */
#define SNMP_TRAP_PORT 162 /* standard UDP port for SNMP      \
                            * managers to receive notificaion \
                            * (trap and inform) messages */
```

**Purpose:** Standard IANA-assigned ports for SNMP operations.

| Port    | Direction       | Purpose                    |
| ------- | --------------- | -------------------------- |
| **161** | Manager → Agent | GET/SET requests to agents |
| **162** | Agent → Manager | Traps/Informs to managers  |

```c
#define SNMP_MAX_LEN 1500    /* typical maximum message size */
#define SNMP_MIN_MAX_LEN 484 /* minimum maximum message size */
#define SNMP_MAX_PACKET_LEN (0x7fffffff)
```

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

