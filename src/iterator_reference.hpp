#pragma once

#include <iterator>

namespace keepasspp {

// TODO: Never used, remove?

// TODO: Small hack to pass iterators by reference
// TODO: If you know good way to do this, please remove this

// TODO: add == / != , ->, ++, *++, swap?
template <typename ForwardIterator>
class ForwardIteratorReference {
public:
    explicit ForwardIteratorReference(ForwardIterator& it)
        : iterator(it) {};

    ForwardIteratorReference& operator ++() {
        this->iterator++;
        return *this;
    };

    typename ForwardIterator::reference operator *() const {
        return *(this->iterator);
    }
private:
    ForwardIterator& iterator;
};

}

namespace std {

template <typename ForwardIterator>
struct iterator_traits<keepasspp::ForwardIteratorReference<ForwardIterator>> {
    using iterator_category = std::forward_iterator_tag;
    using value_type = typename ForwardIterator::value_type;
};

}
