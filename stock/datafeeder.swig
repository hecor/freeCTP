%module datafeeder
%{
    #define SWIG_FILE_WITH_INIT
	#include "datafeeder.h"
%}

namespace _LEVEL2_{
}

%include std_string.i
%include std_vector.i
%include std_map.i

%template() std::vector<std::string>;
%template() std::map<std::string, int>;

%include "datafeeder.h"
