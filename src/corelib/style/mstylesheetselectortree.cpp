#include "mstylesheetselectortree.h"

#include "mstylesheetparser.h"
#include "mstylesheetparser_p.h"

#include <QDebug>

#include <algorithm>

MStyleSheetSelectorTree::MStyleSheetSelectorTree()
{
}

void MStyleSheetSelectorTree::partiallyMatchingSelectors(const MStyleSheetPrivate::StyleSpec &spec, QList<const MStyleSheetSelector*> *selectors) const
{
    QList<const StyleSheetSelectorList*> listOfSelectorLists;
    // fist find all nodes without specified class name
    if (const Node *node =
            reinterpret_cast<const Node*>(childWithIndex(MUniqueStringCache::EmptyStringIndex))) {
        selectorsWithObjectName(spec.objectName, node, &listOfSelectorLists);
    }

    // now the rest
    foreach(const QByteArray& type, spec.classHierarchy) {
        MUniqueStringCache::Index typeId = MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(type);
        if (const Node *node =
                reinterpret_cast<const Node*>(childWithIndex(typeId))) {
            selectorsWithObjectName(spec.objectName, node, &listOfSelectorLists);
        }
    }

    fillSelectorList(listOfSelectorLists, selectors);
}

void MStyleSheetSelectorTree::selectorsWithObjectName(const QByteArray& objectName, const Node* node, QList<const StyleSheetSelectorList*>* listOfSelectorLists) const
{
    if (const StyleSheetSelectorList *sl =
            reinterpret_cast<const StyleSheetSelectorList*>(node->childWithIndex(MUniqueStringCache::EmptyStringIndex))) {
        listOfSelectorLists->append(sl);
    }

    if (objectName.isEmpty()) {
        return;
    }

    MUniqueStringCache::Index objectNameId = MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(objectName);
    if (const StyleSheetSelectorList *sl =
            reinterpret_cast<const StyleSheetSelectorList*>(node->childWithIndex(objectNameId))) {
        listOfSelectorLists->append(sl);
    }
}

void MStyleSheetSelectorTree::fillSelectorList(const QList<const StyleSheetSelectorList*> listOfSelectorLists, QList<const MStyleSheetSelector*> *selectors) const
{
    foreach(const StyleSheetSelectorList* list, listOfSelectorLists) {
        list->extractSelectors(selectors);
    }
}

bool operator <(const Node::ChildHeader &header,
                MUniqueStringCache::Index index)
{
    return header.index < index;
}

const void* Node::childWithIndex(MUniqueStringCache::Index index) const
{
    const ChildHeader *header = childHeaderWithIndex(index);
    if (!header) {
        return 0;
    }

    return (char*)firstChildHeader() + header->offset;
}

const Node::ChildHeader *Node::firstChildHeader() const
{
    // attention: if the size of an inheriting class is not the same as the size of node this will fail!!
    return reinterpret_cast<Node::ChildHeader*>((char*)this + sizeof(Node));
}

const char *Node::firstChild() const
{
    return reinterpret_cast<const char*>(firstChildHeader() + childCount * sizeof(ChildHeader));
}

const Node::ChildHeader* Node::childHeaderWithIndex(MUniqueStringCache::Index index) const
{
    if (childCount == 0) {
        return 0;
    }

    const Node::ChildHeader *header = firstChildHeader();
    if (index == MUniqueStringCache::EmptyStringIndex) {
        // if at all this must be the first entry
        if (header->index == MUniqueStringCache::EmptyStringIndex) {
            return header;
        } else {
            return 0;
        }
    } else {
        const ChildHeader *candidate = std::lower_bound(header, header + childCount, index);
        if (candidate->index == index) {
            return candidate;
        } else {
            return 0;
        }
    }
}

void StyleSheetSelectorList::extractSelectors(QList<const MStyleSheetSelector*> *selectors) const
{
    char *buffer = (char*)this + sizeof(StyleSheetSelectorList);

    for (int i = 0; i < selectorCount; ++i) {
        selectors->push_back(MStyleSheetParserPrivate::readSelector(&buffer));
    }
}
