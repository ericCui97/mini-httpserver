#include <algorithm>

#include "route.h"

using namespace httprouter;

node::node()
	:node_(new tree_node_t{
	  "",
	  "",
	  {},
	  nullptr,
		})
{
}

node::~node(){
    
}

void node::addRoute(std::string path, handler_t handle) {
	std::string fullPath = path;
	auto node = node_;

	// no-empty tree
	if (node->path.size() > 0 || node->children.size() > 0) {
		while (true) {
			bool have_indices = false;

			//find the longest common prefix.
			std::size_t i = 0;
			auto max = std::min(node->path.size(), path.size());
			for (; i < max && path[i] == node->path[i];) {
				i++;
			}

			// Split edge
			if (i < node->path.size()) {
				auto child = std::shared_ptr<tree_node_t>(new tree_node_t{
				  std::string(node->path.c_str() + i),
				  node->indices,
				  std::move(node->children),
				  node->handle,
				});

				node->children = std::vector<std::shared_ptr<tree_node_t>>{ child };
				node->indices = std::string(node->path.c_str() + i, 1);
				node->path = std::string(path.c_str(), i);
				node->handle = nullptr;
			}

			// make new node a child of this node 
			if (i < path.size()) {
				path = std::string(path.c_str() + i);

				char ch = path[0];

				// Check if a child with the next path byte exists
				for (std::size_t i = 0; i < node->indices.size(); i++) {
					if (ch == node->indices[i]) {
						//i = node.incrementChildPrio(i);
						node = node->children[i];
						have_indices = true;
						break;
					}
				}
				if (have_indices) {
					continue;
				}

				//otherwise insert it 
				if (ch != ':' && ch != '*') {
					node->indices += ch;
					auto child = std::shared_ptr<tree_node_t>(new tree_node_t{
					"",
					"",
					{},
					nullptr,
					});
					node->children.push_back(child);
					node = child;
				}
				insertChild(node.get(), path, handle);
				return;
			}
			else if (i == path.size()) {
				if (node->handle) {
					printf("error ! handle already exists.");
                    exit(1);
				}
				node->handle = handle;
			}
			return;
		}
	}
	else { // Empty tree
		insertChild(node.get(), path, handle);
	}

}

void node::insertChild(tree_node_t* node, std::string& path, handler_t handle) {
	node->path = std::string(path.c_str());
	node->handle = handle;
}


handler_t node::handler(std::string path) {
    auto current = node_;
    while (current) {
        if (path.compare(0, current->path.size(), current->path) != 0)
            return nullptr;
        path.erase(0, current->path.size());
        if (path.empty()) return current->handle;
        auto index = current->indices.find(path[0]);
        if (index == std::string::npos) return nullptr;
        current = current->children[index];
    }
    return nullptr;
}
