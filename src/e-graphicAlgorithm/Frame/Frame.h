//--------------------------------------------------------------------------------------
// Copyright (c) longlongwaytogo. All rights reserved.
// longlongway2012@hotmail.com
//--------------------------------------------------------------------------------------

#include <iostream>
#ifdef _DEBUG
#define _WARNING(condition, response) if(condition) {std::clog << response << std::endl;}
#else
#define _WARNING(condition, response) ((void)0)
#endif
