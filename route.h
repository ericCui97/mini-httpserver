/*
** EPITECH PROJECT, 2021
** _ROUTE_
** File description:
** route
*/

#ifndef __ROUTE__
#define __ROUTE__
#include <string>
#include <vector>
#include <functional>
#include <memory>
 #include "http.h"

namespace httprouter
{

    typedef std::function<void(HttpRequest& req)> handler_t;

    typedef struct _tree_node
    {
        std::string path;
        std::string indices;
        std::vector<std::shared_ptr<struct _tree_node>> children;
        handler_t handle;
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

    private:
        void insertChild(tree_node_t *node, std::string &path, handler_t handle);

    private:
        std::shared_ptr<tree_node_t> node_;
    };

}

#endif