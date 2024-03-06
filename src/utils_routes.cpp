#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <unistd.h>
#include <linux/rtnetlink.h>
#include <arpa/inet.h>
#include <iostream>
#include <net/if.h>

struct nl_request {
    struct nlmsghdr nlh;
    struct rtmsg rt;
    char buf[BUFSIZ];
};

void addRoute(const char* destination, const char* gateway, const char* interface) {
    // 创建Netlink套接字
    int nl_sock = socket(AF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
    if (nl_sock < 0) {
        perror("socket");
        return;
    }

    // 构建Netlink请求
    struct nl_request request;
    std::memset(&request, 0, sizeof(request));

    request.nlh.nlmsg_len = NLMSG_LENGTH(sizeof(struct rtmsg));
    request.nlh.nlmsg_type = RTM_NEWROUTE;
    request.nlh.nlmsg_flags = NLM_F_REQUEST | NLM_F_CREATE | NLM_F_EXCL | NLM_F_ACK;
    request.nlh.nlmsg_pid = getpid();

    request.rt.rtm_family = AF_INET;  // IPv4
    request.rt.rtm_table = RT_TABLE_MAIN;
    request.rt.rtm_protocol = RTPROT_STATIC;
    request.rt.rtm_scope = RT_SCOPE_UNIVERSE;
    request.rt.rtm_type = RTN_UNICAST;

    // 添加目标地址和网关
    struct sockaddr_in* dest_addr = (struct sockaddr_in*)(request.buf);
    struct sockaddr_in* gateway_addr = (struct sockaddr_in*)(request.buf + sizeof(struct sockaddr_in));
    
    dest_addr->sin_family = AF_INET;
    dest_addr->sin_addr.s_addr = inet_addr(destination);
    gateway_addr->sin_family = AF_INET;
    gateway_addr->sin_addr.s_addr = inet_addr(gateway);

    // 设置接口
    struct ifaddrmsg* ifa = (struct ifaddrmsg*)(request.buf + 2 * sizeof(struct sockaddr_in));
    ifa->ifa_family = AF_UNSPEC;
    ifa->ifa_index = if_nametoindex(interface);

    // 发送Netlink请求
    if (send(nl_sock, &request, request.nlh.nlmsg_len, 0) == -1) {
        perror("send");
        close(nl_sock);
        return;
    }

    // 等待内核的回应
    char response[BUFSIZ];
    if (recv(nl_sock, response, sizeof(response), 0) == -1) {
        perror("recv");
        close(nl_sock);
        return;
    }

    // 检查回应是否包含错误信息
    struct nlmsghdr *nl_response = (struct nlmsghdr *)response;
    if (nl_response->nlmsg_type == NLMSG_ERROR) {
        struct nlmsgerr *nl_err = (struct nlmsgerr *)NLMSG_DATA(nl_response);
        if (nl_err->error != 0) {
            std::cerr << "Error in Netlink operation: " << strerror(-nl_err->error) << std::endl;
            close(nl_sock);
            return;
        }
    }

    std::cout << "Route added successfully!" << std::endl;

    // 关闭套接字
    close(nl_sock);
}
