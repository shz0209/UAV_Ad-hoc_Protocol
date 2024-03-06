int create_netlink_socket();
void bind_netlink_socket(int sock_fd);
void handle_route_event(struct nlmsghdr *nlh);
void receive_netlink_messages(int sock_fd);