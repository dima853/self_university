#include <stdio.h>
#include <stdint.h>
#include <string.h>

typedef struct {
    uint8_t dest_mac[6];
    uint8_t src_mac[6];
    uint16_t ethertype;
    uint8_t payload[1500];
    uint32_t crc;
} EthernetFrame;

/*
This struct defines the structure of an Ethernet frame.

uint8_t dest_mac[6]: This is an array of 6 unsigned 8-bit integers (bytes) representing the destination MAC address.  A MAC address is a unique identifier for a network interface card (NIC). The destination MAC address indicates where the Ethernet frame is being sent.

uint8_t src_mac[6]: This is an array of 6 unsigned 8-bit integers representing the source MAC address.  It identifies the sending NIC.

uint16_t ethertype: This is an unsigned 16-bit integer representing the Ethernet type. The ethertype specifies the protocol encapsulated within the Ethernet frame's payload (e.g., IPv4, IPv6, ARP). 0x0800 commonly represents IPv4.

uint8_t payload[1500]: This is an array of 1500 unsigned 8-bit integers representing the payload of the Ethernet frame.  The payload contains the data being transmitted.  The maximum size of the payload is often limited by the Ethernet standard.

uint32_t crc: This is an unsigned 32-bit integer representing the Cyclic Redundancy Check (CRC). The CRC is a checksum used for error detection. It allows the receiver to verify the integrity of the received Ethernet frame.
*/


void print_mac(uint8_t mac[6]) {
    printf("%02X:%02X:%02X:%02X:%02X:%02X",
    mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

/*
This function takes a MAC address (represented as an array of 6 unsigned 8-bit integers) as input and prints it to the console in the standard human-readable format (e.g., 00:1A:2B:3C:4D:5E).

printf("%02X:%02X:%02X:%02X:%02X:%02X", ...):  This part uses the printf function to format the output.

%02X:  This is a format specifier.  %X means "print the integer in hexadecimal". 02 means "print the number with leading zeros to ensure it is two digits wide." This is crucial for properly formatting MAC addresses.
mac[0], mac[1], ..., mac[5]:  These are the individual bytes of the MAC address that are being passed to printf.  The order is important to print the MAC address in the correct sequence.
*/

int main() {
    EthernetFrame frame;

    // Set MAC addresses
    memset(frame.dest_mac, 0xFF, 6); // Broadcast address
    uint8_t src[6] = {0x00, 0x1A, 0x2B, 0x3C, 0x4D, 0x5E};
    memcpy(frame.src_mac, src, 6);

    frame.ethertype = 0x0800; // IPv4

    printf("Source MAC: ");
    print_mac(frame.src_mac);
    printf("\nDestination MAC: ");
    print_mac(frame.dest_mac);
    printf("\nEthertype: 0x%04X\n", frame.ethertype);

    return 0;
}

/*
This is the main function where the program execution starts.

EthernetFrame frame:  This declares a variable named frame of the EthernetFrame struct type.  This creates a memory space to store the Ethernet frame data.

memset(frame.dest_mac, 0xFF, 6): This function sets the destination MAC address in the frame to the broadcast address.

memset(): This function fills a block of memory with a specific value.
frame.dest_mac:  This is the memory location (the destination MAC address array within the frame struct) to be filled.
0xFF:  This is the value to be used for filling (0xFF represents all bits set to 1, which is the broadcast MAC address).
6:  This is the number of bytes to be filled.  Since the MAC address is 6 bytes long, we fill all 6 bytes.

uint8_t src[6] = {0x00, 0x1A, 0x2B, 0x3C, 0x4D, 0x5E}:  This declares and initializes an array named src of 6 unsigned 8-bit integers.  This array holds the source MAC address.

memcpy(frame.src_mac, src, 6):  This function copies the source MAC address from the src array to the frame.src_mac array.

memcpy(): This function copies a block of memory from one location to another.
frame.src_mac: This is the destination memory location (the source MAC address array within the frame).
src: This is the source memory location (the src array).
6: This is the number of bytes to copy.

frame.ethertype = 0x0800:  This sets the ethertype field in the frame to 0x0800, indicating that the payload contains an IPv4 packet.

printf("Source MAC: "):  This prints the label "Source MAC: " to the console.
print_mac(frame.src_mac):  This calls the print_mac function to print the source MAC address to the console.
printf("\nDestination MAC: "):  This prints the label "Destination MAC: " to the console, followed by a newline character.
print_mac(frame.dest_mac): This calls the print_mac function to print the destination MAC address to the console.
printf("\nEthertype: 0x%04X\n", frame.ethertype): This prints the ethertype value to the console in hexadecimal format, with leading zeros to ensure it is four digits wide, followed by a newline character.

return 0:  This indicates that the program has executed successfully.

*/