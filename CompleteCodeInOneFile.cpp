#include <map>
#include <memory>
#include <unordered_map>

struct Node {
    std::shared_ptr<Node> prev = nullptr;
    std::shared_ptr<Node> next = nullptr;
    int key = -1;
    int value = -1;
    int lfu = 0;
};

class LFU_SingleFrequency {
    std::shared_ptr<Node> head = nullptr;
    std::shared_ptr<Node> tail = nullptr;
    
    void printList(const std::string msg) {
        cout << "\n" << msg << " : ";
        for(auto curr = head ; tail != curr; curr = curr->prev){
            cout << curr->value << " ; ";
        }

        cout << " || ";

        for(auto curr = tail ; head != curr; curr = curr->next){
            cout << curr->value << " ; ";
        }
        
    }

public:
    LFU_SingleFrequency() {
        head = std::make_shared<Node>();
        tail = std::make_shared<Node>();

        head->prev = tail;
        tail->next = head;
    }

    int RemoveLast() {
        if(IsEmpty())
            return -1;

        auto rem = tail->next;

        auto prev = rem->prev;
        auto next = rem->next;

        prev->next = next;
        next->prev = prev;

        auto key = rem->key;
        rem.reset();
        
        return key;
    }

    void RemoveNode(std::shared_ptr<Node> rem) {
        auto prev = rem->prev;
        auto next = rem->next;

        prev->next = next;
        next->prev = prev;
    }

    void AddNode(std::shared_ptr<Node> add) {
        auto prev = head->prev;

        add->next = head;
        head->prev = add;
        add->prev = prev;
        prev->next = add;
        
    }

    bool IsEmpty() {
        return head->prev == tail;
    }
};

class LFUCache {
    int capacity_;

    using KEY = int;
    using LFU = int;
    std::unordered_map<KEY, std::shared_ptr<Node>> mp;
    std::map<LFU, LFU_SingleFrequency> recent;

    void BumpFrequency(std::shared_ptr<Node> bumped) {
        recent.at(bumped->lfu).RemoveNode(bumped);
        if(recent.at(bumped->lfu).IsEmpty())
            recent.erase(bumped->lfu);

        bumped->lfu++;
        if(recent.find(bumped->lfu) == recent.end())
            recent.insert(std::pair{bumped->lfu, LFU_SingleFrequency()});
        recent.at(bumped->lfu).AddNode(bumped);
    }
    int pop() {
        if(capacity_ != 0){
            --capacity_;
            return -1;
        }

        int returnedKey = -1;
        while(true) {
            if(recent.empty())
                return -1;

            auto lfuCount = recent.begin();
            returnedKey = lfuCount->second.RemoveLast();
            if(-1 == returnedKey)
                recent.erase(lfuCount);
            else
                break;
        }
        return returnedKey;
    }
public:
    LFUCache(int capacity) : capacity_(capacity) {}
    
    int get(int key) {
        if(mp.find(key) == mp.end())
            return -1;
        
        auto bumped = mp.at(key);

        BumpFrequency(bumped);

        return bumped->value;
    }
    
    void put(int key, int value) {
        if(mp.find(key) != mp.end()){
            auto bumped = mp.at(key);
            bumped->value = value;
            BumpFrequency(bumped);
        } else {
            auto newNode = make_shared<Node>();
            newNode->key = key;
            newNode->value = value;
            newNode->lfu = 0;
            auto popedKey = pop();
            if(-1 != popedKey)
                mp.erase(popedKey);
          
            if(recent.find(0) == recent.end())
                recent.insert(std::pair{0, LFU_SingleFrequency()});
            recent.at(0).AddNode(newNode);
            mp.insert({key, newNode});
        }
        return;
    }
};
