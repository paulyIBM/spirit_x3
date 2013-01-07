/*=============================================================================
    Copyright (c) 2001-2013 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(SPIRIT_SEQUENCE_DETAIL_JAN_06_2012_1015AM)
#define SPIRIT_SEQUENCE_DETAIL_JAN_06_2012_1015AM

#if defined(_MSC_VER)
#pragma once
#endif

#include <boost/spirit/home/support/traits/attribute_of.hpp>

#include <boost/fusion/include/begin.hpp>
#include <boost/fusion/include/end.hpp>
#include <boost/fusion/include/advance.hpp>
#include <boost/fusion/include/empty.hpp>
#include <boost/fusion/include/front.hpp>
#include <boost/fusion/include/iterator_range.hpp>
#include <boost/fusion/include/as_deque.hpp>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/push_front.hpp>
#include <boost/mpl/joint_view.hpp>
#include <boost/mpl/copy_if.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/if.hpp>

#include <boost/type_traits/add_reference.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace spirit { namespace x3
{
    template <typename Left, typename Right>
    struct sequence;
}}}

namespace boost { namespace spirit { namespace x3 { namespace detail
{
    template <typename Parser>
    struct sequence_size
    {
        static int const value = Parser::has_attribute;
    };

    template <typename L, typename R>
    struct sequence_size<sequence<L, R>>
    {
        static int const value =
            sequence_size<L>::value + sequence_size<R>::value;
    };

    template <typename Parser, typename Attribute>
    struct pass_sequence_attribute_unused
    {
        typedef unused_type type;

        template <typename Attribute_>
        static unused_type
        call(Attribute_& attr)
        {
            return unused_type();
        }
    };

    template <typename Parser, typename Attribute>
    struct pass_sequence_attribute_used
    {
        typedef typename fusion::result_of::front<Attribute>::type type;

        template <typename Attribute_>
        static typename add_reference<type>::type
        call(Attribute_& attr)
        {
            return fusion::front(attr);
        }
    };

    template <typename Parser, typename Attribute>
    struct pass_sequence_attribute :
        mpl::if_<fusion::result_of::empty<Attribute>
          , pass_sequence_attribute_unused<Parser, Attribute>
          , pass_sequence_attribute_used<Parser, Attribute>>::type
    {
    };

    template <typename L, typename R, typename Attribute>
    struct pass_sequence_attribute<sequence<L, R>, Attribute>
    {
        typedef Attribute type;

        template <typename Attribute_>
        static Attribute_&
        call(Attribute_& attr)
        {
            return attr;
        }
    };

    template <typename L, typename R, typename Attribute>
    struct partition_attribute
    {
        static int const l_size = sequence_size<L>::value;
        static int const r_size = sequence_size<R>::value;

        // If you got an error here, then you are trying to pass
        // a fusion sequence with the wrong number of elements
        // as that expected by the (sequence) parser.
        static_assert(
            fusion::result_of::size<Attribute>::value == (l_size + r_size)
          , "Attribute does not have the expected size."
        );

        typedef typename fusion::result_of::begin<Attribute>::type l_begin;
        typedef typename fusion::result_of::advance_c<l_begin, l_size>::type l_end;
        typedef typename fusion::result_of::end<Attribute>::type r_end;
        typedef fusion::iterator_range<l_begin, l_end> l_range;
        typedef fusion::iterator_range<l_end, r_end> r_range;
        typedef pass_sequence_attribute<L, l_range> l_pass;
        typedef pass_sequence_attribute<R, r_range> r_pass;

        static l_range left(Attribute& s)
        {
            auto i = fusion::begin(s);
            return l_range(i, fusion::advance_c<l_size>(i));
        }

        static r_range right(Attribute& s)
        {
            return r_range(
                fusion::advance_c<l_size>(fusion::begin(s))
              , fusion::end(s));
        }
    };

    template <typename L, typename R>
    struct get_sequence_types
    {
        typedef
            mpl::vector<
                typename traits::attribute_of<L>::type
              , typename traits::attribute_of<R>::type
            >
        type;
    };

    template <typename LL, typename LR, typename R>
    struct get_sequence_types<sequence<LL, LR>, R>
    {
        typedef typename
            mpl::push_front<
                typename get_sequence_types<LL, LR>::type
              , typename traits::attribute_of<R>::type
            >::type
        type;
    };

    template <typename L, typename RL, typename RR>
    struct get_sequence_types<L, sequence<RL, RR>>
    {
        typedef typename
            mpl::push_back<
                typename get_sequence_types<RL, RR>::type
              , typename traits::attribute_of<L>::type
            >::type
        type;
    };

    template <typename LL, typename LR, typename RL, typename RR>
    struct get_sequence_types<sequence<LL, LR>, sequence<RL, RR>>
    {
        typedef
            mpl::joint_view<
                typename get_sequence_types<LL, LR>::type
              , typename get_sequence_types<RL, RR>::type
            >
        type;
    };

    template <typename L, typename R>
    struct attribute_of_sequence
    {
        // Get all sequence attribute types
        typedef typename get_sequence_types<L, R>::type all_types;

        // Filter all unused_types
        typedef typename
            mpl::copy_if<
                all_types
              , mpl::not_<is_same<mpl::_1, unused_type>>
              , mpl::back_inserter<mpl::vector<>>
            >::type
        filtered_types;

        // Build a fusion::deque
        typedef typename
            fusion::result_of::as_deque<filtered_types>::type
        type;
    };
}}}}

#endif