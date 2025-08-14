
double sine(double radian, errror* err){// TODO safe_double_multiplication and safe_double_division
    dbits radian_var = (dbits){.d = radian};
    dbits PI2_var = (dbits){.d = PI2};
    radian = my_fmod(radian_var, PI2_var);
    radian -= (exp_double2uint(radian, 3,err) / factorial(3, err)) - (exp_double2uint(radian, 5,err) / factorial(5, err)) + (exp_double2uint(radian, 7,err) / factorial(7, err)) - (exp_double2uint(radian, 9,err) / factorial(9, err));// UNDERFLOW
    return radian;
}

double cosine(double radian, error* err){// TODO safe_double_multiplication and safe_double_division
    dbits radian_var = (dbits){.d = radian};
    dbits PI2_var = (dbits){.d = PI2};
    radian = my_fmod(radian_var, PI2_var);
    radian = 1 - (exp_double2uint(radian, 2,err) / factorial(2, err)) + (exp_double2uint(radian, 4,err) / factorial(4, err)) - (exp_double2uint(radian, 6,err) / factorial(6, err)) + (exp_double2uint(radian, 8,err) / factorial(8, err));// UNDERFLOW
    return radian;
}
