#ifndef MSTYLESHEETSELECTORTREE_H
#define MSTYLESHEETSELECTORTREE_H

#include <muniquestringcache.h>
#include "mstylesheet_p.h"

#include <QByteArray>

class MStyleSheetSelector;
class StyleSheetSelectorList;
class Node;

/**
  * A node points a a list of children. The children are identified by a integer.
  * The list of children is saved in a header consisting of the integer and an offset
  * to the actual child. As the integers are sorted a child can be found with a binary
  * search.
  */
class Node {
public:
    struct ChildHeader {
        MUniqueStringCache::Index index;
        int offset;
    };

    const void* childWithIndex(MUniqueStringCache::Index index) const;

private:
    friend class MStyleSheetParserPrivate;

    const ChildHeader* childHeaderWithIndex(MUniqueStringCache::Index index) const;
    const Node::ChildHeader *firstChildHeader() const;
    const char *firstChild() const;

    int totalSize;
    int childCount;
};

/**
  * MStyleSheetSelectorTree represents a tree which speeds up the lookup of matching selectors
  * for a given object.
  * Right now the tree has a depth of two. The root node points to children which have a
  * given style class name (e.g. MLableStyle). These children then point to a list of selector all
  * having the same object name.
  * MStyleSheetSelectorTree is a structure which can be loaded from a file. It internally does not
  * store any pointers only offets.
  */
class MStyleSheetSelectorTree : public Node
{
public:
    MStyleSheetSelectorTree();
    void partiallyMatchingSelectors(const MStyleSheetPrivate::StyleSpec &spec, QList<const MStyleSheetSelector*> *selectors) const;

private:
    void fillSelectorList(const QList<const StyleSheetSelectorList*> listOfSelectorLists, QList<const MStyleSheetSelector*> *selectors) const;
    void selectorsWithObjectName(const QByteArray& objectName, const Node* node, QList<const StyleSheetSelectorList*>* listOfSelectorLists) const;
};

class StyleSheetSelectorList {
public:
    void extractSelectors(QList<const MStyleSheetSelector*> *selectors) const;

private:
    friend class MStyleSheetParserPrivate;
    int selectorCount;
};

#endif // MSTYLESHEETSELECTORTREE_H
