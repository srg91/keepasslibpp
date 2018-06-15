1. Переименовать keepasslibpp в какой-нибудь keepasspp, а сам проект в libkeepasspp
2. Заменить const std::string на std::string_view
3. Заменить .size на std::size() (из <iterator>)
4. Заменить .data() и &xxxxx[0] на std::data() (из <iterator>)
5. shared_ptr<int8_t> вместо ByteVector? how to begin / end?
6. заменить все ::value на _v