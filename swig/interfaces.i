%module libh6n
%{
#include "libh6n/libh6n.h"

%}
%nodefaultctor; 
%include <stdint.i>
%include "typemaps.i" 



%apply void *VOID_INT_PTR { void * }
#include "libh6n/libh6n.h"

%clearnodefaultctor;
