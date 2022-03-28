/*
 * =============================================================================
 *
 *       Filename:  net.h
 *
 *    Description:  Net Protocol Header File
 *
 *        Version:  1.0
 *        Created:  2011/11/07 14:04
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  ShadowStar, <orphen.leiliu@gmail.com>
 *        Company:  Gmail
 *
 * =============================================================================
 */

#ifndef __NET_H
#define __NET_H
#include <stdint.h>

#ifndef __BIG_ENDIAN
#define __BIG_ENDIAN        4321
#endif
#ifndef __LITTLE_ENDIAN
#define __LITTLE_ENDIAN     1234
#endif

#ifndef __BYTE_ORDER
#ifdef __BYTE_ORDER__
#define __BYTE_ORDER        __BYTE_ORDER__
#elif defined(__x86_64__) || defined(__i386__) ||                           \
    defined(__MIPSEL) || defined(__ARMEL__)
#define __BYTE_ORDER        __LITTLE_ENDIAN
#elif defined(__MIPSEB) || defined(__ARMEB__)
#define __BYTE_ORDER        __BIG_ENDIAN
#else
#error Unknown Platform
#endif
#endif  /* __BYTE_ORDER */
/*
 *    IEEE 802.3 Ethernet magic constants.  The frame sizes omit the preamble
 *    and FCS/CRC (frame check sequence).
 */

#define ETH_ALEN        6       /* Octets in one ethernet addr   */
#define ETH_HLEN        14      /* Total octets in header.       */
#define ETH_ZLEN        60      /* Min. octets in frame sans FCS */
#define ETH_DATA_LEN    1500    /* Max. octets in payload        */
#define ETH_FRAME_LEN   1514    /* Max. octets in frame sans FCS */
#define ETH_FCS_LEN     4       /* Octets in the FCS             */

/*
 *    These are the defined Ethernet Protocol ID's.
 */

#define ETH_P_LOOP      0x0060  /* Ethernet Loopback packet     */
#define ETH_P_PUP       0x0200  /* Xerox PUP packet             */
#define ETH_P_PUPAT     0x0201  /* Xerox PUP Addr Trans packet  */
#define ETH_P_IP        0x0800  /* Internet Protocol packet     */
#define ETH_P_X25       0x0805  /* CCITT X.25                   */
#define ETH_P_ARP       0x0806  /* Address Resolution packet    */
#define    ETH_P_BPQ    0x08FF  /* G8BPQ AX.25 Ethernet Packet
                                 * [ NOT AN OFFICIALLY REGISTERED ID ] */
#define ETH_P_IEEEPUP   0x0a00  /* Xerox IEEE802.3 PUP packet */
#define ETH_P_IEEEPUPAT 0x0a01  /* Xerox IEEE802.3 PUP Addr Trans packet */
#define ETH_P_DEC       0x6000  /* DEC Assigned proto           */
#define ETH_P_DNA_DL    0x6001  /* DEC DNA Dump/Load            */
#define ETH_P_DNA_RC    0x6002  /* DEC DNA Remote Console       */
#define ETH_P_DNA_RT    0x6003  /* DEC DNA Routing              */
#define ETH_P_LAT       0x6004  /* DEC LAT                      */
#define ETH_P_DIAG      0x6005  /* DEC Diagnostics              */
#define ETH_P_CUST      0x6006  /* DEC Customer use             */
#define ETH_P_SCA       0x6007  /* DEC Systems Comms Arch       */
#define ETH_P_RARP      0x8035  /* Reverse Addr Res packet      */
#define ETH_P_ATALK     0x809B  /* Appletalk DDP                */
#define ETH_P_AARP      0x80F3  /* Appletalk AARP               */
#define ETH_P_8021Q     0x8100  /* 802.1Q VLAN Extended Header  */
#define ETH_P_IPX       0x8137  /* IPX over DIX                 */
#define ETH_P_IPV6      0x86DD  /* IPv6 over bluebook           */
#define ETH_P_PAUSE     0x8808  /* IEEE Pause frames. See 802.3 31B */
#define ETH_P_SLOW      0x8809  /* Slow Protocol. See 802.3ad 43B */
#define ETH_P_WCCP      0x883E  /* Web-cache coordination protocol defined in
                                 * draft-wilson-wrec-wccp-v2-00.txt */
#define ETH_P_PPP_DISC  0x8863  /* PPPoE discovery messages     */
#define ETH_P_PPP_SES   0x8864  /* PPPoE session messages       */
#define ETH_P_MPLS_UC   0x8847  /* MPLS Unicast traffic         */
#define ETH_P_MPLS_MC   0x8848  /* MPLS Multicast traffic       */
#define ETH_P_ATMMPOA   0x884c  /* MultiProtocol Over ATM       */
#define ETH_P_ATMFATE   0x8884  /* Frame-based ATM Transport
                                 * over Ethernet */
#define ETH_P_AOE       0x88A2  /* ATA over Ethernet            */
#define ETH_P_TIPC      0x88CA  /* TIPC                         */

/*
 *    Non DIX types. Won't clash for 1500 types.
 */

#define ETH_P_802_3     0x0001  /* Dummy type for 802.3 frames  */
#define ETH_P_AX25      0x0002  /* Dummy protocol id for AX.25  */
#define ETH_P_ALL       0x0003  /* Every packet (be careful!!!) */
#define ETH_P_802_2     0x0004  /* 802.2 frames                 */
#define ETH_P_SNAP      0x0005  /* Internal only                */
#define ETH_P_DDCMP     0x0006  /* DEC DDCMP: Internal only     */
#define ETH_P_WAN_PPP   0x0007  /* Dummy type for WAN PPP frames */
#define ETH_P_PPP_MP    0x0008  /* Dummy type for PPP MP frames */
#define ETH_P_LOCALTALK 0x0009  /* Localtalk pseudo type        */
#define ETH_P_CAN       0x000C  /* Controller Area Network      */
#define ETH_P_PPPTALK   0x0010  /* Dummy type for Atalk over PPP */
#define ETH_P_TR_802_2  0x0011  /* 802.2 frames                 */
#define ETH_P_MOBITEX   0x0015  /* Mobitex (kaz@cafe.net)       */
#define ETH_P_CONTROL   0x0016  /* Card specific control frames */
#define ETH_P_IRDA      0x0017  /* Linux-IrDA                   */
#define ETH_P_ECONET    0x0018  /* Acorn Econet                 */
#define ETH_P_HDLC      0x0019  /* HDLC frames                  */
#define ETH_P_ARCNET    0x001A  /* 1A for ArcNet :-)            */
#define ETH_P_HEARTBEAT 0x8888
#define ETH_P_RELAY     0x9099  /* identify of relay packet */
#define ETH_P_SIDE      0x9200  /* Safety Isolation and Data Exchange */
#define ETH_P_SIDE_END  0x92FF  /** Safety Isolation and Data Exchange */
#define ETH_P_STA_MESG  0xFFFE  /* identify of status message */
#define ETH_P_CTL_MESG  0xFFFF  /* identify of control message */

#ifndef IPPROTO_RAW
enum {
    IPPROTO_IP      = 0,        /* Dummy protocol for TCP */
    IPPROTO_ICMP    = 1,        /* Internet Control Message Protocol */
    IPPROTO_IGMP    = 2,        /* Internet Group Management Protocol */
    IPPROTO_IPIP    = 4,        /* IPIP tunnels (older KA9Q tunnels use 94) */
    IPPROTO_TCP     = 6,        /* Transmission Control Protocol */
    IPPROTO_EGP     = 8,        /* Exterior Gateway Protocol */
    IPPROTO_PUP     = 12,       /* PUP protocol */
    IPPROTO_UDP     = 17,       /* User Datagram Protocol */
    IPPROTO_IDP     = 22,       /* XNS IDP protocol */
    IPPROTO_DCCP    = 33,       /* Datagram Congestion Control Protocol */
    IPPROTO_RSVP    = 46,       /* RSVP protocol */
    IPPROTO_GRE     = 47,       /* Cisco GRE tunnels (rfc 1701,1702) */
    IPPROTO_IPV6    = 41,       /* IPv6-in-IPv4 tunnelling */
    IPPROTO_ESP     = 50,       /* Encapsulation Security Payload protocol */
    IPPROTO_AH      = 51,       /* Authentication Header protocol */
    IPPROTO_BEETPH  = 94,       /* IP option pseudo header for BEET */
    IPPROTO_PIM     = 103,      /* Protocol Independent Multicast */
    IPPROTO_COMP    = 108,      /* Compression Header protocol */
    IPPROTO_SCTP    = 132,      /* Stream Control Transport Protocol */
    IPPROTO_UDPLITE = 136,      /* UDP-Lite (RFC 3828) */
    IPPROTO_RAW     = 255,      /* Raw IP packets */
    IPPROTO_MAX
};
#endif

#define IPOPT_COPY          0x80
#define IPOPT_CLASS_MASK    0x60
#define IPOPT_NUMBER_MASK   0x1f

#define IPOPT_COPIED(o)     ((o)&IPOPT_COPY)
#define IPOPT_CLASS(o)      ((o)&IPOPT_CLASS_MASK)
#define IPOPT_NUMBER(o)     ((o)&IPOPT_NUMBER_MASK)

#define IPOPT_CONTROL       0x00
#define IPOPT_RESERVED1     0x20
#define IPOPT_MEASUREMENT   0x40
#define IPOPT_RESERVED2     0x60

#define IPOPT_END           (0 |IPOPT_CONTROL)
#define IPOPT_NOOP          (1 |IPOPT_CONTROL)
#define IPOPT_SEC           (2 |IPOPT_CONTROL|IPOPT_COPY)
#define IPOPT_LSRR          (3 |IPOPT_CONTROL|IPOPT_COPY)
#define IPOPT_TIMESTAMP     (4 |IPOPT_MEASUREMENT)
#define IPOPT_CIPSO         (6 |IPOPT_CONTROL|IPOPT_COPY)
#define IPOPT_RR            (7 |IPOPT_CONTROL)
#define IPOPT_SID           (8 |IPOPT_CONTROL|IPOPT_COPY)
#define IPOPT_SSRR          (9 |IPOPT_CONTROL|IPOPT_COPY)
#define IPOPT_RA            (20|IPOPT_CONTROL|IPOPT_COPY)

#define IPVERSION           4
#define MAXTTL              255
#define IPDEFTTL            64

#define IPOPT_OPTVAL        0
#define IPOPT_OLEN          1
#define IPOPT_OFFSET        2
#define IPOPT_MINOFF        4
#define MAX_IPOPTLEN        40
#define IPOPT_NOP           IPOPT_NOOP
#define IPOPT_EOL           IPOPT_END
#define IPOPT_TS            IPOPT_TIMESTAMP

#define IPOPT_TS_TSONLY     0   /* timestamps only */
#define IPOPT_TS_TSANDADDR  1   /* timestamps and addresses */
#define IPOPT_TS_PRESPEC    3   /* specified modules only */

/*
 *    NextHeader field of IPv6 header
 */

#define IP6_NEXTHDR_HOP         0   /* Hop-by-hop option header. */
#define IP6_NEXTHDR_TCP         6   /* TCP segment. */
#define IP6_NEXTHDR_UDP         17  /* UDP message. */
#define IP6_NEXTHDR_IPV6        41  /* IPv6 in IPv6 */
#define IP6_NEXTHDR_ROUTING     43  /* Routing header. */
#define IP6_NEXTHDR_FRAGMENT    44  /* Fragmentation/reassembly header. */
#define IP6_NEXTHDR_ESP         50  /* Encapsulating security payload. */
#define IP6_NEXTHDR_AUTH        51  /* Authentication header. */
#define IP6_NEXTHDR_ICMP        58  /* ICMP for IPv6. */
#define IP6_NEXTHDR_NONE        59  /* No next header */
#define IP6_NEXTHDR_DEST        60  /* Destination options header. */
#define IP6_NEXTHDR_MOBILITY    135 /* Mobility header. */

#define IP6_NEXTHDR_MAX         255

#ifndef __KERNEL__
struct ethhdr {
    unsigned char dest[ETH_ALEN];
    unsigned char source[ETH_ALEN];
    unsigned short proto;
} __attribute__ ((packed));

#define IP_CE                   0x8000      /* Flag: "Congestion"        */
#define IP_DF                   0x4000      /* Flag: "Don't Fragment"    */
#define IP_MF                   0x2000      /* Flag: "More Fragments"    */
#define IP_OFFSET               0x1FFF      /* "Fragment Offset" part    */

#define TCPOPT_NOP                  1       /* Padding */
#define TCPOPT_EOL                  0       /* End of options */
#define TCPOPT_MSS                  2       /* Segment size negotiating */
#define TCPOPT_WINDOW               3       /* Window scaling */
#define TCPOPT_SACK_PERM            4       /* SACK Permitted */
#define TCPOPT_SACK                 5       /* SACK Block */
#define TCPOPT_TIMESTAMP            8       /* Better RTT estimations/PAWS */
#define TCPOPT_MD5SIG               19      /* MD5 Signature (RFC2385) */
#define TCPOPT_FASTOPEN             34      /* Fast open (RFC7413) */
#define TCPOPT_EXP                  254     /* Experimental */
#define TCPOPT_FASTOPEN_MAGIC       0xF989

#define TCPOLEN_MSS                 4
#define TCPOLEN_WINDOW              3
#define TCPOLEN_SACK_PERM           2
#define TCPOLEN_TIMESTAMP           10
#define TCPOLEN_MD5SIG              18
#define TCPOLEN_FASTOPEN_BASE       2
#define TCPOLEN_EXP_FASTOPEN_BASE   4

struct iphdr {
    uint8_t version:4,
            ihl:4;
    uint8_t tos;
    uint16_t tot_len;
    uint16_t id;
    uint16_t frag_off;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t check;
    uint32_t saddr;
    uint32_t daddr;
    /*The options start here. */
} __attribute__ ((packed));

struct ipv6hdr {
    uint8_t version:4,
            priority:4;
    uint8_t flow_lbl[3];
    uint16_t payload_len;
    uint8_t nexthdr;
    uint8_t hop_limit;
    uint64_t saddr[2];
    uint64_t daddr[2];
} __attribute__ ((packed));

struct ipv6_opt_hdr {
    uint8_t nexthdr;
    uint8_t hdrlen;
} __attribute__ ((packed));

struct ipv6_rt_hdr {
    uint8_t nexthdr;
    uint8_t hdrlen;
    uint8_t type;
    uint8_t segments_left;
} __attribute__ ((packed));

struct ipv6_frag_hdr {
    uint8_t nexthdr;
    uint8_t reserved;
    uint16_t frag_off;
    uint32_t identification;
} __attribute__ ((packed));

struct udphdr {
    uint16_t source;
    uint16_t dest;
    uint16_t len;
    uint16_t check;
} __attribute__ ((packed));

#define TCP_F_CWR       0x80
#define TCP_F_ECE       0x40
#define TCP_F_URG       0x20
#define TCP_F_ACK       0x10
#define TCP_F_PSH       0x08
#define TCP_F_RST       0x04
#define TCP_F_SYN       0x02
#define TCP_F_FIN       0x01

struct tcphdr {
    uint16_t source;
    uint16_t dest;
    uint32_t seq;
    uint32_t ack_seq;
    uint8_t doff:4,
            res1:4;
    uint8_t flags;
    uint16_t window;
    uint16_t check;
    uint16_t urg_ptr;
} __attribute__ ((packed));
#endif    /* __KERNEL__ */

#define VLAN_TAG_MASK   0xFFF
#define VLAN_TAG_MAX    4095

struct vlanhdr {
    uint16_t tag;
    uint16_t proto;
} __attribute__ ((packed));

#define VLAN_HLEN ((int)sizeof(struct vlanhdr))

#define MAC_BROADCAST   0xFFFFFFFFFFFFUL

static inline int is_bc_hw_addr(const uint8_t *addr)
{
    return (*(uint64_t *)addr >> 16) == MAC_BROADCAST;
}

static inline int is_mc_hw_addr(const uint8_t *addr)
{
    return (*addr & 0x01);
}

static inline int is_zero_hw_addr(const uint8_t *addr)
{
    return (*(uint64_t *)addr >> 16) == 0x0;
}

static inline int is_eq_hw_addr(const uint8_t *src, const uint8_t *dst)
{
    return ((*(uint64_t *)src ^ *(uint64_t *)dst) >> 16) == 0;
}

static inline int is_invalid_hw_addr(const uint8_t *addr)
{
    return is_bc_hw_addr(addr) || is_mc_hw_addr(addr) || is_zero_hw_addr(addr);
}

static inline int is_bc_ip_addr(const uint32_t ip)
{
    return (ip == 0xFFFFFFFF);
}

static inline int is_loop_ip_addr(const uint32_t ip)
{
    return (ip >> 24) == 127;
}

static inline int is_err_ip_addr(const uint32_t ip)
{
    return (ip >> 24) == 0;
}

static inline int is_invalid_ip(const uint32_t ip)
{
    switch (ip >> 24) {
    case 255:
    case 127:
    case 0:
        return 1;
    default:
        return 0;
    }
}

static inline int is_err_ip_mask(const uint32_t mask)
{
    return (~mask & (~mask + 1)) == 0;
}

static inline int is_ip6_ext_hdr(const uint8_t nexthdr)
{
    return ((nexthdr == IP6_NEXTHDR_HOP) ||
            (nexthdr == IP6_NEXTHDR_ROUTING) ||
            (nexthdr == IP6_NEXTHDR_FRAGMENT) ||
            (nexthdr == IP6_NEXTHDR_ESP) ||
            (nexthdr == IP6_NEXTHDR_AUTH) ||
            (nexthdr == IP6_NEXTHDR_NONE) ||
            (nexthdr == IP6_NEXTHDR_DEST));
}

struct ethvlanhdr {
    uint8_t dest[ETH_ALEN];
    uint8_t source[ETH_ALEN];
    uint16_t p8021q;
    struct vlanhdr v;
} __attribute__ ((packed));

#define ETHVLAN_HLEN    ((int)sizeof(struct ethvlanhdr))

#define ARP_HW_ETHER    0x1
#define ARP_REQUEST     0x1
#define ARP_REPLY       0x2

struct pppoehdr {
    uint8_t type:4,
            ver:4;
    uint8_t code;
    uint16_t sid;
    uint16_t length;
    uint16_t proto;
} __attribute__ ((packed));

#define ETH_PPP_IP          0x0021
#define ETH_PPP_VJ_COMP     0x002d
#define ETH_PPP_VJ_UNCOMP   0x002f
#define PPPOE_HLEN          0x8

struct grehdr {
    uint8_t flag;
    uint8_t version;
    uint16_t proto;
} __attribute__ ((packed));

#define GRE_HLEN            4

static inline uint16_t pppoe_proto(const struct ethhdr *eth)
{
    return *((uint16_t *)((uint8_t *)eth + ETH_HLEN + sizeof(struct pppoehdr)));
}

struct arphdr {
    uint16_t hw_type;
    uint16_t proto_type;
    uint8_t hw_size;
    uint8_t proto_size;
    uint16_t opcode;
    uint8_t sha[ETH_ALEN];
    uint8_t sip[4];
    uint8_t dha[ETH_ALEN];
    uint8_t dip[4];
} __attribute__ ((packed));

#define ARP_HLEN        ((int)sizeof(struct arphdr))

#define IP_HLEN         ((int)sizeof(struct iphdr))

#define UDP_HLEN        ((int)sizeof(struct udphdr))

#define TCP_HLEN        ((int)sizeof(struct tcphdr))

#define ICMP_ECHOREPLY  0
#define ICMP_ECHO       8

struct icmphdr {
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
    union {
        struct {
            uint16_t id;
            uint16_t seq;
        } echo;
        struct {
            uint16_t __unused;
            uint16_t mtu;
        } frag;
        uint32_t gateway;
    } un;
} __attribute__ ((packed));

#define ICMP_HLEN       ((int)sizeof(struct icmphdr))

struct icmpv6hdr {
    uint8_t type;
    uint8_t code;
    uint16_t cksum;
    union {
        uint32_t un32[1];
        uint16_t un16[2];
        uint8_t un8[4];
        struct {
            uint16_t id;
            uint16_t seq;
        } echo;
        struct {
            uint32_t router:1,
                     solicited:1,
                     override:1,
                     reserved:29;
        } advt;
        struct {
            uint8_t hop_limit;
            uint8_t managed:1,
                    other:1,
                    home_agent:1,
                    router_pref:2,
                    reserved:3;
            uint16_t rt_lifetime;
        } ra;
    } un;
} __attribute__ ((packed));

struct vxlanhdr {
    uint32_t flags:8,   /* 0x08 */
             _rsv0:24;
    uint32_t vni:24,
             _rsv1:8;
} __attribute__ ((packed));

#define VXLAN_HLEN      ((int)sizeof(struct vxlanhdr))
#define VXLAN_PORT      4789

struct heartbhdr {
    uint8_t version;
    uint8_t start:1,
            autobc:1,
            master:1,
            len:5;
    uint16_t cksum;
    uint16_t group;
    uint16_t prio;
    uint32_t id;
    uint32_t vip;
    uint32_t tim_tik;
    uint32_t threshold;
} __attribute__ ((packed));

#define HEARTBEAT_HDR_SIZE  (ALIGN((int)sizeof(struct heartbhdr), 4))
#define HEARTBEAT_HLEN      (HEARTBEAT_HDR_SIZE / 4)
#define HEARTBEAT_PAD_SIZE  (HEARTBEAT_HDR_SIZE > 68 ? HEARTBEAT_HDR_SIZE : 68)

#define BFD_STAT_ADMIN_DOWN 0x0
#define BFD_STAT_DOWN       0x1
#define BFD_STAT_INIT       0x2
#define BFD_STAT_UP         0x3

struct bfdhdr {
    uint8_t version:3,
            diag:5;
    uint8_t stat:2,
            poll:1,
            final:1,
            ctl:1,
            auth:1,
            demand:1,
            multi:1;
    uint8_t detect_mult;
    uint8_t len;
    uint32_t my_discr;
    uint32_t you_discr;
    uint32_t tx_interval;
    uint32_t rx_interval;
    uint32_t echo_rx_interval;
} __attribute__ ((packed));

#define BFD_HLEN        ((int)sizeof(struct bfdhdr))

#define DNS_QUERY       0x0000
#define DNS_RESPONSE    0x8000

struct dnshdr {
    uint16_t id;
    uint16_t flags;
    uint16_t ques;
    uint16_t answer;
    uint16_t auth;
    uint16_t addition;
} __attribute__ ((packed));

#define DNS_HLEN        ((int)sizeof(struct dnshdr))

#define ETH_IP_DNS              53
#define ETH_IP_UDP_BFD          3784
#define ETH_IP_UDP_BFD_ECHO     3785

struct ethrelayhdr {
    uint8_t dest[ETH_ALEN];     // ff:ff:ff:ff:ff:ff for local
    uint8_t src[ETH_ALEN];      // original from packet
    uint16_t proto;             // ETH_P_RELAY
    uint8_t port;               // Port ID
#if __BYTE_ORDER == __LITTLE_ENDIAN
    uint8_t reason:7,           // reason ID
            dir:1;              // Input: 0; Output: 1
#else
    uint8_t dir:1,              // Input: 0; Output: 1
            reason:7;           // reason ID
#endif
} __attribute__ ((packed));

#define SIDE_OQ_NUM     128
enum {
    SIDE_OPS_PKT = 1,
    __SIDE_OPS_MAX
};

#define SIDE_MAGIC      0x0182  /**
                                 * In order to force SIDE packets be droped by
                                 * network device, the lowest bit of first byte
                                 * MUST be 1 like the multi-cast SMAC packets
                                 */
struct side_hdr {
    uint8_t oq_id;
    uint8_t ops;
    uint16_t data_len;
    uint16_t oq_avail;
    uint16_t magic_code;        /** SIDE_MAGIC */
    uint32_t seq;
    uint16_t ether_type;        /* ETH_P_SIDE */
    uint16_t sid;
} __attribute__ ((packed));

#endif
