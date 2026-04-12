#ifndef headerfile
    #include "user_defined_datatypes.c"
    #include "constants.c"
    #include "logical_functions_of_decision.c"
#endif

int my_floor(float x, error* err){// ask Amirako
    long int lintx = x;
    lintx -= else0(x < 0, 1);
    *err = ternary((lintx > MAX_INT32) | (lintx < MIN_INT32), ternary(lintx > MAX_INT32, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), *err);
    return lintx;
}

int my_ceil(float x, error* err){// ask Amirako
    long int lintx = x;
    lintx += else0(lintx < x, 1);
    *err = ternary((lintx > MAX_INT32) | (lintx < MIN_INT32), ternary(lintx > MAX_INT32, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), *err);
    return lintx;
}
