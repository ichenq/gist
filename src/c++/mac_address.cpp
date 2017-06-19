#ifdef _WIN32
# include <winsock2.h>
# include <iphlpapi.h>
# pragma comment(lib, "iphlpapi.lib")
#else
# include <sys/types.h>
# include <ifaddrs.h>
# include <netpacket/packet.h>
#endif

#ifdef _WIN32
std::vector<std::string> GetLocalMacAddressList()
{
    std::vector<std::string> list;
    DWORD bufLen = 0;
    DWORD dwErr = GetAdaptersAddresses(0, 0, NULL, NULL, &bufLen);
    if (dwErr == ERROR_BUFFER_OVERFLOW && bufLen > 0)
    {
        std::string buffer(bufLen, '\0');
        IP_ADAPTER_ADDRESSES* ptr = reinterpret_cast<IP_ADAPTER_ADDRESSES*>(&buffer[0]);
        dwErr = GetAdaptersAddresses(0, 0, NULL, ptr, &bufLen);
        if (dwErr == NO_ERROR)
        {
            while (ptr != NULL)
            {
                if (ptr->PhysicalAddressLength > 0)
                {
                    ByteRange addr(ptr->PhysicalAddress, ptr->PhysicalAddressLength);
                    list.push_back(hexlify(addr));
                }
                ptr = ptr->Next;
            }
        }
    }
    return list;
}
    
#else
    
std::vector<std::string> GetLocalMacAddressList()
{
    std::vector<std::string> list;
    ifaddrs* ifap = NULL;
    if (getifaddrs(&ifap) == 0)
    {
        ifaddrs* iter = ifap;
        while (iter != NULL)
        {
            sockaddr_ll* sal = reinterpret_cast<sockaddr_ll*>(iter->ifa_addr);
            if (sal != NULL && sal->sll_family == AF_PACKET)
            {
                if (!(sal->sll_addr[0] == 0 &&
                      sal->sll_addr[1] == 0 &&
                      sal->sll_addr[2] == 0 &&
                      sal->sll_addr[3] == 0 &&
                      sal->sll_addr[4] == 0 &&
                      sal->sll_addr[5] == 0 &&
                      sal->sll_addr[7] == 0
                      ))
                {
                    ByteRange addr((const unsigned char*)sal->sll_addr, (unsigned)sal->sll_halen);
                    list.push_back(hexlify(addr));
                }
            }
            iter = iter->ifa_next;
        }
        freeifaddrs(ifap);
    }
    return list;
}
#endif
    