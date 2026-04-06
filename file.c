int64_t int64_uint64_add(int64_t a, uint64_t b, error* err){
    if(!err){ return a; }
    *err = ternary((MAX_INT64 - a) < b, POSITIVE_OVERFLOW, *err);
    return a + b;
  }
  
  
  // FUNCTION: long unsigned int and long int addition(uint64_t, int64_t, error*)
  
  uint64_t uint64_int64_add(uint64_t a, int64_t b, error* err){
    if(!err){ return a; }
    *err = ternary((MAX_UINT64 - a) < b, POSITIVE_OVERFLOW, *err);
    return a + b;
  }
  
  int is_float_p_err(error err){ 
      return (err == SUBNORM) | (err == QNAN) | (err == SNAN) | (err == UNDERFLOW) | (err == POSITIVE_INFINITY) | (err == NEGATIVE_INFINITY);
  }
  
  
  // FUNCTION: long int and double addition(int64_t, double, error*)
  
  int64_t int64_double_add(int64_t a, dbits b, error* err){
    if(!err){ return a; }
    *err = ternary(b.parts.exp > MAX_NORM_DOUBLE_EXP, QNAN, *err);    
    *err = ternary((*err == QNAN) && b.parts.mantissa, SNAN, *err);
    *err = ternary(!b.parts.exp && b.parts.mantissa, UNDERFLOW, *err);
    if(is_float_p_err(*err)){ return a; }
    *err = ternary((MAX_INT64 - (a * (a > 0))) < b.d, POSITIVE_OVERFLOW, *err);
    *err = ternary((MIN_INT64 - (a * (a < 0))) > b.d, NEGATIVE_OVERFLOW, *err);
    return a + ((int64_t)b.d) * !(*err);
  }
  
  
  // FUNCTION: long unsigned int and double addition(uint64_t, double, error*)
  
  uint64_t uint64_double_add(uint64_t a, dbits b, error* err){
    if(!err){ return a; }
    *err = ternary(b.parts.exp > MAX_NORM_DOUBLE_EXP, QNAN, *err);    
    *err = ternary((*err == QNAN) && b.parts.mantissa, SNAN, *err);
    *err = ternary(!b.parts.exp && b.parts.mantissa, UNDERFLOW, *err);
    if(is_float_p_err(*err)){ return a; }
    *err = ternary(b.d > MAX_UINT64, POSITIVE_OVERFLOW, *err);
    *err = ternary((double_abs((dbits){ .d = b.d }) > a) & (b.d < 0), NEGATIVE_OVERFLOW, *err);
    return a + ((uint64_t)b.d) * !(*err);
  }
  
  
  #define VECTOR_MAX_ELEM_BYTE_SIZE MAX_UINT32
  
  
  vecN vec_create(datatype type, uint64_t n){
      uint32_t condition = ((n * amount_of_type_bytes(type))<= VECTOR_MAX_ELEM_BYTE_SIZE);
      n &= -condition;
  
      void* r = malloc(sizeof(datatype) 
                  + sizeof(uint32_t) 
                  + sizeof(error) 
                  + (n * amount_of_type_bytes(type)) );
  
      vecN result;
          result.type     = (datatype*)   r;
          result.n        = (uint32_t*)  (result.type + 1);
          result.err      = (error*)     (result.n    + 1);
          result.elements = (datapointer)(result.err  + 1);
  
      result.type[0] = type;
      result.n   [0] = n;
      result.err [0] = ternary(condition, NO_ERROR, MEMORY_LIMIT_EXCESS);
  
      return result;
  }
  
  
  error check_float_type_elem_for_err(datapointer data, datatype type, uint32_t n, error* err){ // TODO: what if amount of elements in data is less than n 
      if(!(data.i32 && err)){ return NULL_POINTER; }
      int8_t cond;
      switch(type){
          case FLOAT32:
              fbits fb;
              while(n--){
                  fb = (fbits){ .f = data.f32[n] };
                  cond = fb.parts.exp > MAX_NORM_FLOAT_EXP;
                  *err = ternary(cond, ternary(fb.parts.sign, NEGATIVE_INFINITY, POSITIVE_INFINITY), *err);// check for infinity value
                  *err = ternary(cond && fb.parts.mantissa, QNAN, *err);// check for NaN
                  *err = ternary(!(fb.exp) && fb.parts.mantissa, SUBNORM, *err);
              }
              break;
          case FLOAT64:
          dbits db;
          while(n--){
              db = (dbits){ .d = data.f64[n] };
              cond = db.parts.exp > MAX_NORM_DOUBLE_EXP;
              *err = ternary(cond, ternary(db.parts.sign, NEGATIVE_INFINITY, POSITIVE_INFINITY), *err);// check for infinity value
              *err = ternary(cond && db.parts.mantissa, QNAN, *err);// check for NaN
              *err = ternary(!(db.exp) && db.parts.mantissa, SUBNORM, *err);
          }
      }
      return *err;
  }
  
  
  // TODO: decide what to do with the length of arr
  vecN vec_filler(vecN vec, datapointer arr){// TODO: what if amount of elements in data is less than n
      if(!(vec.n && vec.type && vec.err && vec.elements.i32 && arr.i32)){ 
          error er = NULL_POINTER;
          vec.err = (error*)(long unsigned int*)ternary((long unsigned int)(long unsigned int*)vec.err, (long unsigned int)(long unsigned int*)vec.err, (long unsigned int)(long unsigned int*)&er);
          return vec; 
      }
      uint64_t i = vec.n[0] * amount_of_type_bytes(vec.type[0]);
      while(i--){
          vec.elements.i8[i] = arr.i8[i];
      }
      return vec;
  }
  
  datapointer elem_type_cast(datapointer elem, datatype from, datatype to, uint32_t n, error* err){// TODO: there is an option to convert negative int type to positve unsigned the same way as C does
      // TODO: what if amount of elements in data is less than n
      if(!(elem.i32 && err)){ 
          if(err){ *err = NULL_POINTER; }
          return elem; 
      }
      
      datapointer r = (datapointer){ .vptr = malloc(amount_of_type_bytes(to) * n) };
      switch(from){
              case INT8:
                  switch(to){
                      case INT8:    while(n--){ r.i8 [n] = elem.i8[n]; } break;
                      case INT32:   while(n--){ r.i32[n] = elem.i8[n]; } break;
                      case INT64:   while(n--){ r.i64[n] = elem.i8[n]; } break;
                      case FLOAT32: while(n--){ r.f32[n] = elem.i8[n]; } break;
                      case FLOAT64: while(n--){ r.f64[n] = elem.i8[n]; } break;
                      case UINT8:
                          while(n--){
                              *err = ternary(elem.i8[n] < 0, NEGATIVE_OVERFLOW, *err);
                              r.ui8[n] = elem.i8[n]; 
                          }
                          break;
                      case UINT32: 
                          while(n--){
                              *err = ternary(elem.i8[n] < 0, NEGATIVE_OVERFLOW, *err);
                              r.ui32[n] = elem.i8[n]; 
                          }
                          break;
                      case UINT64: 
                          while(n--){
                              *err = ternary(elem.i8[n] < 0, NEGATIVE_OVERFLOW, *err);
                              r.ui64[n] = elem.i8[n]; 
                          }
                          break;
                  }
                  break;
              case UINT8:
                  switch(to){
                      case INT8: 
                          while(n--){
                              *err = ternary(elem.ui8[n] > MAX_INT8, POSITIVE_OVERFLOW, *err);
                              r.i8[n] = elem.ui8[n];
                          } 
                          break;
                      case UINT8:   while(n--){ r.ui8[n]  = elem.ui8[n]; } break;
                      case INT32:   while(n--){ r.i32[n]  = elem.ui8[n]; } break;
                      case UINT32:  while(n--){ r.ui32[n] = elem.ui8[n]; } break;
                      case INT64:   while(n--){ r.i64[n]  = elem.i8 [n]; } break;
                      case UINT64:  while(n--){ r.i64[n]  = elem.ui8[n]; } break;
                      case FLOAT32: while(n--){ r.f32[n]  = elem.ui8[n]; } break;
                      case FLOAT64: while(n--){ r.f64[n]  = elem.ui8[n]; } 
                  }
                  break;
              case INT32:
                  switch(to){
                      case INT32:   while(n--){ r.i32[n] = elem.i32[n]; } break;
                      case INT64:   while(n--){ r.i64[n] = elem.i32[n]; } break;
                      case FLOAT32: while(n--){ r.f32[n] = elem.i32[n]; } break;
                      case FLOAT64: while(n--){ r.f64[n] = elem.i32[n]; } break; 
                      case INT8:    
                          while(n--){
                              *err = ternary(elem.i32[n] > MAX_INT8, POSITIVE_OVERFLOW, *err);
                              *err = ternary(elem.i32[n] < MIN_INT8, NEGATIVE_OVERFLOW, *err);
                              r.i8[n] = elem.i32[n];
                          }
                          break;
                      case UINT8:   
                          while(n--){ 
                              *err = ternary(elem.i32[n] > MAX_UINT8, POSITIVE_OVERFLOW, *err);
                              *err = ternary(elem.i32[n] < 0, NEGATIVE_OVERFLOW, *err);
                              r.ui8[n] = elem.i32[n];
                          } 
                          break;
                          case UINT32:  
                          while(n--){
                              *err = ternary(elem.i32[n] < 0, NEGATIVE_OVERFLOW, *err);
                              r.ui32[n] = elem.i32[n];
                          } 
                          break;
                      case UINT64:  
                          while(n--){ 
                              *err = ternary(elem.i32[n] < 0, NEGATIVE_OVERFLOW, *err);
                              r.ui64[n] = elem.i32[n];
                          } 
                          break;
                  }
                  break;
              case UINT32:
                  switch(to){
                      case UINT32:  while(n--){ r.ui32[n] = elem.ui32[n]; } break;
                      case INT64:   while(n--){ r.i64[n]  = elem.ui32[n]; } break;
                      case UINT64:  while(n--){ r.i64[n]  = elem.ui32[n]; } break;
                      case FLOAT32: while(n--){ r.f32[n]  = elem.ui32[n]; } break;
                      case FLOAT64: while(n--){ r.f64[n]  = elem.ui32[n]; } break; 
                      case INT8: 
                          while(n--){
                              *err = ternary(elem.ui32[n] > MAX_INT8, POSITIVE_OVERFLOW, *err);
                              r.i8[n] = elem.ui32[n];
                          } 
                          break;
                      case UINT8:   
                          while(n--){ 
                              *err = ternary(elem.ui32[n] > MAX_UINT8, POSITIVE_OVERFLOW, *err);
                              r.ui8[n]  = elem.ui32[n]; 
                          }
                          break;
                      case INT32:   
                          while(n--){
                              *err = ternary(elem.ui32[n] > MAX_UINT32, POSITIVE_OVERFLOW, *err);
                              r.i32[n] = elem.ui32[n];
                          }
                          break;
                  }
                  break;
              case INT64:
                  switch(to){
                      case INT64:   while(n--){ r.i64[n] = elem.i64[n]; } break;
                      case FLOAT32: while(n--){ r.f32[n] = elem.i64[n]; } break;
                      case FLOAT64: while(n--){ r.f64[n] = elem.i64[n]; } break; 
                      case INT8:    
                          while(n--){
                              *err = ternary(elem.i64[n] > MAX_INT8, POSITIVE_OVERFLOW, *err);
                              *err = ternary(elem.i64[n] < MIN_INT8, NEGATIVE_OVERFLOW, *err);
                              r.i8[n] = elem.i64[n];
                          }
                          break;
                      case UINT8:   
                          while(n--){ 
                              *err = ternary(elem.i64[n] > MAX_UINT8, POSITIVE_OVERFLOW, *err);
                              *err = ternary(elem.i64[n] < 0, NEGATIVE_OVERFLOW, *err);
                              r.ui8[n] = elem.i64[n];
                          } 
                          break;
                      case INT32:   
                          while(n--){ 
                              *err = ternary(elem.i64[n] > MAX_UINT32, POSITIVE_OVERFLOW, *err);
                              *err = ternary(elem.i64[n] < MIN_INT32, NEGATIVE_OVERFLOW, *err);                        
                              r.i32[n] = elem.i64[n]; 
                          } 
                          break;
                      case UINT32:  
                          while(n--){
                              *err = ternary(elem.i64[n] > MAX_UINT32, POSITIVE_OVERFLOW, *err);
                              *err = ternary(elem.i64[n] < 0, NEGATIVE_OVERFLOW, *err);
                              r.ui32[n] = elem.i64[n];
                          } 
                          break;
                      case UINT64:  
                          while(n--){ 
                              *err = ternary(elem.i64[n] < 0, NEGATIVE_OVERFLOW, *err);
                              r.ui64[n] = elem.i64[n];
                          } 
                          break;
                  }
                  break;
              case UINT64:
                  switch(to){
                      case UINT64:  while(n--){ r.i64[n]  = elem.ui64[n]; } break;
                      case FLOAT32: while(n--){ r.f32[n]  = elem.ui64[n]; } break;
                      case FLOAT64: while(n--){ r.f64[n]  = elem.ui64[n]; } break; 
                      case INT8: 
                          while(n--){
                              *err = ternary(elem.ui64[n] > MAX_INT8, POSITIVE_OVERFLOW, *err);
                              r.i8[n] = elem.ui64[n];
                          } 
                          break;
                      case UINT8:   
                          while(n--){ 
                              *err = ternary(elem.ui64[n] > MAX_UINT8, POSITIVE_OVERFLOW, *err);
                              r.ui8[n]  = elem.ui64[n]; 
                          }
                          break;
                      case INT32:   
                          while(n--){
                              *err = ternary(elem.ui32[n] > MAX_UINT32, POSITIVE_OVERFLOW, *err);
                              r.i32[n] = elem.ui64[n];
                          }
                          break;
                      case UINT32:  
                          while(n--){ 
                              *err = ternary(elem.ui64[n] > MAX_UINT32, POSITIVE_OVERFLOW, *err);
                              r.ui32[n] = elem.ui64[n]; 
                          } 
                          break;
                      case INT64:   
                          while(n--){ 
                              *err = ternary(elem.ui64[n] > MAX_UINT64, POSITIVE_OVERFLOW, *err);
                              r.i64[n]  = elem.ui64[n]; 
                          } 
                          break;
                  }
                  break;
              case FLOAT32:
                  switch(to){
                      case INT8:    
                          while(n--){
                              *err = ternary(elem.f32[n] > MAX_INT8, POSITIVE_OVERFLOW, *err);
                              *err = ternary(elem.f32[n] < MIN_INT8, NEGATIVE_OVERFLOW, *err);
                              r.i8[n] = elem.f32[n];
                          }
                      break;
                      case UINT8:   
                          while(n--){
                              *err = ternary(elem.f32[n] > MAX_UINT8, POSITIVE_OVERFLOW, *err);
                              *err = ternary(elem.f32[n] < 0, NEGATIVE_OVERFLOW, *err);
                              r.ui8[n] = elem.f32[n];
                          }
                      break;
                      case INT32:   
                          while(n--){
                              *err = ternary(elem.f32[n] > MAX_UINT32, POSITIVE_OVERFLOW, *err);
                              *err = ternary(elem.f32[n] < MIN_INT32, NEGATIVE_OVERFLOW, *err);
                              r.i32[n] = elem.f32[n];
                          }
                      break;
                      case UINT32:  
                          while(n--){
                              *err = ternary(elem.f32[n] > MAX_UINT32, POSITIVE_OVERFLOW, *err);
                              *err = ternary(elem.f32[n] < 0, NEGATIVE_OVERFLOW, *err);
                              r.ui32[n] = elem.f32[n];
                          }
                      break;
                      case INT64:   
                          while(n--){
                              *err = ternary(elem.f32[n] > MAX_UINT64, POSITIVE_OVERFLOW, *err);
                              *err = ternary(elem.f32[n] < MIN_INT64, NEGATIVE_OVERFLOW, *err);
                              r.i64[n] = elem.f32[n];
                          }
                      break;
                      case UINT64:  
                          while(n--){
                              *err = ternary(elem.f32[n] > MAX_UINT64, POSITIVE_OVERFLOW, *err);
                              *err = ternary(elem.f32[n] < 0, NEGATIVE_OVERFLOW, *err);
                              r.ui64[n] = elem.f32[n];
                          }
                      break;
                      case FLOAT32: while(n--){ r.f32 [n] = elem.f32[n]; } break;
                      case FLOAT64: while(n--){ r.f64[n] = elem.f32[n];}
                  }
                  break;
              case FLOAT64:
                  switch(to){
                      case INT8:    
                          while(n--){
                              *err = ternary(elem.f64[n] > MAX_INT8, POSITIVE_OVERFLOW, *err);
                              *err = ternary(elem.f64[n] < MIN_INT8, NEGATIVE_OVERFLOW, *err);
                              r.i8[n] = elem.f64[n];
                          }
                      break;
                      case UINT8:   
                          while(n--){
                              *err = ternary(elem.f64[n] > MAX_UINT8, POSITIVE_OVERFLOW, *err);
                              *err = ternary(elem.f64[n] < 0, NEGATIVE_OVERFLOW, *err);
                              r.ui8[n] = elem.f64[n];
                          }
                      break;
                      case INT32:   
                          while(n--){
                              *err = ternary(elem.f64[n] > MAX_UINT32, POSITIVE_OVERFLOW, *err);
                              *err = ternary(elem.f64[n] < MIN_INT32, NEGATIVE_OVERFLOW, *err);
                              r.i32[n] = elem.f64[n];
                          }
                      break;
                      case UINT32:  
                          while(n--){
                              *err = ternary(elem.f64[n] > MAX_UINT32, POSITIVE_OVERFLOW, *err);
                              *err = ternary(elem.f64[n] < 0, NEGATIVE_OVERFLOW, *err);
                              r.ui32[n] = elem.f64[n];
                          }
                      break;
                      case INT64:   
                          while(n--){
                              *err = ternary(elem.f64[n] > MAX_UINT64, POSITIVE_OVERFLOW, *err);
                              *err = ternary(elem.f64[n] < MIN_INT64, NEGATIVE_OVERFLOW, *err);
                              r.i64[n] = elem.f64[n];
                          }
                      break;
                      case UINT64:  
                          while(n--){
                              *err = ternary(elem.f64[n] > MAX_UINT64, POSITIVE_OVERFLOW, *err);
                              *err = ternary(elem.f64[n] < 0, NEGATIVE_OVERFLOW, *err);
                              r.ui64[n] = elem.f64[n];
                          }
                      break;
                      case FLOAT32: 
                          while(n--){
                              *err = ternary(elem.f64[n] > MAX_FLOAT, POSITIVE_OVERFLOW, *err);
                              *err = ternary(elem.f64[n] < NEG_MAX_FLOAT, NEGATIVE_OVERFLOW, *err);
                              r.f32[n] = elem.f64[n];
                          }
                      break;
                      case FLOAT64: while(n--){ r.f64[n] = elem.f64[n];}
                  }
          }
      return r;
  }
  
  // your result depend on data
  vecN vec_scaler_in_place(vecN vec, datatype scale_type, fundtypeunion scale){// TODO: what if amount of elements in data is less than n
      if(!(vec.elements.i32 && vec.err && vec.type && vec.n)){// TODO: how to inform about NULL 
          if(vec.err){ vec.err[0] = NULL_POINTER; }
          return vec;    
      } 
      uint32_t i = vec.n[0];
      switch(vec.type[0]){
          case INT8:    
              switch(scale_type){
                  case INT8:    
                      while(i--){ vec.elements.i8[i] = int8_mult(vec.elements.i8[i], scale.i8, vec.err); } 
                      break; 
                  case UINT8:   
                      vec.err[0] = ternary(scale.ui8 > MAX_INT8, POSITIVE_OVERFLOW, vec.err[0]); 
                      while(i--){ vec.elements.i8[i] = int8_mult(vec.elements.i8[i], scale.ui8, vec.err); } 
                      break;
                  case INT32:   
                      vec.err[0] = ternary(scale.i32 > MAX_INT8, POSITIVE_OVERFLOW, vec.err[0]);
                      vec.err[0] = ternary(scale.i32 < MIN_INT8, NEGATIVE_OVERFLOW, vec.err[0]);
                      while(i--){ vec.elements.i8[i] = int8_mult(vec.elements.i8[i], scale.i32, vec.err); } 
                      break;
                  case UINT32:  
                      vec.err[0] = ternary(scale.ui32 > MAX_INT8, POSITIVE_OVERFLOW, vec.err[0]);
                      while(i--){ vec.elements.i8[i] = int8_mult(vec.elements.i8[i], scale.ui32, vec.err); } 
                      break;
                  case INT64:   
                      vec.err[0] = ternary(scale.i64 > MAX_INT8, POSITIVE_OVERFLOW, vec.err[0]);
                      vec.err[0] = ternary(scale.i64 < MIN_INT8, NEGATIVE_OVERFLOW, vec.err[0]);
                      while(i--){ vec.elements.i8[i] = int8_mult(vec.elements.i8[i], scale.i64, vec.err); } 
                      break;
                  case UINT64:
                      vec.err[0] = ternary(scale.ui64 > MAX_INT8, POSITIVE_OVERFLOW, vec.err[0]);
                      while(i--){ vec.elements.i8[i] = int8_mult(vec.elements.i8[i], scale.ui64, vec.err); } 
                      break;
                  case FLOAT32: 
                      vec.err[0] = ternary(scale.f32 > MAX_INT8, POSITIVE_OVERFLOW, vec.err[0]);
                      vec.err[0] = ternary(scale.f32 < MIN_INT8, NEGATIVE_OVERFLOW, vec.err[0]);
                      while(i--){ vec.elements.i8[i] = float_mult_round((fbits){ .f = vec.elements.i8[i] }, (fbits){ . f= scale.f32 }, vec.err); } 
                      break;
                  case FLOAT64:
                      vec.err[0] = ternary(scale.f64 > MAX_INT8, POSITIVE_OVERFLOW, vec.err[0]);
                      vec.err[0] = ternary(scale.f64 < MIN_INT8, NEGATIVE_OVERFLOW, vec.err[0]);
                      while(i--){ vec.elements.i8[i] = float_mult_round((fbits){ .f = vec.elements.i8[i] } , (fbits){ .f = scale.f64 }, vec.err); } 
                      break;
              }
              break;
          case UINT8:    
              switch(scale_type){
                  case INT8:    
                      vec.err[0] = ternary(scale.i8 < 0, NEGATIVE_OVERFLOW, vec.err[0]);
                      while(i--){ vec.elements.ui8[i] = uint8_mult(vec.elements.ui8[i], scale.i8, vec.err); } 
                      break; 
                  case UINT8:   
                      while(i--){ vec.elements.ui8[i] = uint8_mult(vec.elements.ui8[i], scale.ui8, vec.err); } 
                      break;
                  case INT32:   
                      vec.err[0] = ternary(scale.i32 > MAX_UINT8, POSITIVE_OVERFLOW, vec.err[0]);
                      vec.err[0] = ternary(scale.i32 < 0, NEGATIVE_OVERFLOW, vec.err[0]);
                      while(i--){ vec.elements.ui8[i] = uint8_mult(vec.elements.ui8[i], scale.i32, vec.err); } 
                      break;
                  case UINT32:  
                      vec.err[0] = ternary(scale.ui32 > MAX_UINT8, POSITIVE_OVERFLOW, vec.err[0]);
                      while(i--){ vec.elements.ui8[i] = uint8_mult(vec.elements.ui8[i], scale.ui32, vec.err); } 
                      break;
                  case INT64:   
                      vec.err[0] = ternary(scale.i64 > MAX_UINT8, POSITIVE_OVERFLOW, vec.err[0]);
                      vec.err[0] = ternary(scale.i64 < 0, NEGATIVE_OVERFLOW, vec.err[0]);
                      while(i--){ vec.elements.ui8[i] = uint8_mult(vec.elements.ui8[i], scale.i64, vec.err); } 
                      break;
                  case UINT64:
                      vec.err[0] = ternary(scale.ui64 > MAX_UINT8, POSITIVE_OVERFLOW, vec.err[0]);
                      while(i--){ vec.elements.ui8[i] = uint8_mult(vec.elements.ui8[i], scale.ui64, vec.err); } 
                      break;
                  case FLOAT32: 
                      vec.err[0] = ternary(scale.f32 > MAX_UINT8, POSITIVE_OVERFLOW, vec.err[0]);
                      vec.err[0] = ternary(scale.f32 < 0, NEGATIVE_OVERFLOW, vec.err[0]);
                      while(i--){ vec.elements.ui8[i] = float_mult_round((fbits){ .f = vec.elements.ui8[i] }, (fbits){ . f= scale.f32 }, vec.err); }  
                      break;
                  case FLOAT64:
                      vec.err[0] = ternary(scale.f64 > MAX_UINT8, POSITIVE_OVERFLOW, vec.err[0]);
                      vec.err[0] = ternary(scale.f64 < 0, NEGATIVE_OVERFLOW, vec.err[0]);
                      while(i--){ vec.elements.ui8[i] = float_mult_round((fbits){ .f = vec.elements.ui8[i] }, (fbits){ . f= scale.f64 }, vec.err); }
                      break;
              }
              break;
          case INT32: 
              switch(scale_type){
                  case INT8:    
                      while(i--){ vec.elements.i32[i] = int32_mult(vec.elements.i32[i], scale.i8, vec.err); } 
                      break; 
                  case UINT8:   
                      while(i--){ vec.elements.i32[i] = int32_mult(vec.elements.i32[i], scale.ui32, vec.err); } 
                      break;
                  case INT32:   
                      while(i--){ vec.elements.i32[i] = int32_mult(vec.elements.i32[i], scale.i32, vec.err); } 
                      break;
                  case UINT32:  
                      vec.err[0] = ternary(scale.ui32 > MAX_INT8, POSITIVE_OVERFLOW, vec.err[0]);
                      while(i--){ vec.elements.i32[i] = int32_mult(vec.elements.i32[i], scale.ui32, vec.err); } 
                      break;
                  case INT64:   
                      vec.err[0] = ternary(scale.i64 > MAX_UINT32, POSITIVE_OVERFLOW, vec.err[0]);
                      vec.err[0] = ternary(scale.i64 < MIN_INT32, NEGATIVE_OVERFLOW, vec.err[0]);
                      while(i--){ vec.elements.i32[i] = int32_mult(vec.elements.i32[i], scale.i64, vec.err); } 
                      break;
                  case UINT64:
                      vec.err[0] = ternary(scale.ui64 > MAX_UINT32, POSITIVE_OVERFLOW, vec.err[0]);
                      while(i--){ vec.elements.i32[i] = int32_mult(vec.elements.i32[i], scale.ui64, vec.err); } 
                      break;
                  case FLOAT32: 
                      vec.err[0] = ternary(scale.f32 > MAX_UINT32, POSITIVE_OVERFLOW, vec.err[0]);
                      vec.err[0] = ternary(scale.f32 < MIN_INT32, NEGATIVE_OVERFLOW, vec.err[0]);
                      while(i--){ vec.elements.i32[i] = float_mult_round((fbits){ .f = vec.elements.i32[i] }, (fbits){ . f= scale.f32 }, vec.err); } 
                      break;
                  case FLOAT64:
                      vec.err[0] = ternary(scale.f64 > MAX_UINT32, POSITIVE_OVERFLOW, vec.err[0]);
                      vec.err[0] = ternary(scale.f64 < MIN_INT32, NEGATIVE_OVERFLOW, vec.err[0]);
                      while(i--){ vec.elements.i32[i] = float_mult_round((fbits){ .f = vec.elements.i32[i] }, (fbits){ . f= scale.f64 }, vec.err); } 
                      break;
              }
              break;
          case UINT32: 
              switch(scale_type){
                  case INT8:    
                      vec.err[0] = ternary(scale.i8 < 0, NEGATIVE_OVERFLOW, vec.err[0]);
                      while(i--){ vec.elements.ui32[i] = uint32_mult(vec.elements.ui32[i], scale.i8, vec.err); } 
                      break; 
                  case UINT8:   
                      while(i--){ vec.elements.ui32[i] = uint32_mult(vec.elements.ui32[i], scale.ui8, vec.err); } 
                      break;
                  case INT32:   
                      vec.err[0] = ternary(scale.i32 < 0, NEGATIVE_OVERFLOW, vec.err[0]);
                      while(i--){ vec.elements.ui32[i] = uint32_mult(vec.elements.ui32[i], scale.i32, vec.err); } 
                      break;
                  case UINT32:  
                      while(i--){ vec.elements.ui32[i] = uint32_mult(vec.elements.ui32[i], scale.ui32, vec.err); } 
                      break;
                  case INT64:   
                      vec.err[0] = ternary(scale.i64 > MAX_UINT32, POSITIVE_OVERFLOW, vec.err[0]);
                      vec.err[0] = ternary(scale.i64 < 0, NEGATIVE_OVERFLOW, vec.err[0]);
                      while(i--){ vec.elements.ui32[i] = uint32_mult(vec.elements.ui32[i], scale.i64, vec.err); } 
                      break;
                  case UINT64:
                      vec.err[0] = ternary(scale.ui64 > MAX_UINT32, POSITIVE_OVERFLOW, vec.err[0]);
                      while(i--){ vec.elements.ui32[i] = uint32_mult(vec.elements.ui32[i], scale.ui64, vec.err); } 
                      break;
                  case FLOAT32: 
                      vec.err[0] = ternary(scale.f32 > MAX_UINT32, POSITIVE_OVERFLOW, vec.err[0]);
                      vec.err[0] = ternary(scale.f32 < 0, NEGATIVE_OVERFLOW, vec.err[0]);
                      while(i--){ vec.elements.ui32[i] = float_mult_round((fbits){ .f = vec.elements.ui32[i] }, (fbits){ . f= scale.f32 }, vec.err); } 
                      break;
                  case FLOAT64:
                      vec.err[0] = ternary(scale.f64 > MAX_UINT32, POSITIVE_OVERFLOW, vec.err[0]);
                      vec.err[0] = ternary(scale.f64 < 0, NEGATIVE_OVERFLOW, vec.err[0]);
                      while(i--){ vec.elements.ui32[i] = float_mult_round((fbits){ .f = vec.elements.ui32[i] }, (fbits){ . f= scale.f64 }, vec.err); } 
                      break;
              }
          case INT64:   
              switch(scale_type){
                  case INT8:    
                      while(i--){ vec.elements.i64[i] = int64_mult(vec.elements.i64[i], scale.i8, vec.err); } 
                      break; 
                  case UINT8:   
                      while(i--){ vec.elements.i64[i] = int64_mult(vec.elements.i64[i], scale.ui32, vec.err); } 
                      break;
                  case INT32:   
                      while(i--){ vec.elements.i64[i] = int64_mult(vec.elements.i64[i], scale.i32, vec.err); } 
                      break;
                  case UINT32:  
                      while(i--){ vec.elements.i64[i] = int64_mult(vec.elements.i64[i], scale.ui32, vec.err); } 
                      break;
                  case INT64:   
                      while(i--){ vec.elements.i64[i] = int64_mult(vec.elements.i64[i], scale.i64, vec.err); } 
                      break;
                  case UINT64:
                      vec.err[0] = ternary(scale.ui64 > MAX_UINT64, POSITIVE_OVERFLOW, vec.err[0]);
                      while(i--){ vec.elements.i64[i] = int64_mult(vec.elements.i64[i], scale.ui64, vec.err); } 
                      break;
                  case FLOAT32: 
                      vec.err[0] = ternary(scale.f32 > MAX_UINT64, POSITIVE_OVERFLOW, vec.err[0]);
                      vec.err[0] = ternary(scale.f32 < MIN_INT64, NEGATIVE_OVERFLOW, vec.err[0]);
                      while(i--){ vec.elements.i64[i] = float_mult_round((fbits){ .f = vec.elements.i64[i] }, (fbits){ . f= scale.f32 }, vec.err); } 
                      break;
                  case FLOAT64:
                      vec.err[0] = ternary(scale.f64 > MAX_UINT64, POSITIVE_OVERFLOW, vec.err[0]);
                      vec.err[0] = ternary(scale.f64 < MIN_INT64, NEGATIVE_OVERFLOW, vec.err[0]);
                      while(i--){ vec.elements.i64[i] = float_mult_round((fbits){ .f = vec.elements.i64[i] }, (fbits){ . f= scale.f64 }, vec.err); }
                      break;
              }
          case UINT64:
              switch(scale_type){
                  case INT8:    
                      vec.err[0] = ternary(scale.i8 < 0, NEGATIVE_OVERFLOW, vec.err[0]);
                      while(i--){ vec.elements.ui64[i] = uint64_mult(vec.elements.ui64[i], scale.i8, vec.err); } 
                      break; 
                  case UINT8:   
                      while(i--){ vec.elements.ui64[i] = uint64_mult(vec.elements.ui64[i], scale.ui8, vec.err); } 
                      break;
                  case INT32:   
                      vec.err[0] = ternary(scale.i32 < 0, NEGATIVE_OVERFLOW, vec.err[0]);
                      while(i--){ vec.elements.ui64[i] = uint64_mult(vec.elements.ui64[i], scale.i32, vec.err); } 
                      break;
                  case UINT32:  
                      while(i--){ vec.elements.ui64[i] = uint64_mult(vec.elements.ui64[i], scale.ui32, vec.err); } 
                      break;
                  case INT64:   
                      vec.err[0] = ternary(scale.i64 < 0, NEGATIVE_OVERFLOW, vec.err[0]);
                      while(i--){ vec.elements.ui64[i] = uint64_mult(vec.elements.ui64[i], scale.i64, vec.err); } 
                      break;
                  case UINT64:
                      while(i--){ vec.elements.ui64[i] = uint64_mult(vec.elements.ui64[i], scale.ui64, vec.err); } 
                      break;
                  case FLOAT32: 
                      vec.err[0] = ternary(scale.f32 > MAX_UINT64, POSITIVE_OVERFLOW, vec.err[0]);
                      vec.err[0] = ternary(scale.f32 < 0, NEGATIVE_OVERFLOW, vec.err[0]);
                      while(i--){ vec.elements.ui64[i] = float_mult_round((fbits){ .f = vec.elements.ui64[i] }, (fbits){ . f= scale.f32 }, vec.err); } 
                      break;
                  case FLOAT64:
                      vec.err[0] = ternary(scale.f64 > MAX_UINT64, POSITIVE_OVERFLOW, vec.err[0]);
                      vec.err[0] = ternary(scale.f64 < 0, NEGATIVE_OVERFLOW, vec.err[0]);
                      while(i--){ vec.elements.ui64[i] = float_mult_round((fbits){ .f = vec.elements.ui64[i] }, (fbits){ . f= scale.f64 }, vec.err); }
                      break;
              }
          case FLOAT32:
              switch(scale_type){
                  case INT8:    while(i--){ vec.elements.f32[i] = float_mult_round((fbits){ .f = vec.elements.f32[i] }, (fbits){ .f = scale.i8   }, vec.err); } break; 
                  case UINT8:   while(i--){ vec.elements.f32[i] = float_mult_round((fbits){ .f = vec.elements.f32[i] }, (fbits){ .f = scale.ui32 }, vec.err); } break;
                  case INT32:   while(i--){ vec.elements.f32[i] = float_mult_round((fbits){ .f = vec.elements.f32[i] }, (fbits){ .f = scale.i32  }, vec.err); } break;
                  case UINT32:  while(i--){ vec.elements.f32[i] = float_mult_round((fbits){ .f = vec.elements.f32[i] }, (fbits){ .f = scale.ui32 }, vec.err); } break;
                  case INT64:   while(i--){ vec.elements.f32[i] = float_mult_round((fbits){ .f = vec.elements.f32[i] }, (fbits){ .f = scale.i64  }, vec.err); } break;
                  case UINT64:  while(i--){ vec.elements.f32[i] = float_mult_round((fbits){ .f = vec.elements.f32[i] }, (fbits){ .f = scale.ui64 }, vec.err); } break;
                  case FLOAT32: while(i--){ vec.elements.f32[i] = float_mult_round((fbits){ .f = vec.elements.f32[i] }, (fbits){ .f = scale.f32  }, vec.err); } break;
                  case FLOAT64:
                      vec.err[0] = ternary(scale.f64 > MAX_UINT64, POSITIVE_OVERFLOW, vec.err[0]);
                      vec.err[0] = ternary(scale.f64 < MIN_INT64, NEGATIVE_OVERFLOW, vec.err[0]);
                      while(i--){ vec.elements.f32[i] = float_mult_round((fbits){ .f = vec.elements.f32[i] } , (fbits){ .f = scale.f64 }, vec.err); } 
                      break;
              }
          case FLOAT64:
              switch(scale_type){
                  case INT8:    while(i--){ vec.elements.f64[i] = double_mult_round((dbits){ .d = vec.elements.f64[i] }, (dbits){ .d = scale.i8   }, vec.err); } break; 
                  case UINT8:   while(i--){ vec.elements.f64[i] = double_mult_round((dbits){ .d = vec.elements.f64[i] }, (dbits){ .d = scale.ui32 }, vec.err); } break;
                  case INT32:   while(i--){ vec.elements.f64[i] = double_mult_round((dbits){ .d = vec.elements.f64[i] }, (dbits){ .d = scale.i32  }, vec.err); } break;
                  case UINT32:  while(i--){ vec.elements.f64[i] = double_mult_round((dbits){ .d = vec.elements.f64[i] }, (dbits){ .d = scale.ui32 }, vec.err); } break;
                  case INT64:   while(i--){ vec.elements.f64[i] = double_mult_round((dbits){ .d = vec.elements.f64[i] }, (dbits){ .d = scale.i64  }, vec.err); } break;
                  case UINT64:  while(i--){ vec.elements.f64[i] = double_mult_round((dbits){ .d = vec.elements.f64[i] }, (dbits){ .d = scale.ui64 }, vec.err); } break;
                  case FLOAT32: while(i--){ vec.elements.f64[i] = double_mult_round((dbits){ .d = vec.elements.f64[i] }, (dbits){ .d = scale.f32  }, vec.err); } break;
                  case FLOAT64: while(i--){ vec.elements.f64[i] = double_mult_round((dbits){ .d = vec.elements.f64[i] }, (dbits){ .d = scale.f64  }, vec.err); } break;
              }
      }
      return vec;
  }
  
  
  vecN vec_neg(vecN vec){// TODO: what if amount of elements in data is less than n
      vecN r;
      if(!(vec.type  && vec.elements.i32 && vec.err && vec.n)){ 
          error er = NULL_POINTER;
          vec.err = (error*)(long unsigned int*)ternary((long unsigned int)(long unsigned int*)vec.err, (long unsigned int)(long unsigned int*)vec.err, (long unsigned int)(long unsigned int*)&er);
          return vec; 
      }
      uint32_t i = vec.n[0];
      r = vec_create( ternary(UINT8 == vec.type[0], INT32, ternary(UINT32 == vec.type[0], INT64, vec.type[0])), vec.n[0]);
      switch(r.type[0]){
          case INT8:    while(i--){ r.elements.i8 [i] = -vec.elements.i8 [i]; } break;
          case INT32:   while(i--){ r.elements.i32[i] = -vec.elements.i32[i]; } break;
          case INT64:   while(i--){ r.elements.i64[i] = -vec.elements.i64[i]; } break;
          case FLOAT32: while(i--){ r.elements.f32[i] = -vec.elements.f32[i]; } break;
          case FLOAT64: while(i--){ r.elements.f64[i] = -vec.elements.f64[i]; } break;
          case UINT8:
              r.type[0] = UINT32;   
              while(i--){ r.elements.i32[i] = -vec.elements.ui8[i]; }// C standard says the unary - promotes unsigned integer types 
              break;
          case UINT32:  
              r.type[0] = UINT64; 
              while(i--){ r.elements.i64[i] = -vec.elements.ui32[i]; } 
              break;            
          case UINT64:
              uint64_t max_negateable_uint64 = MAX_UINT32 + 1;
              r.type[0] = UINT64;
              while(i--){ 
                  r.err[0] = ternary(vec.elements.ui64[i] > max_negateable_uint64, NEGATIVE_OVERFLOW, r.err[0]);
                  r.elements.i64[i] = -vec.elements.ui64[i]; 
              }   
              break;
      }
      return r;
  }
  
  
  vecN vec_neg_in_place(vecN vec){// TODO: what if amount of elements in data is less than n
      if(!(vec.elements.i32 && vec.err && vec.type && vec.n)){ 
          error er = NULL_POINTER;
          vec.err = (error*)(long unsigned int*)ternary((long unsigned int)(long unsigned int*)vec.err, (long unsigned int)(long unsigned int*)vec.err, (long unsigned int)(long unsigned int*)&er);
          return vec; 
      }
      uint32_t i = vec.n[0];
      switch (vec.type[0]){
          case INT8:    
              while(i--){ 
                  r.err[0] = ternary(vec.element.i8[i] == MIN_INT8, POSITIVE_OVERFLOW, r.err[0]);
                  vec.elements.i8 [i] = -vec.elements.i8 [i]; 
              }
          break;
          case INT32:   
              while(i--){ 
                  r.err[0] = ternary(vec.element.i32[i] == MIN_INT32, POSITIVE_OVERFLOW, r.err[0]);
                  vec.elements.i32[i] = -vec.elements.i32[i]; 
                  } 
          break;
          case INT64:   
              while(i--){ 
                  r.err[0] = ternary(vec.element.i64[i] == MIN_INT64, POSITIVE_OVERFLOW, r.err[0]);
                  vec.elements.i64[i] = -vec.elements.i64[i]; 
                  } 
          break;
          case FLOAT32: while(i--){ vec.elements.f32[i] = -vec.elements.f32[i]; } break;
          case FLOAT64: while(i--){ vec.elements.f64[i] = -vec.elements.f64[i]; } break;
          case UINT8:   
              uint8_t max_negateable_uint8 = MAX_INT8 + 1;
              vec.type[0] = INT8;
              while(i--){ 
                  vec.err[0] = ternary(vec.elements.ui8[i] > max_negateable_uint8, NEGATIVE_OVERFLOW, vec.err[0]);
                  vec.elements.i8[i] = -vec.elements.ui8[i]; 
              }
              break;
          case UINT32:  
              uint32_t max_negateable_uint32 = MAX_INT32 + 1;
              vec.type[0] = INT32; 
              while(i--){
                  vec.err[0] = ternary(vec.elements.ui32[i] > max_negateable_uint32, NEGATIVE_OVERFLOW, vec.err[0]);
                  vec.elements.i32[i] = -vec.elements.ui32[i]; 
              }
              break;
          case UINT64:
              uint64_t max_negateable_uint64 = MAX_INT64 + 1;
              vec.type[0] = INT64;
              while(i--){
                  vec.err[0] = ternary(vec.elements.ui64[i] > max_negateable_uint64, NEGATIVE_OVERFLOW, vec.err[0]);
                  vec.elements.i64[i] = -vec.elements.ui64[i];
              }
              break;
          }
      return vec;
  }
  
  
  vecN vec_add_first_arg_type(vecN a, vecN b){// TODO: what if amount of elements in data is less than n
      if(!(a.type && a.n && a.err && a.elements.i32 && b.type && b.n && b.err && b.elements.i32)){ 
          if(a.err){ a.err[0] = NULL_POINTER; }
          return a;
      }
      vecN r = vec_create(a.type[0], ternary(a.n[0] > b.n[0], a.n[0], b.n[0]));
      uint32_t i = r.n[0], al, bl, an = a.n[0], bn = b.n[0];
      fundtypeunion val;
      switch(a.type[0]){
          case INT8:
              while(i--){
                  al = i < an; // TODO: do something with the name. At least remove lt_aN
                  bl = i < bn; // lt_bN
                  switch(b.type[0]){
                      case INT8:    r.elements.i8[i] = int8_add(a.elements.i8[i % an] * al, b.elements.i8[i % bn] * bl, r.err);  break;
                      case UINT8:   
                          val.i32 = int32_add(a.elements.i8[i % an] * al, b.elements.ui8[i % bn] * bl, r.err);
                          r.err[0] = ternary(val.i32 > MAX_INT8, POSITIVE_OVERFLOW, r.err[0]);
                          r.elements.i8[i] = val.i32 * !(r.err[0]);
                      break;
                      case INT32:  
                          val.i32 = int32_add(a.elements.i8[i % an] * al, b.elements.i32[i % bn] * bl, r.err);
                          r.err[0] = ternary(val.i32 > MAX_INT8, POSITIVE_OVERFLOW, r.err[0]);
                          r.err[0] = ternary(val.i32 < MIN_INT8, NEGATIVE_OVERFLOW, r.err[0]);
                          r.elements.i8[i] = val.i32 * !(r.err[0]);
                      break;
                      case UINT32:  
                          val.i64 = int64_add(a.elements.i8[i % an] * al, b.elements.ui32[i % bn] * bl, r.err);
                          r.err[0] = ternary(val.i64 > MAX_INT8, POSITIVE_OVERFLOW, r.err[0]);
                          r.elements.i8[i] = val.i64 * !(r.err[0]);
                      break;
                      case INT64:   
                          val.i64 = int64_add(a.elements.i8[i % an] * al, b.elements.i64[i % bn] * bl, r.err);
                          r.err[0] = ternary(val.i64 > MAX_INT8, POSITIVE_OVERFLOW, r.err[0]);
                          r.err[0] = ternary(val.i64 < MIN_INT8, NEGATIVE_OVERFLOW, r.err[0]);
                          r.elements.i8[i] = val.i64 * !(r.err[0]);
                      break;
                      case UINT64:  
                          val.i64 = int64_uint64_add(a.elements.i8[i % an] * al, b.elements.ui64[i % bn] * bl, r.err); 
                          r.err[0] = ternary(val.i64 > MAX_INT8, POSITIVE_OVERFLOW, r.err[0]);
                          r.elements.i8[i] = val.i64 * !(r.err[0]);                    
                      break;
                      case FLOAT32: 
                          val.i64 = int64_double_add(a.elements.i8[i % an] * al, (dblts){ .d = b.elements.f32[i % bn] * bl }, r.err);
                          r.err[0] = ternary(val.i64 > MAX_INT8, POSITIVE_OVERFLOW, r.err[0]);
                          r.err[0] = ternary(val.i64 < MIN_INT8, NEGATIVE_OVERFLOW, r.err[0]);
                          r.elements.i8[i] = val.i64 * !(r.err[0]);
                      break;
                      case FLOAT64: 
                          val.i64 = int64_double_add(a.elements.i8[i % an] * al, (dblts){ .d = b.elements.f64[i % bn] * bl }, r.err);
                          r.err[0] = ternary(val.i64 > MAX_INT8, POSITIVE_OVERFLOW, r.err[0]);
                          r.err[0] = ternary(val.i64 < MIN_INT8, NEGATIVE_OVERFLOW, r.err[0]);
                          r.elements.i8[i] = val.i64 * !(r.err[0]);                    
                  }        
              }
              break;
          case UINT8:   
              while(i--){
                  al = i < an;
                  bl = i < bn;
                  switch(b.type[0]){
                      case UINT8:  r.elements.ui8[i] = uint8_add(a.elements.ui8[i % an] * al, b.elements.ui8[i % bn] * bl, r.err); break;
                      case INT8:  
                          val.i32  = int32_add(a.elements.ui8[i % an] * al, b.elements.i8[i % bn] * bl, r.err);  
                          r.err[0] = ternary(val.i32 > MAX_UINT8, POSITIVE_OVERFLOW, r.err[0]);
                          r.err[0] = ternary(val.i32 < 0, NEGATIVE_OVERFLOW, r.err[0]);
                          r.elements.ui8[i] = val.i32 * !(r.err[0]);
                      break;
                      case INT32:  
                          val.i32  = int32_add(a.elements.ui8[i % an] * al, b.elements.i32[i % bn] * bl, r.err);
                          r.err[0] = ternary(val.i32 > MAX_UINT8, POSITIVE_OVERFLOW, r.err[0]);
                          r.err[0] = ternary(val.i32 < 0, NEGATIVE_OVERFLOW, r.err[0]);
                          r.elements.ui8[i] = val.i32 * !(r.err[0]);
                      break;
                      case UINT32: 
                          val.ui32 = uint32_add(a.elements.ui8[i % an] * al, b.elements.ui32[i % bn] * bl, r.err);
                          r.err[0] = ternary(val.ui32 > MAX_UINT8, POSITIVE_OVERFLOW, r.err[0]);
                          r.elements.ui8[i] = val.ui32 * !(r.err[0]);
                      break;
                      case INT64:   
                          val.i64  = int64_add(a.elements.ui8[i % an] * al, b.elements.i64[i % bn] * bl, r.err);
                          r.err[0] = ternary(val.i64 > MAX_UINT8, POSITIVE_OVERFLOW, r.err[0]);
                          r.err[0] = ternary(val.i64 < 0, NEGATIVE_OVERFLOW, r.err[0]);
                          r.elements.ui8[i] = val.i64 * !(r.err[0]);
                      break;
                      case UINT64:  
                          val.ui64  = uint64_add(a.elements.ui8[i % an] * al, b.elements.ui64[i % bn] * bl, r.err); 
                          r.err[0] = ternary(val.ui64 > MAX_UINT8, POSITIVE_OVERFLOW, r.err[0]);
                          r.elements.ui8[i] = val.ui64 * !(r.err[0]);                    
                      break;
                      case FLOAT32: 
                          val.ui64  = uint64_double_add(a.elements.ui8[i % an] * al, (dblts){ .d = b.elements.f32[i % bn] * bl }, r.err);
                          r.err[0] = ternary(val.ui64 > MAX_UINT8, POSITIVE_OVERFLOW, r.err[0]);
                          r.elements.ui8[i] = val.ui64 * !(r.err[0]);
                      break;
                      case FLOAT64: 
                          val.ui64  = uint64_double_add(a.elements.ui8[i % an] * al, (dblts){ .d = b.elements.f64[i % bn] * bl }, r.err);
                          r.err[0] = ternary(val.ui64 > MAX_UINT8, POSITIVE_OVERFLOW, r.err[0]);
                          r.elements.ui8[i] = val.ui64 * !(r.err[0]);                    
                  }        
              }
              break;
          case INT32:   
              while(i--){
                  al = i < an;
                  bl = i < bn;
                  switch(b.type[0]){
                      case INT8:    r.elements.i32[i] = int32_add(a.elements.i32[i % an] * al, b.elements.i8[i % bn] * bl, r.err);  break;
                      case UINT8:   r.elements.i32[i] = int32_add(a.elements.i32[i % an] * al, b.elements.ui8[i % bn] * bl, r.err);  break;
                      case INT32:   r.elements.i32[i] = int32_add(a.elements.i32[i % an] * al, b.elements.i32[i % bn] * bl, r.err);  break;
                      case UINT32:  
                          val.i64  = int64_add(a.elements.i32[i % an] * al, b.elements.ui32[i % bn] * bl, r.err);
                          r.err[0] = ternary(val.i64 > MAX_INT32, POSITIVE_OVERFLOW, r.err[0]);
                          r.elements.i32[i] = val.i64 * !(r.err[0]);
                      break;
                      case INT64:   
                          val.i64  = int64_add(a.elements.i32[i % an] * al, b.elements.i64[i % bn] * bl, r.err);
                          r.err[0] = ternary(val.i64 > MAX_INT32, POSITIVE_OVERFLOW, r.err[0]);
                          r.err[0] = ternary(val.i64 < MIN_INT32, NEGATIVE_OVERFLOW, r.err[0]);
                          r.elements.i32[i] = val.i64 * !(r.err[0]);
                      break;
                      case UINT64:  
                          val.i64  = int64_uint64_add(a.elements.i32[i % an] * al, b.elements.ui64[i % bn] * bl, r.err); 
                          r.err[0] = ternary(val.i64 > MAX_INT32, POSITIVE_OVERFLOW, r.err[0]);
                          r.elements.i32[i] = val.i64 * !(r.err[0]);                    
                      break;
                      case FLOAT32: 
                          val.i64  = int64_double_add(a.elements.i32[i % an] * al, (dblts){ .d = b.elements.f32[i % bn] * bl }, r.err);
                          r.err[0] = ternary(val.i64 > MAX_INT32, POSITIVE_OVERFLOW, r.err[0]);
                          r.err[0] = ternary(val.i64 < MIN_INT32, NEGATIVE_OVERFLOW, r.err[0]);
                          r.elements.i32[i] = val.i64 * !(r.err[0]);
                      break;
                      case FLOAT64: 
                          val.i64  = int64_double_add(a.elements.i32[i % an] * al, (dblts){ .d = b.elements.f64[i % bn] * bl }, r.err);
                          r.err[0] = ternary(val.i64 > MAX_INT32, POSITIVE_OVERFLOW, r.err[0]);
                          r.err[0] = ternary(val.i64 < MIN_INT32, NEGATIVE_OVERFLOW, r.err[0]);
                          r.elements.i32[i] = val.i64 * !(r.err[0]);                    
                  }        
              }
              break;
          case UINT32:  
          while(i--){
              al = i < an;
              bl = i < bn;
              switch(b.type[0]){
                  case UINT8:   r.elements.ui32[i] = uint8_add (a.elements.ui32[i % an] * al, b.elements.ui8[i % bn] * bl, r.err); break;
                  case UINT32:  r.elements.ui32[i] = uint32_add(a.elements.ui32[i % an] * al, b.elements.ui32[i % bn] * bl, r.err); break;
                  case INT8:  
                      val.i64  = int64_add(a.elements.ui32[i % an] * al, b.elements.i8[i % bn] * bl, r.err);  
                      r.err[0] = ternary(val.i64 > MAX_UINT32, POSITIVE_OVERFLOW, r.err[0]);
                      r.err[0] = ternary(val.i64 < 0, NEGATIVE_OVERFLOW, r.err[0]);
                      r.elements.ui32[i] = val.i64 * !(r.err[0]);
                  break;
                  case INT32:  
                      val.i64  = int64_add(a.elements.ui32[i % an] * al, b.elements.i32[i % bn] * bl, r.err);  
                      r.err[0] = ternary(val.i64 > MAX_UINT32, POSITIVE_OVERFLOW, r.err[0]);
                      r.err[0] = ternary(val.i64 < 0, NEGATIVE_OVERFLOW, r.err[0]);
                      r.elements.ui32[i] = val.i64 * !(r.err[0]);
                  break;
                  case INT64:   
                      val.i64  = int64_add(a.elements.ui32[i % an] * al, b.elements.i64[i % bn] * bl, r.err);
                      r.err[0] = ternary(val.i64 > MAX_UINT32, POSITIVE_OVERFLOW, r.err[0]);
                      r.err[0] = ternary(val.i64 < 0, NEGATIVE_OVERFLOW, r.err[0]);
                      r.elements.ui32[i] = val.i64 * !(r.err[0]);
                  break;
                  case UINT64:  
                      val.ui64  = int64_add(a.elements.ui32[i % an] * al, b.elements.ui64[i % bn] * bl, r.err); 
                      r.err[0] = ternary(val.ui64 > MAX_UINT32, POSITIVE_OVERFLOW, r.err[0]);
                      r.elements.ui32[i] = val.ui64 * !(r.err[0]);                    
                  break;
                  case FLOAT32: 
                      val.ui64  = uint64_double_add(a.elements.ui32[i % an] * al, (dblts){ .d = b.elements.f32[i % bn] * bl }, r.err);
                      r.err[0] = ternary(val.ui64 > MAX_UINT32, POSITIVE_OVERFLOW, r.err[0]);
                      r.elements.ui32[i] = val.ui64 * !(r.err[0]);
                  break;
                  case FLOAT64: 
                      val.ui64  = uint64_double_add(a.elements.ui32[i % an] * al, (dblts){ .d = b.elements.f64[i % bn] * bl }, r.err);
                      r.err[0] = ternary(val.ui64 > MAX_UINT32, POSITIVE_OVERFLOW, r.err[0]);
                      r.elements.ui32[i] = val.ui64 * !(r.err[0]);                    
              }        
          }
              break;
          case INT64:   
              while(i--){
                  al = i < an;
                  bl = i < bn;
                  switch(b.type[0]){
                      case INT8:    r.elements.i64[i] = int64_add(a.elements.i64[i % an] * al, b.elements.i8  [i % bn] * bl, r.err); break;
                      case UINT8:   r.elements.i64[i] = int64_add(a.elements.i64[i % an] * al, b.elements.ui8 [i % bn] * bl, r.err); break;
                      case INT32:   r.elements.i64[i] = int64_add(a.elements.i64[i % an] * al, b.elements.i32 [i % bn] * bl, r.err); break;
                      case UINT32:  r.elements.i64[i] = int64_add(a.elements.i64[i % an] * al, b.elements.ui32[i % bn] * bl, r.err); break;
                      case INT64:   r.elements.i64[i] = int64_add(a.elements.i64[i % an] * al, b.elements.i64 [i % bn] * bl, r.err); break;
                      case UINT64:  r.elements.i64[i] = int64_uint64_add(a.elements.i64[i % an] * al, b.elements.ui64[i % bn] * bl, r.err); break;
                      case FLOAT32: r.elements.i64[i] = int64_double_add(a.elements.i64[i % an] * al, (dblts){ .d = b.elements.f32[i % bn] * bl }, r.err); break;
                      case FLOAT64: r.elements.i64[i] = int64_double_add(a.elements.i64[i % an] * al, (dblts){ .d = b.elements.f64[i % bn] * bl }, r.err);
                  }        
              }
              break;
          case UINT64:  
              while(i--){
                  al = i < an;
                  bl = i < bn;
                  switch(b.type[0]){
                      case UINT8:   r.elements.ui64[i] = uint64_add(a.elements.ui64[i % an] * al, b.elements.ui8 [i % bn] * bl, r.err); break;
                      case UINT32:  r.elements.ui64[i] = uint64_add(a.elements.ui64[i % an] * al, b.elements.ui32[i % bn] * bl, r.err); break;
                      case UINT64:  r.elements.ui64[i] = uint64_add(a.elements.ui64[i % an] * al, b.elements.ui64[i % bn] * bl, r.err); break;
                      case INT8:    r.elements.ui64[i] = uint64_int64_add(a.elements.ui64[i % an] * al, b.elements.i8 [i % bn] * bl, r.err); break;
                      case INT32:   r.elements.ui64[i] = uint64_int64_add(a.elements.ui64[i % an] * al, b.elements.i32[i % bn] * bl, r.err); break;
                      case INT64:   r.elements.ui64[i] = uint64_int64_add(a.elements.ui64[i % an] * al, b.elements.i64[i % bn] * bl, r.err); break;
                      case FLOAT32: r.elements.ui64[i] = uint64_double_add(a.elements.ui64[i % an] * al, (dblts){ .d = b.elements.f32[i % bn] * bl }, r.err); break;
                      case FLOAT64: r.elements.ui64[i] = uint64_double_add(a.elements.ui64[i % an] * al, (dblts){ .d = b.elements.f64[i % bn] * bl }, r.err); 
                  }
              }
              break;
          case FLOAT32: 
              while(i--){
                  al = i < an;
                  bl = i < bn;
                  switch(b.type[0]){
                      case INT8:    r.elements.f32[i] = float_add((fbits){ .f = a.elements.f32[i % an] * al }, (fbits){ .f = b.elements.i8  [i % bn] * bl }, r.err); break;
                      case UINT8:   r.elements.f32[i] = float_add((fbits){ .f = a.elements.f32[i % an] * al }, (fbits){ .f = b.elements.ui8 [i % bn] * bl }, r.err); break;
                      case INT32:   r.elements.f32[i] = float_add((fbits){ .f = a.elements.f32[i % an] * al }, (fbits){ .f = b.elements.i32 [i % bn] * bl }, r.err); break;
                      case UINT32:  r.elements.f32[i] = float_add((fbits){ .f = a.elements.f32[i % an] * al }, (fbits){ .f = b.elements.ui32[i % bn] * bl }, r.err); break;
                      case INT64:   r.elements.f32[i] = float_add((fbits){ .f = a.elements.f32[i % an] * al }, (fbits){ .f = b.elements.i64 [i % bn] * bl }, r.err); break;
                      case UINT64:  r.elements.f32[i] = float_add((fbits){ .f = a.elements.f32[i % an] * al }, (fbits){ .f = b.elements.ui64[i % bn] * bl }, r.err); break;
                      case FLOAT32: r.elements.f32[i] = float_add((fbits){ .f = a.elements.f32[i % an] * al }, (fbits){ .f = b.elements.f32 [i % bn] * bl }, r.err); break;
                      case FLOAT64: r.elements.f32[i] = float_add((fbits){ .f = a.elements.f32[i % an] * al }, (fbits){ .f = b.elements.f64 [i % bn] * bl }, r.err); 
                  }
              }
              break;
          case FLOAT64: 
              while(i--){
                  al = i < an;
                  bl = i < bn;
                  switch(b.type[0]){
                      case INT8:    r.elements.f64[i] = double_add((dbits){ .d = a.elements.f64[i % an] * al }, (dbits){ .d = b.elements.i8  [i % bn] * bl }, r.err); break;
                      case UINT8:   r.elements.f64[i] = double_add((dbits){ .d = a.elements.f64[i % an] * al }, (dbits){ .d = b.elements.ui8 [i % bn] * bl }, r.err); break;
                      case INT32:   r.elements.f64[i] = double_add((dbits){ .d = a.elements.f64[i % an] * al }, (dbits){ .d = b.elements.i32 [i % bn] * bl }, r.err); break;
                      case UINT32:  r.elements.f64[i] = double_add((dbits){ .d = a.elements.f64[i % an] * al }, (dbits){ .d = b.elements.ui32[i % bn] * bl }, r.err); break;
                      case INT64:   r.elements.f64[i] = double_add((dbits){ .d = a.elements.f64[i % an] * al }, (dbits){ .d = b.elements.i64 [i % bn] * bl }, r.err); break;
                      case UINT64:  r.elements.f64[i] = double_add((dbits){ .d = a.elements.f64[i % an] * al }, (dbits){ .d = b.elements.ui64[i % bn] * bl }, r.err); break;
                      case FLOAT32: r.elements.f64[i] = double_add((dbits){ .d = a.elements.f64[i % an] * al }, (dbits){ .d = b.elements.f32 [i % bn] * bl }, r.err); break;
                      case FLOAT64: r.elements.f64[i] = double_add((dbits){ .d = a.elements.f64[i % an] * al }, (dbits){ .d = b.elements.f64 [i % bn] * bl }, r.err); 
                  }
              }
  
      }
      return r;
  }
  
  
  
  void int_n_to_int_k_len_big_end(void* from, uint32_t from_s, void* to, uint32_t to_s, void* sec_arg, error* err){}
  void int_n_to_int_k_len_lit_end(void* from, uint32_t from_s, void* to, uint32_t to_s, void* sec_arg, error* err){}
  
  void uint_n_to_int_k_len_big_end(void* from, uint32_t from_s, void* to, uint32_t to_s, void* sec_arg, error* err){}
  void uint_n_to_int_k_len_lit_end(void* from, uint32_t from_s, void* to, uint32_t to_s, void* sec_arg, error* err){}
  
  void float_n_to_int_k_len_big_end(void* from, uint32_t from_s, void* to, uint32_t to_s, void* sec_arg, error* err){}
  void float_n_to_int_k_len_lit_end(void* from, uint32_t from_s, void* to, uint32_t to_s, void* sec_arg, error* err){}
  
  
  void int_n_to_uint_k_len_big_end(void* from, uint32_t from_s, void* to, uint32_t to_s, void* sec_arg, error* err){}
  void int_n_to_uint_k_len_lit_end(void* from, uint32_t from_s, void* to, uint32_t to_s, void* sec_arg, error* err){}
  
  void uint_n_to_uint_k_len_big_end(void* from, uint32_t from_s, void* to, uint32_t to_s, void* sec_arg, error* err){}
  void uint_n_to_uint_k_len_lit_end(void* from, uint32_t from_s, void* to, uint32_t to_s, void* sec_arg, error* err){
      uint8_t i = 0;
      if(from_s > to_s){ 
          for( ; i < to_s; i++){ ((int8_t*)to)[i] = ((int8_t*)from)[i]; }
          for( ; i < from_s; i++){ }
      }
      else{   
          for( ; i < from_s; i++){
  
          }
      }
  }
  
  void float_n_to_uint_k_len_big_end(void* from, uint32_t from_s, void* to, uint32_t to_s, void* sec_arg, error* err){}
  void float_n_to_uint_k_len_lit_end(void* from, uint32_t from_s, void* to, uint32_t to_s, void* sec_arg, error* err){}
  
  
  void int_n_to_float_k_len_big_end(void* from, uint32_t from_s, void* to, uint32_t to_s, void* sec_arg, error* err){}
  void int_n_to_float_k_len_lit_end(void* from, uint32_t from_s, void* to, uint32_t to_s, void* sec_arg, error* err){}
  
  void uint_n_to_float_k_len_big_end(void* from, uint32_t from_s, void* to, uint32_t to_s, void* sec_arg, error* err){}
  void uint_n_to_float_k_len_lit_end(void* from, uint32_t from_s, void* to, uint32_t to_s, void* sec_arg, error* err){}
  
  void float_n_to_float_k_len_big_end(void* from, uint32_t from_s, void* to, uint32_t to_s, void* sec_arg, error* err){}
  void float_n_to_float_k_len_lit_end(void* from, uint32_t from_s, void* to, uint32_t to_s, void* sec_arg, error* err){}
  
  #define FUNC_ARR_ELEM(a, b, c) ((a) * 6) + ((b) * 2) + (c) // this macro counts which element(type_convertion_function) of type_conv_func_arr will be called, for principle of it's work please reference to string number 741
  //  a * 6 stand for a arg type's category(int, uint or float), b * 2 stand for which category does b arg type belongs to, and c determines whether to use little endian function of big endian fuction 
  
  void (*type_conv_func_arr[18])(void*, uint32_t, void*, uint32_t, void*, error*) = 
          { int_n_to_int_k_len_big_end, int_n_to_int_k_len_lit_end, uint_n_to_int_k_len_big_end, uint_n_to_int_k_len_lit_end , float_n_to_int_k_len_big_end, float_n_to_int_k_len_lit_end,
            int_n_to_uint_k_len_big_end, int_n_to_uint_k_len_lit_end, uint_n_to_uint_k_len_big_end, uint_n_to_uint_k_len_lit_end, float_n_to_uint_k_len_big_end, float_n_to_uint_k_len_lit_end,
            int_n_to_float_k_len_big_end, int_n_to_float_k_len_lit_end, uint_n_to_float_k_len_big_end, uint_n_to_float_k_len_lit_end, float_n_to_float_k_len_big_end, float_n_to_float_k_len_lit_end };
  
  
  int is_little_end(){
      int32_t i = 0x01020304;
      return *((int8_t*)&i) == 4; 
  }
  
  
  vecN vec_mult_first_arg_t(vecN a, vecN b){// TODO: what if amount of elements in data is less than n
      if(!(a.type && a.n && a.err && a.elements.i32 && b.type && b.n && b.err && b.elements.i32)){ 
          if(a.err){ a.err[0] = NULL_POINTER; }
          return a; 
      }
      if(a.n[0] ^ b.n[0]){ a.err[0] = SIZE_DOES_NOT_MATCH; return a; }
      uint32_t i = a.n[0];
      vecN r = vec_create(a.type[0], i);
      uint8_t end = is_little_end(), a_size = amount_of_type_bytes(a.type[0]), b_size = amount_of_type_bytes(b.type[0]), a_type_cat = int_uint_float_t(a.type[0]), b_type_cat = int_uint_float_t(b.type[0]);
      datapointer ptr;
      while(i--){
          type_conv_func_arr[ FUNC_ARR_ELEM( a_type_cat, b_type_cat, end ) ] (b.elements.i8 + (i * b_size), b_size, ptr.vptr, a_size, a.elements.i8 + (i * a_size), r.err);
          switch(a.type[0]){
              case INT8:    r.elements.i8  [i] = int8_mult  (a.elements.i8  [i], ptr.i8  [0], r.err); break; 
              case UINT8:   r.elements.ui8 [i] = uint8_mult (a.elements.ui8 [i], ptr.ui8 [0], r.err); break;
              case INT32:   r.elements.i32 [i] = int32_mult (a.elements.i32 [i], ptr.i32 [0], r.err); break;
              case UINT32:  r.elements.ui32[i] = uint32_mult(a.elements.ui32[i], ptr.ui32[0], r.err); break;
              case INT64:   r.elements.i64 [i] = int64_mult (a.elements.i64 [i], ptr.i64 [0], r.err); break; 
              case UINT64:  r.elements.ui64[i] = uint64_mult(a.elements.ui64[i], ptr.ui64[0], r.err); break;
              case FLOAT32: r.elements.f32 [i] = float_mult((fbits){ .f = a.elements.f32[i] }, (fbits){ .f = ptr.f32[0] }, r.err); break; 
              case FLOAT64: r.elements.f64 [i] = float_mult((dbits){ .d = a.elements.f64[i] }, (dbits){ .d = ptr.f64[0] }, r.err); break; 
          }
      } 
      return r;
  }
  
  
  int main(){
      FILE* fptr = fopen("pic.ppm", "w");
      char str[100] = {"P6\n128 128\n255\n"};
      uint32_t i, d; 
      for(i = 0; str[i]; i++){}
      for(d = i + 128 * 128 * 3; i < d; i+=3){
          str[i]  = 255;
          str[i + 1] = 1; 
          str[i + 2] = 1;
      }
      str[i] = 0;
      fprintf(fptr, "%s", str);
      fclose(fptr);
      return 0;
  }
  
  
  /*
   graphic:
    left side = color_1, right side = color_2
    top = color_1, bottom = color_2
    checkerboard pattern:
      even pixel = color_1, odd_pixel = color_2
      N pixels = color_1, N next pixels = color_2
    vertical line
      1px at any position
      of width N
      of width N with offset X
    horizontal line
      1px at any position
      of width N
      of width N with offset Y
    coordinate axis:
      1 vertical line + 1 horizontal line, intersecting at the center of the image
    horizontal gradient:
      left side: red = 0; green = 0; blue = 0; right side: red = 255, green = 0, blue = 0
      BLACK                      RED
      ______________________________
      |                            |
      |                            |
      |                            |
    vertical gradient:
      top: red =0; green = 0; blue = 0; bottom: red = 0; green = 255; blue = 0;
    diagonal gradient:
      top left corner: red = 0; green = 0; blue = 0; bottom right corner: red = 255;
  /*
  /*    https://www.shadertoy.com/view/XsBXWt
      https://www.shadertoy.com/view/sdSyDt
  */
  