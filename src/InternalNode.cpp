#include "InternalNode.hpp"

//creates internal node pointed to by tree_ptr or creates a new one
InternalNode::InternalNode(const TreePtr &tree_ptr) : TreeNode(INTERNAL, tree_ptr) {
    this->keys.clear();
    this->tree_pointers.clear();
    if (!is_null(tree_ptr))
        this->load();
}

//max element from tree rooted at this node
Key InternalNode::max() {
    Key max_key = DELETE_MARKER;
    TreeNode* last_tree_node = TreeNode::tree_node_factory(this->tree_pointers[this->size - 1]);
    max_key = last_tree_node->max();
    delete last_tree_node;
    return max_key;
}

//if internal node contains a single child, it is returned
TreePtr InternalNode::single_child_ptr() {
    if (this->size == 1)
        return this->tree_pointers[0];
    return NULL_PTR;
}

//inserts <key, record_ptr> into subtree rooted at this node.
//returns pointer to split node if exists
//TODO: InternalNode::insert_key to be implemented
TreePtr InternalNode::insert_key(const Key &key, const RecordPtr &record_ptr) {
    TreePtr new_tree_ptr = NULL_PTR;
    int index=0;

    while(index < this->keys.size() && key > this->keys[index]) ++index;
    TreePtr tree_ptr = this->tree_pointers[index];
    TreeNode* tree_node = TreeNode::tree_node_factory(tree_ptr);
    TreePtr split_node_ptr = tree_node->insert_key(key, record_ptr);

    if(!is_null(split_node_ptr)) {
        Key split_key = tree_node->max();
        this->keys.insert(this->keys.begin()+index, split_key);
        this->tree_pointers.insert(this->tree_pointers.begin()+index+1, split_node_ptr);
        this->size++;

        if(this->tree_pointers.size() > FANOUT) {
            int new_size = ceil((float)FANOUT / 2.0);
            InternalNode* new_tree_node = new InternalNode();
            new_tree_node->keys.insert(new_tree_node->keys.end(), this->keys.begin()+new_size, this->keys.end());
            new_tree_node->tree_pointers.insert(new_tree_node->tree_pointers.end(), this->tree_pointers.begin()+new_size, this->tree_pointers.end()); 
            this->keys.erase(this->keys.begin()+new_size-1, this->keys.end());
            this->tree_pointers.erase(this->tree_pointers.begin()+new_size, this->tree_pointers.end());
            new_tree_node->size = new_tree_node->tree_pointers.size();
            this->size = this->tree_pointers.size();
            new_tree_node->dump();
            new_tree_ptr = new_tree_node->tree_ptr;
            delete new_tree_node;
        }
    }
    this->dump();
    delete tree_node;
    return new_tree_ptr;
}
 void InternalNode::redistribute(TreeNode* Sibling,string redistributeStrategy){
    // cout<<"redistribute"<<endl;
    InternalNode* Sibling_node = dynamic_cast<InternalNode*>(Sibling);
    if(redistributeStrategy=="RIGHT"){
        // cout<<"RIGHT1"<<endl;
         // redistributed "RIGHT"
    TreePtr tree_ptr = Sibling_node->tree_pointers.back();
    TreeNode* leftSiblingLastNode = TreeNode::tree_node_factory(tree_ptr);
    auto pushedvalue=leftSiblingLastNode->max();
    Sibling_node->keys.push_back(pushedvalue);
    auto pushedTreeValue=this->tree_pointers[0];
    Sibling_node->tree_pointers.push_back(pushedTreeValue);
    delete leftSiblingLastNode;
    Sibling_node->size= Sibling_node->size+1;
    int length=this->size;
    this->size--;
    length=length-2;
    int idx=0;
    for(;idx<length;idx++){
        int nextidx=idx+1;
        this->keys[idx] = this->keys[idx+1];

    }
    idx=0;
    length=length+1;
    this->keys.pop_back();
    for(;idx<length;idx++){
        int nextidx=idx+1;
        this->tree_pointers[idx] = this->tree_pointers[idx+1];

    }
    this->tree_pointers.pop_back();
    this->dump();

    }
    else if(redistributeStrategy=="LEFT"){
        // cout<<"LEFT2"<<endl;
        //  redistributed "LEFT"
        
        int length=Sibling_node->size;
        Sibling_node->size=Sibling_node->size+1;
        Sibling_node->keys.push_back(DELETE_MARKER);
        Sibling_node->tree_pointers.push_back(NULL_PTR);

        int idx=length;
        for( ;idx>0;idx--){
            int idx2=idx-1;
            Sibling_node->tree_pointers[idx] = Sibling_node->tree_pointers[idx2];
        }
        Sibling_node->tree_pointers[0] = this->tree_pointers[this->size - 1];
        idx=length-1;
        for( ;idx>0;idx--){
            int idx2=idx-1;
            Sibling_node->keys[idx] = Sibling_node->keys[idx2];
        }
    TreePtr tree_ptr = Sibling_node->tree_pointers[0];
    TreeNode* rightSiblingFirstChildNode = TreeNode::tree_node_factory(tree_ptr);
    Sibling_node->keys[0]=rightSiblingFirstChildNode->max();
    this->keys.pop_back();
    this->tree_pointers.pop_back();
    this->size=this->size-1;
    delete rightSiblingFirstChildNode;
    
    this->dump();
    



    }
    else if(redistributeStrategy=="MERG"){
        // cout<<"MERG3"<<endl;
        //  Merg nodes
    TreePtr tree_ptr = this->tree_pointers.back();
    TreeNode* selfLastNode = TreeNode::tree_node_factory(tree_ptr);
    auto maxValue=selfLastNode->max();
    this->keys.push_back(maxValue);
    delete selfLastNode;
    int idx=0;
    int length=Sibling_node->size;
    this->size+=Sibling_node->size;
    length--;
    for(;idx<length;idx++){
        auto pushedValue=Sibling_node->keys[idx];
        this->keys.push_back(pushedValue);
    }
    idx=0;
    length++;
    for(;idx<length;idx++){
        auto pushedValue=Sibling_node->tree_pointers[idx];
        this->tree_pointers.push_back(pushedValue);
    }
    Sibling_node->delete_node();
    this->dump();

    }
   Sibling_node->dump();


 }
//deletes key from subtree rooted at this if exists
//TODO: InternalNode::delete_key to be implemented
void InternalNode::delete_key(const Key &key) {
    TreePtr new_tree_ptr = NULL_PTR;
    int index=0;
    while(index < this->keys.size() && key > this->keys[index]&& DELETE_MARKER!=this->keys[index]) ++index;
    TreePtr tree_ptr = this->tree_pointers[index];
    TreeNode* tree_node = TreeNode::tree_node_factory(tree_ptr);
    tree_node->delete_key(key);
    if(index<this->keys.size()){
        this->keys[index]=tree_node->max();
    }

    if(tree_node->underflows()==false){
        delete tree_node;
        this->dump();
        return;

    }
    // cout<<index<<endl;
    if(index==0){
        
        TreePtr right_tree_ptr = this->tree_pointers[index+1];
        TreeNode* rightSiblingNode = TreeNode::tree_node_factory(right_tree_ptr);
        auto total_size=rightSiblingNode->size + tree_node->size ;
        // cout<<total_size<<"size"<<endl;
        if(total_size >= MIN_OCCUPANCY*2) {
            // cout<<"if"<<endl;
            string method="RIGHT";
            rightSiblingNode->redistribute(tree_node,method);
             auto maxval=tree_node->max();
             this->keys[index] =maxval;
        }
        else{
            string method="MERG";
            // cout<<"else"<<endl;
           tree_node->redistribute(rightSiblingNode,method);
        //    cout<<"udrasht"<<endl;
           int idx=index;
           int length=this->size;
           length=length-2;
           for(;idx<length;idx++){
            int idx2=idx+1;
            this->keys[idx] = this->keys[idx2];

           }
           idx=index+1;
           length=length+1;
           for(;idx<length;idx++){
            int idx2=idx+1;
            this->tree_pointers[idx] = this->tree_pointers[idx2];

           }
           this->size=this->size-1;
            this->tree_pointers.pop_back();
            this->keys.pop_back();
            
           

        //     yaha baki h
        }
        delete rightSiblingNode;
    }
    else{
        // cout<<"left"<<endl;
        TreePtr left_tree_ptr = this->tree_pointers[index-1];
        TreeNode* leftSiblingNode = TreeNode::tree_node_factory(left_tree_ptr);
        auto total_size=leftSiblingNode->size + tree_node->size ;
        if(total_size >= MIN_OCCUPANCY*2) {
            string method="LEFT";
            leftSiblingNode->redistribute(tree_node,method);
            auto maxVal=leftSiblingNode->max();
            this->keys[index-1] =maxVal;
        }
        else{
            string method="MERG";
           leftSiblingNode->redistribute(tree_node,method);
           int idx=index;
           int length=this->size;
           length=length-1;
           for(;idx<length;idx++){
            int idx2=idx+1;
            this->tree_pointers[idx] = this->tree_pointers[idx2];

           }
           idx=index-1;
           length=length-1;
           for(;idx<length;idx++){
            int idx2=idx+1;
            this->keys[idx] = this->keys[idx2];

           }
           this->size=this->size-1;
            this->tree_pointers.pop_back();
            this->keys.pop_back();

        }
        delete leftSiblingNode;

        
    }
    delete tree_node;
    this->dump();
}

//runs range query on subtree rooted at this node
void InternalNode::range(ostream &os, const Key &min_key, const Key &max_key) const {
    BLOCK_ACCESSES++;
    for (int i = 0; i < this->size - 1; i++) {
        if (min_key <= this->keys[i]) {
            auto* child_node = TreeNode::tree_node_factory(this->tree_pointers[i]);
            child_node->range(os, min_key, max_key);
            delete child_node;
            return;
        }
    }
    auto* child_node = TreeNode::tree_node_factory(this->tree_pointers[this->size - 1]);
    child_node->range(os, min_key, max_key);
    delete child_node;
}

//exports node - used for grading
void InternalNode::export_node(ostream &os) {
    TreeNode::export_node(os);
    for (int i = 0; i < this->size - 1; i++)
        os << this->keys[i] << " ";
    os << endl;
    for (int i = 0; i < this->size; i++) {
        auto child_node = TreeNode::tree_node_factory(this->tree_pointers[i]);
        child_node->export_node(os);
        delete child_node;
    }
}

//writes subtree rooted at this node as a mermaid chart
void InternalNode::chart(ostream &os) {
    string chart_node = this->tree_ptr + "[" + this->tree_ptr + BREAK;
    chart_node += "size: " + to_string(this->size) + BREAK;
    chart_node += "]";
    os << chart_node << endl;

    for (int i = 0; i < this->size; i++) {
        auto tree_node = TreeNode::tree_node_factory(this->tree_pointers[i]);
        tree_node->chart(os);
        delete tree_node;
        string link = this->tree_ptr + "-->|";

        if (i == 0)
            link += "x <= " + to_string(this->keys[i]);
        else if (i == this->size - 1) {
            link += to_string(this->keys[i - 1]) + " < x";
        } else {
            link += to_string(this->keys[i - 1]) + " < x <= " + to_string(this->keys[i]);
        }
        link += "|" + this->tree_pointers[i];
        os << link << endl;
    }
}

ostream& InternalNode::write(ostream &os) const {
    TreeNode::write(os);
    for (int i = 0; i < this->size - 1; i++) {
        if (&os == &cout)
            os << "\nP" << i + 1 << ": ";
        os << this->tree_pointers[i] << " ";
        if (&os == &cout)
            os << "\nK" << i + 1 << ": ";
        os << this->keys[i] << " ";
    }
    if (&os == &cout)
        os << "\nP" << this->size << ": ";
    os << this->tree_pointers[this->size - 1];
    return os;
}

istream& InternalNode::read(istream& is) {
    TreeNode::read(is);
    this->keys.assign(this->size - 1, DELETE_MARKER);
    this->tree_pointers.assign(this->size, NULL_PTR);
    for (int i = 0; i < this->size - 1; i++) {
        if (&is == &cin)
            cout << "P" << i + 1 << ": ";
        is >> this->tree_pointers[i];
        if (&is == &cin)
            cout << "K" << i + 1 << ": ";
        is >> this->keys[i];
    }
    if (&is == &cin)
        cout << "P" << this->size;
    is >> this->tree_pointers[this->size - 1];
    return is;
}
