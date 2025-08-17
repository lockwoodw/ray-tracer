#ifndef RAY_TRACER_POROUS_SHEET_H
#define RAY_TRACER_POROUS_SHEET_H

#include "sheet.h"
#include "space.h"
#include "utils.h"

typedef bool(*PassFn)(const Point&);

class PorousSheet: public Sheet {
    // A sheet that renders points according to a given pass function

    PassFn pass_fn_;

    public:
        PorousSheet(): Sheet {}, pass_fn_ { nullptr } {}
        PorousSheet(PassFn pf): Sheet {}, pass_fn_ { pf } {}
        PorousSheet(const PorousSheet& p): Sheet {}, pass_fn_ { p.pass_fn_ } {}

        bool operator==(const Shape& s) const;
        bool Intersect(IntersectionList& list, const Ray& ray) const override;
};

#endif