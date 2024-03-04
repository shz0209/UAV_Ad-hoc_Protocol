#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>

using namespace std;

// 定义节点
struct Node {
    int id;
    unordered_map<int, int> routingTable;  // 路由表：目标节点 -> 下一跳节点
};

// 定义路由请求
struct RouteRequest {
    int source;
    int destination;
};

class AODV {
private:
    vector<Node> nodes;

public:
    // 添加节点
    void addNode(int nodeId) {
        Node node;
        node.id = nodeId;
        nodes.push_back(node);
    }

    // 处理路由请求
    void handleRouteRequest(const RouteRequest& request) {
        // 在节点中查找目标节点的路由信息
        int nextHop = findRoute(request.source, request.destination);

        if (nextHop != -1) {
            cout << "Node " << request.source << " forwards data to Node " << nextHop << " for destination " << request.destination << endl;
        } else {
            cout << "Route not found for destination " << request.destination << " from Node " << request.source << endl;
        }
    }

    // 处理路由回复
    void handleRouteReply(int source, int destination, int nextHop) {
        // 更新路由表
        updateRoutingTable(source, destination, nextHop);
    }

private:
    // 查找路由表中的下一跳节点
    int findRoute(int source, int destination) {
        if (source >= nodes.size() || destination >= nodes.size()) {
            return -1;  // 节点不存在
        }

        // 查找路由表
        auto it = nodes[source].routingTable.find(destination);
        if (it != nodes[source].routingTable.end()) {
            return it->second;  // 返回下一跳节点
        }

        return -1;  // 未找到路由
    }

    // 更新路由表
    void updateRoutingTable(int source, int destination, int nextHop) {
        if (source >= nodes.size() || destination >= nodes.size() || nextHop >= nodes.size()) {
            return;  // 节点不存在
        }

        // 更新路由表
        nodes[source].routingTable[destination] = nextHop;
    }
};

int main() {
    AODV aodv;

    // 添加节点
    aodv.addNode(0);
    aodv.addNode(1);
    aodv.addNode(2);

    // 模拟路由请求
    RouteRequest request;
    request.source = 0;
    request.destination = 2;

    // 处理路由请求
    aodv.handleRouteRequest(request);

    // 模拟路由回复
    aodv.handleRouteReply(1, 2, 1);

    // 再次处理路由请求
    aodv.handleRouteRequest(request);

    return 0;
}
