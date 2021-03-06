/*=============================================================================
    Copyright (c) 2001-2013 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_PARSE_APRIL_16_2006_0442PM)
#define BOOST_SPIRIT_PARSE_APRIL_16_2006_0442PM

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/support/context.hpp>
#include <boost/spirit/home/x3/core/parser.hpp>
#include <boost/spirit/home/x3/core/skip_over.hpp>
#include <boost/concept_check.hpp>

namespace boost { namespace spirit { namespace x3
{
    ///////////////////////////////////////////////////////////////////////////
    template <typename Iterator, typename Parser, typename Attribute>
    inline bool
    parse(
        Iterator& first
      , Iterator last
      , Parser const& p
      , Attribute& attr)
    {
        // Make sure the iterator is at least a forward_iterator. If you got a
        // compilation error here, then you are using an input_iterator while
        // calling this function. You need to supply at least a forward_iterator
        // instead.
        BOOST_CONCEPT_ASSERT((ForwardIterator<Iterator>));

        // If you get an error no matching function for call to 'as_parser'
        // here, then p is not a parser or there is no suitable conversion
        // from p to a parser.
        return as_parser(p).parse(first, last, unused, attr);
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename Iterator, typename Parser>
    inline bool
    parse(
        Iterator& first
      , Iterator last
      , Parser const& p)
    {
        return parse(first, last, p, unused);
    }

    ///////////////////////////////////////////////////////////////////////////
    enum class skip_flag
    {
        post_skip,      // force post-skipping in phrase_parse()
        dont_post_skip  // inhibit post-skipping in phrase_parse()
    };

    ///////////////////////////////////////////////////////////////////////////
    template <typename Iterator, typename Parser, typename Skipper, typename Attribute>
    inline bool
    phrase_parse(
        Iterator& first
      , Iterator last
      , Parser const& p
      , Skipper const& s
      , Attribute& attr
      , skip_flag post_skip = skip_flag::post_skip)
    {
        // Make sure the iterator is at least a forward_iterator. If you got a
        // compilation error here, then you are using an input_iterator while
        // calling this function. You need to supply at least a forward_iterator
        // instead.
        BOOST_CONCEPT_ASSERT((ForwardIterator<Iterator>));

        // If you get an error no matching function for call to 'as_parser'
        // here, for either p or s, then p or s is not a parser or there is
        // no suitable conversion from p to a parser.
        context<skipper_tag, Skipper const> skipper(as_parser(s));
        bool r = as_parser(p).parse(first, last, skipper, attr);
        if (post_skip == skip_flag::post_skip)
            x3::skip_over(first, last, skipper);
        return r;
    }

    ///////////////////////////////////////////////////////////////////////////
    template <typename Iterator, typename Parser, typename Skipper>
    inline bool
    phrase_parse(
        Iterator& first
      , Iterator last
      , Parser const& p
      , Skipper const& s
      , skip_flag post_skip = skip_flag::post_skip)
    {
        return phrase_parse(first, last, p, s, unused, post_skip);
    }
}}}

#endif

