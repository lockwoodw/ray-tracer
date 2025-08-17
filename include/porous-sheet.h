#ifndef RAY_TRACER_POROUS_SHEET_H
#define RAY_TRACER_POROUS_SHEET_H

#include "sheet.h"
#include "space.h"
#include "utils.h"

typedef bool(*PassThroughFunction)(const Point&);

class PorousSheet: public Sheet {
    // A sheet that renders points according to a given pass-through function

    PassThroughFunction pass_through_fn_;

    public:
        PorousSheet(): Sheet {}, pass_through_fn_ { nullptr } {}
        PorousSheet(PassThroughFunction ptf): Sheet {}, pass_through_fn_ { ptf } {}
        PorousSheet(const PorousSheet& p): Sheet {},
            pass_through_fn_ { p.pass_through_fn_ } {}

        bool operator==(const Shape& s) const;
        bool Intersect(IntersectionList& list, const Ray& ray) const override;
};

#endif