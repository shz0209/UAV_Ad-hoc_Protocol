#include <iostream>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <unistd.h>
#include <arpa/inet.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>

#define BUFFER_SIZE 8192

// Netlink message header structure
struct nlmsghdr* getNextNetlinkMessage(struct nlmsghdr* nlh, int remaining) {
    while (NLMSG_OK(nlh, remaining)) {
        if (nlh->nlmsg_type == NLMSG_DONE) {
            return nullptr;  // End of the list
        }
        nlh = NLMSG_NEXT(nlh, remaining);
    }
    return nullptr;
}

// Parse route attributes
void parseRouteAttributes(struct rtmsg* route, struct rtattr* rta, int remaining) {
    while (RTA_OK(rta, remaining)) {
        switch (rta->rta_type) {
            case RTA_DST:
                std::cout << "Destination: " << inet_ntoa(*(struct in_addr*)RTA_DATA(rta)) << std::endl;
                break;
            case RTA_GATEWAY:
                std::cout << "Gateway: " << inet_ntoa(*(struct in_addr*)RTA_DATA(rta)) << std::endl;
                break;
            case RTA_OIF:
                std::cout << "Outgoing Interface Index: " << *reinterpret_cast<int*>(RTA_DATA(rta)) << std::endl;
                break;
            // Add more cases for other attributes as needed
            default:
                break;
        }
        rta = RTA_NEXT(rta, remaining);
    }
}

int main() {
    // Create a Netlink socket
    int sockfd = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
    if (sockfd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Prepare a Netlink request
    struct sockaddr_nl sa;
    std::memset(&sa, 0, sizeof(sa));
    sa.nl_family = AF_NETLINK;
    sa.nl_pid = getpid();
    sa.nl_groups = RTMGRP_IPV4_ROUTE;  // Monitoring IPv4 routing events

    // Bind the socket
    if (bind(sockfd, (struct sockaddr*)&sa, sizeof(sa)) == -1) {
        perror("bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Prepare a Netlink message
    struct nlmsghdr* nlh = nullptr;
    struct msghdr msg;
    struct iovec iov;
    char buffer[BUFFER_SIZE];

    nlh = (struct nlmsghdr*)buffer;
    iov.iov_base = (void*)nlh;
    iov.iov_len = NLMSG_LENGTH(sizeof(struct rtmsg));
    msg.msg_name = (void*)&sa;
    msg.msg_namelen = sizeof(sa);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    int i = 0;
    // Receive Netlink messages
    while (true) {
        printf("%d\n",i++);
        ssize_t len = recvmsg(sockfd, &msg, 0);
        if (len == -1) {
            perror("recvmsg");
            close(sockfd);
            exit(EXIT_FAILURE);
        }

        // Parse the received messages
        for (nlh = (struct nlmsghdr*)buffer; NLMSG_OK(nlh, len); nlh = getNextNetlinkMessage(nlh, len)) {
            struct rtmsg* route = (struct rtmsg*)NLMSG_DATA(nlh);
            struct rtattr* rta = RTM_RTA(route);
            int remaining = RTM_PAYLOAD(nlh);

            // Parse route attributes
            parseRouteAttributes(route, rta, remaining);
        }
    }

    // Close the socket
    close(sockfd);

    return 0;
}
