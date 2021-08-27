

#pragma once
#include <functional>
#include <memory>
#include <string>
#include <vector>
#include "../http/http.h"

namespace httprouter
{
typedef std::function<void(HttpRequest &)> handler_t;

typedef struct _tree_node {
    std::string path;
    std::string indices;
    std::vector<std::shared_ptr<struct _tree_node>> children;
    handler_t handle;
    int fd;
} tree_node_t;

class node
{
public:
    //! ctor
    node();

    //! dtor
    ~node(void);

    //! copy ctor
    node(const node &) = delete;
    //! assignment operator
    node &operator=(const node &) = delete;

    //! addRouter adds a node with the given handle to the path
    //! Not concurrency-safe!
    void addRoute(std::string path, handler_t handle);

    //! get path handler
    handler_t handler(std::string path);
    void handle(HttpRequest &req) { handler(req.getUrl()); }
    void setFd(HttpRequest &req, int fd)
    {
        std::string path = req.getUrl();
        auto node = node_;
        while (true) {
            if (path.size() > node->path.size()) {
                if (std::string(path.c_str(), node->path.size()) ==
                    node->path) {
                    path = std::string(path.c_str() + node->path.size());
                }

                char ch = path[0];
                for (std::size_t i = 0; i < node->indices.size(); i++) {
                    if (ch == node->indices[i]) {
                        node = node->children[i];
                        continue;
                    }
                }
                // handle wildcard child
                // fix me
            } else if (path == node->path) {
                node->fd = fd;
            }
        }
    }

private:
    void insertChild(tree_node_t *node, std::string &path, handler_t handle);

private:
    std::shared_ptr<tree_node_t> node_;
};

}  // namespace httprouter
