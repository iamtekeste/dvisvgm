/*************************************************************************
** DependencyGraph.hpp                                                  **
**                                                                      **
** This file is part of dvisvgm -- a fast DVI to SVG converter          **
** Copyright (C) 2005-2017 Martin Gieseking <martin.gieseking@uos.de>   **
**                                                                      **
** This program is free software; you can redistribute it and/or        **
** modify it under the terms of the GNU General Public License as       **
** published by the Free Software Foundation; either version 3 of       **
** the License, or (at your option) any later version.                  **
**                                                                      **
** This program is distributed in the hope that it will be useful, but  **
** WITHOUT ANY WARRANTY; without even the implied warranty of           **
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the         **
** GNU General Public License for more details.                         **
**                                                                      **
** You should have received a copy of the GNU General Public License    **
** along with this program; if not, see <http://www.gnu.org/licenses/>. **
*************************************************************************/

#ifndef DEPENDENCYGRAPH_HPP
#define DEPENDENCYGRAPH_HPP

#include <map>
#include <memory>
#include <vector>
#include "utility.hpp"

template <typename T>
class DependencyGraph {
	struct GraphNode {
		using Dependees = typename std::vector<GraphNode*>;

		GraphNode (const T &k) : key(k), dependent() {}

		void addDependee (GraphNode *node) {
			if (node) {
				node->dependent = this;
				dependees.push_back(node);
			}
		}

		void unlinkDependees () {
			for (GraphNode *dependee : dependees)
				dependee->dependent = nullptr;
		}

		T key;
		GraphNode *dependent;
		Dependees dependees;
	};

	using NodeMap = std::map<T, std::unique_ptr<GraphNode>>;

	public:
		/** Inserts a new isolated node into the dependency graph. */
		void insert (const T &key) {
			if (!contains(key))
				_nodeMap.emplace(key, util::make_unique<GraphNode>(key));
		}

		/** Inserts a new node to the graph and adds a dependency on an existing one to it.
		 *  @param[in] key ID of new node to insert
		 *  @param[in] dependantKey ID of node the new node should depend on */
		void insert (const T &dependentKey, const T &key) {
			if (!contains(key)) {
				auto dependentIter = _nodeMap.find(dependentKey);
				if (dependentIter != _nodeMap.end()) {
					auto node = util::make_unique<GraphNode>(key);
					dependentIter->second->addDependee(node.get());
					_nodeMap.emplace(key, std::move(node));
				}
			}
		}

		/** Removes a node and all its dependents from the graph. */
		void removeDependencyPath (const T &key) {
			auto it = _nodeMap.find(key);
			if (it != _nodeMap.end()) {
				GraphNode *startNode = it->second.get();
				for (GraphNode *node=startNode; node; node=node->dependent) {
					node->unlinkDependees();
					_nodeMap.erase(node->key);
				}
			}
		}

		/** Returns the IDs of all nodes present in the graph. */
		std::vector<T> getKeys () const {
			std::vector<T> keys;
			for (auto &entry : _nodeMap)
				keys.emplace_back(entry.first);
			return keys;
		}

		bool contains (const T &value) const {
			return _nodeMap.find(value) != _nodeMap.end();
		}

		bool empty () const {
			return _nodeMap.empty();
		}

#if 0
		void writeDOT (std::ostream &os) const {
			os << "digraph {\n";
			for (auto it=_nodeMap.begin(); it != _nodeMap.end(); ++it) {
				GraphNode *node = it->second;
				if (node->dependent)
					os << (node->key) << " -> " << (node->dependent->key) << ";\n";
				else if (node->dependees.empty())
					os << (node->key) << ";\n";
			}
			os << "}\n";
		}
#endif

	private:
		NodeMap _nodeMap;
};

#endif
