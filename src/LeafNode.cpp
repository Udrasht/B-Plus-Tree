#include "RecordPtr.hpp"
#include "LeafNode.hpp"

LeafNode::LeafNode(const TreePtr &tree_ptr) : TreeNode(LEAF, tree_ptr)
{
    this->data_pointers.clear();
    this->next_leaf_ptr = NULL_PTR;
    if (!is_null(tree_ptr))
        this->load();
}

// returns max key within this leaf
Key LeafNode::max()
{
    auto it = this->data_pointers.rbegin();
    return it->first;
}

// inserts <key, record_ptr> to leaf. If overflow occurs, leaf is split
// split node is returned
// TODO: LeafNode::insert_key to be implemented
TreePtr LeafNode::insert_key(const Key &key, const RecordPtr &record_ptr)
{
    TreePtr new_leaf = NULL_PTR; // if leaf is split, new_leaf = ptr to new split node ptr
    data_pointers[key] = record_ptr;
    this->size++;
    if (this->size > FANOUT)
    {
        LeafNode *new_leaf_node = new LeafNode();
        int new_size = ceil((float)FANOUT / 2.0);

        auto map_iterator = this->data_pointers.begin();
        while (new_size > 0)
        {
            ++map_iterator;
            --new_size;
        }

        for (auto it = map_iterator; it != data_pointers.end(); ++it)
        {
            new_leaf_node->data_pointers.insert(*it);
        }

        new_leaf_node->size = new_leaf_node->data_pointers.size();
        new_leaf_node->dump();
        this->data_pointers.erase(map_iterator, this->data_pointers.end());
        this->size = this->data_pointers.size();
        new_leaf = new_leaf_node->tree_ptr;
        this->next_leaf_ptr = new_leaf;
        delete new_leaf_node;
    }
    this->dump();
    return new_leaf;
}

// key is deleted from leaf if exists
// TODO: LeafNode::delete_key to be implemented
void LeafNode::delete_key(const Key &key)
{
    // cout << "LeafNode::delete_key not implemented" << endl;
    auto map = this->data_pointers;
    if (map.find(key) == map.end())
    {
        return;
    }
    this->data_pointers.erase(key);

    this->size--;
    this->dump();
}

void LeafNode::redistribute(TreeNode *siblingNode, string method)
{
    if (method == "RIGHT" || method == "LEFT")
    {

        if (method == "RIGHT")
        {
            auto it = this->data_pointers.begin();
            siblingNode->insert_key(it->first, it->second);
            this->delete_key(it->first);
        }
        else
        {
            auto it = this->data_pointers.rbegin();
            siblingNode->insert_key(it->first, it->second);
            this->delete_key(it->first);
        }
    }
    else if (method == "MERG")
    {
        
        LeafNode *sibling_node = dynamic_cast<LeafNode *>(siblingNode);
        this->next_leaf_ptr = sibling_node->next_leaf_ptr;
        this->size = this->size +sibling_node->size;
        for(auto it:sibling_node->data_pointers){
            this->data_pointers[it.first]=it.second;
        }
        sibling_node->delete_node();
        this->dump();
    }
}

// runs range query on leaf
void LeafNode::range(ostream &os, const Key &min_key, const Key &max_key) const
{
    BLOCK_ACCESSES++;
    for (const auto &data_pointer : this->data_pointers)
    {
        if (data_pointer.first >= min_key && data_pointer.first <= max_key)
            data_pointer.second.write_data(os);
        if (data_pointer.first > max_key)
            return;
    }
    if (!is_null(this->next_leaf_ptr))
    {
        auto next_leaf_node = new LeafNode(this->next_leaf_ptr);
        next_leaf_node->range(os, min_key, max_key);
        delete next_leaf_node;
    }
}

// exports node - used for grading
void LeafNode::export_node(ostream &os)
{
    TreeNode::export_node(os);
    for (const auto &data_pointer : this->data_pointers)
    {
        os << data_pointer.first << " ";
    }
    os << endl;
}

// writes leaf as a mermaid chart
void LeafNode::chart(ostream &os)
{
    string chart_node = this->tree_ptr + "[" + this->tree_ptr + BREAK;
    chart_node += "size: " + to_string(this->size) + BREAK;
    for (const auto &data_pointer : this->data_pointers)
    {
        chart_node += to_string(data_pointer.first) + " ";
    }
    chart_node += "]";
    os << chart_node << endl;
}

ostream &LeafNode::write(ostream &os) const
{
    TreeNode::write(os);
    for (const auto &data_pointer : this->data_pointers)
    {
        if (&os == &cout)
            os << "\n"
               << data_pointer.first << ": ";
        else
            os << "\n"
               << data_pointer.first << " ";
        os << data_pointer.second;
    }
    os << endl;
    os << this->next_leaf_ptr << endl;
    return os;
}

istream &LeafNode::read(istream &is)
{
    TreeNode::read(is);
    this->data_pointers.clear();
    for (int i = 0; i < this->size; i++)
    {
        Key key = DELETE_MARKER;
        RecordPtr record_ptr;
        if (&is == &cin)
            cout << "K: ";
        is >> key;
        if (&is == &cin)
            cout << "P: ";
        is >> record_ptr;
        this->data_pointers.insert(pair<Key, RecordPtr>(key, record_ptr));
    }
    is >> this->next_leaf_ptr;
    return is;
}