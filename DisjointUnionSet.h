//
// Created by 冒家豪 on 2023/9/8.
//

#ifndef MAZE_DISJOINTUNIONSET_H
#define MAZE_DISJOINTUNIONSET_H

#include <vector>

class DisjointUnionSet {
public:
    /**
     * Initialize n elements with each form a one-element set
     * Index of elements is from 0~n-1.
     * @param n: number of elements
     */
    void Initialize(int n) {
        parent.resize(n);
        std::fill(parent.begin(), parent.end(), -1);
        num_disjoint_sets = n;
    }

    /**
     * Get parent of elem
     * @param elem: index of elem, from 0 to n-1.
     * @return parent of elem.
     */
    int GetParent(int elem) {
        if (parent[elem] == -1) {
            return elem;
        }
        return parent[elem] = GetParent(parent[elem]);
    }

    /**
     * Union set include elem1 and set include elem2.
     * @param elem1
     * @param elem2
     * @return true if elem1 and elem2 are in two disjoint sets;
     * otherwise return false.
     */
    bool Union(int elem1, int elem2) {
        int parent1 = GetParent(elem1);
        int parent2 = GetParent(elem2);
        if (parent1 == parent2) return false;
        parent[parent2] = parent1;
        --num_disjoint_sets;
        return true;
    }

    int GetNumDisjointSets() const { return num_disjoint_sets; }

private:
    std::vector<int> parent;
    int num_disjoint_sets;
};


#endif //MAZE_DISJOINTUNIONSET_H
