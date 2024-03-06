#include "../src/utils_events.h"
#include <fstream>
#include <unistd.h>

int main() {
    int sock_fd = create_netlink_socket();
    bind_netlink_socket(sock_fd);
    receive_netlink_messages(sock_fd);

    // 不会执行到这里，因为循环监听会一直进行
    close(sock_fd);
    return 0;
}