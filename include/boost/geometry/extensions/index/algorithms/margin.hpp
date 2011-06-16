// Boost.Geometry (aka GGL, Generic Geometry Library)
//
// Boost.SpatialIndex - n-dimensional box's margin value, 2d perimeter, 3d surface, etc...
//
// Copyright 2011 Adam Wulkiewicz.
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GEOMETRY_EXTENSIONS_INDEX_ALGORITHMS_MARGIN_HPP
#define BOOST_GEOMETRY_EXTENSIONS_INDEX_ALGORITHMS_MARGIN_HPP

namespace boost { namespace geometry { namespace index {

template <typename Box>
struct default_margin_result
{
    typedef typename select_most_precise<
        typename coordinate_type<Box>::type,
        long double
    >::type type;
};

namespace detail {

template <typename Box, size_t CurrentDimension, size_t EdgeDimension>
struct margin_for_each_edge
{
    BOOST_STATIC_ASSERT(0 < CurrentDimension);
    BOOST_STATIC_ASSERT(0 < EdgeDimension);

    static inline typename default_margin_result<Box>::type apply(Box const& b)
    {
        return margin_for_each_edge<Box, CurrentDimension, EdgeDimension - 1>::apply(b) *
            ( geometry::get<max_corner, EdgeDimension - 1>(b) - geometry::get<min_corner, EdgeDimension - 1>(b) );
    }
};

template <typename Box, size_t CurrentDimension>
struct margin_for_each_edge<Box, CurrentDimension, CurrentDimension>
{
    BOOST_STATIC_ASSERT(0 < CurrentDimension);

    static inline typename default_margin_result<Box>::type apply(Box const& b)
    {
        return margin_for_each_edge<Box, CurrentDimension, CurrentDimension - 1>::apply(b);
    }
};

template <typename Box, size_t CurrentDimension>
struct margin_for_each_edge<Box, CurrentDimension, 1>
{
    BOOST_STATIC_ASSERT(0 < CurrentDimension);

    static inline typename default_margin_result<Box>::type apply(Box const& b)
    {
        return geometry::get<max_corner, 0>(b) - geometry::get<min_corner, 0>(b);
    }
};

template <typename Box>
struct margin_for_each_edge<Box, 1, 1>
{
    static inline typename default_margin_result<Box>::type apply(Box const& /*b*/)
    {
        return 1;
    }
};

template <typename Box, size_t CurrentDimension>
struct margin_for_each_dimension
{
    BOOST_STATIC_ASSERT(0 < CurrentDimension);
    BOOST_STATIC_ASSERT(CurrentDimension <= traits::dimension<Box>::value);

    static inline typename default_margin_result<Box>::type apply(Box const& b)
    {
        return margin_for_each_dimension<Box, CurrentDimension - 1>::apply(b) +
            2 * margin_for_each_edge<Box, CurrentDimension, traits::dimension<Box>::value>::apply(b);
    }
};

template <typename Box>
struct margin_for_each_dimension<Box, 1>
{
    static inline typename default_margin_result<Box>::type apply(Box const& b)
    {
        return 2 * margin_for_each_edge<Box, 1, traits::dimension<Box>::value>::apply(b);
    }
};

} // namespace detail

template <typename Box>
typename default_margin_result<Box>::type margin(Box const& b)
{
    return detail::margin_for_each_dimension<Box, traits::dimension<Box>::value>::apply(b);
}

}}} // namespace boost::geometry::index

#endif // BOOST_GEOMETRY_EXTENSIONS_INDEX_ALGORITHMS_MARGIN_HPP
