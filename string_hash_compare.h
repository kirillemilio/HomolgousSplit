#ifndef STRING_HASH_COMPARE_H
#define STRING_HASH_COMPARE_H

template <class _CharT, class _Traits, class _Alloc>
std::size_t
string_hash(const std::basic_string<_CharT,_Traits,_Alloc>& __s)
{
  unsigned long __h = 0;
  typedef typename std::basic_string<_CharT,_Traits,_Alloc>::const_pointer const_ptr;
  size_t __len = __s.size();
  const _CharT* __data = __s.data();
  for ( size_t __i = 0; __i < __len; ++__i)
    __h = 5*__h + __data[__i];
  return size_t(__h);
}


template <
  class _CharT,
  class _Traits = std::char_traits<_CharT>,
  class _Alloc = std::allocator<_CharT>,
  class _Pr = std::less< std::basic_string<_CharT, _Traits, _Alloc> >
>
class basic_string_hash_compare
{    // traits class for hash containers
public:
    typedef std::basic_string<_CharT, _Traits, _Alloc> _Kty;   // traits class for hash containers
    enum
        {    // parameters for hash table
        bucket_size = 4,    // 0 < bucket_size
        min_buckets = 8
        };    // min_buckets = 2 ^^ N, 0 < N

    basic_string_hash_compare()
        : comp()
        {    // construct with default comparator
        }

    basic_string_hash_compare(_Pr _Pred)
        : comp(_Pred)
        {    // construct with _Pred comparator
        }

    size_t operator()(const _Kty& _Keyval) const
        {    // hash _Keyval to size_t value
        return string_hash(_Keyval);
        }

    bool operator()(const _Kty& _Keyval1, const _Kty& _Keyval2) const
        {    // test if _Keyval1 ordered before _Keyval2
        return (comp(_Keyval1, _Keyval2));
        }

private:
    _Pr comp;    // the comparator object
};

typedef basic_string_hash_compare<char> string_hash_compare;


#endif // STRING_HASH_COMPARE_H
