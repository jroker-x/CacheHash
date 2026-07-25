#include<iostream>
#include<vector>
#include<functional>
#include <cassert>
#include <cstdint>

enum class State{

    Empty,
    Occupied,
    Tombestone
};


template <typename K, typename V>
struct Bucket{
    
    K key;
    V value;

    State state;
    Bucket():
    state(State::Empty){}
    uint8_t probeDistance = 0;

    
};

template <typename K, typename V>
class HashTable{
    private:
        
        size_t size = 0;
        size_t capacity = 16;
        std::vector<Bucket<K,V>> table;
        double maxLoadFactor = 0.7;
    
        size_t hash(const K& str)
        {
            size_t hashvalue = std::hash<K>{}(str);
            return hashvalue;
        }

        void RobinHoodInsert(const K& key,const V& value,std::vector<Bucket<K,V>>& t, size_t& c, size_t& s){
            
            Bucket <K,V> incoming;
            incoming.key = key;
            incoming.value = value;
            incoming.state = State::Occupied;
            incoming.probeDistance = 0;
            size_t bucketindex = hash(key);
            bucketindex %= c;
            size_t counter = 0;        
            while (counter < c)
            {   
                size_t index = (bucketindex + counter) % c;
                if (t[index].state == State::Occupied)
                {
                    if (t[index].key == key)
                    {
                        t[index].value = value;
                        break;
                    }
                }
                if (t[index].state == State::Occupied && t[index].probeDistance < incoming.probeDistance)
                {
                    std::swap(t[index],incoming);
                }
               
                if (t[index].state == State::Empty)
                {
                    t[index] = incoming;
                    s++;
                    break;
                }
                
                incoming.probeDistance++;
                counter++;
            }

        }

    public:

        
        HashTable(size_t initialcapacity)
        {
            capacity = initialcapacity;
            table.resize(capacity);
            
        }

        void insertLP(const K& key,const V& value){

            if (Bucket<K,V>* b = find(key))
            {
                b->value = value;
                return;
            }
    
            if (static_cast<double>(size + 1) / static_cast<double>(table.size()) > maxLoadFactor)
            {
                ReHashLP();
            }
            
            size_t bucketindex = hash(key);
            bucketindex %= table.size();
            Bucket<K,V> b1;
            b1.key = key;
            b1.value = value;
            b1.state = State::Occupied;
            size_t counter = 0;
            size_t FirstTomb; 
            bool Tfound = false;
            while (counter < capacity)
            {
                size_t index = (bucketindex + counter) % capacity;
                if (table[index].state == State::Occupied && table[index].key == b1.key)
                {
                    table[index].value = value;
                    break;
                }
                if (!Tfound)
                {
                    if (table[index].state == State::Tombestone)
                    {
                        FirstTomb = index;
                        Tfound = true;
                    }
                }
                if (table[index].state == State::Empty && Tfound)
                {
                    table[FirstTomb] = b1;
                    size++;
                    break;
                }
                if (table[index].state == State::Empty && !Tfound)
                {
                    table[index] = b1;
                    size++;
                    break;
                }
                counter++;
            }
        }

        Bucket<K,V>* find(const K& key){

            size_t bucketindex = hash(key);
            bucketindex %= table.size();
            size_t counter = 0;

            while (counter < capacity)
            {
                size_t index = (bucketindex + counter) % capacity;
                if (table[index].state == State::Occupied)
                {
                    if (table[index].key == key)
                    {
                       return &table[index];
                    }
                }
                if (table[index].state == State::Empty)
                {
                    break;
                }
                counter++;
            }
            return nullptr;
        
        }

        bool eraseLP(const K& key){

            size_t bucketindex = hash(key);
            bucketindex %= table.size();
            size_t counter = 0;

            while (counter < capacity)
            {
                size_t index = (bucketindex + counter) % capacity;
                if (table[index].state == State::Empty)
                {
                    break;
                }
                if (table[index].state == State::Occupied)
                {
                    if (table[index].key == key)
                    {
                       table[index].state = State::Tombestone;
                       size--;
                       return true;
                    }
                }
                
                counter++;
            }
            return false;

        }

        void ReHashLP(){
            size_t newcapacity = capacity *2;
            std::vector<Bucket<K,V>> REtable(newcapacity);
            
            for (size_t i = 0; i < table.size(); i++)
            {   
                size_t counter = 0;
                if (table[i].state == State::Occupied)
                {
                    size_t newbucketindex = hash(table[i].key);
                    newbucketindex %= REtable.size();
                    while (counter < newcapacity)
                    {
                        size_t index = (newbucketindex + counter) % newcapacity;
                        if (REtable[index].state == State::Empty)
                        {
                            REtable[index] = std::move(table[i]);
                            break;
                            
                        }
                        counter++;
                    }
    
                }
            }
            
            table = std::move(REtable);
            capacity = newcapacity;

           
        }

        double loadFactor() const
        {
            return static_cast<double>(size) / table.size();
        }

        bool contains(const K& key){

            if (Bucket<K,V>* b = find(key)){

                return true;
            }

            return false;

        }

        void clear(){
            table.clear();
        }

        size_t Size(){
            return size;
        }

        size_t Capacity(){
            return capacity;
        }

    void RobinHood(const K& key, const V& value){
        
        if (Bucket<K,V>* b = find(key))
            {
                b->value = value;
                return;
            }
    
            if (static_cast<double>(size + 1) / static_cast<double>(table.size()) > maxLoadFactor)
            {
                ReHashRH();
            }

        RobinHoodInsert(key, value, table, capacity, size);
    }

        bool eraseRH(const K& key){

            size_t bucketindex = hash(key);
            bucketindex %= capacity;
            size_t counter = 0;
            bool found = false;

            while (counter < capacity)
            {
                size_t index = (bucketindex + counter) % capacity;
                if (table[index].state == State::Empty)
                {
                    break;
                }
                if (table[index].state == State::Occupied)
                {
                    if (table[index].key == key)
                    {
                        table[index].state = State::Empty;
                        found = true;
                        bucketindex = index;
                        size--;
                        break;
                    }
                }
                
                counter++;
            }
            if (!found)
            {
                return false;
            }
            
            backShift(bucketindex);
            return true;
        }

        void backShift(const size_t index){

            size_t hole = index;
            size_t next = 0; 
            
            while (true)
            {
                next = (hole + 1 ) % capacity;

                if (table[next].state == State::Empty)
                {
                    break;
                }
                if (table[next].probeDistance == 0)
                {
                    break;
                }
                std::swap(table[hole],table[next]);
                table[hole].probeDistance--;
                hole = next;

               
            }

        }

        void printTable(){
        
            for (size_t i = 0; i < table.size(); i++)
            {
                if (table[i].state == State::Occupied)
                {
                    std::cout << " KEY -> " << table[i].key << " VALUE -> " << table[i].value << " DISTANCE -> " << static_cast<int>(table[i].probeDistance) << '\n';
                }
            }
        
        }

        void ReHashRH(){
            size_t newcapacity = capacity *2;
            std::vector<Bucket<K,V>> REtable(newcapacity);
            size_t newsize = 0;
            for (size_t i = 0; i < table.size(); i++)
            {   
                if (table[i].state == State::Occupied)
                {
                    RobinHoodInsert(table[i].key,table[i].value,REtable,newcapacity,newsize);
    
                }
            }
            table = std::move(REtable);
            capacity = newcapacity;
            size = newsize;
        }
       




};


int main(){

}




