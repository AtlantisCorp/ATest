//
//  ATComparator.h
//  ATest
//
//  Created by jacques tronconi on 16/06/2019.
//  Copyright © 2019 jacques tronconi. All rights reserved.
//

#ifndef ATComparator_h
#define ATComparator_h

#include "ATStdIncludes.h"

namespace ATest
{
    template < typename Result >
    struct Comparator {
        virtual bool compare(const Result& rhs, const Result& lhs) const = 0;
    };
    
    template < typename Result >
    struct IsEqual : public Comparator<Result> {
        bool compare(const Result& rhs, const Result& lhs) const {
            return rhs == lhs;
        }
    };
    
    template < typename Result >
    struct IsDifferent : public Comparator<Result> {
        bool compare(const Result& rhs, const Result& lhs) const {
            return rhs != lhs;
        }
    };
    
    template < typename Result >
    struct IsGreater : public Comparator<Result> {
        bool compare(const Result& rhs, const Result& lhs) const {
            return rhs > lhs;
        }
    };
    
    template < typename Result >
    struct IsGreaterOrEqual : public Comparator<Result> {
        bool compare(const Result& rhs, const Result& lhs) const {
            return rhs >= lhs;
        }
    };
    
    template < typename Result >
    struct IsLesser : public Comparator<Result> {
        bool compare(const Result& rhs, const Result& lhs) const {
            return rhs < lhs;
        }
    };
    
    template < typename Result >
    struct IsLesserOrEqual : public Comparator<Result> {
        bool compare(const Result& rhs, const Result& lhs) const {
            return rhs <= lhs;
        }
    };
}

#endif /* ATComparator_h */
