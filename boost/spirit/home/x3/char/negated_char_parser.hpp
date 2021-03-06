/*=============================================================================
    Copyright (c) 2001-2013 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(BOOST_SPIRIT_NEGATED_CHAR_PARSER_APR_16_2006_0906AM)
#define BOOST_SPIRIT_NEGATED_CHAR_PARSER_APR_16_2006_0906AM

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/x3/char/char_parser.hpp>

namespace boost { namespace spirit { namespace x3
{
    ///////////////////////////////////////////////////////////////////////////
    // negated_char_parser handles ~cp expressions (cp is a char_parser)
    ///////////////////////////////////////////////////////////////////////////
    template <typename Positive>
    struct negated_char_parser :
        char_parser<negated_char_parser<Positive>>
    {
        typedef typename Positive::attribute_type attribute_type;
        static bool const has_attribute = Positive::has_attribute;

        negated_char_parser(Positive const& positive)
          : positive(positive) {}

        template <typename CharParam, typename Context>
        bool test(CharParam ch, Context const& context) const
        {
            return !positive.test(ch, context);
        }

        Positive positive;
    };

    template <typename Positive>
    inline negated_char_parser<Positive>
    operator~(char_parser<Positive> const& cp)
    {
        return negated_char_parser<Positive>(cp.derived());
    }

    template <typename Positive>
    inline Positive const&
    operator~(negated_char_parser<Positive> const& cp)
    {
        return cp.positive;
    }
}}}

#endif
