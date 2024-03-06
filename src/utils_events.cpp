#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <arpa/inet.h>
#include <iostream>
#include "utils_events.h"

#define MAX_BUFFER_SIZE 4096

int create_netlink_socket() {
    int sock_fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
    if (sock_fd == -1) {
        perror("Error creating Netlink socket");
        exit(EXIT_FAILURE);
    }
    return sock_fd;
}

void bind_netlink_socket(int sock_fd) {
    struct sockaddr_nl sa;
    memset(&sa, 0, sizeof(sa));
    sa.nl_family = AF_NETLINK;
    sa.nl_groups = RTMGRP_IPV4_ROUTE;

    if (bind(sock_fd, (struct sockaddr*)&sa, sizeof(sa)) == -1) {
        perror("Error binding Netlink socket");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }
}

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
            case RTA_TABLE:
                std::cout << "Routing Table ID: " << *reinterpret_cast<unsigned int*>(RTA_DATA(rta)) << std::endl;
                break;
            // Add more cases for other attributes as needed
            default:
                std::cout << "Unable to Parse: " << rta->rta_type << std::endl;
                break;
        }
        rta = RTA_NEXT(rta, remaining);
    }
}

void handle_route_event(struct nlmsghdr *nlh) {
    struct rtmsg *rtm = (struct rtmsg *)NLMSG_DATA(nlh);
    struct rtattr* rta = RTM_RTA(rtm);
    int remaining = RTM_PAYLOAD(nlh);

    if (nlh->nlmsg_type == RTM_NEWROUTE) {
        printf("Received new route event\n");
        // 处理新增路由事件
    } else if (nlh->nlmsg_type == RTM_DELROUTE) {
        printf("Received delete route event\n");
        // 处理删除路由事件
    } else {
        printf("Received unknown route event\n");
        // 处理其他事件类型
    }

    parseRouteAttributes(rtm, rta, remaining);
}

void receive_netlink_messages(int sock_fd) {
    char buffer[MAX_BUFFER_SIZE];

    while (1) {
        ssize_t len = recv(sock_fd, buffer, sizeof(buffer), 0);
        if (len == -1) {
            perror("Error receiving Netlink message");
            close(sock_fd);
            exit(EXIT_FAILURE);
        }

        struct nlmsghdr *nlh = (struct nlmsghdr *)buffer;

        // 处理消息
        while (NLMSG_OK(nlh, len)) {
            handle_route_event(nlh);

            // 移动到下一个消息
            nlh = NLMSG_NEXT(nlh, len);
        }
    }
}
