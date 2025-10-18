#ifndef SNMP_H
#define SNMP_H

#ifdef __cplusplus
extern "C"
{
#endif

#define SNMP_PORT 161      /* standard UDP port for SNMP agents \
                            * to receive requests messages */
#define SNMP_TRAP_PORT 162 /* standard UDP port for SNMP      \
                            * managers to receive notificaion \
                            * (trap and inform) messages */

#define SNMP_MAX_LEN 1500    /* typical maximum message size */
#define SNMP_MIN_MAX_LEN 484 /* minimum maximum message size */
#define SNMP_MAX_PACKET_LEN (0x7fffffff)

    /*
     * versions based on version field
     */
#define SNMP_VERSION_3 3

    /*
     * PDU types in SNMPv3
     */
#define SNMP_MSG_GET (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x0)      /* a0=160 */
#define SNMP_MSG_GETNEXT (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x1)  /* a1=161 */
#define SNMP_MSG_RESPONSE (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x2) /* a2=162 */
#ifndef NETSNMP_NO_WRITE_SUPPORT
#define SNMP_MSG_SET (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x3) /* a3=163 */
#endif                                                     /* !NETSNMP_NO_WRITE_SUPPORT */

    /*
     * PDU types in SNMPv3
     */
#define SNMP_MSG_GETBULK (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x5) /* a5=165 */
#define SNMP_MSG_INFORM (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x6)  /* a6=166 */
#define SNMP_MSG_TRAP2 (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x7)   /* a7=167 */

    /*
     * PDU types in SNMPv3
     */
#define SNMP_MSG_REPORT (ASN_CONTEXT | ASN_CONSTRUCTOR | 0x8) /* a8=168 */

#ifndef NETSNMP_NO_WRITE_SUPPORT
    /*
     * internal modes that should never be used by the protocol for the
     * pdu type.
     *
     * All modes < 128 are reserved for SET requests.
     */
#define SNMP_MSG_INTERNAL_SET_BEGIN -1
#define SNMP_MSG_INTERNAL_SET_RESERVE1 0 /* these should match snmp.h */
#define SNMP_MSG_INTERNAL_SET_RESERVE2 1
#define SNMP_MSG_INTERNAL_SET_ACTION 2
#define SNMP_MSG_INTERNAL_SET_COMMIT 3
#define SNMP_MSG_INTERNAL_SET_FREE 4
#define SNMP_MSG_INTERNAL_SET_UNDO 5
#define SNMP_MSG_INTERNAL_SET_MAX 6

#define SNMP_MSG_INTERNAL_CHECK_VALUE 17
#define SNMP_MSG_INTERNAL_ROW_CREATE 18
#define SNMP_MSG_INTERNAL_UNDO_SETUP 19
#define SNMP_MSG_INTERNAL_SET_VALUE 20
#define SNMP_MSG_INTERNAL_CHECK_CONSISTENCY 21
#define SNMP_MSG_INTERNAL_UNDO_SET 22
#define SNMP_MSG_INTERNAL_COMMIT 23
#define SNMP_MSG_INTERNAL_UNDO_COMMIT 24
#define SNMP_MSG_INTERNAL_IRREVERSIBLE_COMMIT 25
#define SNMP_MSG_INTERNAL_UNDO_CLEANUP 26
#endif /* !NETSNMP_NO_WRITE_SUPPORT */

    /*
     * modes > 128 for non sets.
     * Note that 160-168 overlap with SNMP ASN1 pdu types
     */
#define SNMP_MSG_INTERNAL_PRE_REQUEST 128
#define SNMP_MSG_INTERNAL_OBJECT_LOOKUP 129
#define SNMP_MSG_INTERNAL_POST_REQUEST 130
#define SNMP_MSG_INTERNAL_GET_STASH 131

    /*
     * test for member of Confirmed Class i.e., reportable
     */
#ifdef NETSNMP_NO_WRITE_SUPPORT
#define SNMP_CMD_CONFIRMED(c) (c == SNMP_MSG_INFORM || c == SNMP_MSG_GETBULK || \
                               c == SNMP_MSG_GETNEXT || c == SNMP_MSG_GET)
#else /* !NETSNMP_NO_WRITE_SUPPORT */
#define SNMP_CMD_CONFIRMED(c) (c == SNMP_MSG_INFORM || c == SNMP_MSG_GETBULK || \
                               c == SNMP_MSG_GETNEXT || c == SNMP_MSG_GET ||    \
                               c == SNMP_MSG_SET)
#endif /* !NETSNMP_NO_WRITE_SUPPORT */

    /*
     * Exception values for SNMPv3
     */
#define SNMP_NOSUCHOBJECT (ASN_CONTEXT | ASN_PRIMITIVE | 0x0)   /* 80=128 */
#define SNMP_NOSUCHINSTANCE (ASN_CONTEXT | ASN_PRIMITIVE | 0x1) /* 81=129 */
#define SNMP_ENDOFMIBVIEW (ASN_CONTEXT | ASN_PRIMITIVE | 0x2)   /* 82=130 */

    /*
     * Error codes (the value of the field error-status in PDUs)
     */

    /*
     * in SNMPv3 PDUs
     */
#define SNMP_ERR_NOERROR (0) /* XXX  Used only for PDUs? */
#define SNMP_ERR_TOOBIG (1)
#define SNMP_ERR_NOSUCHNAME (2)
#define SNMP_ERR_BADVALUE (3)
#define SNMP_ERR_READONLY (4)
#define SNMP_ERR_GENERR (5)

    /*
     * in SNMPv3 PDUs
     */
#define SNMP_ERR_NOACCESS (6)
#define SNMP_ERR_WRONGTYPE (7)
#define SNMP_ERR_WRONGLENGTH (8)
#define SNMP_ERR_WRONGENCODING (9)
#define SNMP_ERR_WRONGVALUE (10)
#define SNMP_ERR_NOCREATION (11)
#define SNMP_ERR_INCONSISTENTVALUE (12)
#define SNMP_ERR_RESOURCEUNAVAILABLE (13)
#define SNMP_ERR_COMMITFAILED (14)
#define SNMP_ERR_UNDOFAILED (15)
#define SNMP_ERR_AUTHORIZATIONERROR (16)
#define SNMP_ERR_NOTWRITABLE (17)

    /*
     * in SNMPv3 PDUs
     */
#define SNMP_ERR_INCONSISTENTNAME (18)

#define MAX_SNMP_ERR 18

#define SNMP_VALIDATE_ERR(x) ((x > MAX_SNMP_ERR) ? SNMP_ERR_GENERR : (x < SNMP_ERR_NOERROR) ? SNMP_ERR_GENERR \
                                                                                            : x)

    /*
     * row status values
     */
#define SNMP_ROW_NONEXISTENT 0
#define SNMP_ROW_ACTIVE 1
#define SNMP_ROW_NOTINSERVICE 2
#define SNMP_ROW_NOTREADY 3
#define SNMP_ROW_CREATEANDGO 4
#define SNMP_ROW_CREATEANDWAIT 5
#define SNMP_ROW_DESTROY 6

    /*
     * row storage values
     */
#define SNMP_STORAGE_NONE 0
#define SNMP_STORAGE_OTHER 1
#define SNMP_STORAGE_VOLATILE 2
#define SNMP_STORAGE_NONVOLATILE 3
#define SNMP_STORAGE_PERMANENT 4
#define SNMP_STORAGE_READONLY 5

    /*
     * message processing models
     */
#define SNMP_MP_MODEL_SNMPv3 3

    /*
     * security values
     */
#define SNMP_SEC_MODEL_ANY 0
#define SNMP_SEC_MODEL_USM 3
#define SNMP_SEC_MODEL_TSM 4

#define SNMP_SEC_LEVEL_NOAUTH 1
#define SNMP_SEC_LEVEL_AUTHNOPRIV 2
#define SNMP_SEC_LEVEL_AUTHPRIV 3

#define SNMP_MSG_FLAG_AUTH_BIT 0x01
#define SNMP_MSG_FLAG_PRIV_BIT 0x02
#define SNMP_MSG_FLAG_RPRT_BIT 0x04

    /*
     * control PDU handling characteristics
     */
/** NOTE low byte used for AGENTX_MSG_FLAG_*   */
#define UCD_MSG_FLAG_RESPONSE_PDU 0x100
#define UCD_MSG_FLAG_EXPECT_RESPONSE 0x200
#define UCD_MSG_FLAG_FORCE_PDU_COPY 0x400
#define UCD_MSG_FLAG_ALWAYS_IN_VIEW 0x800
#define UCD_MSG_FLAG_PDU_TIMEOUT 0x1000
#define UCD_MSG_FLAG_ONE_PASS_ONLY 0x2000
#define UCD_MSG_FLAG_TUNNELED 0x4000
#ifdef NETSNMP_USE_REVERSE_ASNENCODING
#define UCD_MSG_FLAG_FORWARD_ENCODE 0x8000
#endif
#define UCD_MSG_FLAG_BULK_TOOBIG 0x010000

    /*
     * view status
     */
#define SNMP_VIEW_INCLUDED 1
#define SNMP_VIEW_EXCLUDED 2

    /*
     * basic oid values
     */
#define SNMP_OID_INTERNET 1, 3, 6, 1
#define SNMP_OID_ENTERPRISES SNMP_OID_INTERNET, 4, 1
#define SNMP_OID_MIB2 SNMP_OID_INTERNET, 2, 1
#define SNMP_OID_SNMPV2 SNMP_OID_INTERNET, 6
#define SNMP_OID_SNMPMODULES SNMP_OID_SNMPV2, 3

    /*
     * lengths as defined by TCs
     */
#define SNMPADMINLENGTH 255

    NETSNMP_IMPORT
    char *uptime_string(u_long, char *);
    char *uptime_string_n(u_long, char *, size_t);
    NETSNMP_IMPORT
    void xdump(const void *, size_t, const char *);
    NETSNMP_IMPORT
    u_char *snmp_parse_var_op(u_char *, oid *, size_t *, u_char *,
                              size_t *, u_char **, size_t *);
    NETSNMP_IMPORT
    u_char *snmp_build_var_op(u_char *, const oid *, size_t *, u_char,
                              size_t, const void *, size_t *);

#ifdef NETSNMP_USE_REVERSE_ASNENCODING
    int snmp_realloc_rbuild_var_op(u_char **pkt,
                                   size_t *pkt_len,
                                   size_t *offset,
                                   int allow_realloc,
                                   const oid *name,
                                   size_t *name_len,
                                   u_char value_type,
                                   u_char *value,
                                   size_t value_length);
#endif

#ifdef __cplusplus
}
#endif
#endif /* SNMP_H */