#ifndef RAY_TRACER_UTILS_H
#define RAY_TRACER_UTILS_H

static const double kAbsEpsilon = 1e-12;
static const double kRelEpsilon = 1e-8;
static const double kEpsilon = 1e-5;

bool floating_point_relative_compare(double a, double b, double rel_epsilon = kRelEpsilon);
bool floating_point_compare(double a, double b, double abs_epsilon = kAbsEpsilon, double rel_epsilon = kRelEpsilon);
bool simple_floating_point_compare(double a, double b, double epsilon = kEpsilon);

class FloatingPointComparatorInterface {
    public:
        virtual ~FloatingPointComparatorInterface() = default;
        virtual bool compare(double a, double b) = 0;
};

class FloatingPointComparator: public FloatingPointComparatorInterface {
    double abs_epsilon_;
    double rel_epsilon_;

    public:
        FloatingPointComparator(): abs_epsilon_ { kAbsEpsilon }, rel_epsilon_ { kRelEpsilon } {}
        FloatingPointComparator(double abs_epsilon, double rel_epsilon):
            abs_epsilon_ { abs_epsilon }, rel_epsilon_ { rel_epsilon } {}
        bool compare(double a, double b) override;
};

class SimpleFloatingPointComparator: public FloatingPointComparatorInterface {
    double epsilon_;
    public:
        SimpleFloatingPointComparator(): epsilon_ { 1e-5 } {}
        SimpleFloatingPointComparator(double epsilon): epsilon_ { epsilon } {}
        bool compare(double a, double b) override;
};

#endif