#pragma once
#ifndef SNMP_H
#define SNMP_H

#ifdef __cplusplus
extern "C"
{
#endif

// ==================== Network Constants ====================
#define SNMP_PORT 161      // SNMP agent request port
#define SNMP_TRAP_PORT 162 // SNMP manager notification port

#define SNMP_MAX_MSG_SIZE 1500          // Ethernet MTU
#define SNMP_MIN_MSG_SIZE 484           // RFC 3416 minimum
#define SNMP_MAX_PACKET_SIZE 0x7FFFFFFF // Maximum theoretical size

// ==================== Protocol Version ====================
#define SNMP_VERSION_3 3 // SNMPv3

    // ==================== PDU Type Constants ====================
    typedef enum
    {
        SNMP_PDU_GET = 0xA0,      // 160 - GetRequest
        SNMP_PDU_GETNEXT = 0xA1,  // 161 - GetNextRequest
        SNMP_PDU_RESPONSE = 0xA2, // 162 - Response
        SNMP_PDU_SET = 0xA3,      // 163 - SetRequest
        SNMP_PDU_GETBULK = 0xA5,  // 165 - GetBulkRequest
        SNMP_PDU_INFORM = 0xA6,   // 166 - InformRequest
        SNMP_PDU_TRAP2 = 0xA7,    // 167 - SNMPv2-Trap
        SNMP_PDU_REPORT = 0xA8    // 168 - Report
    } SnmpPduType;

#define SNMP_MSG_GET SNMP_PDU_GET
#define SNMP_MSG_GETNEXT SNMP_PDU_GETNEXT
#define SNMP_MSG_RESPONSE SNMP_PDU_RESPONSE
#define SNMP_MSG_SET SNMP_PDU_SET
#define SNMP_MSG_GETBULK SNMP_PDU_GETBULK
#define SNMP_MSG_INFORM SNMP_PDU_INFORM
#define SNMP_MSG_TRAP2 SNMP_PDU_TRAP2
#define SNMP_MSG_REPORT SNMP_PDU_REPORT

    // ==================== Internal Processing States ====================
    typedef enum
    {
        SNMP_STATE_SET_BEGIN = -1,
        SNMP_STATE_SET_RESERVE1 = 0,
        SNMP_STATE_SET_RESERVE2 = 1,
        SNMP_STATE_SET_ACTION = 2,
        SNMP_STATE_SET_COMMIT = 3,
        SNMP_STATE_SET_FREE = 4,
        SNMP_STATE_SET_UNDO = 5,

        SNMP_STATE_CHECK_VALUE = 17,
        SNMP_STATE_ROW_CREATE = 18,
        SNMP_STATE_UNDO_SETUP = 19,
        SNMP_STATE_SET_VALUE = 20,
        SNMP_STATE_CHECK_CONSISTENCY = 21,
        SNMP_STATE_UNDO_SET = 22,
        SNMP_STATE_COMMIT = 23,
        SNMP_STATE_UNDO_COMMIT = 24,
        SNMP_STATE_IRREVERSIBLE_COMMIT = 25,
        SNMP_STATE_UNDO_CLEANUP = 26,

        SNMP_STATE_PRE_REQUEST = 128,
        SNMP_STATE_OBJECT_LOOKUP = 129,
        SNMP_STATE_POST_REQUEST = 130,
        SNMP_STATE_GET_STASH = 131
    } SnmpProcessingState;

    // ==================== Confirmed PDU Detection ====================
    static inline int snmp_is_confirmed_pdu(int pdu_type)
    {
        switch (pdu_type)
        {
        case SNMP_PDU_INFORM:
        case SNMP_PDU_GETBULK:
        case SNMP_PDU_GETNEXT:
        case SNMP_PDU_GET:
        case SNMP_PDU_SET:
            return 1;
        default:
            return 0;
        }
    }

#define SNMP_CMD_CONFIRMED(c) snmp_is_confirmed_pdu(c)

    // ==================== Exception Values ====================
    typedef enum
    {
        SNMP_EXCEPTION_NO_SUCH_OBJECT = 0x80,   // 128
        SNMP_EXCEPTION_NO_SUCH_INSTANCE = 0x81, // 129
        SNMP_EXCEPTION_END_OF_MIB_VIEW = 0x82   // 130
    } SnmpExceptionType;

    // ==================== Error Codes ====================
    typedef enum
    {
        SNMP_ERROR_NO_ERROR = 0,
        SNMP_ERROR_TOO_BIG = 1,
        SNMP_ERROR_NO_SUCH_NAME = 2,
        SNMP_ERROR_BAD_VALUE = 3,
        SNMP_ERROR_READ_ONLY = 4,
        SNMP_ERROR_GENERIC = 5,
        SNMP_ERROR_NO_ACCESS = 6,
        SNMP_ERROR_WRONG_TYPE = 7,
        SNMP_ERROR_WRONG_LENGTH = 8,
        SNMP_ERROR_WRONG_ENCODING = 9,
        SNMP_ERROR_WRONG_VALUE = 10,
        SNMP_ERROR_NO_CREATION = 11,
        SNMP_ERROR_INCONSISTENT_VALUE = 12,
        SNMP_ERROR_RESOURCE_UNAVAILABLE = 13,
        SNMP_ERROR_COMMIT_FAILED = 14,
        SNMP_ERROR_UNDO_FAILED = 15,
        SNMP_ERROR_AUTHORIZATION = 16,
        SNMP_ERROR_NOT_WRITABLE = 17,
        SNMP_ERROR_INCONSISTENT_NAME = 18
    } SnmpErrorCode;

#define MAX_SNMP_ERROR SNMP_ERROR_INCONSISTENT_NAME

    // ==================== Error Validation ====================
    static inline SnmpErrorCode snmp_validate_error(int error_code)
    {
        if (error_code < SNMP_ERROR_NO_ERROR || error_code > MAX_SNMP_ERROR)
        {
            return SNMP_ERROR_GENERIC;
        }
        return (SnmpErrorCode)error_code;
    }

#define SNMP_VALIDATE_ERR(x) snmp_validate_error(x)

    // ==================== Row Management ====================
    typedef enum
    {
        SNMP_ROW_NONEXISTENT = 0,
        SNMP_ROW_ACTIVE = 1,
        SNMP_ROW_NOT_IN_SERVICE = 2,
        SNMP_ROW_NOT_READY = 3,
        SNMP_ROW_CREATE_AND_GO = 4,
        SNMP_ROW_CREATE_AND_WAIT = 5,
        SNMP_ROW_DESTROY = 6
    } SnmpRowStatus;

    typedef enum
    {
        SNMP_STORAGE_NONE = 0,
        SNMP_STORAGE_OTHER = 1,
        SNMP_STORAGE_VOLATILE = 2,
        SNMP_STORAGE_NONVOLATILE = 3,
        SNMP_STORAGE_PERMANENT = 4,
        SNMP_STORAGE_READONLY = 5
    } SnmpStorageType;

// ==================== Security Framework ====================
#define SNMP_MP_MODEL_SNMPv3 3

    typedef enum
    {
        SNMP_SEC_MODEL_ANY = 0,
        SNMP_SEC_MODEL_USM = 3,
        SNMP_SEC_MODEL_TSM = 4
    } SnmpSecurityModel;

    typedef enum
    {
        SNMP_SEC_LEVEL_NO_AUTH_NO_PRIV = 1,
        SNMP_SEC_LEVEL_AUTH_NO_PRIV = 2,
        SNMP_SEC_LEVEL_AUTH_PRIV = 3
    } SnmpSecurityLevel;

    typedef enum
    {
        SNMP_MSG_FLAG_AUTH = 0x01,
        SNMP_MSG_FLAG_PRIV = 0x02,
        SNMP_MSG_FLAG_REPORT = 0x04
    } SnmpMessageFlags;

    // ==================== Control Flags ====================
    typedef enum
    {
        UCD_FLAG_RESPONSE_PDU = 0x100,
        UCD_FLAG_EXPECT_RESPONSE = 0x200,
        UCD_FLAG_FORCE_PDU_COPY = 0x400,
        UCD_FLAG_ALWAYS_IN_VIEW = 0x800,
        UCD_FLAG_PDU_TIMEOUT = 0x1000,
        UCD_FLAG_ONE_PASS_ONLY = 0x2000,
        UCD_FLAG_TUNNELED = 0x4000,
        UCD_FLAG_FORWARD_ENCODE = 0x8000,
        UCD_FLAG_BULK_TOOBIG = 0x010000
    } SnmpControlFlags;

// ==================== OID Base Definitions ===================
#define SNMP_OID_INTERNET 1, 3, 6, 1
#define SNMP_OID_ENTERPRISES SNMP_OID_INTERNET, 4, 1
#define SNMP_OID_MIB2 SNMP_OID_INTERNET, 2, 1
#define SNMP_OID_SNMPV2 SNMP_OID_INTERNET, 6
#define SNMP_OID_SNMPMODULES SNMP_OID_SNMPV2, 3

#define SNMP_ADMIN_STRING_LENGTH 255

    // ==================== Function Declarations ====================
    // Utility functions
    NETSNMP_IMPORT char *snmp_uptime_to_string(unsigned long timeticks, char *buffer);
    NETSNMP_IMPORT char *snmp_uptime_to_string_safe(unsigned long timeticks, char *buffer, size_t buffer_size);
    NETSNMP_IMPORT void snmp_hex_dump(const void *data, size_t data_size, const char *prefix);

    // ASN.1 BER encoding/decoding
    NETSNMP_IMPORT unsigned char *snmp_decode_variable(
        unsigned char *input_data,
        unsigned int *object_id,
        size_t *object_id_length,
        unsigned char *value_type,
        size_t *type_length,
        unsigned char **value_data,
        size_t *value_length);

    NETSNMP_IMPORT unsigned char *snmp_encode_variable(
        unsigned char *output_buffer,
        const unsigned int *object_id,
        size_t *object_id_length,
        unsigned char value_type,
        size_t value_length,
        const void *value_data,
        size_t *buffer_used);

#ifdef NETSNMP_USE_REVERSE_ASNENCODING
    NETSNMP_IMPORT int snmp_encode_variable_realloc(
        unsigned char **packet,
        size_t *packet_size,
        size_t *offset,
        int allow_reallocation,
        const unsigned int *object_id,
        size_t *object_id_length,
        unsigned char value_type,
        unsigned char *value_data,
        size_t value_length);
#endif

#ifdef __cplusplus
}
#endif

#endif // SNMP_H
