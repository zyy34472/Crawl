#include "Common.hpp"
#include <cassert>
#include <iterator>

#define Buck set

template <class T>  
class Bucket {
public:
    class Iterator /*:public std::iterator*/ {
    public:
        Iterator(Bucket<T> &B): B_(B) {
            curBuck_ = index_ = 0;
        }
        Iterator(Bucket<T> &B, int init) : B_(B) {
            curBuck_ = index_ = init;
        }
        bool operator == (const Iterator &other) {
            return index_ == other.index_ && curBuck_ == other.curBuck_;
        }
        bool operator != (const Iterator &other) {
            return !(*this == other);
        }
        Iterator& operator ++() {
            if (++index_ == B_.buckets_.at(curBuck_).size()) {
                index_ = 0;
                if (++curBuck_ == B_.buckets_.size()) {
                    curBuck_ = -1;
                    index_ = -1;
                }
            }
            return *this;
        }
        const Iterator operator ++(int) {
            Iterator old = *this;
            ++(*this);
            return old;
        }
    private:
        Bucket<T>& B_;
        int curBuck_;
        int index_;
        //Buck::iterator
    };
public:
    /** @brief Bucket: Construction */
    Bucket() { buckets_.resize(_defaultSize_); }
    explicit Bucket(int size) { buckets_.resize(size); }
    /** @brief Iterator begin and end */
    Iterator begin() { return Iterator(*this); }
    Iterator end() { return Iterator(*this, -1); }
    /**
     * @brief addBucketItem: add and test if exist
     */
    inline int addBucketItem(T item, int pos) {
        Buck<T>& buck = buckets_.at(pos);
        size_t before = buck.size();
        buck.insert(item);
        return before == buck.size();
    }
    /**
     * @brief isExist: item if exist in Bucket
     */
    inline bool isExist(T& item, int pos) {
        assert(pos >= 0 && pos < buckets_.size());
        return buckets_.at(pos).count(item);
    }
    /**
     * @brief getPosBuck: get buck reference under pos number
     */
    inline const Buck<T>& getPosBuck(int pos) {
        assert(pos >= 0 && pos < buckets_.size());
        return buckets_.at(pos);
    }

    void outputOver(ostream &out, int pos) {
        int index = 0;
        pos = pos < 0? 0: pos;
        for (int i = buckets_.size()-1; i >= pos; i--)
            for (auto item: buckets_[i])
                out << "|"<< ++index << item << endl;
    }

    void outputTOP(ostream &out, int topk) {
        int index = 0;
        for (int i = buckets_.size()-1; i >= 0; i--)
            for (auto item: buckets_[i]) {
                out << "|"<< ++index << item << endl;
                if (index == topk)
                    return;
            }
    }

    friend ostream & operator <<(ostream &out, const Buck<T> &b) {
        int index = 0;
        for (auto item: b)
            out << "|"<< ++index << item << endl;
        return out;
    }

    //From High to Low
    friend ostream & operator <<(ostream &out, const Bucket<T> &B) {
        int index = 0;
        for (int i = B.buckets_.size()-1; i >= 0; i--)
            for (auto item: B.buckets_[i])
                out << "|"<< ++index << item << endl;
        return out;
    }

private:
    static size_t _defaultSize_;
    vector<Buck<T>> buckets_;
};

template <class T>
size_t Bucket<T>::_defaultSize_ = 100;

/*
class Bucket {
public:
	Bucket (int Size) {
		buckets_.resize(Size);
	}
	Bucket (int level, int per) {
		levels_(level, per);
		int sum = 1;
		for (auto level: levels_)
			sum *= level;
		buckets_.resize(sum);
	}

	Bucket (vector<int>& l) {
		levels_.assign(l.begin(), l.end());
		int sum = 1;
		for (auto level: levels_)
			sum *= level;
		buckets_.resize(sum);
	}
	
private:
    vector<int> levels_; 
    vector<set<int>> buckets_; 
    set<string> urls_; 
}; 
*/
