#include <cassert>
#include <iostream>
#include <vector>
#include <random>

namespace {

template <typename Key, typename Value> class RBTree {
private:
  class RBTreeNode {
  public:
    enum Color {
      RED = 0,
      BLACK = 1,
    };
    enum Side {
      LEFT = 0,
      RIGHT = 1,
    };
    //node constructor
    RBTreeNode(Key k, Value *o, RBTree *t)
        : obj(o), key(k), color(RED), parent(nullptr), tree(t) {
      this->link[LEFT] = RBTree<Key, Value>::nil;
      this->link[RIGHT] = RBTree<Key, Value>::nil;

      if (nullptr == o) {
        this->color = BLACK;
        this->link[LEFT] = nullptr;
        this->link[RIGHT] = nullptr;
      }
    }
    ~RBTreeNode() {
      if (this->link[LEFT]->isNil() == false)
        delete this->link[LEFT];
      if (this->link[RIGHT]->isNil() == false)
        delete this->link[RIGHT];
    }
    inline bool isNil() { return (this->obj == nullptr); }
    inline void swapColor(RBTreeNode &node) {
      Color c = this->color;
      this->color = node.color;
      node.color = c;
    }
    inline Value *getObj() { return this->obj; }
    inline void setBlack() { this->color = BLACK; }
    inline void setRed() { this->color = RED; }
    inline Color getColor() { return this->color; }
    inline bool isBlack() { return (this->color == BLACK); }
    inline bool isRed() { return (this->color == RED); }
    
    inline Side whichSide(RBTreeNode &node) {
      if (this->link[LEFT] == &node)
        return LEFT;
      else if (this->link[RIGHT] == &node)
        return RIGHT;
      else
        assert(0);
    }

    inline Side otherSide(Side s) {
      assert(s == LEFT || s == RIGHT);
      return (s == LEFT ? RIGHT : LEFT);
    }

    inline RBTreeNode *getBrother() {
      if (this->parent == nullptr)
        return nullptr;

      assert(this->parent->link[LEFT] == this ||
             this->parent->link[RIGHT] == this);
      return (this->parent->link[LEFT] == this ? this->parent->link[RIGHT]
                                               : this->parent->link[LEFT]);
    }

    inline void attach(RBTreeNode &node) {
      assert(this->key != node.key);
      Side s = (node.key < this->key ? LEFT : RIGHT);
      this->attach(s, node);
    }

    inline void attach(Side s, RBTreeNode &node) {
      assert(s == LEFT || s == RIGHT);
      assert(this != &node);
      assert(this->link[s]->isNil());
      this->link[s] = &node;
      if (!node.isNil())
        node.parent = this;
    }
    
    inline RBTreeNode *detach(Side s) {
      assert(s == LEFT || s == RIGHT);

      if (this->isNil() || this->link[s]->isNil())
        return RBTree<Key, Value>::nil;

      RBTreeNode *node = this->link[s];
      this->link[s]->parent = nullptr;
      this->link[s] = RBTree<Key, Value>::nil;
      return node;
    }

    inline RBTreeNode *detach(RBTreeNode &node) {
      if (this->link[RIGHT] == &node)
        return this->detach(RIGHT);
      else if (this->link[LEFT] == &node)
        return this->detach(LEFT);
      else
        assert(0);

      return nullptr;
    }
    
    //find the max node in a tree
    inline RBTreeNode *searchMax() {
      if (!this->link[RIGHT]->isNil())
        return this->link[RIGHT]->searchMax();
      else
        return this;
    }
    //find the min node in a tree
    inline RBTreeNode *searchMin() {
      if (!this->link[LEFT]->isNil())
        return this->link[LEFT]->searchMax();
      else
        return this;
    }
    //used in the case of rotation when inserting a node with a value into the tree
    void rotate(Side s) {
      RBTreeNode *nLeaf;
      RBTreeNode *nParent;
      RBTreeNode *nGrand;
      Side r = otherSide(s);

      nGrand = this->parent;
      nParent = this->detach(r);
      assert(nParent);

      nLeaf = nParent->detach(s);

      if (nGrand) {
        Side ps = nGrand->whichSide(*this);
        nGrand->detach(ps);
        nGrand->attach(ps, *nParent);
      } else {
        this->tree->root = nParent;
      }

      nParent->attach(s, *this);

      if (!nLeaf->isNil())
        this->attach(r, *nLeaf);
    }

    void adjustInsert() {
      if (this->parent == nullptr) {
        this->setBlack();
        return;
      } else {
        if (this->parent->isRed()) {
          assert(this->parent->parent);
          assert(this->parent->parent->isBlack());
          RBTreeNode *cParent = this->parent;
          RBTreeNode *grand = this->parent->parent;
          RBTreeNode *uncle = this->parent->getBrother();
          Side s;

          if (uncle->isRed()) {
            uncle->setBlack();
            this->parent->setBlack();
            grand->setRed();
            grand->adjustInsert();
          } else {
            if (this->parent->whichSide(*this) !=
                grand->whichSide(*this->parent)) {
              s = otherSide(cParent->whichSide(*this));
              cParent->rotate(s);
              cParent = this;
            }

            s = otherSide(grand->whichSide(*cParent));
            grand->rotate(s);

            assert(grand->isBlack() && cParent->isRed());
            grand->swapColor(*cParent);
          }
        }
      }
    }
    
    //returns false if the tree already contains a key, returns true if an item is inserted to tree
    bool insert(RBTreeNode &node) {
      if (this->key == node.key) {
        return false;
      } else {
        Side s = (node.key < this->key ? LEFT : RIGHT);
        if (!this->link[s]->isNil())
          return this->link[s]->insert(node);
        else
          this->attach(s, node);
      }
      node.adjustInsert();
      return true;
    }
    //find a value within the tree
    RBTreeNode *lookup(Key k) {
      if (this->key == k) {
        return this;
      } else {
        Side s = (k < this->key ? LEFT : RIGHT);
        return (this->link[s]->isNil() ? nullptr : this->link[s]->lookup(k));
      }
    }
    void leave() {
      RBTreeNode *cParent = this->parent;

      if (this->link[LEFT]->isNil() && this->link[RIGHT]->isNil()) {
        if (cParent) {
          Side s = cParent->whichSide(*this);
          cParent->detach(*this);

          if (this->isBlack()) {
            cParent->link[s]->adjustLeave(cParent);
          }
        } else {
          this->tree->root = nullptr;
        }
      } else if ((this->link[LEFT]->isNil()) ^ (this->link[RIGHT]->isNil())) {
        Side s = (this->link[LEFT]->isNil() ? RIGHT : LEFT);
        RBTreeNode *cTarget = this->detach(s);

        if (cParent) {
          cParent->detach(*this);
          cParent->attach(*cTarget);
        } else
          this->tree->root = cTarget;

        if (this->isBlack())
          cTarget->adjustLeave(cParent);
      } else {
        assert(!this->link[LEFT]->isNil() && !this->link[RIGHT]->isNil());

        RBTreeNode *cMax = this->link[LEFT]->searchMax();
        RBTreeNode *mParent = cMax->parent;
        RBTreeNode *cLeft = this->detach(LEFT);
        RBTreeNode *cRight = this->detach(RIGHT);
        RBTreeNode *mLeft = cMax->detach(LEFT);

        this->attach(*mLeft);
        if (cParent) {
          cParent->detach(*this);
        } else {
          this->tree->root = nullptr;
        }

        if (cMax != cLeft) {
          mParent->detach(*cMax);
          mParent->attach(*this);
          cMax->attach(LEFT, *cLeft);
          cMax->attach(RIGHT, *cRight);
        } else {
          assert(cMax->link[RIGHT]->isNil());
          cMax->attach(RIGHT, *cRight);
          cMax->attach(LEFT, *this);
        }

        if (cParent) {
          cParent->attach(*cMax);
        } else {
          this->tree->root = cMax;
        }

        this->swapColor(*cMax);
        this->leave();
      }
    }

    void adjustLeave(RBTreeNode *cParent) {
      if (nullptr == cParent) {
        this->setBlack();
        return;
      }
      if (this->isRed()) {
        this->setBlack();
        return;
      }

      RBTreeNode *cNeighbor =
          cParent->link[otherSide(cParent->whichSide(*this))];

      assert(cNeighbor);

      if (cNeighbor->isRed()) {
        Side s = cParent->whichSide(*this);
        assert(cParent->isBlack());
        cParent->swapColor(*cNeighbor);
        cParent->rotate(s);
        cNeighbor = cParent->link[otherSide(s)];
      } else if (cParent->isBlack() && cNeighbor->link[LEFT]->isBlack() &&
                 cNeighbor->link[RIGHT]->isBlack()) {
        assert(cNeighbor->isBlack());
        cNeighbor->setRed();
        return cParent->adjustLeave(cParent->parent);
      }

      if (cParent->isRed() && cNeighbor->link[LEFT]->isBlack() &&
          cNeighbor->link[RIGHT]->isBlack()) {
        assert(cNeighbor->isBlack());
        cParent->swapColor(*cNeighbor);
      } else {
        Side ns = cParent->whichSide(*cNeighbor);
        Side os = otherSide(ns);

        if (cNeighbor->link[os]->isRed() && cNeighbor->link[ns]->isBlack()) {
          cNeighbor->swapColor(*cNeighbor->link[os]);
          cNeighbor->rotate(ns);
          cNeighbor = cParent->link[ns];
        }

        if (cNeighbor->link[ns]->isRed()) {
          cNeighbor->link[ns]->setBlack();
          cParent->swapColor(*cNeighbor);
          cParent->rotate(os);
        }
      }
    }
    //checkrep helper *************************************
    
    //recursive call into this method to perform checkrep on the whole tree
    int checkRepHelper() {
      assert(obj);
      assert(!isNil());
      
      int leftSide = 0;
      if (!link[LEFT]->isNil()) {
        assert(link[LEFT]->parent == this);
        assert(!isRed() || ! parent->isRed()); //need to make sure that either the parent or the child is not red
        assert(link[LEFT]->key < key);//for the left side, need to make sure that the child has a lower value than the parent
        //recurse down the left side
        leftSide = link[LEFT]->checkRepHelper();
      }
      int rightSide = 0;
      if (!link[RIGHT]->isNil()) {
        assert(!isRed() || ! parent->isRed());
        assert(link[RIGHT]->parent == this);
        //for the left side, need to make sure that the child has a greater value than the parent
        assert(link[RIGHT]->key > key);
        //recurse down the right side
        rightSide = link[RIGHT]->checkRepHelper();
      }
      //red black tree should be the same height on the left side and right side
      //if this last node is black, we add one, else nothing is added
      assert(leftSide == rightSide);
      return leftSide + this->getColor();
    }

  private:
    Value *obj;
    Key key;
    Color color;
    RBTreeNode *parent, *link[2];
    RBTree *tree;
  };
  
  RBTreeNode *root;
  
  static RBTreeNode *nil;

public:
  RBTree() : root(nullptr) {}
  ~RBTree() { delete this->root; }
  // checkRep *************************************************
  
  
  //to gain access to the root, needed this to be a method of the tree, which then gives access to the nodes
  void checkRep() {
    if (root != NULL) {
      root->checkRepHelper();
    }
  }
  bool insert(Key key, Value *p) {
    RBTreeNode *node = new RBTreeNode(key, p, this);
    assert(key >= 0);

    if (this->root) {
      if (!this->root->insert(*node)) {
        delete node;
        return false;
      }
    } else {
      this->root = node;
      node->setBlack();
    }
    return true;
  }

  
  Value *lookup(Key key) {
    if (nullptr == this->root)
      return nullptr;

    RBTreeNode *node = this->root->lookup(key);
    return (node ? node->getObj() : nullptr);
  }

  bool remove(Key key) {
    if (nullptr == this->root)
      return false;

    RBTreeNode *node = this->root->lookup(key);
    if (nullptr == node)
      return false;

    node->leave();
    delete node;

    return true;
  }
};

template <typename Key, typename Value>
class RBTree<Key, Value>::RBTreeNode *RBTree<Key, Value>::nil = new RBTreeNode(-1, 0, 0);

} // namespace

