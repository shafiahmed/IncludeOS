#ifndef NET_DNS_HPP
#define NET_DNS_HPP

/**
 * DNS message
 * +---------------------+
 * | Header              |
 * +---------------------+
 * | Question            | the question for the name server
 * +---------------------+
 * | Answer              | RRs answering the question
 * +---------------------+
 * | Authority           | RRs pointing toward an authority
 * +---------------------+
 * | Additional          | RRs holding additional information
 * +---------------------+
 * 
 * DNS header
 * +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 * |                     ID                        |
 * +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 * |QR| Opcode    |AA|TC|RD|RA| Z      |  RCODE    |
 * +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 * |                   QDCOUNT                     |
 * +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 * |                   ANCOUNT                     |
 * +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 * |                   NSCOUNT                     |
 * +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 * |                   ARCOUNT                     |
 * +--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+--+
 * 
 **/

#include <net/class_udp.hpp> // UDP headers
#include <string>
#include <vector>
#include <functional>

namespace net
{
#define DNS_QR_QUERY     0
#define DNS_QR_RESPONSE  1

#define DNS_TC_NONE    0 // no truncation
#define DNS_TC_TRUNC   1 // truncated message

#define DNS_CLASS_INET   1

#define DNS_TYPE_A    1  // A record
#define DNS_TYPE_NS   2  // respect mah authoritah
#define DNS_TYPE_ALIAS 5 // name alias

#define DNS_TYPE_SOA  6  // start of authority zone
#define DNS_TYPE_PTR 12  // domain name pointer
#define DNS_TYPE_MX  15  // mail routing information

#define DNS_Z_RESERVED   0


  class DNS
  {
  public:
    static const unsigned short DNS_SERVICE_PORT = 53;
    
    struct header
    {
      unsigned short id;       // identification number
      unsigned char rd :1;     // recursion desired
      unsigned char tc :1;     // truncated message
      unsigned char aa :1;     // authoritive answer
      unsigned char opcode :4; // purpose of message
      unsigned char qr :1;     // query/response flag
      unsigned char rcode :4;  // response code
      unsigned char cd :1;     // checking disabled
      unsigned char ad :1;     // authenticated data
      unsigned char z :1;      // reserved, set to 0
      unsigned char ra :1;     // recursion available
      unsigned short q_count;    // number of question entries
      unsigned short ans_count;  // number of answer entries
      unsigned short auth_count; // number of authority entries
      unsigned short add_count;  // number of resource entries
    } __attribute__ ((packed));
    
    struct question
    {
      unsigned short qtype;
      unsigned short qclass;
    };
    
#pragma pack(push, 1)
    struct rr_data // resource record data
    {
      unsigned short type;
      unsigned short _class;
      unsigned int   ttl;
      unsigned short data_len;
    };
#pragma pack(pop)
    
    enum resp_code
    {
      NO_ERROR     = 0,
      FORMAT_ERROR = 1,
      SERVER_FAIL  = 2,
      NAME_ERROR   = 3,
      NOT_IMPL     = 4, // unimplemented feature
      OP_REFUSED   = 5, // for political reasons
    };
    
    struct full_header
    {
      UDP::full_header full_udp_header;
      header dns_header;
    };
    
    typedef std::function<std::vector<IP4::addr>* (const std::string&)> lookup_func;
    
    static int createResponse(header& hdr, lookup_func func);
    
    static std::string question_string(unsigned short type)
    {
      switch (type)
      {
      case DNS_TYPE_A:
        return "IPv4 address";
      case DNS_TYPE_ALIAS:
        return "Alias";
      case DNS_TYPE_MX:
        return "Mail exchange";
      case DNS_TYPE_NS:
        return "Name server";
      default:
        return "FIXME DNS::question_string(type = " + std::to_string(type) + ")";
      }
    }
    
  };
  
}

#endif
