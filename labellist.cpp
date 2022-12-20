#include "labellist.h"
#include <cassert>

// Complete - Do not alter
LabelList::MsgNode::MsgNode(const MsgType& msg)
    : msg_(msg)
{
    // Every message is part of the "all" (index 0) label
    labelled_.push_back(true);
    next_.push_back(nullptr);
    prev_.push_back(nullptr);
}


// Complete - Do not alter
LabelList::MsgToken::MsgToken()
 : node_(nullptr), list_(nullptr)
{
}

// To be completed - must adhere to any requirements in the .h file - done
LabelList::MsgToken::MsgToken(MsgNode* node, LabelList* list):node_(node), list_(list)
// Add any initializers here

{

}

// To be completed - must adhere to any requirements in the .h file-done
bool LabelList::MsgToken::operator==(const MsgToken& rhs) const
{
    return(this->node_ == rhs.node_);
}

// Complete - Do not alter
bool LabelList::MsgToken::operator!=(const MsgToken& rhs) const
{
    return !operator==(rhs);
}

// To be completed - must adhere to any requirements in the .h file-done
LabelList::MsgToken::operator bool() const
{
    return(this->isValid());
}

// Complete - Do not alter
bool LabelList::MsgToken::isValid() const
{
    return this->node_ != nullptr;
}

// Complete - Do not alter
LabelList::MsgToken& LabelList::MsgToken::next(const LabelType& label)
{
    // Add your code here
    size_t index = this->list_->findLabelIndex(label);
    if( false == this->isLabelMember(index) ) 
    {
        throw std::invalid_argument("Node is not part of the given label");
    }
    this->node_ = this->node_->next_[index];

    // Should return reference to self
    return *this;
}

// Complete - Do not alter
LabelList::MsgToken& LabelList::MsgToken::prev(const LabelType& label)
{
    // Add your code here
    size_t index = this->list_->findLabelIndex(label);
    if( false == this->isLabelMember(index) ) 
    {
        throw std::invalid_argument("Node is not part of the given label");
    }
    this->node_ = this->node_->prev_[index];

    // Should return reference to self
    return *this;
}

// Complete - Do not alter
bool LabelList::MsgToken::isLabelMember(const LabelType& label) const //O(n)
{
    size_t index = this->list_->findLabelIndex(label);
    return isLabelMember(index);
}

// Complete - Do not alter
bool LabelList::MsgToken::isLabelMember(size_t index) const //O(1)
{
    return this->isValid() && 
            index != INVALID_LABEL &&
            index < this->node_->labelled_.size() &&
            this->node_->labelled_[index] ;
}

// To be completed - must adhere to any requirements in the .h file-done
LabelList::MsgType const & LabelList::MsgToken::msg() const
{
    if (!this->isValid())
    {
        throw std:: logic_error ("Token references an invalid message");
    }
    return(this->node_->msg_);
}
// To be completed - must adhere to any requirements in the .h file-done
LabelList::MsgType& LabelList::MsgToken::msg() 
{
    if (!this->isValid())
    {
        throw std:: logic_error ("Token references an invalid message");
    }
    return(this->node_->msg_);
}

// To be completed - must adhere to any requirements in the .h file - done
std::ostream& operator<<(std::ostream& ostr, const LabelList::MsgToken& token)
{
    if(token.isValid())
    {
        ostr << token.node_->msg_;
    }
    return ostr;
}

// Static constant 
const LabelList::MsgToken LabelList::end_(nullptr, nullptr);

// To be completed - must adhere to any requirements in the .h file-done
LabelList::LabelList()
// Add any initializers here
{
    this->heads_.push_back(nullptr);
    this->tail=nullptr;
    this->labels_.push_back("all");
}

// To be completed - must adhere to any requirements in the .h file - done
LabelList::~LabelList()
{
    clear();
}

// To be completed - must adhere to any requirements in the .h file - done
void LabelList::clear()  // O(n)
{
    MsgNode* tempTail;
    while (!empty()) // tails_[0]!=nullptr && heads_[0]!=nullptr
    {
        tempTail= this->tail;
        this->tail=tail->prev_[0];
        if(this->tail!=nullptr)
        {
            this->tail->next_[0]=nullptr;
        }
        else
        {
            this->heads_[0]=nullptr;
        }
        delete tempTail;
    }
}

// To be completed - must adhere to any requirements in the .h file -done
bool LabelList::empty() const
{
    return(this->heads_[0]==nullptr && this->tail==nullptr);
}

// Complete
size_t LabelList::size() const
{
    MsgNode* n = this->heads_[0];
    size_t cnt = 0;
    while(n != nullptr){
        cnt++;
        n = n->next_[0];
    }
    return cnt;
}

// To be completed - must adhere to any requirements in the .h file -done
LabelList::MsgToken LabelList::add(const MsgType& msg) //O(1)
{
    MsgNode* newPtr= new MsgNode(msg);
    if(this->heads_[0]==nullptr && this->tail==nullptr) // if heads_[0] is empty
    {
        this->tail=newPtr;
        this->heads_[0]= this->tail; //in this case both heads_ and tails_ are poitning to the same node
    }
    else // if heads_[0] is not empty
    {
        this->tail->next_[0]=newPtr;
        newPtr->prev_[0]=tail;
        this->tail=newPtr;    
    }
     MsgToken newToken (newPtr, this);//msgToken for the new message 
     return (newToken);
}


// To be completed - must adhere to any requirements in the .h file - done
void LabelList::remove(const MsgToken& token)
{
    if(!token.isValid())
    {
        return; //if token is not valid, do nothing and return
    }
    MsgNode* temp=token.node_;
    for ( unsigned int i=0; i<labels_.size();i++) //starting from the last label  O(|L|)
    {
        if (token.isLabelMember(i)) //O(1)
        {
            this->unlabelByIndex(token.node_, i); //O(1)
        }
    }  
    delete temp; 
}

LabelList::MsgNode*  LabelList:: findBeforeNode (const MsgNode* node, size_t index) //O(n)
{
    MsgNode* tempHead= heads_[0];
    MsgNode* beforeToken=nullptr;
    while(tempHead!= node) // the last temp saved is the one that comes right before token in all label
    {
        if(tempHead->labelled_.size()>=index+1)
        {
            if(tempHead->labelled_[index]== true  )
            {
                beforeToken=tempHead;
            }
        }    
        tempHead= tempHead->next_[0];
    }
    return (beforeToken);
}
void LabelList::resizeNode (MsgNode* node, size_t newSize) // O(newSize)
{
    node->next_.resize(newSize); 
    node->labelled_.resize(newSize,false);  
    node->prev_.resize(newSize); 
}

// To be completed - must adhere to any requirements in the .h file - done
void LabelList::label(const MsgToken& token, const LabelType& label)
{
    if(!token.isValid())
    {
        throw std::invalid_argument("Bad token in label()");
    }
    size_t index= this->findLabelIndex(label); // O(|L|)
    
    if(index == INVALID_LABEL)// if label does not exist
    {    
        size_t lastIndex;
        labels_.push_back(label); // add a new label
        heads_.push_back(token.node_); 
        resizeNode(token.node_, heads_.size()); 

        lastIndex= heads_.size()-1;  
        //adding the token to the head of that label
        heads_[ lastIndex]->labelled_[lastIndex]=true;  //pointing to the token node
        heads_[ lastIndex]->next_[lastIndex]=nullptr;
        heads_[ lastIndex]->prev_[lastIndex]=nullptr;

    }
    else //if label exits                         
    {
        if(!token.isLabelMember(index)) //label only if the msg is not labelled already , O(1)
        {
            if(index>=token.node_->labelled_.size()) //resize only if the index is greater than size of the labeled/prev/next
            {
                resizeNode(token.node_, index+1 ); //O(|L|), can be resized upto a maximum of the size of labelled_ vector
            } 
            token.node_->labelled_[index]=true; 

            //search for the last message w/ same lable which comes right before token in the all label   
            MsgNode* temp= nullptr; 
            MsgNode* beforeToken= findBeforeNode(token.node_, index); //O(n)
                    
            //if  beforeToken is not nullptr
            if(beforeToken!=nullptr)
            {
                temp= beforeToken->next_[index];
                beforeToken->next_[index]=token.node_;   
                token.node_->prev_[index]=beforeToken; 
                token.node_->next_[index]=temp;
                if(temp!=nullptr)
                {
                    temp->prev_[index]=token.node_;
                }
            }
             else //if the beforeToken is nullptr
             {
                 token.node_->next_[index]=heads_[index];
                 token.node_->prev_[index]=nullptr; 
                 heads_[index]=token.node_;
             }
        }
        
    }

}

void LabelList::unlabelByIndex ( MsgNode* node, size_t index ) //O(1)
{
    node->labelled_[index]=false;
    if(node->next_[index]!=nullptr && node->prev_[index]!=nullptr) //if there are other messages in that label
        {
            node->prev_[index]->next_[index]=node->next_[index];
            node->next_[index]->prev_[index]=node->prev_[index];
            node->next_[index]=nullptr;
            node->prev_[index]=nullptr;
        }  
        else if(node->prev_[index]==nullptr && node->next_[index]!=nullptr)
        {
            heads_[index]=node->next_[index];
            node->next_[index]->prev_[index]=nullptr;
            node->next_[index]=nullptr;
        }
        else if (node->next_[index]==nullptr && node->prev_[index]!=nullptr )
        {
            node->prev_[index]->next_[index]=nullptr;
            node->prev_[index]=nullptr;
        }
        else if (node->prev_[index]==nullptr && node->next_[index]==nullptr) // if token is the only msg of that label
        {  
            heads_[index]=nullptr;
            if(index==0)
            {
                tail=nullptr;
            }
        }
}
// To be completed - must adhere to any requirements in the .h file - done
void LabelList::unlabel(const MsgToken& token, const LabelType& label)
{
    if(!token.isValid())
    {
        throw std::invalid_argument("Bad token in label()");
    }
    size_t index= findLabelIndex(label);  //index of the given label, O(|L|)
    if(index == INVALID_LABEL)
    {
        throw std::out_of_range("Bad token in unabel()");
    }
    else if(index>0 && token.isLabelMember(index)) //O(1) ) // unlabel only if the index is greater than 0 (can't unlabel from the "all" label)
    {
        this->unlabelByIndex(token.node_, index); //O(1)
    }
   
}

// Complete - Do not alter
LabelList::MsgToken LabelList::find(const MsgType& msg, const LabelType& label)
{
    // See if the label exists, will throw if not
    size_t level = this->getLabelIndex(label);
    MsgNode* n = this->heads_[level];
    while(n != nullptr)
    {
        if(n->msg_ == msg)
        {
            return MsgToken(n, this);
        }
        n = n->next_[level];
    }
    return end();
}

// To be completed - must adhere to any requirements in the .h file - done 
LabelList::MsgToken LabelList::find(size_t index, const LabelType& label)
{
    size_t level = this->getLabelIndex(label); //throws std::out_of_range if a non-existent label is provided, O(|L|)
    MsgNode* temp= heads_[level];
    size_t msgIndex=0;
    while(temp!=nullptr) //O(index) 
    {
        if(msgIndex==index)
        {
            return MsgToken(temp,this);
        }  
        msgIndex++;
        temp= temp->next_[level];
    }
    return end();
}

// Complete - Do not alter
LabelList::MsgToken const & LabelList::end() const
{
    return end_;
}

// Complete - Do not alter
void LabelList::print(std::ostream& ostr, const LabelType& label, char separator) const
{
    size_t level = this->getLabelIndex(label);
    MsgNode* n = this->heads_[level];
    while(nullptr != n)
    {
        ostr << n->msg_ << separator;
        n = n->next_[level];
    }
    ostr << std::endl;
}

// To be completed - must adhere to any requirements in the .h file-done
size_t LabelList::findLabelIndex(const LabelType& label) const //O(|L|)
{
   for ( unsigned int i=0; i<labels_.size(); i++)
    {
        if( label ==labels_[i])
        {
            return (i); //i will be returned if label found
        }
    }
    return INVALID_LABEL;
}

// Complete - Do not alter
size_t LabelList::getLabelIndex(const LabelType& label) const
{
    size_t retval = this->findLabelIndex(label);
    if(INVALID_LABEL == retval)
    {
        throw std::out_of_range("Label doesn't exist");
    }
    return retval;
}